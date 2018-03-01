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


#include "markdownplugin.h"
#include <KLocalizedString>
#include <kpluginfactory.h>
#include "kcoreaddons_kdepim_compat.h"

K_PLUGIN_CLASS_WITH_JSON(MarkdownPlugin, "kmail_markdownplugin.json")

MarkdownPlugin::MarkdownPlugin(QObject *parent, const QList<QVariant> &)
    : MessageComposer::PluginEditorConvertText(parent)
{

}

MarkdownPlugin::~MarkdownPlugin()
{

}


MessageComposer::PluginEditorConverttextInterface *MarkdownPlugin::createInterface(QObject *parent)
{
    //TODO
    return nullptr;
}


bool MarkdownPlugin::hasConfigureDialog() const
{
    return false;
}

void MarkdownPlugin::showConfigureDialog(QWidget *parent)
{
    //TODO
}

QString MarkdownPlugin::description() const
{
    //TODO
    return {};
}

#include "markdownplugin.moc"