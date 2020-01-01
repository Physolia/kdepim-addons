/*
    Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "markdowncreatelinkwidgettest.h"
#include "markdowncreatelinkwidget.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(MarkdownCreateLinkWidgetTest)

MarkdownCreateLinkWidgetTest::MarkdownCreateLinkWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MarkdownCreateLinkWidgetTest::shouldHaveDefaultValue()
{
    MarkdownCreateLinkWidget w;

    QFormLayout *mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QLineEdit *mTitle = w.findChild<QLineEdit *>(QStringLiteral("title"));
    QVERIFY(mTitle);
    QVERIFY(mTitle->text().isEmpty());
    QVERIFY(mTitle->isClearButtonEnabled());

    QLineEdit *mLink = w.findChild<QLineEdit *>(QStringLiteral("link"));
    QVERIFY(mLink);
    QVERIFY(mLink->text().isEmpty());
    QVERIFY(mLink->isClearButtonEnabled());
}

void MarkdownCreateLinkWidgetTest::shouldGenerateLink()
{
    MarkdownCreateLinkWidget w;
    QLineEdit *mTitle = w.findChild<QLineEdit *>(QStringLiteral("title"));
    QLineEdit *mLink = w.findChild<QLineEdit *>(QStringLiteral("link"));
    mLink->setText(QStringLiteral("http://www.kde.org"));
    mTitle->setText(QStringLiteral("TITLE"));
    QCOMPARE(w.linkStr(), QStringLiteral("[TITLE](http://www.kde.org)"));
}

void MarkdownCreateLinkWidgetTest::shouldEmitSignal()
{
    MarkdownCreateLinkWidget w;
    QSignalSpy spy(&w, &MarkdownCreateLinkWidget::enabledOkButton);
    QLineEdit *mTitle = w.findChild<QLineEdit *>(QStringLiteral("title"));
    QLineEdit *mLink = w.findChild<QLineEdit *>(QStringLiteral("link"));
    mTitle->setText(QStringLiteral("foo"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<bool>(), false);
    spy.clear();

    mLink->setText(QStringLiteral("dd"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<bool>(), true);
}
