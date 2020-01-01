/*
   Copyright (C) 2014-2020 Laurent Montel <montel@kde.org>

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
#include "mergecontactinfowidget.h"

#include "KaddressbookGrantlee/GrantleeContactViewer"

#include <AkonadiCore/Item>

#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLabel>

using namespace KABMergeContacts;

MergeContactInfoWidget::MergeContactInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    mStackWidget = new QStackedWidget;
    mStackWidget->setObjectName(QStringLiteral("stackedwidget"));

    mContactViewer = new KAddressBookGrantlee::GrantleeContactViewer;
    mContactViewer->setObjectName(QStringLiteral("contactwidget"));
    mContactViewer->setForceDisableQRCode(true);

    mStackWidget->addWidget(mContactViewer);

    mNoContactSelected = new QLabel;
    mNoContactSelected->setObjectName(QStringLiteral("nocontact"));
    mStackWidget->addWidget(mNoContactSelected);

    lay->addWidget(mStackWidget);
    mStackWidget->setCurrentWidget(mNoContactSelected);
}

MergeContactInfoWidget::~MergeContactInfoWidget()
{
}

void MergeContactInfoWidget::setContact(const Akonadi::Item &item)
{
    if (item.isValid()) {
        mContactViewer->setContact(item);
        mStackWidget->setCurrentWidget(mContactViewer);
    } else {
        mStackWidget->setCurrentWidget(mNoContactSelected);
    }
}
