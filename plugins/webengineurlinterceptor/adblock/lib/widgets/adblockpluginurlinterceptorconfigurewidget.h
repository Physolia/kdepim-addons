/*
   Copyright (C) 2016 Montel Laurent <montel@kde.org>

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

#ifndef ADBLOCKPLUGINURLINTERCEPTORCONFIGUREWIDGET_H
#define ADBLOCKPLUGINURLINTERCEPTORCONFIGUREWIDGET_H

#include <WebEngineViewer/NetworkPluginUrlInterceptorConfigureWidget>
#include "adblocklib_export.h"

namespace AdBlock
{
class AdBlockSettingWidget;
class ADBLOCKLIB_EXPORT AdblockPluginUrlInterceptorConfigureWidget : public WebEngineViewer::NetworkPluginUrlInterceptorConfigureWidget
{
    Q_OBJECT
public:
    explicit AdblockPluginUrlInterceptorConfigureWidget(QWidget *parent = nullptr);
    ~AdblockPluginUrlInterceptorConfigureWidget();

    void loadSettings() Q_DECL_OVERRIDE;
    void saveSettings() Q_DECL_OVERRIDE;
    void resetSettings() Q_DECL_OVERRIDE;

private:
    AdBlock::AdBlockSettingWidget *mConfigureWidget;

};
}
#endif // ADBLOCKPLUGINURLINTERCEPTORCONFIGUREWIDGET_H
