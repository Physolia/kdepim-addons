/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "checkattachmentjob.h"

#include <KEmailAddress>
#include <PimCommon/PimUtil>

CheckAttachmentJob::CheckAttachmentJob()
{
}

CheckAttachmentJob::~CheckAttachmentJob()
{
}

void CheckAttachmentJob::start()
{
    mResultList.clear();

    for (const QString &email : qAsConst(mOriginalEmails)) {
        QString tname, temail;
        KEmailAddress::extractEmailAddressAndName(email, temail, tname);    // ignore return value
        if (!temail.isEmpty()) {
            if (!mResultList.contains(temail)) {
                mResultList.append(temail);
            }
        }
    }
}

QStringList CheckAttachmentJob::originalEmails() const
{
    return mOriginalEmails;
}

void CheckAttachmentJob::setOriginalEmails(const QStringList &list)
{
    mOriginalEmails = PimCommon::Util::generateEmailList(list);
}

QStringList CheckAttachmentJob::resultList() const
{
    return mResultList;
}

void CheckAttachmentJob::setResultList(const QStringList &resultList)
{
    mResultList = resultList;
}
