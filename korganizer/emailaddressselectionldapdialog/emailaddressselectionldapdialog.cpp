/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "emailaddressselectionldapdialog.h"
#include <Akonadi/RecipientsPickerWidget>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KSharedConfig>
#include <PimCommonAkonadi/LdapSearchDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
static const char myConfigEmailAddressSelectionLdapDialog[] = "EmailAddressSelectionLdapDialog";
}
K_PLUGIN_CLASS_WITH_JSON(EmailAddressSelectionLdapDialog, "emailaddressselectionldapdialog.json")

EmailAddressSelectionLdapDialog::EmailAddressSelectionLdapDialog(QWidget *parent, const QList<QVariant> &)
    : Akonadi::AbstractEmailAddressSelectionDialog(parent)
    , mView(new Akonadi::RecipientsPickerWidget(true, nullptr, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mView);
    connect(mView->emailAddressSelectionWidget()->view(), &QTreeView::doubleClicked, this, &QDialog::accept);

    auto searchLDAPButton = new QPushButton(i18n("Search &Directory Service"), this);
    connect(searchLDAPButton, &QPushButton::clicked, this, &EmailAddressSelectionLdapDialog::slotSearchLDAP);
    mainLayout->addWidget(searchLDAPButton);

    KConfig config(QStringLiteral("kabldaprc"));
    KConfigGroup group = config.group("LDAP");
    int numHosts = group.readEntry("NumSelectedHosts", 0);
    if (!numHosts) {
        searchLDAPButton->setVisible(false);
    }

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttonBox);
    readConfig();
}

void EmailAddressSelectionLdapDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigEmailAddressSelectionLdapDialog);
    const QSize size = group.readEntry("Size", QSize());
    if (size.isValid()) {
        resize(size);
    } else {
        resize(sizeHint().width(), sizeHint().height());
    }
}

void EmailAddressSelectionLdapDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigEmailAddressSelectionLdapDialog);
    group.writeEntry("Size", size());
}

EmailAddressSelectionLdapDialog::~EmailAddressSelectionLdapDialog()
{
    writeConfig();
}

Akonadi::EmailAddressSelection::List EmailAddressSelectionLdapDialog::selectedAddresses() const
{
    return mView->emailAddressSelectionWidget()->selectedAddresses();
}

Akonadi::EmailAddressSelectionWidget *EmailAddressSelectionLdapDialog::view() const
{
    return mView->emailAddressSelectionWidget();
}

void EmailAddressSelectionLdapDialog::slotSearchLDAP()
{
    if (!mLdapSearchDialog) {
        mLdapSearchDialog = new PimCommon::LdapSearchDialog(this);
        connect(mLdapSearchDialog, &PimCommon::LdapSearchDialog::contactsAdded, this, &EmailAddressSelectionLdapDialog::ldapSearchResult);
    }

    mLdapSearchDialog->setSearchText(mView->emailAddressSelectionWidget()->searchLineEdit()->text());
    mLdapSearchDialog->show();
}

void EmailAddressSelectionLdapDialog::ldapSearchResult()
{
    const KContacts::Addressee::List contacts = mLdapSearchDialog->selectedContacts();
    Q_EMIT insertAddresses(contacts);
}

#include "emailaddressselectionldapdialog.moc"
