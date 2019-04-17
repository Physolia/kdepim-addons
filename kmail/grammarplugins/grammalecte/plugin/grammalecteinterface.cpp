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

#include "grammalecteinterface.h"
#include "grammalecteresultwidget.h"
#include "grammalecteplugin_debug.h"

#include <KPIMTextEdit/RichTextComposer>

#include <KToggleAction>
#include <KLocalizedString>
#include <KActionCollection>

#include <QHBoxLayout>
#include <QTextBlock>

GrammalecteInterface::GrammalecteInterface(KActionCollection *ac, QWidget *parent)
    : MessageComposer::PluginEditorGrammarCustomToolsViewInterface(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    mGrammarResultWidget = new GrammalecteResultWidget(this);
    connect(mGrammarResultWidget, &GrammalecteResultWidget::replaceText, this, &GrammalecteInterface::slotReplaceText);
    connect(mGrammarResultWidget, &GrammalecteResultWidget::checkAgain, this, &GrammalecteInterface::checkAgain);
    connect(mGrammarResultWidget, &GrammalecteResultWidget::closeChecker, this, &GrammalecteInterface::closeChecker);

    layout->addWidget(mGrammarResultWidget);
    createAction(ac);
}

GrammalecteInterface::~GrammalecteInterface()
{
}

void GrammalecteInterface::slotReplaceText(const MessageComposer::PluginGrammarAction &act)
{
    if (richTextEditor()) {
        QTextBlock block = richTextEditor()->document()->findBlockByNumber(act.blockId() - 1);
        if (block.isValid()) {
            QTextCursor cur(block);
            const int position = cur.position() + act.start();
            cur.setPosition(position);
            cur.setPosition(position + act.length(), QTextCursor::KeepAnchor);
            cur.insertText(act.replacement());
        }
    }
}

void GrammalecteInterface::slotActivateGrammalecte(bool state)
{
    if (state) {
        mGrammarResultWidget->show();
        checkAgain();
        Q_EMIT activateView(this);
    } else {
        closeChecker();
    }
}

void GrammalecteInterface::closeChecker()
{
    mGrammarResultWidget->hide();
    Q_EMIT activateView(nullptr);
}

KToggleAction *GrammalecteInterface::action() const
{
    return mAction;
}

void GrammalecteInterface::createAction(KActionCollection *ac)
{
    mAction = new KToggleAction(i18n("&Check Grammar (Grammalecte)"), this);
    connect(mAction, &KToggleAction::triggered, this, &GrammalecteInterface::slotActivateGrammalecte);
    if (ac) {
        ac->addAction(QStringLiteral("checkgrammar-grammalecte"), mAction);
    }
    mAction->setChecked(false);
}

void GrammalecteInterface::checkAgain()
{
    if (richTextEditor()) {
        mGrammarResultWidget->setText(richTextEditor()->toPlainText());
        mGrammarResultWidget->checkGrammar();
    } else {
        qCWarning(KMAIL_EDITOR_GRAMMALECTE_PLUGIN_LOG) << "richtexteditor not setted, it's a bug";
    }
}
