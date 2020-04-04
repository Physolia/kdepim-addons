/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

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

#ifndef QUICKTEXTPLUGINEDITOR_H
#define QUICKTEXTPLUGINEDITOR_H

#include <MessageComposer/PluginEditor>

#include <MessageComposer/PluginEditorInterface>
#include <QVariant>

class QuickTextPluginEditor : public MessageComposer::PluginEditor
{
    Q_OBJECT
public:
    explicit QuickTextPluginEditor(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>());
    ~QuickTextPluginEditor() override;

    MessageComposer::PluginEditorInterface *createInterface(QObject *parent = nullptr) override;
    Q_REQUIRED_RESULT bool hasStatusBarSupport() const override;

    Q_REQUIRED_RESULT bool hasConfigureDialog() const override;
    void showConfigureDialog(QWidget *parent) override;

    Q_REQUIRED_RESULT bool canProcessKeyEvent() const override;
};

#endif
