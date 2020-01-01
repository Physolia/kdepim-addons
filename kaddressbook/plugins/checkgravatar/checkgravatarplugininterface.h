/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#ifndef CHECKGRAVATARPLUGININTERFACE_H
#define CHECKGRAVATARPLUGININTERFACE_H

#include <PimCommonAkonadi/GenericPluginInterface>

class CheckGravatarPluginInterface : public PimCommon::GenericPluginInterface
{
public:
    explicit CheckGravatarPluginInterface(QObject *parent = nullptr);
    ~CheckGravatarPluginInterface() override;

    void exec() override;
    void createAction(KActionCollection *ac) override;
    void setCurrentItems(const Akonadi::Item::List &items) override;
    Q_REQUIRED_RESULT PimCommon::GenericPluginInterface::RequireTypes requires() const override;
    void updateActions(int numberOfSelectedItems, int numberOfSelectedCollections) override;

private:
    void slotActivated();
    void slotModifyContactFinished(KJob *job);
    Akonadi::Item::List mListItems;
    QAction *mAction = nullptr;
};

#endif // CHECKGRAVATARPLUGININTERFACE_H
