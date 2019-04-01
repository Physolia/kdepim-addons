/*
   Copyright (C) 2014-2019 Montel Laurent <montel@kde.org>

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

#include "mergecontactwidgetlisttest.h"
#include "../manualmerge/mergecontactwidgetlist.h"
#include <QTest>
#include <AkonadiCore/Item>
using namespace KContacts;

MergeContactWidgetListTest::MergeContactWidgetListTest(QObject *parent)
    : QObject(parent)
{
}

MergeContactWidgetListTest::~MergeContactWidgetListTest()
{
}

void MergeContactWidgetListTest::shouldHaveDefaultValue()
{
    KABMergeContacts::MergeContactWidgetList w;
    QCOMPARE(w.count(), 0);
}

void MergeContactWidgetListTest::shouldCleanListWhenSetItems()
{
    KABMergeContacts::MergeContactWidgetList w;
    Akonadi::Item::List lst;
    lst << Akonadi::Item(42);
    lst << Akonadi::Item(42);
    w.fillListContact(lst);
    //We don't have KABC::Address
    QCOMPARE(w.count(), 0);

    Addressee address;
    Akonadi::Item item;
    address.setName(QStringLiteral("foo1"));
    item.setPayload<Addressee>(address);
    lst << item;
    w.fillListContact(lst);
    QCOMPARE(w.count(), 1);

    //it must clear;
    w.fillListContact(lst);
    QCOMPARE(w.count(), 1);
}

QTEST_MAIN(MergeContactWidgetListTest)
