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
#include "grammarresultwidget.h"

#include <KPIMTextEdit/RichTextComposer>

#include <KToggleAction>
#include <KLocalizedString>
#include <KActionCollection>

#include <QDebug>
#include <QHBoxLayout>


GrammalecteInterface::GrammalecteInterface(KActionCollection *ac, QWidget *parent)
    : MessageComposer::PluginEditorGrammarCustomToolsViewInterface(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    mGrammarResultWidget = new GrammarResultWidget(this);

//TODO ????
    //connect(mTranslatorWidget, &PimCommon::TranslatorWidget::toolsWasClosed, this, &TranslatorView::toolsWasClosed);

    layout->addWidget(mGrammarResultWidget);
    createAction(ac);
}

GrammalecteInterface::~GrammalecteInterface()
{

}

void GrammalecteInterface::slotActivateGrammalecte(bool state)
{
    if (state) {
        mGrammarResultWidget->show();
        Q_EMIT activateView(this);
    } else {
        mGrammarResultWidget->hide();
        Q_EMIT activateView(nullptr);
    }
}

void GrammalecteInterface::setText(const QString &text)
{
    //TOOD ???? using it ?
    Q_UNUSED(text);
    if (richTextEditor()) {
        mGrammarResultWidget->setText(richTextEditor()->toPlainText());
    } else {
        //Add qdebug categories
        qWarning() << "RichTextEditor not setted";
    }
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
        //ac->setDefaultShortcut(mAction, QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_T));
    }
    mAction->setChecked(false);
}