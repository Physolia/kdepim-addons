/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <PimCommon/GenericPlugin>

#include <PimCommonAkonadi/GenericPluginInterface>

#include <QVariant>

class AntiSpamPlugin : public PimCommon::GenericPlugin
{
    Q_OBJECT
public:
    explicit AntiSpamPlugin(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>());
    ~AntiSpamPlugin() override;

    PimCommon::GenericPluginInterface *createInterface(QObject *parent = nullptr) override;
};
