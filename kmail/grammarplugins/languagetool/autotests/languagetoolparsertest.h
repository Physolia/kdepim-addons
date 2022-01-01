/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class LanguageToolParserTest : public QObject
{
    Q_OBJECT
public:
    explicit LanguageToolParserTest(QObject *parent = nullptr);
    ~LanguageToolParserTest() override = default;
private Q_SLOTS:
    void shouldParseJson();
    void shouldParseJson_data();
};

