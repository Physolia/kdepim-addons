/*
    This file is part of Akonadi.

    Copyright (c) 2011 Stephen Kelly <steveire@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#ifndef TRIPWIDGET_H
#define TRIPWIDGET_H

#include <QWidget>

class QLabel;
class QTextBrowser;

class Trip;
class FolderContentsWidget;

class TripWidget : public QWidget
{
    Q_OBJECT
public:
    TripWidget(Trip *trip, QWidget *parent = nullptr);

    QString eventName() const;

Q_SIGNALS:
    void deleteThis(TripWidget *);

private Q_SLOTS:
    void doDeleteThis();
    void updateName();
    void updateDescription();

private:
    FolderContentsWidget *createView(const QString &type, int role);

    Trip *const m_trip;
    QTextBrowser *m_eventBrowser;
    QLabel *m_eventName;
};

#endif
