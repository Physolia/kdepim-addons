/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTWINDOWCONTACTTEST_H
#define IMPORTWINDOWCONTACTTEST_H

#include <QObject>

class ImportWindowContactTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportWindowContactTest(QObject *parent = nullptr);
    ~ImportWindowContactTest() = default;
private Q_SLOTS:
    void shouldImportWindowContact_data();
    void shouldImportWindowContact();
};

#endif // IMPORTWINDOWCONTACTTEST_H
