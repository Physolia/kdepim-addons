/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "folderconfiguretreewidget.h"
#include "foldersettingfilterproxymodel.h"
#include <KCheckableProxyModel>
#include <KLocalizedString>
#include <MailCommon/FolderTreeView>
#include <MailCommon/FolderTreeWidget>
#include <QPushButton>
#include <QVBoxLayout>

FolderConfigureTreeWidget::FolderConfigureTreeWidget(QWidget *parent)
    : QWidget(parent)
    , mFolderSettingFilterProxyModel(new FolderSettingFilterProxyModel(this))
    , mFolderTreeWidget(new MailCommon::FolderTreeWidget(this,
                                                         nullptr,
                                                         MailCommon::FolderTreeWidget::TreeViewOptions(MailCommon::FolderTreeWidget::UseDistinctSelectionModel
                                                                                                       | MailCommon::FolderTreeWidget::HideStatistics
                                                                                                       | MailCommon::FolderTreeWidget::HideHeaderViewMenu)))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mFolderTreeWidget->setObjectName(QStringLiteral("foldertreewidget"));
    mFolderTreeWidget->folderTreeView()->setDragEnabled(false);
    mFolderTreeWidget->folderTreeView()->setSelectionMode(QAbstractItemView::ExtendedSelection);

    auto ftv = mFolderTreeWidget->folderTreeView();
    auto sourceModel = ftv->model();
    auto selectionModel = mFolderTreeWidget->selectionModel();

    auto checkable = new KCheckableProxyModel(this);
    checkable->setObjectName(QStringLiteral("checkable"));
    checkable->setSourceModel(sourceModel);
    checkable->setSelectionModel(selectionModel);

    mFolderSettingFilterProxyModel->setObjectName(QStringLiteral("folderSettingFilterProxyModel"));
    mFolderSettingFilterProxyModel->setSourceModel(checkable);

    ftv->setModel(mFolderSettingFilterProxyModel);
    ftv->expandAll();
    mainLayout->addWidget(mFolderTreeWidget);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->setObjectName(QStringLiteral("buttonLayout"));
    mainLayout->addLayout(buttonLayout);

    mSelectFolder = new QPushButton(i18n("Select"), this);
    mSelectFolder->setObjectName(QStringLiteral("selectFolder"));
    buttonLayout->addWidget(mSelectFolder);
    mSelectFolder->setEnabled(false);
    connect(mSelectFolder, &QPushButton::clicked, this, [this]() {
        changeFolderSelection(true);
    });

    mUnSelectFolder = new QPushButton(i18n("Unselect"), this);
    mUnSelectFolder->setObjectName(QStringLiteral("unSelectFolder"));
    mUnSelectFolder->setEnabled(false);
    buttonLayout->addWidget(mUnSelectFolder);
    connect(mUnSelectFolder, &QPushButton::clicked, this, [this]() {
        changeFolderSelection(false);
    });
    connect(mFolderTreeWidget->folderTreeView()->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &FolderConfigureTreeWidget::slotSelectionChanged);
}

FolderConfigureTreeWidget::~FolderConfigureTreeWidget() = default;

void FolderConfigureTreeWidget::slotSelectionChanged()
{
    const QModelIndexList indexes = mFolderTreeWidget->folderTreeView()->selectionModel()->selection().indexes();
    bool checkedFolder = false;
    bool uncheckedFolder = false;
    for (const QModelIndex &selectedIndex : indexes) {
        bool b = mFolderSettingFilterProxyModel->data(selectedIndex, Qt::CheckStateRole).toBool();
        if (b) {
            checkedFolder = true;
        } else {
            uncheckedFolder = true;
        }
    }
    mSelectFolder->setEnabled(uncheckedFolder);
    mUnSelectFolder->setEnabled(checkedFolder);
}

void FolderConfigureTreeWidget::changeFolderSelection(bool select)
{
    const QModelIndexList indexes = mFolderTreeWidget->folderTreeView()->selectionModel()->selectedIndexes();
    for (const QModelIndex &selectedIndex : indexes) {
        mFolderSettingFilterProxyModel->setData(selectedIndex, select ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }
    slotSelectionChanged();
}

Akonadi::Collection::List FolderConfigureTreeWidget::listCollections() const
{
    return mFolderSettingFilterProxyModel->listCollections();
}

#include "moc_folderconfiguretreewidget.cpp"
