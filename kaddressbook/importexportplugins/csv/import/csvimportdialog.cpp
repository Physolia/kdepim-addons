/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "csvimportdialog.h"

#include "dateparser.h"
#include "qcsvmodel.h"
#include "templateselectiondialog.h"

#include <KConfig>
#include <KLineEdit>
#include <KLocalizedString>
#include <KMessageBox>
#include <KUrlRequester>
#include <QComboBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QProgressDialog>

#include <KConfigGroup>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QRadioButton>
#include <QStandardPaths>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTextCodec>
#include <QThread>
#include <QUuid>
#include <QVBoxLayout>

enum {
    Local = 0,
    Latin1 = 1,
    Uni = 2,
    MSBug = 3,
    Codec = 4,
};

class ContactFieldComboBox : public QComboBox
{
    Q_OBJECT
public:
    ContactFieldComboBox(QWidget *parent = nullptr)
        : QComboBox(parent)
    {
        fillFieldMap();

        addItem(KAddressBookImportExport::ContactFields::label(KAddressBookImportExport::ContactFields::Undefined),
                KAddressBookImportExport::ContactFields::Undefined);

        QMapIterator<QString, KAddressBookImportExport::ContactFields::Field> it(mFieldMap);
        while (it.hasNext()) {
            it.next();

            addItem(it.key(), QVariant(it.value()));
        }

        int maxLength = 0;
        for (int i = 0; i < count(); ++i) {
            maxLength = qMax(maxLength, itemText(i).length());
        }

        setMinimumContentsLength(maxLength);
        setSizeAdjustPolicy(AdjustToContents);
        setFixedSize(sizeHint());
    }

    void setCurrentField(KAddressBookImportExport::ContactFields::Field field)
    {
        setCurrentIndex(findData((uint)field));
    }

    [[nodiscard]] KAddressBookImportExport::ContactFields::Field currentField() const
    {
        return (KAddressBookImportExport::ContactFields::Field)itemData(currentIndex()).toUInt();
    }

private:
    static void fillFieldMap()
    {
        if (!mFieldMap.isEmpty()) {
            return;
        }

        KAddressBookImportExport::ContactFields::Fields fields = KAddressBookImportExport::ContactFields::allFields();
        fields.remove(KAddressBookImportExport::ContactFields::Undefined);

        for (int i = 0, total = fields.count(); i < total; ++i) {
            mFieldMap.insert(KAddressBookImportExport::ContactFields::label(fields.at(i)), fields.at(i));
        }
    }

    static QMap<QString, KAddressBookImportExport::ContactFields::Field> mFieldMap;
};

QMap<QString, KAddressBookImportExport::ContactFields::Field> ContactFieldComboBox::mFieldMap;

