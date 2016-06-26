/*
   Copyright (C) 2015-2016 Montel Laurent <montel@kde.org>

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

#include "headerstylemenu_gui.h"
#include "messageviewer/headerstylemenumanager.h"
#include <QStandardPaths>
#include <KLocalizedString>

#include <QApplication>
#include <KAboutData>
#include <QCommandLineParser>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

#include <kactioncollection.h>
#include <QTextEdit>
#include <KActionMenu>
#include <messageviewer/headerstrategy.h>
#include <messageviewer/headerstyle.h>
#include <messageviewer/headerstyleplugin.h>

HeaderStyleMenuTest::HeaderStyleMenuTest(QWidget *parent)
    : QWidget(parent)
{
    QMenuBar *menu = new QMenuBar(this);
    QVBoxLayout *hbox = new QVBoxLayout(this);
    hbox->addWidget(menu);
    MessageViewer::HeaderStyleMenuManager *manager = new MessageViewer::HeaderStyleMenuManager(new KActionCollection(this), this);
    menu->addAction(manager->menu());
    mTextEdit = new QTextEdit(this);
    mTextEdit->setReadOnly(true);
    hbox->addWidget(mTextEdit);
    connect(manager, &MessageViewer::HeaderStyleMenuManager::styleChanged, this, &HeaderStyleMenuTest::styleChanged);
    manager->readConfig();
}

HeaderStyleMenuTest::~HeaderStyleMenuTest()
{

}

void HeaderStyleMenuTest::styleChanged(MessageViewer::HeaderStylePlugin *plugin)
{
    mTextEdit->append(QStringLiteral("strategy: %1, headerstyle: %2").arg(QLatin1String(plugin->headerStrategy()->name()))
                      .arg(QLatin1String(plugin->headerStyle()->name())));
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    KAboutData aboutData(QStringLiteral("headerstylemenu_gui"), i18n("headerstylemenu_Gui"), QStringLiteral("1.0"));
    aboutData.setShortDescription(i18n("Test for headerstylemenu"));
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    HeaderStyleMenuTest *w = new HeaderStyleMenuTest();
    w->resize(800, 200);
    w->show();
    app.exec();
    delete w;
    return 0;
}

