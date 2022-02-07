/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "openurlwithconfigurecreatewidgettest.h"
#include "openurlwithconfigurecreatewidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(OpenUrlWithConfigureCreateWidgetTest)
OpenUrlWithConfigureCreateWidgetTest::OpenUrlWithConfigureCreateWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OpenUrlWithConfigureCreateWidgetTest::shouldHaveDefaultValues()
{
    OpenUrlWithConfigureCreateWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mServerName = w.findChild<QLineEdit *>(QStringLiteral("mServerName"));
    QVERIFY(mServerName);
    QVERIFY(mServerName->text().isEmpty());

    auto mCommand = w.findChild<QLineEdit *>(QStringLiteral("mCommand"));
    QVERIFY(mCommand);
    QVERIFY(mCommand->text().isEmpty());

    auto formatHelp = w.findChild<QLabel *>(QStringLiteral("formatHelp"));
    QVERIFY(formatHelp);
    QCOMPARE(formatHelp->contextMenuPolicy(), Qt::NoContextMenu);
}
