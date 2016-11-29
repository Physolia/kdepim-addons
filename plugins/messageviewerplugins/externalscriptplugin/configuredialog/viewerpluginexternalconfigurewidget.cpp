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

#include "viewerpluginexternalconfigurewidget.h"
#include "externalscriptplugin_debug.h"
#include "viewerpluginexternaleditdialog.h"
#include <QVBoxLayout>
#include <QStandardPaths>
#include <KLocalizedString>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <KMessageBox>
#include <QPointer>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <KDesktopFile>
#include <KConfigGroup>

class ViewerPluginExternalScriptItem : public QListWidgetItem
{
public:
    ViewerPluginExternalScriptItem(QListWidget *parent);

    void setScriptInfo(const ViewerPluginExternalScriptInfo &scriptInfo);
    ViewerPluginExternalScriptInfo scriptInfo() const;
private:
    ViewerPluginExternalScriptInfo mScriptInfo;
};

ViewerPluginExternalScriptItem::ViewerPluginExternalScriptItem(QListWidget *parent)
    : QListWidgetItem(parent)
{

}

void ViewerPluginExternalScriptItem::setScriptInfo(const ViewerPluginExternalScriptInfo &scriptInfo)
{
    mScriptInfo = scriptInfo;
    setText(mScriptInfo.name());
    setToolTip(mScriptInfo.description());
}

ViewerPluginExternalScriptInfo ViewerPluginExternalScriptItem::scriptInfo() const
{
    return mScriptInfo;
}

ViewerPluginExternalConfigureWidget::ViewerPluginExternalConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("layout"));
    mainLayout->setMargin(0);

    QLabel *lab = new QLabel(i18n("External Script:"), this);
    lab->setObjectName(QStringLiteral("lab"));
    mainLayout->addWidget(lab);

    QHBoxLayout *listLayout = new QHBoxLayout;
    mainLayout->addLayout(listLayout);

    mListExternal = new QListWidget(this);
    mListExternal->setObjectName(QStringLiteral("listexternal"));
    mListExternal->setSelectionMode(QAbstractItemView::SingleSelection);
    listLayout->addWidget(mListExternal);
    connect(mListExternal, &QListWidget::itemSelectionChanged, this, &ViewerPluginExternalConfigureWidget::updateButtons);
    connect(mListExternal, &QListWidget::itemDoubleClicked, this, &ViewerPluginExternalConfigureWidget::slotDoubleClicked);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    listLayout->addLayout(buttonLayout);

    mAddScript = new QPushButton(i18n("Add Script..."), this);
    connect(mAddScript, &QPushButton::clicked, this, &ViewerPluginExternalConfigureWidget::slotAddScript);
    mAddScript->setObjectName(QStringLiteral("addscript"));
    buttonLayout->addWidget(mAddScript);

    mModifyScript = new QPushButton(i18n("Modify Script..."), this);
    connect(mModifyScript, &QPushButton::clicked, this, &ViewerPluginExternalConfigureWidget::slotModifyScript);
    mModifyScript->setObjectName(QStringLiteral("modifyscript"));
    buttonLayout->addWidget(mModifyScript);

    mRemoveScript = new QPushButton(i18n("Remove Script"), this);
    connect(mRemoveScript, &QPushButton::clicked, this, &ViewerPluginExternalConfigureWidget::slotRemoveScript);
    mRemoveScript->setObjectName(QStringLiteral("removescript"));
    buttonLayout->addWidget(mRemoveScript);
    buttonLayout->addStretch(1);
    updateButtons();
}

ViewerPluginExternalConfigureWidget::~ViewerPluginExternalConfigureWidget()
{

}

void ViewerPluginExternalConfigureWidget::slotRemoveScript()
{
    QListWidgetItem *item = mListExternal->currentItem();
    if (item) {
        ViewerPluginExternalScriptItem *scriptItem = static_cast<ViewerPluginExternalScriptItem *>(item);
        if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Do you want to remove this script?"), i18n("Remove External Script"))) {
            mFilesToRemove.append(scriptItem->scriptInfo().fileName());
            delete mListExternal->takeItem(mListExternal->currentRow());
        }
    }
}

void ViewerPluginExternalConfigureWidget::slotModifyScript()
{
    QListWidgetItem *item = mListExternal->currentItem();
    if (item) {
        modifyScript(item);
    }
}

