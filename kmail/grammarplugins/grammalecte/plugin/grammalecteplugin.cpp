/*
   Copyright (C) 2019 Montel Laurent <montel@kde.org>

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

#include "grammalecteplugin.h"
#include <KLocalizedString>
#include <kpluginfactory.h>
K_PLUGIN_CLASS_WITH_JSON(GrammalectePlugin, "kmail_grammalecteplugin.json")
GrammalectePlugin::GrammalectePlugin(QObject *parent, const QList<QVariant> &)
    : PimCommon::CustomToolsPlugin(parent)
{

}

GrammalectePlugin::~GrammalectePlugin()
{

}


PimCommon::CustomToolsViewInterface *GrammalectePlugin::createView(KActionCollection *ac, PimCommon::CustomToolsWidgetNg *parent)
{
    //TODO
    return {};
}

QString GrammalectePlugin::customToolName() const
{
    return i18n("Grammalecte Plugin");
}

bool GrammalectePlugin::hasConfigureDialog() const
{
    return true;
}

void GrammalectePlugin::showConfigureDialog(QWidget *parent)
{
    //TODO
}

QString GrammalectePlugin::description() const
{
    //TODO
    return {};
}
#include "grammalecteplugin.moc"