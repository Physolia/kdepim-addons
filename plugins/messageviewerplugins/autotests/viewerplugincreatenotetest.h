/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VIEWERPLUGINCREATENOTETEST_H
#define VIEWERPLUGINCREATENOTETEST_H

#include <QObject>

class ViewerPluginCreateNoteTest : public QObject
{
    Q_OBJECT
public:
    explicit ViewerPluginCreateNoteTest(QObject *parent = nullptr);
    ~ViewerPluginCreateNoteTest();

private Q_SLOTS:
    void shouldCreateAction();
    void shouldShowWidget();
};

#endif // VIEWERPLUGINCREATENOTETEST_H
