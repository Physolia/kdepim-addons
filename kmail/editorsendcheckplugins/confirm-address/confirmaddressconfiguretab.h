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


#ifndef CONFIRMADDRESSCONFIGURETAB_H
#define CONFIRMADDRESSCONFIGURETAB_H

#include <QWidget>
class QTabWidget;
class KConfigGroup;
class ConfirmAddressConfigureTabWidget;
class ConfirmAddressConfigureTab : public QWidget
{
    Q_OBJECT
public:
    explicit ConfirmAddressConfigureTab(QWidget *parent = Q_NULLPTR);
    ~ConfirmAddressConfigureTab();

    void loadSettings(const KConfigGroup &grp);
    void saveSettings(KConfigGroup &grp);
    void resetSettings();

Q_SIGNALS:
    void configureChanged();

private:
    void initTab();
    QTabWidget *mTabWidget;
    QList<ConfirmAddressConfigureTabWidget *> mListTabWidget;
};

#endif // CONFIRMADDRESSCONFIGURETAB_H
