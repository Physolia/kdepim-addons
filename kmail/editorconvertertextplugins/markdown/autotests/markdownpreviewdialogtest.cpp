/*
   Copyright (C) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "markdownpreviewdialogtest.h"
#include "markdownpreviewdialog.h"
#include "markdownpreviewwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MarkdownPreviewDialogTest)

MarkdownPreviewDialogTest::MarkdownPreviewDialogTest(QObject *parent)
    : QObject(parent)
{
}

void MarkdownPreviewDialogTest::shouldHaveDefaultValue()
{
    MarkdownPreviewDialog dlg;
    QVBoxLayout *mainLayout = dlg.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);

    MarkdownPreviewWidget *mPreviewWidget = dlg.findChild<MarkdownPreviewWidget *>(QStringLiteral("previewwidget"));
    QVERIFY(mPreviewWidget);

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);
}
