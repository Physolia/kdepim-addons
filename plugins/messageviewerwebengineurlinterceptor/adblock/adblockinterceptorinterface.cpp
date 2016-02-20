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

#include "adblockinterceptorinterface.h"
#include <QtWebEngineCore/qwebengineurlrequestinfo.h>

AdblockInterceptorInterface::AdblockInterceptorInterface(QObject *parent)
    : MessageViewer::MailNetworkPluginUrlInterceptorInterface(parent)
{

}

AdblockInterceptorInterface::~AdblockInterceptorInterface()
{

}

void AdblockInterceptorInterface::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    //TODO enable/disable
    const QString urlString = info.requestUrl().toString().toLower();
    const QString host = info.requestUrl().host().toLower();
    const QString scheme = info.requestUrl().scheme().toLower();
    //TODO
}
