/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "folderconfiguresettingsplugin.h"
#include "folderconfiguresettingsdialog.h"
#include <KPluginFactory>
#include <QPointer>

K_PLUGIN_CLASS_WITH_JSON(FolderConfigureSettingsPlugin, "messageviewer_folderconfiguresettingsplugin.json")

FolderConfigureSettingsPlugin::FolderConfigureSettingsPlugin(QObject *parent, const QList<QVariant> &)
    : MessageViewer::MessageViewerConfigureSettingsPlugin(parent)
{
}

FolderConfigureSettingsPlugin::~FolderConfigureSettingsPlugin()
{
}

void FolderConfigureSettingsPlugin::showConfigureDialog(QWidget *parent)
{
    QPointer<FolderConfigureSettingsDialog> dlg = new FolderConfigureSettingsDialog(parent);
    dlg->exec();
    delete dlg;
}

#include "folderconfiguresettingsplugin.moc"
