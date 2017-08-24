/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>
   based on subscriptiondialog from kdepim-runtime

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

#include "selectimapfolderwidget.h"
#include "selectimaploadfoldersjob.h"
#include "selectimapfoldertreeview.h"
#include "selectimapfoldermodel.h"
#include "imapfoldercompletionplugin_debug.h"
#include <KMessageBox>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLineEdit>
#include <QPainter>
#include <QInputDialog>
#include <QSortFilterProxyModel>

#include <KLocalizedString>
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
SearchFilterProxyModel::SearchFilterProxyModel(QObject *parent)
    : KRecursiveFilterProxyModel(parent)
{
}

void SearchFilterProxyModel::setSearchPattern(const QString &pattern)
{
    if (mPattern != pattern) {
        mPattern = pattern;
        invalidate();
    }
}

bool SearchFilterProxyModel::acceptRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!mPattern.isEmpty()) {
        const QString text = sourceIndex.data(Qt::DisplayRole).toString();
        return text.contains(mPattern, Qt::CaseInsensitive);
    } else {
        return true;
    }
}
#endif

SelectImapFolderWidget::SelectImapFolderWidget(const KSieveUi::SieveImapAccountSettings &account, QWidget *parent)
    : QWidget(parent)
    , mAccount(account)
{
    mModel = SelectImapFolderModel::self()->folderModel(mAccount);
    connect(SelectImapFolderModel::self(), &SelectImapFolderModel::modelLoaded, this, &SelectImapFolderWidget::slotModelLoaded);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setMargin(0);

    mSearchLineEdit = new QLineEdit(this);
    mSearchLineEdit->setObjectName(QStringLiteral("searchline"));
    mSearchLineEdit->setClearButtonEnabled(true);
    mSearchLineEdit->setPlaceholderText(i18n("Search..."));
    mainLayout->addWidget(mSearchLineEdit);

    mTreeView = new SelectImapFolderTreeView(this);
    mTreeView->setObjectName(QStringLiteral("treeview"));
    mTreeView->header()->hide();
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
    mFilter = new SearchFilterProxyModel(this);
    mFilter->setSourceModel(mModel);
#else
    mFilter = new QSortFilterProxyModel(this);
    mFilter->setRecursiveFiltering(true);
    mFilter->setSourceModel(mModel);
#endif

    mTreeView->setModel(mFilter);
    connect(mTreeView, &QTreeView::doubleClicked, this, &SelectImapFolderWidget::slotDoubleClicked);
    mainLayout->addWidget(mTreeView);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &SelectImapFolderWidget::slotSearchPattern);
    connect(mTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &SelectImapFolderWidget::slotCurrentChanged);
}

SelectImapFolderWidget::~SelectImapFolderWidget()
{
}

void SelectImapFolderWidget::slotModelLoaded(QStandardItemModel *model, bool success)
{
    if (mModel == model) {
        mTreeView->setStatus(success ? SelectImapFolderTreeView::Success : SelectImapFolderTreeView::Failed);
    }
}

void SelectImapFolderWidget::slotSearchPattern(const QString &pattern)
{
    mTreeView->expandAll();
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
    mFilter->setSearchPattern(pattern);
#else
    mFilter->setFilterFixedString(pattern);
#endif
}

void SelectImapFolderWidget::slotDoubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        Q_EMIT folderSelected();
    }
}

QString SelectImapFolderWidget::selectedFolderName() const
{
    QString currentPath;
    const QModelIndex index = mTreeView->currentIndex();
    if (index.isValid()) {
        currentPath = index.data(SelectImapLoadFoldersJob::PathRole).toString();
    }
    return currentPath;
}

void SelectImapFolderWidget::createFolder()
{
    const QModelIndex index = mTreeView->currentIndex();
    if (index.isValid()) {
        const QString name = QInputDialog::getText(this, i18n("Create Folder"), i18n("Folder Name:"));
        if (!name.trimmed().isEmpty()) {
            const QString currentPath = index.data(SelectImapLoadFoldersJob::PathRole).toString();
            if (name.contains(QLatin1Char('/'))) {
                KMessageBox::error(this, i18n("Slashes are not allowed in folder names."), i18n("Create Folder"));
                return;
            }
            //TODO more check for folder name ?
            SelectImapFolderModel::self()->createNewFolder(mAccount, currentPath + QLatin1Char('/') + name);
        }
    }
}

void SelectImapFolderWidget::slotCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    Q_EMIT folderIsSelected(current.isValid());
}
