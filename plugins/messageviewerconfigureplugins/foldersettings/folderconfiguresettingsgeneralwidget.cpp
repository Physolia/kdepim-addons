/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "folderconfiguresettingsgeneralwidget.h"
#include <MailCommon/CollectionGeneralWidget>
#include <QVBoxLayout>

FolderConfigureSettingsGeneralWidget::FolderConfigureSettingsGeneralWidget(QWidget *parent)
    : FolderConfigureSettingsPageBase(parent)
{
    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);
    mCollectionGeneralWidget = new MailCommon::CollectionGeneralWidget(this);
    mCollectionGeneralWidget->setObjectName(QStringLiteral("mCollectionGeneralWidget"));
    mainLayout->addWidget(mCollectionGeneralWidget);
    addMainWidget(mainWidget);
    mMainLayout->addStretch();
}

FolderConfigureSettingsGeneralWidget::~FolderConfigureSettingsGeneralWidget()
{
}

void FolderConfigureSettingsGeneralWidget::save(Akonadi::Collection &col)
{
    if (wasModified()) {
        mCollectionGeneralWidget->save(col);
    }
}
