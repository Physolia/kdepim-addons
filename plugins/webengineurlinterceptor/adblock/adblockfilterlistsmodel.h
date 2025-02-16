/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "adblockfilter.h"
#include <QAbstractListModel>
class AdblockFilterListsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AdblockFilterRoles {
        UrlRole,
        LastColumn = UrlRole,
    };
    Q_ENUM(AdblockFilterRoles)

    explicit AdblockFilterListsModel(QObject *parent = nullptr);
    ~AdblockFilterListsModel() override;
    [[nodiscard]] QList<AdblockFilter> adblockFilter() const;
    void setAdblockFilter(const QList<AdblockFilter> &newAdblockFilter);

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void insertList(const AdblockFilter &filter);
    void removeList(const QString &identifier);

private:
    QList<AdblockFilter> mAdblockFilter;
};
