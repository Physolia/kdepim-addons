/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "grammalecteconfigdialogtest.h"
#include "grammalecteconfigdialog.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
#include <grammalecteconfigwidget.h>
QTEST_MAIN(GrammalecteConfigDialogTest)
GrammalecteConfigDialogTest::GrammalecteConfigDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void GrammalecteConfigDialogTest::shouldHaveDefaultValue()
{
    GrammalecteConfigDialog w(nullptr, true);
    QVERIFY(!w.windowTitle().isEmpty());

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);

    GrammalecteConfigWidget *mConfigWidget = w.findChild<GrammalecteConfigWidget *>(QStringLiteral("configwidget"));
    QVERIFY(mConfigWidget);

    QDialogButtonBox *box = w.findChild<QDialogButtonBox *>(QStringLiteral("box"));
    QVERIFY(box);
}
