/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SelectImapCreateJobTest : public QObject
{
    Q_OBJECT
public:
    explicit SelectImapCreateJobTest(QObject *parent = nullptr);
    ~SelectImapCreateJobTest() override;
};
