/*
   Copyright (C) 2015-2016 Montel Laurent <montel@kde.org>

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

#include "antivirusplugininterfacetest.h"
#include "../antivirusplugininterface.h"
#include <KActionCollection>
#include <QTest>
#include <QAction>
#include <QSignalSpy>

AntiVirusPluginInterfaceTest::AntiVirusPluginInterfaceTest(QObject *parent)
    : QObject(parent)
{

}

AntiVirusPluginInterfaceTest::~AntiVirusPluginInterfaceTest()
{

}

void AntiVirusPluginInterfaceTest::shouldHaveDefaultValue()
{
    AntiVirusPluginInterface interface;
    QVERIFY(!interface.parentWidget());
    QVERIFY(!interface.actionType().action());
}

void AntiVirusPluginInterfaceTest::shouldCreateAction()
{
    AntiVirusPluginInterface interface;
    interface.createAction(new KActionCollection(this));
    QVERIFY(interface.actionType().action());
}

void AntiVirusPluginInterfaceTest::shouldEmitActivatedSignal()
{
    AntiVirusPluginInterface interface;
    interface.createAction(new KActionCollection(this));

    QSignalSpy spy1(&interface, SIGNAL(emitPluginActivated(PimCommon::GenericPluginInterface*)));
    interface.actionType().action()->trigger();
    QCOMPARE(spy1.count(), 1);
}

QTEST_MAIN(AntiVirusPluginInterfaceTest)
