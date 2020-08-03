/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "expireaccounttrashfolderconfigwidget.h"
#include <MailCommon/CollectionExpiryWidget>
#include <AkonadiCore/AgentManager>
#include <akonadi/kmime/specialmailcollections.h>
#include <QVBoxLayout>

ExpireAccountTrashFolderConfigWidget::ExpireAccountTrashFolderConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);


     mCollectionExpiryWidget = new MailCommon::CollectionExpiryWidget(this);
     mCollectionExpiryWidget->setObjectName(QStringLiteral("mCollectionExpiryWidget"));
     connect(mCollectionExpiryWidget, &MailCommon::CollectionExpiryWidget::saveAndExpireRequested, this, &ExpireAccountTrashFolderConfigWidget::slotSaveAndExpireRequested);
     mainLayout->addWidget(mCollectionExpiryWidget);
}

ExpireAccountTrashFolderConfigWidget::~ExpireAccountTrashFolderConfigWidget()
{

}

void ExpireAccountTrashFolderConfigWidget::save()
{
    const MailCommon::CollectionExpirySettings settings = mCollectionExpiryWidget->settings();
    const Akonadi::AgentInstance::List agents = Akonadi::AgentManager::self()->instances();
    for (const Akonadi::AgentInstance &agent : agents) {
        Akonadi::Collection trashCol = Akonadi::SpecialMailCollections::self()->collection(Akonadi::SpecialMailCollections::Trash, agent);
        if (trashCol.isValid()) {
            mCollectionExpiryWidget->save(settings, trashCol, true, true);
        }
    }
}

void ExpireAccountTrashFolderConfigWidget::slotSaveAndExpireRequested()
{
    qWarning() << " void ExpireAccountTrashFolderConfigWidget::slotSaveAndExpireRequested() not implement yet";
    //mCollectionExpiryWidget->save(collection, saveSettings, expireNow);
    //TODO
}
