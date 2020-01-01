/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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

#ifndef CHECKDUPLICATEEMAILSDIALOG_H
#define CHECKDUPLICATEEMAILSDIALOG_H

#include <QDialog>

class QListWidget;
class CheckDuplicateEmailsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CheckDuplicateEmailsDialog(QWidget *parent = nullptr);
    ~CheckDuplicateEmailsDialog();

    void setDuplicatedEmails(const QMap<QString, int> &emails);
private:
    void readConfig();
    void writeConfig();
    QListWidget *mListWidget = nullptr;
};

#endif // CHECKDUPLICATEEMAILSDIALOG_H
