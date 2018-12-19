/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

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

#include "gravatarconfiguresettingsplugin.h"
#include "gravatarconfiguresettingsplugindialog.h"
#include <kpluginfactory.h>

K_PLUGIN_CLASS_WITH_JSON(GravatarConfigureSettingsPlugin, "messageviewer_gravatarconfigplugin.json")

GravatarConfigureSettingsPlugin::GravatarConfigureSettingsPlugin(QObject *parent, const QList<QVariant> &)
    : MessageViewer::MessageViewerConfigureSettingsPlugin(parent)
{

}


GravatarConfigureSettingsPlugin::~GravatarConfigureSettingsPlugin()
{

}

void GravatarConfigureSettingsPlugin::showConfigureDialog(QWidget *parent)
{
    QPointer<GravatarConfigureSettingsPluginDialog> dlg = new GravatarConfigureSettingsPluginDialog(parent);
    dlg->exec();
    delete dlg;
}

#include "gravatarconfiguresettingsplugin.moc"