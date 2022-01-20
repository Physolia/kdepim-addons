/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "openurlwithconfigurecreatewidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
OpenUrlWithConfigureCreateWidget::OpenUrlWithConfigureCreateWidget(QWidget *parent)
    : QWidget{parent}
    , mServerName(new QLineEdit(this))
    , mCommand(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins(QMargins());

    auto label = new QLabel(i18n("Server Name:"), this);
    mainLayout->addWidget(label);

    mServerName->setObjectName(QStringLiteral("mServerName"));
    mainLayout->addWidget(mServerName);

    mCommand->setObjectName(QStringLiteral("mCommand"));
    mainLayout->addWidget(mCommand);
}

OpenUrlWithConfigureCreateWidget::~OpenUrlWithConfigureCreateWidget()
{
}

OpenUrlWithConfigureCreateWidget::OpenUrlWithInfo OpenUrlWithConfigureCreateWidget::info() const
{
    OpenUrlWithConfigureCreateWidget::OpenUrlWithInfo info;
    info.url = mServerName->text().trimmed();
    info.command = mCommand->text().trimmed();
    return info;
}

void OpenUrlWithConfigureCreateWidget::setInfo(const OpenUrlWithInfo &i)
{
    mServerName->setText(i.url);
    mCommand->setText(i.command);
}