class ContactFieldDelegate : public QStyledItemDelegate
{
public:
    ContactFieldDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    [[nodiscard]] QString displayText(const QVariant &value, const QLocale &) const override
    {
        return KAddressBookImportExport::ContactFields::label((KAddressBookImportExport::ContactFields::Field)value.toUInt());
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        auto editor = new ContactFieldComboBox(parent);

        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        const unsigned int value = index.model()->data(index, Qt::EditRole).toUInt();

        auto fieldCombo = static_cast<ContactFieldComboBox *>(editor);
        fieldCombo->setCurrentField((KAddressBookImportExport::ContactFields::Field)value);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        auto fieldCombo = static_cast<ContactFieldComboBox *>(editor);

        model->setData(index, fieldCombo->currentField(), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const override
    {
        editor->setGeometry(option.rect);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        if (index.row() == 0) {
            QStyleOptionViewItem headerOption(option);
            headerOption.font.setBold(true);

            QStyledItemDelegate::paint(painter, headerOption, index);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

CSVImportDialog::CSVImportDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "CSV Import Dialog"));
    setModal(true);

    mModel = new QCsvModel(this);

    initGUI();

    reloadCodecs();

    connect(mUrlRequester, &KUrlRequester::returnPressed, this, &CSVImportDialog::setFile);
    connect(mUrlRequester, &KUrlRequester::urlSelected, this, &CSVImportDialog::setUrl);
    connect(mUrlRequester->lineEdit(), &QLineEdit::textChanged, this, &CSVImportDialog::urlChanged);
    connect(mDelimiterGroup, &QButtonGroup::buttonClicked, this, [this](QAbstractButton *button) {
        if (button) {
            const int val = mDelimiterGroup->id(button);
            delimiterClicked(val);
        }
    });
    connect(mDelimiterEdit, &QLineEdit::returnPressed, this, [this]() {
        customDelimiterChanged();
    });
    connect(mDelimiterEdit, &QLineEdit::textChanged, this, [this](const QString &str) {
        customDelimiterChanged(str);
    });
    connect(mComboQuote, &QComboBox::textActivated, this, [this](const QString &str) {
        textQuoteChanged(str);
    });
    connect(mCodecCombo, &QComboBox::textActivated, this, [this]() {
        codecChanged();
    });

    connect(mSkipFirstRow, &QCheckBox::toggled, this, [this](bool b) {
        skipFirstRowChanged(b);
    });

    connect(mModel, &QCsvModel::finishedLoading, this, &CSVImportDialog::modelFinishedLoading);

    delimiterClicked(0);
    textQuoteChanged(QStringLiteral("\""));
    skipFirstRowChanged(false);
}

CSVImportDialog::~CSVImportDialog()
{
    delete mDevice;
}

KContacts::AddresseeList CSVImportDialog::contacts() const
{
    KContacts::AddresseeList contacts;
    DateParser dateParser(mDatePatternEdit->text());

    QProgressDialog progressDialog(const_cast<CSVImportDialog *>(this));
    progressDialog.setAutoClose(true);
    progressDialog.setMaximum(mModel->rowCount());
    progressDialog.setLabelText(i18nc("@label", "Importing contacts"));
    progressDialog.show();

    qApp->processEvents();

    for (int row = 1; row < mModel->rowCount(); ++row) {
        KContacts::Addressee contact;
        bool emptyRow = true;

        for (int column = 0; column < mModel->columnCount(); ++column) {
            QString value = mModel->data(mModel->index(row, column), Qt::DisplayRole).toString();

            if (!value.isEmpty()) {
                emptyRow = false;

                const KAddressBookImportExport::ContactFields::Field field =
                    (KAddressBookImportExport::ContactFields::Field)mModel->data(mModel->index(0, column)).toUInt();

                // convert the custom date format to ISO format
                if (field == KAddressBookImportExport::ContactFields::Birthday || field == KAddressBookImportExport::ContactFields::Anniversary) {
                    value = dateParser.parse(value).toString(Qt::ISODate);
                }

                value.replace(QLatin1String("\\n"), QStringLiteral("\n"));

                KAddressBookImportExport::ContactFields::setValue(field, value, contact);
            }
        }

        qApp->processEvents();

        if (progressDialog.wasCanceled()) {
            return {};
        }

        progressDialog.setValue(progressDialog.value() + 1);

        if (!emptyRow && !contact.isEmpty()) {
            contacts.append(contact);
        }
    }

    return contacts;
}

void CSVImportDialog::initGUI()
{
    auto page = new QWidget(this);

    auto mainLayout = new QVBoxLayout(this);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(page);

    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setDefault(true);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    mUser1Button = new QPushButton;
    buttonBox->addButton(mUser1Button, QDialogButtonBox::ActionRole);
    connect(mUser1Button, &QAbstractButton::clicked, this, &CSVImportDialog::slotApplyTemplate);
    mUser2Button = new QPushButton;
    connect(mUser2Button, &QAbstractButton::clicked, this, &CSVImportDialog::slotSaveTemplate);
    buttonBox->addButton(mUser2Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CSVImportDialog::slotOk);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto layout = new QGridLayout;
    mainLayout->addLayout(layout);
    layout->setContentsMargins({});

    auto hbox = new QHBoxLayout;

    auto label = new QLabel(i18nc("@label", "File to import:"), page);
    mainLayout->addWidget(label);
    hbox->addWidget(label);

    mUrlRequester = new KUrlRequester(page);
    mainLayout->addWidget(mUrlRequester);
    mUrlRequester->setMimeTypeFilters({QStringLiteral("text/csv")});
    mUrlRequester->lineEdit()->setTrapReturnKey(true);
    mUrlRequester->setToolTip(i18nc("@info:tooltip", "Select a csv file to import"));
    mUrlRequester->setWhatsThis(i18nc("@info:whatsthis",
                                      "Click this button to start a file chooser that will allow you to "
                                      "select a csv file to import."));
    hbox->addWidget(mUrlRequester);

    layout->addLayout(hbox, 0, 0, 1, 5);

    // Delimiter: comma, semicolon, tab, space, other
    auto group = new QGroupBox(i18nc("@title:group", "Delimiter"), page);
    mainLayout->addWidget(group);
    auto delimiterLayout = new QGridLayout;
    group->setLayout(delimiterLayout);
    delimiterLayout->setAlignment(Qt::AlignTop);
    layout->addWidget(group, 1, 0, 4, 1);

    mDelimiterGroup = new QButtonGroup(this);
    mDelimiterGroup->setExclusive(true);

    auto button = new QRadioButton(i18nc("@option:radio Field separator", "Comma"));
    button->setToolTip(i18nc("@info:tooltip", "Set the field separator to a comma"));
    button->setWhatsThis(i18nc("@info:whatsthis", "Select this option if your csv file uses the comma as a field separator."));
    button->setChecked(true);
    mDelimiterGroup->addButton(button, 0);
    delimiterLayout->addWidget(button, 0, 0);

    button = new QRadioButton(i18nc("@option:radio Field separator", "Semicolon"));
    button->setToolTip(i18nc("@info:tooltip", "Set the field separator to a semicolon"));
    button->setWhatsThis(i18nc("@info:whatsthis", "Select this option if your csv file uses the semicolon as a field separator."));
    mDelimiterGroup->addButton(button, 1);
    delimiterLayout->addWidget(button, 0, 1);

    button = new QRadioButton(i18nc("@option:radio Field separator", "Tabulator"));
    button->setToolTip(i18nc("@info:tooltip", "Set the field separator to a tab character"));
    button->setWhatsThis(i18nc("@info:whatsthis", "Select this option if your csv file uses the tab character as a field separator."));
    mDelimiterGroup->addButton(button, 2);
    delimiterLayout->addWidget(button, 1, 0);

    button = new QRadioButton(i18nc("@option:radio Field separator", "Space"));
    button->setToolTip(i18nc("@info:tooltip", "Set the field separator to a space character"));
    button->setWhatsThis(i18nc("@info:whatsthis", "Select this option if your csv file uses the space character as a field separator."));
    mDelimiterGroup->addButton(button, 3);
    delimiterLayout->addWidget(button, 1, 1);

    button = new QRadioButton(i18nc("@option:radio Custom field separator", "Other"));
    button->setToolTip(i18nc("@info:tooltip", "Set the field separator to a custom character"));
    button->setWhatsThis(i18nc("@info:whatsthis",
                               "Select this option if to use some other character as the field delimiter "
                               "for the data in your csv file."));
    mDelimiterGroup->addButton(button, 4);
    delimiterLayout->addWidget(button, 0, 2);

    mDelimiterEdit = new QLineEdit(group);
    mDelimiterEdit->setToolTip(i18nc("@info:tooltip", "Set the custom delimiter character"));
    mDelimiterEdit->setWhatsThis(i18nc("@info:whatsthis",
                                       "Enter a custom character to use as the delimiter character. "
                                       "If you enter more than 1 character, only the first will be used and "
                                       "the remaining characters will be ignored."));
    delimiterLayout->addWidget(mDelimiterEdit, 1, 2);

    // text quote
    label = new QLabel(i18nc("@label:listbox", "Text quote:"), page);
    mainLayout->addWidget(label);
    layout->addWidget(label, 1, 2);

    mComboQuote = new QComboBox(page);
    mainLayout->addWidget(mComboQuote);
    mComboQuote->setToolTip(i18nc("@info:tooltip", "Select the quote character"));
    mComboQuote->setWhatsThis(i18nc("@info:whatsthis",
                                    "Choose the character that your csv data uses to \"quote\" the field delimiter "
                                    "if that character happens to occur within the data.  For example, if the "
                                    "comma is the field delimiter, then any comma occurring with the data "
                                    "will be \"quoted\" by the character specified here."));
    mComboQuote->setEditable(false);
    mComboQuote->addItem(i18nc("@item:inlistbox Quote character option", "\""), 0);
    mComboQuote->addItem(i18nc("@item:inlistbox Quote character option", "'"), 1);
    mComboQuote->addItem(i18nc("@item:inlistbox Quote character option", "None"), 2);
    layout->addWidget(mComboQuote, 1, 3);

    // date format
    label = new QLabel(i18nc("@label:listbox", "Date format:"), page);
    mainLayout->addWidget(label);
    layout->addWidget(label, 2, 2);

    mDatePatternEdit = new QLineEdit(page);
    mainLayout->addWidget(mDatePatternEdit);
    mDatePatternEdit->setText(QStringLiteral("Y-M-D")); // ISO 8601 date format as default
    mDatePatternEdit->setToolTip(xi18nc("@info:tooltip",
                                        "<para><list><item>y: year with 2 digits</item>"
                                        "<item>Y: year with 4 digits</item>"
                                        "<item>m: month with 1 or 2 digits</item>"
                                        "<item>M: month with 2 digits</item>"
                                        "<item>d: day with 1 or 2 digits</item>"
                                        "<item>D: day with 2 digits</item>"
                                        "<item>H: hours with 2 digits</item>"
                                        "<item>I: minutes with 2 digits</item>"
                                        "<item>S: seconds with 2 digits</item>"
                                        "</list></para>"));
    mDatePatternEdit->setWhatsThis(xi18nc("@info:whatsthis",
                                          "<para>Specify a format to use for dates included in your csv data. "
                                          "Use the following sequences to help you define the format:</para>"
                                          "<para><list><item>y: year with 2 digits</item>"
                                          "<item>Y: year with 4 digits</item>"
                                          "<item>m: month with 1 or 2 digits</item>"
                                          "<item>M: month with 2 digits</item>"
                                          "<item>d: day with 1 or 2 digits</item>"
                                          "<item>D: day with 2 digits</item>"
                                          "<item>H: hours with 2 digits</item>"
                                          "<item>I: minutes with 2 digits</item>"
                                          "<item>S: seconds with 2 digits</item>"
                                          "</list></para>"
                                          "<para>Example: \"Y-M-D\" corresponds to a date like \"2012-01-04\"</para>"));
    layout->addWidget(mDatePatternEdit, 2, 3);

    // text codec
    label = new QLabel(i18nc("@label:listbox", "Text codec:"), page);
    mainLayout->addWidget(label);
    layout->addWidget(label, 3, 2);

    mCodecCombo = new QComboBox(page);
    mainLayout->addWidget(mCodecCombo);
    mCodecCombo->setToolTip(i18nc("@info:tooltip", "Select the text codec"));
    mCodecCombo->setWhatsThis(i18nc("@info:whatsthis", "Choose the character encoding of the data in your csv file."));
    layout->addWidget(mCodecCombo, 3, 3);

    // skip first line
    mSkipFirstRow = new QCheckBox(i18nc("@option:check", "Skip first row of file"), page);
    mainLayout->addWidget(mSkipFirstRow);
    mSkipFirstRow->setToolTip(i18nc("@info:tooltip", "Skip first row of csv file when importing"));
    mSkipFirstRow->setWhatsThis(i18nc("@info:whatsthis",
                                      "Check this box if you want the import to skip over the first row "
                                      "of the csv data. In many cases, the first line of a csv file will be a "
                                      "comment line describing the order of the data fields included in the file."));
    layout->addWidget(mSkipFirstRow, 4, 2, 1, 2);

    // csv view
    mTable = new QTableView(page);
    mainLayout->addWidget(mTable);
    mTable->setModel(mModel);
    mTable->setItemDelegateForRow(0, new ContactFieldDelegate(this));
    mTable->horizontalHeader()->hide();
    mTable->verticalHeader()->hide();
    mTable->setEditTriggers(QAbstractItemView::CurrentChanged);
    mTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    layout->addWidget(mTable, 5, 0, 1, 5);

    mUser1Button->setText(i18nc("@action:button", "Apply Template..."));
    mUser2Button->setText(i18nc("@action:button", "Save Template..."));

    mOkButton->setEnabled(false);
    mUser1Button->setEnabled(false);
    mUser2Button->setEnabled(false);
    mainLayout->addWidget(buttonBox);

    resize(500, 400);
}

void CSVImportDialog::reloadCodecs()
{
    mCodecCombo->clear();

    mCodecs.clear();

    const QList<QByteArray> lstCodec = QTextCodec::availableCodecs();
    for (const QByteArray &name : lstCodec) {
        mCodecs.append(QTextCodec::codecForName(name));
    }

    mCodecCombo->addItem(i18nc("@item:inlistbox Codec setting", "Local (%1)", QLatin1String(QTextCodec::codecForLocale()->name())), Local);
    mCodecCombo->addItem(i18nc("@item:inlistbox Codec setting", "Latin1"), Latin1);
    mCodecCombo->addItem(i18nc("@item:inlistbox Codec setting", "Unicode"), Uni);
    mCodecCombo->addItem(i18nc("@item:inlistbox Codec setting", "Microsoft Unicode"), MSBug);

    for (int i = 0, total = mCodecs.count(); i < total; ++i) {
        mCodecCombo->addItem(QLatin1String(mCodecs.at(i)->name()), Codec + i);
    }
}

void CSVImportDialog::customDelimiterChanged()
{
    if (mDelimiterGroup->checkedId() == 4) {
        delimiterClicked(4);
    }
}

void CSVImportDialog::customDelimiterChanged(const QString &, bool reload)
{
    mDelimiterGroup->button(4)->setChecked(true);
    delimiterClicked(4, reload); // other
}

void CSVImportDialog::delimiterClicked(int id, bool reload)
{
    switch (id) {
    case 0: // comma
        mModel->setDelimiter(QLatin1Char(','));
        break;
    case 4: // other
        mDelimiterEdit->setFocus(Qt::OtherFocusReason);
        if (!mDelimiterEdit->text().isEmpty()) {
            mModel->setDelimiter(mDelimiterEdit->text().at(0));
        }
        break;
    case 2: // tab
        mModel->setDelimiter(QLatin1Char('\t'));
        break;
    case 3: // space
        mModel->setDelimiter(QLatin1Char(' '));
        break;
    case 1: // semicolon
        mModel->setDelimiter(QLatin1Char(';'));
        break;
    }

    if (mDevice && reload) {
        mModel->load(mDevice);
    }
}

void CSVImportDialog::textQuoteChanged(const QString &mark, bool reload)
{
    if (mComboQuote->currentIndex() == 2) {
        mModel->setTextQuote(QChar());
    } else {
        mModel->setTextQuote(mark.at(0));
    }

    if (mDevice && reload) {
        mModel->load(mDevice);
    }
}

void CSVImportDialog::skipFirstRowChanged(bool checked, bool reload)
{
    mFieldSelection.clear();
    for (int column = 0; column < mModel->columnCount(); ++column) {
        mFieldSelection.append((KAddressBookImportExport::ContactFields::Field)mModel->data(mModel->index(0, column)).toInt());
    }

    if (checked) {
        mModel->setStartRow(1);
    } else {
        mModel->setStartRow(0);
    }

    if (mDevice && reload) {
        mModel->load(mDevice);
    }
}

void CSVImportDialog::slotApplyTemplate()
{
    applyTemplate();
}

void CSVImportDialog::slotSaveTemplate()
{
    saveTemplate();
}

void CSVImportDialog::slotOk()
{
    bool assigned = false;

    for (int column = 0; column < mModel->columnCount(); ++column) {
        if (mModel->data(mModel->index(0, column), Qt::DisplayRole).toUInt() != KAddressBookImportExport::ContactFields::Undefined) {
            assigned = true;
            break;
        }
    }

    if (!assigned) {
        KMessageBox::error(this, i18nc("@info:status", "You must assign at least one column."));
    } else {
        accept();
    }
}

void CSVImportDialog::applyTemplate()
{
    QPointer<TemplateSelectionDialog> dlg = new TemplateSelectionDialog(this);
    if (!dlg->templatesAvailable()) {
        KMessageBox::error(this, i18nc("@label", "There are no templates available yet."), i18nc("@title:window", "No templates available"));
        delete dlg;
        return;
    }

    if (!dlg->exec()) {
        delete dlg;
        return;
    }

    const QString templateFileName = dlg->selectedTemplate();
    delete dlg;

    KConfig config(templateFileName, KConfig::SimpleConfig);

    const KConfigGroup generalGroup(&config, "General");
    mDatePatternEdit->setText(generalGroup.readEntry("DatePattern", "Y-M-D"));
    mDelimiterEdit->setText(generalGroup.readEntry("DelimiterOther"));

    const int delimiterButton = generalGroup.readEntry("DelimiterType", 0);
    const int quoteType = generalGroup.readEntry("QuoteType", 0);
    const bool skipFirstRow = generalGroup.readEntry("SkipFirstRow", false);

    mDelimiterGroup->button(delimiterButton)->setChecked(true);
    delimiterClicked(delimiterButton, false);

    mComboQuote->setCurrentIndex(quoteType);
    textQuoteChanged(mComboQuote->currentText(), false);

    // do block signals here, otherwise it will trigger a reload of the model and
    // the following skipFirstRowChanged call end up with an empty model
    mSkipFirstRow->blockSignals(true);
    mSkipFirstRow->setChecked(skipFirstRow);
    mSkipFirstRow->blockSignals(false);

    skipFirstRowChanged(skipFirstRow, false);

    if (mDevice) {
        mModel->load(mDevice);
    }

    setProperty("TemplateFileName", templateFileName);
    connect(mModel, &QCsvModel::finishedLoading, this, &CSVImportDialog::finalizeApplyTemplate);
}

void CSVImportDialog::finalizeApplyTemplate()
{
    const QString templateFileName = property("TemplateFileName").toString();

    KConfig config(templateFileName, KConfig::SimpleConfig);

    const KConfigGroup generalGroup(&config, "General");
    const uint columns = generalGroup.readEntry("Columns", 0);

    // create the column map
    const KConfigGroup columnMapGroup(&config, "csv column map");

    for (uint i = 0; i < columns; ++i) {
        const uint assignedField = columnMapGroup.readEntry(QString::number(i), 0);
        mModel->setData(mModel->index(0, i), assignedField, Qt::EditRole);
    }
}

void CSVImportDialog::saveTemplate()
{
    const QString name = QInputDialog::getText(this, i18nc("@title:window", "Template Name"), i18nc("@info", "Please enter a name for the template:"));

    if (name.isEmpty()) {
        return;
    }

    const int numberOfColumn(mModel->columnCount());
    if (numberOfColumn == 0) {
        return;
    }

    const QString fileName = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/kaddressbook/csv-templates/")
        + QUuid::createUuid().toString() + QStringLiteral(".desktop");

    QFileInfo fileInfo(fileName);
    QDir().mkpath(fileInfo.absolutePath());

    KConfig config(fileName);
    KConfigGroup generalGroup(&config, QStringLiteral("General"));
    generalGroup.writeEntry("DatePattern", mDatePatternEdit->text());
    generalGroup.writeEntry("Columns", mModel->columnCount());
    generalGroup.writeEntry("DelimiterType", mDelimiterGroup->checkedId());
    generalGroup.writeEntry("DelimiterOther", mDelimiterEdit->text());
    generalGroup.writeEntry("SkipFirstRow", mSkipFirstRow->isChecked());
    generalGroup.writeEntry("QuoteType", mComboQuote->currentIndex());

    KConfigGroup miscGroup(&config, QStringLiteral("Misc"));
    miscGroup.writeEntry("Name", name);

    KConfigGroup columnMapGroup(&config, QStringLiteral("csv column map"));
    for (int column = 0; column < numberOfColumn; ++column) {
        columnMapGroup.writeEntry(QString::number(column), mModel->data(mModel->index(0, column), Qt::DisplayRole).toUInt());
    }

    config.sync();
}

void CSVImportDialog::setUrl(const QUrl &fileName)
{
    setFile(fileName.toLocalFile());
}

void CSVImportDialog::setFile(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return;
    }

    auto file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly)) {
        KMessageBox::error(this, i18nc("@info:status", "Cannot open input file."));
        delete file;
        return;
    }

    delete mDevice;

    mDevice = file;

    mModel->load(mDevice);
}

