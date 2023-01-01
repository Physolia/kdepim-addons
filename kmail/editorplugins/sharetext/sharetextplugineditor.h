/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <MessageComposer/PluginEditor>

#include <MessageComposer/PluginEditorInterface>
#include <QVariant>

class ShareTextPluginEditor : public MessageComposer::PluginEditor
{
    Q_OBJECT
public:
    explicit ShareTextPluginEditor(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>());
    ~ShareTextPluginEditor() override;

    MessageComposer::PluginEditorInterface *createInterface(QObject *parent = nullptr) override;
    Q_REQUIRED_RESULT bool hasPopupMenuSupport() const override;
};
