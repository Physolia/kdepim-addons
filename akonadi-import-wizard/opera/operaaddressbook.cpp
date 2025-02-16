/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "operaaddressbook.h"

#include "operaplugin_debug.h"
#include <KContacts/Addressee>
#include <QFile>
#include <QUrl>

OperaAddressBook::OperaAddressBook(const QString &filename)
    : mFileName(filename)
{
}

void OperaAddressBook::importAddressBook()
{
    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qCDebug(OPERAPLUGIN_LOG) << " We can't open file" << mFileName;
        return;
    }

    QTextStream stream(&file);
    bool foundContact = false;
    KContacts::Addressee *contact = nullptr;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line == QLatin1String("#CONTACT")) {
            appendContact(contact);
            foundContact = true;
        } else if (line == QLatin1String("#FOLDER")) {
            appendContact(contact);
            foundContact = false;
            // TODO
        } else if (foundContact) {
            line = line.trimmed();
            if (!contact) {
                contact = new KContacts::Addressee;
            }
            if (line.startsWith(QLatin1String("ID"))) {
                // Nothing
            } else if (line.startsWith(QLatin1String("NAME"))) {
                contact->setName(line.remove(QStringLiteral("NAME=")));
            } else if (line.startsWith(QLatin1String("URL"))) {
                KContacts::ResourceLocatorUrl url;
                url.setUrl(QUrl(line.remove(QStringLiteral("URL="))));
                contact->setUrl(url);
            } else if (line.startsWith(QLatin1String("DESCRIPTION"))) {
                contact->setNote(line.remove(QStringLiteral("DESCRIPTION=")));
            } else if (line.startsWith(QLatin1String("PHONE"))) {
                contact->insertPhoneNumber(KContacts::PhoneNumber(line.remove(QStringLiteral("PHONE=")), KContacts::PhoneNumber::Home));
            } else if (line.startsWith(QLatin1String("FAX"))) {
                contact->insertPhoneNumber(KContacts::PhoneNumber(line.remove(QStringLiteral("FAX=")), KContacts::PhoneNumber::Fax));
            } else if (line.startsWith(QLatin1String("POSTALADDRESS"))) {
                // TODO
            } else if (line.startsWith(QLatin1String("PICTUREURL"))) {
                // TODO
            } else if (line.startsWith(QLatin1String("ICON"))) {
                // TODO
            } else if (line.startsWith(QLatin1String("SHORT NAME"))) {
                contact->setNickName(line.remove(QStringLiteral("SHORT NAME=")));
            }
        }
    }
    appendContact(contact);
}

OperaAddressBook::~OperaAddressBook() = default;

void OperaAddressBook::appendContact(KContacts::Addressee *contact)
{
    if (contact) {
        addImportContactNote(*contact, QStringLiteral("Opera"));
        createContact(*contact);
        delete contact;
    }
}
