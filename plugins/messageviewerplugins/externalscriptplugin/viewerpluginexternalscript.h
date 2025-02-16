/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <MessageViewer/ViewerPlugin>
#include <QVariant>
namespace MessageViewer
{
class ViewerPluginExternalscript : public MessageViewer::ViewerPlugin
{
    Q_OBJECT
public:
    explicit ViewerPluginExternalscript(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>());

    ViewerPluginInterface *createView(QWidget *parent, KActionCollection *ac) override;
    [[nodiscard]] QString viewerPluginName() const override;

    void showConfigureDialog(QWidget *parent) override;
    [[nodiscard]] bool hasConfigureDialog() const override;
};
}
