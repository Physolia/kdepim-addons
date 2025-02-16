/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "adblockplugin_private_export.h"
#include <QDialog>
class AdblockFilterWidget;
class LIBADBLOCKPLUGIN_TESTS_EXPORT AdblockFilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdblockFilterDialog(QWidget *parent = nullptr);
    ~AdblockFilterDialog() override;

private:
    AdblockFilterWidget *const mAdblockFilterWidget;
};
