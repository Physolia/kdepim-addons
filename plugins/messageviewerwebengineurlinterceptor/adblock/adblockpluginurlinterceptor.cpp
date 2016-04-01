/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "adblockpluginurlinterceptor.h"
#include "adblockinterceptorinterface.h"
#include "../lib/widgets/adblockpluginurlinterceptorconfigurewidget.h"
#include "lib/adblockmanager.h"
#include <MessageViewer/NetworkPluginUrlInterceptorInterface>
#include <kpluginfactory.h>
#include <KLocalizedString>

K_PLUGIN_FACTORY_WITH_JSON(AdblockPluginUrlInterceptorFactory, "messageviewer_adblockurlinterceptor.json", registerPlugin<AdblockPluginUrlInterceptor>();)
AdblockPluginUrlInterceptor::AdblockPluginUrlInterceptor(QObject *parent, const QList<QVariant> &)
    : MessageViewer::NetworkPluginUrlInterceptor(parent)
{
}

AdblockPluginUrlInterceptor::~AdblockPluginUrlInterceptor()
{

}

MessageViewer::NetworkPluginUrlInterceptorInterface *AdblockPluginUrlInterceptor::createInterface(QWebEngineView *webEngine, QObject *parent)
{
    AdblockInterceptorInterface *adblockInterface = new AdblockInterceptorInterface(parent);
    adblockInterface->setWebEngineView(webEngine);
    return adblockInterface;
}

bool AdblockPluginUrlInterceptor::hasConfigureSupport() const
{
    return true;
}

MessageViewer::NetworkPluginUrlInterceptorConfigureWidgetSetting AdblockPluginUrlInterceptor::createConfigureWidget(QWidget *parent)
{
    MessageViewer::NetworkPluginUrlInterceptorConfigureWidgetSetting settings;
    settings.name = i18n("AdBlock");

    AdBlock::AdblockPluginUrlInterceptorConfigureWidget *configureWidget = new AdBlock::AdblockPluginUrlInterceptorConfigureWidget(parent);
    connect(configureWidget, &AdBlock::AdblockPluginUrlInterceptorConfigureWidget::configChanged, AdBlock::AdblockManager::self(), &AdBlock::AdblockManager::reloadConfig);
    settings.configureWidget = configureWidget;
    return settings;
}

#include "adblockpluginurlinterceptor.moc"
