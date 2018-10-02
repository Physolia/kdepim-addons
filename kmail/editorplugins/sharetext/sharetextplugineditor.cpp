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

#include "sharetextplugineditor.h"
#include "sharetextplugineditorinterface.h"
#include <kpluginfactory.h>

K_PLUGIN_CLASS_WITH_JSON(ShareTextPluginEditor, "kmail_sharetexteditorplugin.json")

ShareTextPluginEditor::ShareTextPluginEditor(QObject *parent, const QList<QVariant> &)
    : MessageComposer::PluginEditor(parent)
{
}

ShareTextPluginEditor::~ShareTextPluginEditor()
{
}

bool ShareTextPluginEditor::hasPopupMenuSupport() const
{
    return false;
}

MessageComposer::PluginEditorInterface *ShareTextPluginEditor::createInterface(KActionCollection *ac, QObject *parent)
{
    ShareTextPluginEditorInterface *interface = new ShareTextPluginEditorInterface(parent);
    interface->createAction(ac);
    return interface;
}

#include "sharetextplugineditor.moc"