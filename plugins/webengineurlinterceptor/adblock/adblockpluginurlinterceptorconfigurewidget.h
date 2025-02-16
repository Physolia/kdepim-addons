/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "adblockplugin_private_export.h"
#include <WebEngineViewer/NetworkPluginUrlInterceptorConfigureWidget>

class AdblockFilterWidget;
class LIBADBLOCKPLUGIN_TESTS_EXPORT AdblockPluginUrlInterceptorConfigureWidget : public WebEngineViewer::NetworkPluginUrlInterceptorConfigureWidget
{
    Q_OBJECT
public:
    explicit AdblockPluginUrlInterceptorConfigureWidget(QWidget *parent = nullptr);
    ~AdblockPluginUrlInterceptorConfigureWidget() override;

    void loadSettings() override;
    void saveSettings() override;
    void resetSettings() override;

private:
    AdblockFilterWidget *const mAdblockFilterWidget;
};
