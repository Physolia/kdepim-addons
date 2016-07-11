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

#ifndef AUTOMATICADDCONTACTSPLUGIN_H
#define AUTOMATICADDCONTACTSPLUGIN_H

#include <MessageComposer/PluginEditorCheckBeforeSend>

class AutomaticAddContactsPlugin : public MessageComposer::PluginEditorCheckBeforeSend
{
    Q_OBJECT
public:
    explicit AutomaticAddContactsPlugin(QObject *parent = Q_NULLPTR, const QList<QVariant> & = {});
    ~AutomaticAddContactsPlugin();

    MessageComposer::PluginEditorCheckBeforeSendInterface *createInterface(QObject *parent) Q_DECL_OVERRIDE;
    bool hasConfigureSupport() const Q_DECL_OVERRIDE;
    MessageComposer::PluginEditorCheckBeforeSendConfigureWidgetSetting createConfigureWidget(QWidget *parent) const Q_DECL_OVERRIDE;
};

#endif // AUTOMATICADDCONTACTSPLUGIN_H
