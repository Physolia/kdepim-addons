/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef QUICKTEXTPLUGINEDITORINTERFACE_H
#define QUICKTEXTPLUGINEDITORINTERFACE_H

#include <MessageComposer/PluginEditorInterface>
#include <KPIMTextEdit/SelectSpecialCharDialog>
#include <MailCommon/SnippetsModel>
#include <QPointer>
#include <QTextCursor>
class QuickTextPluginEditorInterface : public MessageComposer::PluginEditorInterface
{
    Q_OBJECT
public:
    explicit QuickTextPluginEditorInterface(QObject *parent = nullptr);
    ~QuickTextPluginEditorInterface() override;

    void createAction(KActionCollection *ac) override;
    void exec() override;
    Q_REQUIRED_RESULT bool processProcessKeyEvent(QKeyEvent *event) override;

    void reloadConfig();
private:
    void updateSnippetsInfo();
    void slotActivated();
    void selectPreviousWord(QTextCursor &cursor, int cursorPosition);
    QVector<MailCommon::SnippetsInfo> mSnippetsInfo;
    MailCommon::SnippetsModel *const mModel;
};

#endif
