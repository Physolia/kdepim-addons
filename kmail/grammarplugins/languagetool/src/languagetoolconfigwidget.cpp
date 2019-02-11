/*
   Copyright (C) 2019 Montel Laurent <montel@kde.org>

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

#include "languagetoolconfigwidget.h"

#include <KMessageBox>
#include <KLocalizedString>

#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QScrollArea>
#include <QFormLayout>
#include <QVariant>

#include <KUrlRequester>
LanguagetoolConfigWidget::LanguagetoolConfigWidget(QWidget *parent, bool disableMessageBox)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setMargin(0);
}

LanguagetoolConfigWidget::~LanguagetoolConfigWidget()
{
    saveSettings();
}

void LanguagetoolConfigWidget::loadSettings()
{
}

void LanguagetoolConfigWidget::saveSettings()
{
}