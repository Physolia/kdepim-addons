/*
   Copyright (C) 2019 Montel Laurent <montel@kde.org>

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
#include "quicktextconfiguredialog.h"
#include "quicktextconfigurewidget.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace {
static const char myConfigGroupName[] = "QuickTextConfigureDialog";
}
QuickTextConfigureDialog::QuickTextConfigureDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mQuickTextConfigureWidget = new QuickTextConfigureWidget(this);
    mQuickTextConfigureWidget->setObjectName(QStringLiteral("quicktextconfigurewidget"));
    mainLayout->addWidget(mQuickTextConfigureWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QuickTextConfigureDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QuickTextConfigureDialog::reject);
    mainLayout->addWidget(buttonBox);
    writeConfig();
}

QuickTextConfigureDialog::~QuickTextConfigureDialog()
{
    readConfig();
}

void QuickTextConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void QuickTextConfigureDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 350));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}