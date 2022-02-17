/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "openurlwith_private_export.h"
#include <QWidget>
class QListWidget;
class OpenUrlWithConfigureItem;
namespace MessageViewer
{
class OpenWithUrlInfo;
}
class LIBOPENURLWITHCONFIGURE_TESTS_EXPORT OpenUrlWithConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpenUrlWithConfigureWidget(QWidget *parent = nullptr);
    ~OpenUrlWithConfigureWidget() override;
    void loadSettings();
    void writeSettings();

private:
    void slotCustomContextMenuRequested(const QPoint &p);
    void slotAddRule();
    void slotEditRule();
    void slotRemoveRule();
    void displayText(const MessageViewer::OpenWithUrlInfo &r, OpenUrlWithConfigureItem *item);
    QListWidget *const mListWidget;
};
