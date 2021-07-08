/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "confirmbeforedeletingcreaterulewidgettest.h"
#include "confirmbeforedeletingcreaterulewidget.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(ConfirmBeforeDeletingCreateRuleWidgetTest)
ConfirmBeforeDeletingCreateRuleWidgetTest::ConfirmBeforeDeletingCreateRuleWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ConfirmBeforeDeletingCreateRuleWidgetTest::shouldHaveDefaultValues()
{
    ConfirmBeforeDeletingCreateRuleWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mPatternLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mPatternLineEdit"));
    QVERIFY(mPatternLineEdit);
    QVERIFY(mPatternLineEdit->text().isEmpty());

    auto mRuleTypeComboBox = w.findChild<QComboBox *>(QStringLiteral("mRuleTypeComboBox"));
    QVERIFY(mRuleTypeComboBox);
    QVERIFY(mRuleTypeComboBox->count() > 0);

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(!label->text().isEmpty());
}
