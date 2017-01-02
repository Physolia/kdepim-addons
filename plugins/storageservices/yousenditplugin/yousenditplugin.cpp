/*
    Copyright (c) 2015-2017 Montel Laurent <montel@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "yousenditplugin.h"
#include <kpluginfactory.h>
#include <QUrl>
#include <KLocalizedString>

using namespace PimCommon;
K_PLUGIN_FACTORY_WITH_JSON(PimCommonYouSendItPluginFactory, "pimcommon_yousenditplugin.json", registerPlugin<YouSendItPlugin>();)

YouSendItPlugin::YouSendItPlugin(QObject *parent, const QList<QVariant> &)
    : PimCommon::StorageServicePlugin(parent)
{

}

YouSendItPlugin::~YouSendItPlugin()
{

}

QString YouSendItPlugin::youSendItApiKey() const
{
    return QString();
}

QString YouSendItPlugin::description() const
{
    return i18n("YouSendIt is a file hosting that offers cloud storage, file synchronization, and client software.");
}

QUrl YouSendItPlugin::serviceUrl() const
{
    return QUrl(QStringLiteral("https://www.yousendit.com/"));
}

QString YouSendItPlugin::storageServiceName() const
{
    return QStringLiteral("yousendit");
}

StorageServicePlugin::Capabilities YouSendItPlugin::capabilities() const
{
    StorageServicePlugin::Capabilities cap;
    cap |= AccountInfoCapability;
    cap |= UploadFileCapability;
    //cap |= DownloadFileCapability;
    cap |= CreateFolderCapability;
    cap |= DeleteFolderCapability;
    cap |= ListFolderCapability;
    //cap |= ShareLinkCapability;
    cap |= DeleteFileCapability;
    cap |= RenameFolderCapability;
    cap |= RenameFileCapabilitity;
    cap |= MoveFileCapability;
    cap |= MoveFolderCapability;

    //Can not be implemented.
    //cap |= CopyFileCapability;
    //cap |= CopyFolderCapability;

    return cap;
}

PimCommon::StorageServiceInterface *YouSendItPlugin::createStorageService(const QString &identifier)
{
    //TODO
    return nullptr;
}

#include "yousenditplugin.moc"
