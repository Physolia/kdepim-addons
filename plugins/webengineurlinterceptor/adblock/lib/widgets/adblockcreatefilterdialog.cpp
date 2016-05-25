/*
  Copyright (c) 2013-2016 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "adblockcreatefilterdialog.h"
#include "ui_adblockcreatefilterwidget.h"

#include <KLocalizedString>

#include <KSharedConfig>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>

using namespace AdBlock;
AdBlockCreateFilterDialog::AdBlockCreateFilterDialog(QWidget *parent)
    : QDialog(parent),
      mCurrentType(AdBlockBlockableItemsWidget::None)
{
    setWindowTitle(i18n("Create Filter"));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AdBlockCreateFilterDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AdBlockCreateFilterDialog::reject);

    QWidget *w = new QWidget;
    mUi = new Ui::AdBlockCreateFilterWidget;
    mUi->setupUi(w);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(w);
    mainLayout->addWidget(buttonBox);
    connect(mUi->filtercustom, &QLineEdit::textChanged, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->blockingFilter, &QRadioButton::toggled, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->exceptionFilter, &QRadioButton::toggled, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->atTheBeginning, &QCheckBox::toggled, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->atTheEnd, &QCheckBox::toggled, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->restrictToDomain, &QCheckBox::toggled, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->restrictToDomainStr, &QLineEdit::textChanged, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->firstPartOnly, &QCheckBox::toggled, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->matchCase, &QCheckBox::toggled, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    connect(mUi->applyListElement, &QListWidget::itemChanged, this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    mUi->collapseBlocked->addItem(i18n("Use default"), QString());
    mUi->collapseBlocked->addItem(i18n("Yes"), QStringLiteral("collapse"));
    mUi->collapseBlocked->addItem(i18n("Not"), QStringLiteral("~collapse"));
    connect(mUi->collapseBlocked, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &AdBlockCreateFilterDialog::slotUpdateFilter);
    readConfig();
}

AdBlockCreateFilterDialog::~AdBlockCreateFilterDialog()
{
    writeConfig();
    delete mUi;
    mUi = 0;
}

void AdBlockCreateFilterDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "AdBlockCreateFilterDialog");
    group.writeEntry("Size", size());
}

void AdBlockCreateFilterDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "AdBlockCreateFilterDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdBlockCreateFilterDialog::setPattern(AdBlockBlockableItemsWidget::TypeElement type, const QString &pattern)
{
    if (mPattern != pattern) {
        mPattern = pattern;
        mCurrentType = type;
        initialize();
    }
}

void AdBlockCreateFilterDialog::initialize()
{
    mUi->applyListElement->clear();
    for (int i = AdBlockBlockableItemsWidget::None + 1; i < AdBlockBlockableItemsWidget::MaxTypeElement; ++i) {
        QListWidgetItem *item = new QListWidgetItem(AdBlockBlockableItemsWidget::elementTypeToI18n(static_cast<AdBlockBlockableItemsWidget::TypeElement>(i)), mUi->applyListElement);
        item->setData(ElementValue, static_cast<AdBlockBlockableItemsWidget::TypeElement>(i));
        item->setCheckState(Qt::Unchecked);
        if (i == (int)mCurrentType) {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }

    }
    mUi->blockingFilter->setChecked(true);
    mUi->filtercustom->setText(mPattern);
    slotUpdateFilter();
}

QString AdBlockCreateFilterDialog::filter() const
{
    return mUi->filterName->text();
}

void AdBlockCreateFilterDialog::slotUpdateFilter()
{
    QString pattern = mUi->filtercustom->text();
    if (mUi->atTheBeginning->isChecked()) {
        pattern = QLatin1String("|") + pattern;
    }
    if (mUi->atTheEnd->isChecked()) {
        pattern += QLatin1String("|");
    }

    const QString collapseValue = mUi->collapseBlocked->itemData(mUi->collapseBlocked->currentIndex()).toString();
    if (!collapseValue.isEmpty()) {
        pattern += QLatin1Char('$') + collapseValue;
    }

    pattern += (collapseValue.isEmpty() ? QLatin1String("$") : QLatin1String(",")) + AdBlockBlockableItemsWidget::elementType(mCurrentType);

    if (mUi->exceptionFilter->isChecked()) {
        pattern = QLatin1String("@@") + pattern;
    }
    const int numberOfElement(mUi->applyListElement->count());
    for (int i = 0; i < numberOfElement; ++i) {
        QListWidgetItem *item = mUi->applyListElement->item(i);
        if ((item->checkState() == Qt::Checked) && (item->flags() & Qt::ItemIsEnabled)) {
            pattern += QLatin1Char(',') + AdBlockBlockableItemsWidget::elementType(static_cast<AdBlockBlockableItemsWidget::TypeElement>(item->data(ElementValue).toInt()));
        }
    }

    if (mUi->restrictToDomain->isChecked()) {
        if (!mUi->restrictToDomainStr->text().isEmpty()) {
            pattern += QLatin1String(",domain=") + mUi->restrictToDomainStr->text();
        }
    }
    if (mUi->matchCase->isChecked()) {
        pattern += QLatin1String(",match-case");
    }
    if (mUi->firstPartOnly->isChecked()) {
        pattern += QLatin1String(",~third-party");
    }
    mUi->filterName->setText(pattern);
}

