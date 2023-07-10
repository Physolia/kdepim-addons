/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class KDescendantsProxyModel;
class QAbstractProxyModel;
namespace KSieveCore
{
class SieveImapAccountSettings;
}
class SelectImapLineEditCompleterModel : public QObject
{
    Q_OBJECT
public:
    explicit SelectImapLineEditCompleterModel(const KSieveCore::SieveImapAccountSettings &account, QObject *parent = nullptr);
    ~SelectImapLineEditCompleterModel() override;

    Q_REQUIRED_RESULT QAbstractProxyModel *completerModel() const;

private:
    KDescendantsProxyModel *const mFlatProxy;
};
