/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ADBLOCKPLUGINURLINTERCEPTORCONFIGUREWIDGET_H
#define ADBLOCKPLUGINURLINTERCEPTORCONFIGUREWIDGET_H

#include <WebEngineViewer/NetworkPluginUrlInterceptorConfigureWidget>
#include "adblocklib_export.h"

namespace AdBlock {
class AdBlockSettingWidget;
class ADBLOCKLIB_EXPORT AdblockPluginUrlInterceptorConfigureWidget : public WebEngineViewer::NetworkPluginUrlInterceptorConfigureWidget
{
    Q_OBJECT
public:
    explicit AdblockPluginUrlInterceptorConfigureWidget(QWidget *parent = nullptr);
    ~AdblockPluginUrlInterceptorConfigureWidget() override;

    void loadSettings() override;
    void saveSettings() override;
    void resetSettings() override;

private:
    AdBlock::AdBlockSettingWidget *mConfigureWidget = nullptr;
};
}
#endif // ADBLOCKPLUGINURLINTERCEPTORCONFIGUREWIDGET_H
