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
    QAction *action = ac->addAction(QStringLiteral("file_import_ldap"));
    action->setText(i18n("Import From LDAP server..."));
    action->setWhatsThis(i18n("Import contacts from an LDAP server."));
    setImportActions(QList<QAction *>() << action);
    connect(action, &QAction::triggered, this, &LDapImportExportPluginInterface::slotImportLdap);
}

void LDapImportExportPluginInterface::exec()
{
    switch(mImportExportAction) {
    case Import:
        break;
    case Export:
        break;
    }
    //TODO
}

void LDapImportExportPluginInterface::slotImportLdap()
{
    mImportExportAction = Import;
    Q_EMIT emitPluginActivated(this);
}