void ViewerPluginExternalConfigureWidget::slotAddScript()
{
    QStringList existingNames;
    const int numberOfElement(mListExternal->count());
    for (int i = 0; i < numberOfElement; ++i) {
        ViewerPluginExternalScriptItem *item = static_cast<ViewerPluginExternalScriptItem *>(mListExternal->item(i));
        existingNames << item->text();
    }
    QPointer<ViewerPluginExternalEditDialog> dlg = new ViewerPluginExternalEditDialog(this);
    dlg->setExistingsNames(existingNames);
    if (dlg->exec()) {
        ViewerPluginExternalScriptItem *item = new ViewerPluginExternalScriptItem(mListExternal);
        item->setScriptInfo(dlg->scriptInfo());
    }
    delete dlg;
}

void ViewerPluginExternalConfigureWidget::load()
{
    ViewerPluginExternalScriptsLoadJob job;
    const QStringList lst = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("messageviewerplugins/"), QStandardPaths::LocateDirectory);
    job.setExternalScriptsDirectories(lst);
    job.start();
    const QVector<ViewerPluginExternalScriptInfo> scriptInfos = job.scriptInfos();
    fillScriptInfo(scriptInfos);
}

void ViewerPluginExternalConfigureWidget::fillScriptInfo(const QVector<ViewerPluginExternalScriptInfo> &scriptInfos)
{
    Q_FOREACH (const ViewerPluginExternalScriptInfo &script, scriptInfos) {
        ViewerPluginExternalScriptItem *item = new ViewerPluginExternalScriptItem(mListExternal);
        item->setScriptInfo(script);
    }
}

void ViewerPluginExternalConfigureWidget::save()
{
    Q_FOREACH (const QString &path, mFilesToRemove) {
        QFile f(path);
        if (!f.remove()) {
            qCWarning(EXTERNALSCRIPTPLUGIN_LOG) << " Impossible to delete " << path;
        }
    }
    const QString writablePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/messageviewerplugins/");
    const int numberOfElement(mListExternal->count());
    for (int i = 0; i < numberOfElement; ++i) {
        ViewerPluginExternalScriptItem *item = static_cast<ViewerPluginExternalScriptItem *>(mListExternal->item(i));
        const ViewerPluginExternalScriptInfo &scriptInfo = item->scriptInfo();
        QString filenamepath = scriptInfo.fileName();
        if (filenamepath.isEmpty()) {
            filenamepath = writablePath + QStringLiteral("%1.desktop").arg(scriptInfo.name());
            int fileIndex = 1;
            while(QFileInfo(filenamepath).exists()) {
                filenamepath = writablePath + QStringLiteral("%1-%2.desktop").arg(scriptInfo.name()).arg(fileIndex);
                fileIndex++;
            }
        }
        KDesktopFile desktopFile(filenamepath);
        desktopFile.desktopGroup().writeEntry(QStringLiteral("Name"), scriptInfo.name());
        desktopFile.desktopGroup().writeEntry(QStringLiteral("Description"), scriptInfo.description());
        desktopFile.desktopGroup().writeEntry(QStringLiteral("Executable"), scriptInfo.executable());
        desktopFile.desktopGroup().writeEntry(QStringLiteral("CommandLine"), scriptInfo.commandLine());
        desktopFile.desktopGroup().sync();
    }
}

void ViewerPluginExternalConfigureWidget::updateButtons()
{
    QListWidgetItem *item = mListExternal->currentItem();
    if (item) {
        ViewerPluginExternalScriptItem *scriptItem = static_cast<ViewerPluginExternalScriptItem *>(item);
        const bool isReadOnly = scriptItem->scriptInfo().isReadOnly();
        mRemoveScript->setEnabled(!isReadOnly);
        mModifyScript->setEnabled(!isReadOnly);
    } else {
        mRemoveScript->setEnabled(false);
        mModifyScript->setEnabled(false);
    }
}

void ViewerPluginExternalConfigureWidget::slotDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        modifyScript(item);
    }
}

void ViewerPluginExternalConfigureWidget::modifyScript(QListWidgetItem *item)
{
    ViewerPluginExternalScriptItem *scriptItem = static_cast<ViewerPluginExternalScriptItem *>(item);
    if (!scriptItem->scriptInfo().isReadOnly()) {
        QPointer<ViewerPluginExternalEditDialog> dlg = new ViewerPluginExternalEditDialog(this);
        dlg->setScriptInfo(scriptItem->scriptInfo());
        if (dlg->exec()) {
            scriptItem->setScriptInfo(dlg->scriptInfo());
        }
        delete dlg;
    }
}
