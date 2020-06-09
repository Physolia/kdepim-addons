/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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

#ifndef ADBLOCKBLOCKABLEITEMSDIALOG_H
#define ADBLOCKBLOCKABLEITEMSDIALOG_H

#include <QDialog>
#include "adblocklib_export.h"
namespace AdBlock {
class AdBlockResult;
class AdBlockBlockableItemsWidget;
class ADBLOCKLIB_EXPORT AdBlockBlockableItemsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdBlockBlockableItemsDialog(QWidget *parent = nullptr);
    ~AdBlockBlockableItemsDialog() override;

    void saveFilters();
    void setAdblockResult(const QVector<AdBlock::AdBlockResult> &result);

    void accept() override;
private:
    void writeConfig();
    void readConfig();
    AdBlockBlockableItemsWidget *mBlockableItems = nullptr;
};
}
#endif // ADBLOCKBLOCKABLEITEMSDIALOG_H
