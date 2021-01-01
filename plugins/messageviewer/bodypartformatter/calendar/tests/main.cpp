/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QStandardPaths>
#include <QCommandLineParser>
#include <QApplication>
#include <../attendeeselector.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto *dialog = new AttendeeSelector;
    dialog->resize(800, 600);
    dialog->show();
    app.exec();
    delete dialog;
    return 0;
}
