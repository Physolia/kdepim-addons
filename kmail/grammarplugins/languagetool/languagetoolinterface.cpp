/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "languagetoolinterface.h"
#include "languagetoolplugin_debug.h"
#include <KMessageBox>
#include <KPIMTextEdit/RichTextComposer>
#ifdef HAVE_KTEXTADDONS_TEXT_SUPPORT
#include <TextGrammarCheck/GrammarAction>
#include <TextGrammarCheck/LanguageToolManager>
#include <TextGrammarCheck/LanguageToolResultWidget>
#else
#include <PimCommonTextGrammarCheck/GrammarAction>
#include <PimCommonTextGrammarCheck/LanguageToolManager>
#include <PimCommonTextGrammarCheck/LanguageToolResultWidget>
#endif
#include <KActionCollection>
#include <KLocalizedString>
#include <KToggleAction>

#include <QHBoxLayout>
#include <QTextBlock>

LanguageToolInterface::LanguageToolInterface(KActionCollection *ac, QWidget *parent)
    : MessageComposer::PluginEditorGrammarCustomToolsViewInterface(parent)
#ifdef HAVE_KTEXTADDONS_TEXT_SUPPORT
    , mGrammarResultWidget(new TextGrammarCheck::LanguageToolResultWidget(this))
#else
    , mGrammarResultWidget(new PimCommonTextGrammarCheck::LanguageToolResultWidget(this))
#endif
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins({});
#ifdef HAVE_KTEXTADDONS_TEXT_SUPPORT
    connect(mGrammarResultWidget, &TextGrammarCheck::LanguageToolResultWidget::replaceText, this, &LanguageToolInterface::slotReplaceText);
    connect(mGrammarResultWidget, &TextGrammarCheck::LanguageToolResultWidget::checkAgain, this, &LanguageToolInterface::checkAgain);
    connect(mGrammarResultWidget, &TextGrammarCheck::LanguageToolResultWidget::closeChecker, this, &LanguageToolInterface::closeChecker);
    connect(mGrammarResultWidget, &TextGrammarCheck::LanguageToolResultWidget::configure, this, [this]() {
        Q_EMIT configure(parentWidget());
    });
#else
    connect(mGrammarResultWidget, &PimCommonTextGrammarCheck::LanguageToolResultWidget::replaceText, this, &LanguageToolInterface::slotReplaceText);
    connect(mGrammarResultWidget, &PimCommonTextGrammarCheck::LanguageToolResultWidget::checkAgain, this, &LanguageToolInterface::checkAgain);
    connect(mGrammarResultWidget, &PimCommonTextGrammarCheck::LanguageToolResultWidget::closeChecker, this, &LanguageToolInterface::closeChecker);
    connect(mGrammarResultWidget, &PimCommonTextGrammarCheck::LanguageToolResultWidget::configure, this, [this]() {
        Q_EMIT configure(parentWidget());
    });

#endif

    layout->addWidget(mGrammarResultWidget);
    createAction(ac);
}

LanguageToolInterface::~LanguageToolInterface() = default;

#ifdef HAVE_KTEXTADDONS_TEXT_SUPPORT
void LanguageToolInterface::slotReplaceText(const TextGrammarCheck::GrammarAction &act)
#else
void LanguageToolInterface::slotReplaceText(const PimCommonTextGrammarCheck::GrammarAction &act)
#endif
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

void LanguageToolInterface::closeChecker()
{
    mGrammarResultWidget->hide();
    Q_EMIT activateView(nullptr);
}

void LanguageToolInterface::slotActivateGrammalecte(bool state)
{
    if (state) {
        if (!checkAgain()) {
            mAction->setChecked(false);
            return;
        }
        mGrammarResultWidget->show();
        Q_EMIT activateView(this);
    } else {
        closeChecker();
    }
}

KToggleAction *LanguageToolInterface::action() const
{
    return mAction;
}

void LanguageToolInterface::createAction(KActionCollection *ac)
{
    mAction = new KToggleAction(i18n("&Check Grammar (LanguageTool)"), this);
    connect(mAction, &KToggleAction::triggered, this, &LanguageToolInterface::slotActivateGrammalecte);
    if (ac) {
        ac->addAction(QStringLiteral("checkgrammar-languagetool"), mAction);
    }
    mAction->setChecked(false);
}

bool LanguageToolInterface::checkAgain()
{
    if (richTextEditor()) {
#ifdef HAVE_KTEXTADDONS_TEXT_SUPPORT
        if (!TextGrammarCheck::LanguageToolManager::self()->useLocalInstance()) {
#else
        if (!PimCommonTextGrammarCheck::LanguageToolManager::self()->useLocalInstance()) {
#endif
            if (KMessageBox::warningTwoActions(
                    parentWidget(),
                    i18n("You do not use local instance.\nYour text will send on a external web site (https://languagetool.org/).\nDo you want to continue?"),
                    i18n("Check Grammar with LanguageTool"),
                    KStandardGuiItem::cont(),
                    KStandardGuiItem::cancel(),
                    QStringLiteral("send_data_on_languagetool"))
                == KMessageBox::ButtonCode::SecondaryAction) {
                return false;
            }
        }
        mGrammarResultWidget->setText(richTextEditor()->toPlainText());
        mGrammarResultWidget->checkGrammar();
    } else {
        qCWarning(KMAIL_EDITOR_LANGUAGETOOL_PLUGIN_LOG) << "richtexteditor not set, it's a bug";
    }
    return true;
}
