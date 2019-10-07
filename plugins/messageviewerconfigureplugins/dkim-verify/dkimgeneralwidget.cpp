/*
   Copyright (C) 2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#include "dkimgeneralwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>

DKIMGeneralWidget::DKIMGeneralWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mEnableDkimSupport = new QCheckBox(i18n("Enable DKIM Support"));
    mEnableDkimSupport->setObjectName(QStringLiteral("enableDkimSupport"));
    mEnableDkimSupport->setChecked(false);
    mainLayout->addWidget(mEnableDkimSupport);

    mSaveResult = new QCheckBox(i18n("Save DKIM Result"));
    mSaveResult->setObjectName(QStringLiteral("mSaveResult"));
    mSaveResult->setChecked(false);
    mainLayout->addWidget(mSaveResult);

    mSaveKey = new QCheckBox(i18n("Save Key"));
    mSaveKey->setObjectName(QStringLiteral("mSaveKey"));
    mSaveKey->setChecked(false);
    mainLayout->addWidget(mSaveKey);
}

DKIMGeneralWidget::~DKIMGeneralWidget()
{

}

void DKIMGeneralWidget::loadSettings()
{
    //TODO
}

void DKIMGeneralWidget::saveSettings()
{
    //TODO
}

void DKIMGeneralWidget::resetSettings()
{
    //TODO
}
