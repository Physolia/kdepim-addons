/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "viewerplugincreatenotetest.h"
#include "../createnoteplugin/viewerplugincreatenote.h"
#include <KActionCollection>
#include <MessageViewer/ViewerPluginInterface>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTest>

namespace MessageViewer
{
extern MESSAGEVIEWER_EXPORT QAbstractItemModel *_k_noteEditStubModel;
}

ViewerPluginCreateNoteTest::ViewerPluginCreateNoteTest(QObject *parent)
    : QObject(parent)
{
    auto model = new QStandardItemModel;
    MessageViewer::_k_noteEditStubModel = model;
}

ViewerPluginCreateNoteTest::~ViewerPluginCreateNoteTest()
{
}

void ViewerPluginCreateNoteTest::shouldCreateAction()
{
    MessageViewer::ViewerPluginCreatenote *note = new MessageViewer::ViewerPluginCreatenote(this);
    QVERIFY(!note->viewerPluginName().isEmpty());
    QWidget *parent = new QWidget(nullptr);
    parent->setLayout(new QHBoxLayout);
    MessageViewer::ViewerPluginInterface *interface = note->createView(parent, new KActionCollection(this));
    QVERIFY(interface);
    QVERIFY(!interface->actions().isEmpty());
}

void ViewerPluginCreateNoteTest::shouldShowWidget()
{
    MessageViewer::ViewerPluginCreatenote *note = new MessageViewer::ViewerPluginCreatenote(this);
    QWidget *parent = new QWidget(nullptr);
    parent->setLayout(new QHBoxLayout);
    MessageViewer::ViewerPluginInterface *interface = note->createView(parent, new KActionCollection(this));
    interface->execute();
    auto *createnotewidget = parent->findChild<QWidget *>(QStringLiteral("noteedit"));
    QVERIFY(createnotewidget);
    QCOMPARE(createnotewidget->isHidden(), false);
}

QTEST_MAIN(ViewerPluginCreateNoteTest)
