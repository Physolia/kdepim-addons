/*
   Copyright (C) 2016-2018 Montel Laurent <montel@kde.org>

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

#include "adblockmanager.h"
#include "adblockmatcher.h"
#include "adblocksubscription.h"
#include "adblockinterceptor_debug.h"
#include "globalsettings_webengineurlinterceptoradblock.h"
#include <KConfig>
#include <KConfigGroup>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>
#include <QUrl>

using namespace AdBlock;

AdblockManager *AdblockManager::self()
{
    static AdblockManager s_self;
    return &s_self;
}

AdblockManager::AdblockManager(QObject *parent)
    : QObject(parent)
{
    mAdBlockMatcher = new AdBlockMatcher(this);
    reloadConfig();
}

AdblockManager::~AdblockManager()
{
    qDeleteAll(mSubscriptions);
}

void AdblockManager::reloadConfig()
{
    loadSubscriptions();
    const bool enabled = AdBlock::AdBlockSettings::self()->adBlockEnabled();
    Q_EMIT enabledChanged(enabled);
}

void AdblockManager::loadSubscriptions()
{
    //Clear subscription
    qDeleteAll(mSubscriptions);
    mSubscriptions.clear();
    QDir adblockDir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/adblock"));
    // Create if necessary
    if (!adblockDir.exists()) {
        QDir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)).mkdir(QStringLiteral("adblock"));
    }

    for ( const QString &fileName : adblockDir.entryList(QStringList() << QLatin1String("*.txt"), QDir::Files)) {
        if (fileName == QLatin1String("customlist.txt")) {
            continue;
        }

        const QString absolutePath = adblockDir.absoluteFilePath(fileName);
        QFile file(absolutePath);
        if (!file.open(QFile::ReadOnly)) {
            continue;
        }

        QTextStream textStream(&file);
        textStream.setCodec("UTF-8");
        QString title = textStream.readLine(1024).remove(QLatin1String("Title: "));
        QUrl url = QUrl(textStream.readLine(1024).remove(QLatin1String("Url: ")));

        if (title.isEmpty() || !url.isValid()) {
            qCWarning(ADBLOCKINTERCEPTOR_LOG) << "AdBlockManager: Invalid subscription file" << absolutePath;
            continue;
        }

        AdBlockSubscription *subscription = new AdBlockSubscription(title, this);
        subscription->setUrl(url);
        subscription->setFilePath(absolutePath);

        mSubscriptions.append(subscription);
    }

    AdBlockCustomList *customList = new AdBlockCustomList(this);
    mSubscriptions.append(customList);
    // Load all subscriptions
    foreach (AdBlockSubscription* subscription, mSubscriptions) {
        subscription->loadSubscription(mDisabledRules);
    }
    if (!mSubscriptions.isEmpty()) {
        KConfig config(QStringLiteral("AdBlockadblockrc"));
        KConfigGroup general = config.group(QStringLiteral("General"));
        const QDateTime lastUpdate = general.readEntry(QStringLiteral("lastUpdate"), QDateTime());
        if (lastUpdate.addDays(AdBlock::AdBlockSettings::self()->adBlockUpdateInterval()) < QDateTime::currentDateTime()) {
            QTimer::singleShot(1000 * 60, this, &AdblockManager::updateAllSubscriptions);
        }
    }
}

void AdblockManager::save()
{
    foreach (AdBlockSubscription *subscription, mSubscriptions) {
        subscription->saveSubscription();
    }
}

QStringList AdblockManager::disabledRules() const
{
    return mDisabledRules;
}

void AdblockManager::removeDisabledRule(const QString &filter)
{
     mDisabledRules.removeOne(filter);
}

void AdblockManager::addDisabledRule(const QString &filter)
{
    mDisabledRules.append(filter);
}

bool AdblockManager::isEnabled() const
{
    return mAdBlockMatcher->isEnabled();
}

bool AdblockManager::interceptRequest(const QWebEngineUrlRequestInfo &info)
{
    bool result = false;
    QUrl url = info.requestUrl();
    const QString urlString = url.toString().toLower();
    const QString host = url.host().toLower();
    const QString scheme = url.scheme().toLower();

    if (!canRunOnScheme(scheme)) {
        return result;
    }

    const AdBlockRule *blockedRule = mAdBlockMatcher->match(info, host, urlString);
    if (blockedRule) {
        result = true;
        //TODO
    }
    return result;
}

bool AdblockManager::canRunOnScheme(const QString &scheme) const
{
    return scheme != QLatin1String("file");
}

QList<AdBlockSubscription *> AdblockManager::subscriptions() const
{
    return mSubscriptions;
}

void AdblockManager::updateAllSubscriptions()
{
    foreach (AdBlockSubscription* subscription, mSubscriptions) {
        subscription->updateSubscription();
    }

    KConfig config(QStringLiteral("AdBlockadblockrc"));
    KConfigGroup general = config.group(QStringLiteral("General"));
    general.writeEntry(QStringLiteral("lastUpdate"), QDateTime::currentDateTime());
}
