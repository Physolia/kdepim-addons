/*
   Copyright (C) 2015-2019 Montel Laurent <montel@kde.org>

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
#include "antispamplugin.h"
#include "antispamplugininterface.h"
#include <kpluginfactory.h>

K_PLUGIN_CLASS_WITH_JSON(AntiSpamPlugin, "kmail_antispamplugin.json")

AntiSpamPlugin::AntiSpamPlugin(QObject *parent, const QList<QVariant> &)
    : PimCommon::GenericPlugin(parent)
{
}

AntiSpamPlugin::~AntiSpamPlugin()
{
}

PimCommon::GenericPluginInterface *AntiSpamPlugin::createInterface(QObject *parent)
{
    AntiSpamPluginInterface *interface = new AntiSpamPluginInterface(parent);
    return interface;
}

#include "antispamplugin.moc"
