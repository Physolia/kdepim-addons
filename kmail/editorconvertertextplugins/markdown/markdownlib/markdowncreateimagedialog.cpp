/*
    SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markdowncreateimagedialog.h"
#include "markdowncreateimagewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myMarkdownCreateImageDialogGroupName[] = "MarkdownCreateImageDialog";
}

MarkdownCreateImageDialog::MarkdownCreateImageDialog(QWidget *parent)
    : QDialog(parent)
    , mMarkdownCreateImageWidget(new MarkdownCreateImageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Image"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mMarkdownCreateImageWidget->setObjectName(QStringLiteral("markdowncreateimagewidget"));

    mainLayout->addWidget(mMarkdownCreateImageWidget);

    auto box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    box->setObjectName(QStringLiteral("buttonbox"));
    mOkButton = box->button(QDialogButtonBox::Ok);
    mOkButton->setObjectName(QStringLiteral("okbutton"));
    mOkButton->setEnabled(false);
    mainLayout->addWidget(box);
    connect(box, &QDialogButtonBox::accepted, this, &MarkdownCreateImageDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &MarkdownCreateImageDialog::reject);
    connect(mMarkdownCreateImageWidget, &MarkdownCreateImageWidget::enabledOkButton, this, &MarkdownCreateImageDialog::slotEnabledOkButton);
    readConfig();
}

MarkdownCreateImageDialog::~MarkdownCreateImageDialog()
{
    writeConfig();
}

void MarkdownCreateImageDialog::slotEnabledOkButton(bool enabled)
{
    mOkButton->setEnabled(enabled);
}

QString MarkdownCreateImageDialog::linkStr() const
{
    return mMarkdownCreateImageWidget->linkStr();
}

void MarkdownCreateImageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myMarkdownCreateImageDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void MarkdownCreateImageDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 200));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myMarkdownCreateImageDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

#include "moc_markdowncreateimagedialog.cpp"
