/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

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

#ifndef GRAMMALECTEWIDGET_H
#define GRAMMALECTEWIDGET_H

#include <PluginEditorGrammarCustomToolsViewInterface>
#include <QWidget>
#include "grammalectegenerateconfigoptionjob.h"
class GrammalecteResultWidget;
class QTextEdit;
class GrammalecteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GrammalecteWidget(QWidget *parent = nullptr);
    ~GrammalecteWidget();

private:
    void slotReplaceText(const MessageComposer::PluginGrammarAction &act);
    void slotCheckGrammar();
    void slotGetSettings();
    void slotGetSettingsFinished(const QVector<GrammalecteGenerateConfigOptionJob::Option> &result);
    void slotResultFinished(const QString &result);
    QTextEdit *mInput = nullptr;
    GrammalecteResultWidget *mResultWidget = nullptr;
};

#endif // GRAMMALECTEWIDGET_H
