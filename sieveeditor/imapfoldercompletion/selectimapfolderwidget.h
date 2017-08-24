/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>

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

#ifndef SELECTIMAPFOLDERWIDGET_H
#define SELECTIMAPFOLDERWIDGET_H

#include <QWidget>
#include <QTreeView>

#include <KSieveUi/SieveImapAccountSettings>

class QStandardItemModel;
class QLineEdit;
class SelectImapFolderTreeView;
class QSortFilterProxyModel;
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
#include <KRecursiveFilterProxyModel>
class SearchFilterProxyModel : public KRecursiveFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchFilterProxyModel(QObject *parent = nullptr);

public Q_SLOTS:
    void setSearchPattern(const QString &pattern);

protected:
    bool acceptRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString mPattern;
};
#endif

class SelectImapFolderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectImapFolderWidget(const KSieveUi::SieveImapAccountSettings &account, QWidget *parent = nullptr);
    ~SelectImapFolderWidget();

    QString selectedFolderName() const;
    void createFolder();

Q_SIGNALS:
    void folderSelected();
    void folderIsSelected(bool selected);

private Q_SLOTS:
    void slotModelLoaded(QStandardItemModel *model, bool success);

private:
    void slotDoubleClicked(const QModelIndex &index);
    void slotSearchPattern(const QString &pattern);
    void slotCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

    KSieveUi::SieveImapAccountSettings mAccount;
    QLineEdit *mSearchLineEdit;
    SelectImapFolderTreeView *mTreeView;
    QStandardItemModel *mModel;
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
    SearchFilterProxyModel *mFilter;
#else
    QSortFilterProxyModel *mFilter;
#endif
};

#endif // SELECTIMAPFOLDERWIDGET_H
