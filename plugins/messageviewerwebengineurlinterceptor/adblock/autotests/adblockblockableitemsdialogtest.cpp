/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "adblockblockableitemsdialogtest.h"
#include "../lib/adblockblockableitemsdialog.h"
#include "../lib/adblockblockableitemswidget.h"

#include <QDialogButtonBox>
#include <QTest>
using namespace AdBlock;
AdBlockBlockableItemsDialogTest::AdBlockBlockableItemsDialogTest(QObject *parent)
    : QObject(parent)
{

}

AdBlockBlockableItemsDialogTest::~AdBlockBlockableItemsDialogTest()
{

}

void AdBlockBlockableItemsDialogTest::shouldHaveDefaultValue()
{
    AdBlockBlockableItemsDialog w;
    QDialogButtonBox *buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    AdBlockBlockableItemsWidget *mBlockableItems = w.findChild<AdBlockBlockableItemsWidget *>(QStringLiteral("blockableitemswidget"));
    QVERIFY(mBlockableItems);

}

QTEST_MAIN(AdBlockBlockableItemsDialogTest)
