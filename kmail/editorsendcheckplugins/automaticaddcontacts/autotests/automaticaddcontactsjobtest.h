/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AutomaticAddContactsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit AutomaticAddContactsJobTest(QObject *parent = nullptr);
    ~AutomaticAddContactsJobTest() override;
};

