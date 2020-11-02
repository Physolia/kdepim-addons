/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "adblockblockableelementgui.h"
#include "../lib/widgets/adblockblockableitemsdialog.h"
#include "../lib/adblockblockableitemsjob.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QWebEngineView>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QApplication>
using namespace AdBlock;
AdblockBlockableElementGui::AdblockBlockableElementGui(QWidget *parent)
    : QWidget(parent)
{
    auto *vbox = new QVBoxLayout(this);

    mWebEngineView = new QWebEngineView(this);
    mWebEngineView->load(QUrl(QStringLiteral("http://www.kde.org")));
    vbox->addWidget(mWebEngineView);
    QPushButton *button = new QPushButton(QStringLiteral("search adblock"), this);
    connect(button, &QPushButton::clicked, this, &AdblockBlockableElementGui::slotSearchAdblock);
    vbox->addWidget(button);
}

AdblockBlockableElementGui::~AdblockBlockableElementGui()
{
}

void AdblockBlockableElementGui::slotSearchItemsDone(const QVector<AdBlock::AdBlockResult> &result)
{
    AdBlockBlockableItemsDialog dlg(this);
    dlg.setAdblockResult(result);
    dlg.exec();
}

void AdblockBlockableElementGui::slotSearchAdblock()
{
    auto *job = new AdBlockBlockableItemsJob(this);
    job->setWebEngineView(mWebEngineView);
    connect(job, &AdBlockBlockableItemsJob::searchItemsDone, this, &AdblockBlockableElementGui::slotSearchItemsDone);
    job->start();
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto *dialog = new AdblockBlockableElementGui;
    dialog->resize(800, 600);
    dialog->show();
    app.exec();
    delete dialog;
    return 0;
}
