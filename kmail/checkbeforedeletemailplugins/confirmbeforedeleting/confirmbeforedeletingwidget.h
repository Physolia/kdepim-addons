/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "confirmbeforedeleting_private_export.h"
#include <QWidget>
class QTreeWidget;
class KMAILCONFIRMBEFOREDELETING_TESTS_EXPORT ConfirmBeforeDeletingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfirmBeforeDeletingWidget(QWidget *parent = nullptr);
    ~ConfirmBeforeDeletingWidget() override;

private:
    void slotCustomContextMenuRequested(const QPoint &p);
    void slotRemoveRule();
    QTreeWidget *const mTreeWidget;
};