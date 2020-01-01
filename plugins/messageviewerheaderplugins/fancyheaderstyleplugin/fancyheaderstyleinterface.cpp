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

#include "fancyheaderstyleinterface.h"
#include <KToggleAction>
#include <KLocalizedString>
#include <KActionCollection>

using namespace MessageViewer;
FancyHeaderStyleInterface::FancyHeaderStyleInterface(MessageViewer::HeaderStylePlugin *plugin, QObject *parent)
    : MessageViewer::HeaderStyleInterface(plugin, parent)
{
}

FancyHeaderStyleInterface::~FancyHeaderStyleInterface()
{
}

void FancyHeaderStyleInterface::createAction(KActionMenu *menu, QActionGroup *actionGroup, KActionCollection *ac)
{
    KToggleAction *act = new KToggleAction(i18nc("View->headers->", "&Fancy Headers"), this);
    ac->addAction(QStringLiteral("view_headers_fancy"), act);
    connect(act, &KToggleAction::triggered, this, &FancyHeaderStyleInterface::slotStyleChanged);
    addHelpTextAction(act, i18n("Show the list of headers in a fancy format"));
    mAction.append(act);
    addActionToMenu(menu, actionGroup);
}

void FancyHeaderStyleInterface::activateAction()
{
    mAction.at(0)->setChecked(true);
}
