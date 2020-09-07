/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef QUICKTEXT_QUICKTEXTTREEWIDGET_H
#define QUICKTEXT_QUICKTEXTTREEWIDGET_H

#include <QTreeView>
#include "quicktext_export.h"
class QContextMenuEvent;

class QuicktextManager;
class QUICKTEXT_EXPORT QuicktextTreeWidget : public QTreeView
{
    Q_OBJECT
public:
    explicit QuicktextTreeWidget(QuicktextManager *manager, QWidget *parent = nullptr);
    ~QuicktextTreeWidget() override;

    QuicktextManager *snippetsManager() const;

protected:
    void contextMenuEvent(QContextMenuEvent *) override;
    void dropEvent(QDropEvent *) override;

Q_SIGNALS:
    void insertSnippetText(const QString &str);

    void addSnippet();
    void editSnippet();
    void addSnippetGroup();
    void editSnippetGroup();

private:
    void deleteSnippet();
    void deleteSnippetGroup();
    void selectionWasChanged();
    QuicktextManager *const mSnippetsManager;
    QAction *mAddSnippetAction = nullptr;
    QAction *mEditSnippetAction = nullptr;
    QAction *mDeleteSnippetAction = nullptr;
    QAction *mAddSnippetGroupAction = nullptr;
    QAction *mEditSnippetGroupAction = nullptr;
    QAction *mDeleteSnippetGroupAction = nullptr;
};

#endif
