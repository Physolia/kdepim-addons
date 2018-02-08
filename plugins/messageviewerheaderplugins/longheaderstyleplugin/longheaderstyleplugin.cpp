/*
   Copyright (C) 2015-2018 Montel Laurent <montel@kde.org>

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

#include "longheaderstyleinterface.h"
#include "longheaderstyleplugin.h"
#include "messageviewer/plainheaderstyle.h"

#include "messageviewer/richheaderstrategy.h"
#include <kpluginfactory.h>
#include "kcoreaddons_kdepim_compat.h"
using namespace MessageViewer;

K_PLUGIN_CLASS_WITH_JSON(LongHeaderStylePlugin, "messageviewer_longheaderstyleplugin.json")

LongHeaderStylePlugin::LongHeaderStylePlugin(QObject *parent, const QList<QVariant> &)
    : MessageViewer::HeaderStylePlugin(parent)
    , mHeaderStyle(new PlainHeaderStyle)
    , mHeaderStrategy(new RichHeaderStrategy)
{
}

LongHeaderStylePlugin::~LongHeaderStylePlugin()
{
    delete mHeaderStyle;
    delete mHeaderStrategy;
}

HeaderStyle *LongHeaderStylePlugin::headerStyle() const
{
    return mHeaderStyle;
}

HeaderStrategy *LongHeaderStylePlugin::headerStrategy() const
{
    return mHeaderStrategy;
}

HeaderStyleInterface *LongHeaderStylePlugin::createView(KActionMenu *menu, QActionGroup *actionGroup, KActionCollection *ac, QObject *parent)
{
    MessageViewer::HeaderStyleInterface *view = new MessageViewer::LongHeaderStyleInterface(this, parent);
    if (ac) {
        view->createAction(menu, actionGroup, ac);
    }
    return view;
}

QString LongHeaderStylePlugin::name() const
{
    return QStringLiteral("long-header");
}

#include "longheaderstyleplugin.moc"
