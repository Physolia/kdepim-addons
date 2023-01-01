/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "akonadidatabasetoolsjob.h"
#include "akonadidatasetools_debug.h"
#include <QProcess>
#include <QStandardPaths>

AkonadiDatabaseToolsJob::AkonadiDatabaseToolsJob(QObject *parent)
    : QObject{parent}
{
}

AkonadiDatabaseToolsJob::~AkonadiDatabaseToolsJob() = default;

bool AkonadiDatabaseToolsJob::canStart() const
{
    return !akonadiProcessPath().isEmpty() && (mTool != AkonadiDatabaseToolsUtils::AkonadiDatabaseTool::Unknown);
}

QString AkonadiDatabaseToolsJob::akonadiProcessPath() const
{
    return QStandardPaths::findExecutable(QStringLiteral("akonadictl"));
}

void AkonadiDatabaseToolsJob::setTool(AkonadiDatabaseToolsUtils::AkonadiDatabaseTool newTool)
{
    mTool = newTool;
}

void AkonadiDatabaseToolsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return;
    }
    switch (mTool) {
    case AkonadiDatabaseToolsUtils::Unknown:
        qCWarning(AKONADIDATABASETOOLS_LOG) << "mTool is unknown it's a bug! ";
        break;
    case AkonadiDatabaseToolsUtils::Vacuum: {
        mProcess = new QProcess(this);
        mProcess->setProgram(akonadiProcessPath());
        mProcess->setArguments(QStringList() << QStringLiteral("vacuum"));
        connect(mProcess, &QProcess::finished, this, [this]() {
            Q_EMIT finished();
            deleteLater();
        });
        connect(mProcess, &QProcess::readyReadStandardError, this, [this]() {
            Q_EMIT receivedStandardError(QLatin1String(mProcess->readAllStandardError()));
        });
        connect(mProcess, &QProcess::readyReadStandardOutput, this, [this]() {
            Q_EMIT receivedStandardOutput(QLatin1String(mProcess->readAllStandardOutput()));
        });
        mProcess->start();
        break;
    }
    case AkonadiDatabaseToolsUtils::Fsck: {
        mProcess = new QProcess(this);
        mProcess->setProgram(akonadiProcessPath());
        mProcess->setArguments(QStringList() << QStringLiteral("fsck"));
        connect(mProcess, &QProcess::finished, this, [this]() {
            Q_EMIT finished();
            deleteLater();
        });
        connect(mProcess, &QProcess::readyReadStandardError, this, [this]() {
            Q_EMIT receivedStandardError(QLatin1String(mProcess->readAllStandardError()));
        });
        connect(mProcess, &QProcess::readyReadStandardOutput, this, [this]() {
            Q_EMIT receivedStandardOutput(QLatin1String(mProcess->readAllStandardOutput()));
        });
        mProcess->start();
        break;
    }
    }
}
