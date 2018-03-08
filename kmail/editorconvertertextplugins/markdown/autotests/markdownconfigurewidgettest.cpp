/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

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

#include "markdownconfigurewidgettest.h"
#include "../markdownconfigurewidget.h"
#include <QHBoxLayout>
#include <QTest>

QTEST_MAIN(MarkdownConfigureWidgetTest)
MarkdownConfigureWidgetTest::MarkdownConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{

}

void MarkdownConfigureWidgetTest::shouldHaveDefaultValue()
{
    MarkdownConfigureWidget w;
    QHBoxLayout *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->margin(), 0);
}