void CSVImportDialog::urlChanged(const QString &file)
{
    bool state = !file.isEmpty();

    mOkButton->setEnabled(state);
    mUser1Button->setEnabled(state);
    mUser2Button->setEnabled(state);
}

void CSVImportDialog::codecChanged(bool reload)
{
    const int code = mCodecCombo->currentIndex();

    if (code == Local) {
        mModel->setTextCodec(QTextCodec::codecForLocale());
    } else if (code >= Codec) {
        mModel->setTextCodec(mCodecs.at(code - Codec));
    } else if (code == Uni) {
        mModel->setTextCodec(QTextCodec::codecForName("UTF-16"));
    } else if (code == MSBug) {
        mModel->setTextCodec(QTextCodec::codecForName("UTF-16LE"));
    } else if (code == Latin1) {
        mModel->setTextCodec(QTextCodec::codecForName("ISO 8859-1"));
    } else {
        mModel->setTextCodec(QTextCodec::codecForName("UTF-8"));
    }

    if (mDevice && reload) {
        mModel->load(mDevice);
    }
}

void CSVImportDialog::modelFinishedLoading()
{
    auto box = new ContactFieldComboBox();
    int preferredWidth = box->sizeHint().width();
    delete box;

    for (int i = 0; i < mModel->columnCount(); ++i) {
        mTable->setColumnWidth(i, preferredWidth);
    }

    for (int column = 0; column < mFieldSelection.count(); ++column) {
        mModel->setData(mModel->index(0, column), mFieldSelection.at(column), Qt::EditRole);
    }
    mFieldSelection.clear();
}

#include "csvimportdialog.moc"

#include "moc_csvimportdialog.cpp"
