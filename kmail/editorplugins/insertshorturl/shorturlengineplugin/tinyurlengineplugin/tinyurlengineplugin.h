/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "../shorturlengineplugin.h"

#include <QVariant>

class TinyUrlEnginePlugin : public ShortUrlEnginePlugin
{
    Q_OBJECT
public:
    explicit TinyUrlEnginePlugin(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>());
    ~TinyUrlEnginePlugin() override;

    ShortUrlEngineInterface *createInterface(QObject *parent) override;
    QString engineName() const override;
};
