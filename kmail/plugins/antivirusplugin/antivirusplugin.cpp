/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

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

#include "antivirusplugin.h"
#include "antivirusplugininterface.h"
#include <KLocalizedString>
#include <KToggleAction>
#include <kpluginfactory.h>

K_PLUGIN_FACTORY_WITH_JSON(KMailAntiSpamPluginFactory, "kmail_antivirusplugin.json", registerPlugin<AntiVirusPlugin>();)

AntiVirusPlugin::AntiVirusPlugin(QObject *parent, const QList<QVariant> &)
    : PimCommon::GenericPlugin(parent)
{

}

AntiVirusPlugin::~AntiVirusPlugin()
{

}

PimCommon::GenericPluginInterface *AntiVirusPlugin::createInterface(KActionCollection *ac, QWidget *parent)
{
    AntiVirusPluginInterface *interface = new AntiVirusPluginInterface(this);
    interface->setParentWidget(parent);
    interface->createAction(ac);
    return interface;
}

#include "antivirusplugin.moc"
