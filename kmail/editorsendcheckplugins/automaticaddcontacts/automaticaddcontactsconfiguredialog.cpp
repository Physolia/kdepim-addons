/*
   Copyright (C) 2016-2019 Montel Laurent <montel@kde.org>

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

#include "automaticaddcontactsconfiguredialog.h"
#include "automaticaddcontactsconfigurewidget.h"
#include <KLocalizedString>

AutomaticAddContactsConfigureDialog::AutomaticAddContactsConfigureDialog(QWidget *parent)
    : PimCommon::ConfigurePluginDialog(parent)
    , mConfigureWidget(nullptr)
{
    setWindowTitle(i18nc("@title:window", "Configure \"Automatic Add Contacts\" Plugin"));
    initLayout();
}

AutomaticAddContactsConfigureDialog::~AutomaticAddContactsConfigureDialog()
{
}

QWidget *AutomaticAddContactsConfigureDialog::createLayout()
{
    mConfigureWidget = new AutomaticAddContactsConfigureWidget(this);
    return mConfigureWidget;
}

void AutomaticAddContactsConfigureDialog::save()
{
    mConfigureWidget->saveSettings();
}

void AutomaticAddContactsConfigureDialog::load()
{
    mConfigureWidget->loadSettings();
}

void AutomaticAddContactsConfigureDialog::reset()
{
    mConfigureWidget->resetSettings();
}

void AutomaticAddContactsConfigureDialog::help()
{
    showHelp(QStringLiteral("kmail2"), mConfigureWidget->helpAnchor());
}
