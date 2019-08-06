/*
    Copyright (C) 2019 Laurent Montel <montel@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef MARKDOWNCREATELINKDIALOG_H
#define MARKDOWNCREATELINKDIALOG_H

#include <QDialog>
#include "libkmailmarkdown_export.h"
class MarkdownCreateLinkWidget;
class QPushButton;
class LIBKMAILMARKDOWN_EXPORT MarkdownCreateLinkDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MarkdownCreateLinkDialog(QWidget *parent = nullptr);
    ~MarkdownCreateLinkDialog();
    Q_REQUIRED_RESULT QString linkStr() const;
private:
    void writeConfig();
    void readConfig();
    void slotEnabledOkButton(bool enabled);
    MarkdownCreateLinkWidget *mMarkdownCreateLinkWidget = nullptr;
    QPushButton *mOkButton = nullptr;
};

#endif // MARKDOWNCREATELINKDIALOG_H