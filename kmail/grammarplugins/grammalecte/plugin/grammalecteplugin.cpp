/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "grammalecteplugin.h"
#include "grammalecteinterface.h"
#include "grammalecteconfigdialog.h"
#include "grammalectemanager.h"
#include <PimCommon/CustomToolsWidgetng>
#include <KLocalizedString>
#include <KPluginFactory>

#include <QPointer>

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
    auto *view = new GrammalecteInterface(ac, parent);

    connect(view, &GrammalecteInterface::toolsWasClosed, parent, &PimCommon::CustomToolsWidgetNg::slotToolsWasClosed);
    connect(view, &GrammalecteInterface::insertText, parent, &PimCommon::CustomToolsWidgetNg::insertText);
    connect(view, &GrammalecteInterface::activateView, parent, &PimCommon::CustomToolsWidgetNg::slotActivateView);
    return view;
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
    QPointer<GrammalecteConfigDialog> dlg = new GrammalecteConfigDialog(parent);
    if (dlg->exec()) {
        GrammalecteManager::self()->loadSettings();
    }
    delete dlg;
}

#include "grammalecteplugin.moc"
