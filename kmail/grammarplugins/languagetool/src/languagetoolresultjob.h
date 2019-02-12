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

#ifndef LANGUAGETOOLRESULTJOB_H
#define LANGUAGETOOLRESULTJOB_H

#include <QObject>
#include "liblanguagetool_private_export.h"
class QNetworkRequest;
class QNetworkAccessManager;
class LIBLANGUAGETOOLPRIVATE_TESTS_EXPORT LanguagetoolResultJob : public QObject
{
    Q_OBJECT
public:
    explicit LanguagetoolResultJob(QObject *parent = nullptr);
    ~LanguagetoolResultJob();
    bool canStart() const;
    void start();
    Q_REQUIRED_RESULT QStringList arguments() const;
    void setArguments(const QStringList &arguments);

    QNetworkAccessManager *networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    Q_REQUIRED_RESULT QString text() const;
    void setText(const QString &text);

Q_SIGNALS:
    void finished(const QString &result);
    void error();

private:
    Q_DISABLE_COPY(LanguagetoolResultJob)
    void slotCheckGrammarFinished();
    void addRequestAttribute(QNetworkRequest &request) const;
    QStringList mArguments;
    QString mText;
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
};

#endif // LANGUAGETOOLRESULTJOB_H
