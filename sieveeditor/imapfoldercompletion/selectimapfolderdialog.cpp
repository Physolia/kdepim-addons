/*
   Copyright (C) 2016 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "selectimapfolderdialog.h"
#include "selectimapfolderwidget.h"

#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

SelectImapFolderDialog::SelectImapFolderDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Select IMAP folder"));
    QVBoxLayout *layout = new QVBoxLayout(this);

    mSelectImapFolderWidget = new SelectImapFolderWidget(this);
    mSelectImapFolderWidget->setObjectName(QStringLiteral("selectimapfolderwidget"));
    layout->addWidget(mSelectImapFolderWidget);
    connect(mSelectImapFolderWidget, &SelectImapFolderWidget::enableOkButton, this, &SelectImapFolderDialog::slotEnableOkButton);
    connect(mSelectImapFolderWidget, &SelectImapFolderWidget::folderSelected, this, &SelectImapFolderDialog::accept);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttonBox);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    // Set the button actions
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectImapFolderDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectImapFolderDialog::reject);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    readConfig();
}

SelectImapFolderDialog::~SelectImapFolderDialog()
{
    writeConfig();
}

void SelectImapFolderDialog::slotEnableOkButton(bool enabled)
{
    mOkButton->setEnabled(enabled);
}

void SelectImapFolderDialog::setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account)
{
    mSelectImapFolderWidget->setSieveImapAccountSettings(account);
}

QString SelectImapFolderDialog::selectedFolderName() const
{
    return mSelectImapFolderWidget->selectedFolderName();
}

void SelectImapFolderDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SelectImapFolderDialog");
    group.writeEntry("Size", size());
}

void SelectImapFolderDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SelectImapFolderDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(500, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
