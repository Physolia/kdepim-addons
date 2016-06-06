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

#include "confirmaddressdialog.h"
#include "confirmaddresswidget.h"
#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ConfirmAddressDialog::ConfirmAddressDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Confirm Addresses"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mConfirmWidget = new ConfirmAddressWidget(this);
    mConfirmWidget->setObjectName(QStringLiteral("confirmwidget"));
    mainLayout->addWidget(mConfirmWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfirmAddressDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfirmAddressDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

ConfirmAddressDialog::~ConfirmAddressDialog()
{
    writeConfig();
}

void ConfirmAddressDialog::slotAccepted()
{
    //TODO
    accept();
}


void ConfirmAddressDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "ConfirmAddressDialog");
    group.writeEntry("Size", size());
}

void ConfirmAddressDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "ConfirmAddressDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(500, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
