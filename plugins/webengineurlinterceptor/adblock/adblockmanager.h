/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "adblockfilter.h"
#include "libadblockplugin_export.h"
#include <QList>
#include <QObject>
#include <QWebEngineUrlRequestInfo>

#include <adblock.rs.h>
#include <future>
#include <optional>
class AdblockListsManager;
class LIBADBLOCKPLUGIN_EXPORT AdblockManager : public QObject
{
    Q_OBJECT
public:
    explicit AdblockManager(QObject *parent = nullptr);
    ~AdblockManager() override;

    static AdblockManager *self();

    [[nodiscard]] QList<AdblockFilter> adblockFilterLists() const;
    void setAdblockFilterLists(const QList<AdblockFilter> &newAdblockFilterLists);

    [[nodiscard]] bool interceptRequest(QWebEngineUrlRequestInfo &info);
Q_SIGNALS:
    void enabledChanged(bool enabled);
    void adblockInitialized();

private:
    void reloadConfig();
    [[nodiscard]] QString adblockCacheLocation() const;

    /// If an adblock cache is found, loads it, otherwise creates a new adblock
    /// from the current filter lists.
    rust::Box<Adblock> createOrRestoreAdblock();

    std::future<rust::Box<Adblock>> mAdblockInitFuture;
    std::optional<rust::Box<Adblock>> mAdblock;
    QList<AdblockFilter> mAdblockFilterLists;
    AdblockListsManager *const mAdblockListManager;
};

extern "C" {
void q_cdebug_adblock(const char *message);
}
