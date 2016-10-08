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

#include "ldapimportexportplugininterface.h"
#include <KLocalizedString>
#include <KActionCollection>
#include <QAction>

LDapImportExportPluginInterface::LDapImportExportPluginInterface(QObject *parent)
    : KAddressBookImportExport::KAddressBookImportExportPluginInterface(parent)
{

}

LDapImportExportPluginInterface::~LDapImportExportPluginInterface()
{

}


void LDapImportExportPluginInterface::createAction(KActionCollection *ac)
{
#if 0
    QAction *action = ac->addAction(QStringLiteral("file_import_ldap"));
    action->setText(i18n("Import LDap..."));
    action->setWhatsThis(i18n("Import contacts from a LDap file."));
    setImportActions(QList<QAction *>() << action);

    QList<QAction *> exportActionList;

    action = ac->addAction(QStringLiteral("file_import_ldap"));
    action->setText(i18n("Import LDap..."));
    action->setWhatsThis(i18n("Import contacts from a LDap file."));
    exportActionList << action;

    action = ac->addAction(QStringLiteral("file_export_ldap40"));
    action->setWhatsThis(i18n("Export contacts to a LDap 4.0 file."));
    action->setText(i18n("Export LDap 4.0..."));
    exportActionList << action;

    action = ac->addAction(QStringLiteral("file_export_ldap30"));
    action->setText(i18n("Export LDap 3.0..."));
    action->setWhatsThis(i18n("Export contacts to a LDap 3.0 file."));
    exportActionList << action;

    action = ac->addAction(QStringLiteral("file_export_ldap21"));
    action->setText(i18n("Export LDap 2.1..."));
    action->setWhatsThis(i18n("Export contacts to a LDap 2.1 file."));
    exportActionList << action;
    setExportActions(exportActionList);
#endif
}

void LDapImportExportPluginInterface::exec()
{
}
