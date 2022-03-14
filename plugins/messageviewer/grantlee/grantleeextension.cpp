/*
   SPDX-FileCopyrightText: 2018 Volker Krause <vkrause@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grantleeextension.h"

#include <KContacts/Address>
#include <KItinerary/Place>
#include <kcontacts_version.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <grantlee/exception.h>
#include <grantlee/parser.h>
#else
#include <KTextTemplate/exception.h>
#include <KTextTemplate/parser.h>
#endif

#include <QDateTime>
#include <QTimeZone>

QVariant AddressFormatter::doFilter(const QVariant &input, const QVariant &arg, bool autoescape) const
{
    Q_UNUSED(arg)
    Q_UNUSED(autoescape)

    if (!KItinerary::JsonLd::isA<KItinerary::PostalAddress>(input)) {
        return {};
    }
    const auto a = input.value<KItinerary::PostalAddress>();

    KContacts::Address address;
    address.setStreet(a.streetAddress());
    address.setPostalCode(a.postalCode());
    address.setLocality(a.addressLocality());
    address.setRegion(a.addressRegion());
    address.setCountry(a.addressCountry());

#if KContacts_VERSION < QT_VERSION_CHECK(5, 92, 0)
    auto s = escape(address.formattedAddress().trimmed().replace(QLatin1String("\n\n"), QLatin1String("\n")));
#else
    auto s = escape(address.formatted(KContacts::AddressFormatStyle::MultiLineInternational));
#endif
    return s.get().replace(QLatin1Char('\n'), QLatin1String("<br/>"));
}

bool AddressFormatter::isSafe() const
{
    return true;
}

QVariant DateFormatter::doFilter(const QVariant &input, const QVariant &arg, bool autoescape) const
{
    Q_UNUSED(arg)
    Q_UNUSED(autoescape)

    const auto dt = input.value<QDateTime>();
    if (!dt.isValid()) {
        return {};
    }

    return QLocale().toString(dt.date(), QLocale::ShortFormat);
}

QVariant DateTimeFormatter::doFilter(const QVariant &input, const QVariant &arg, bool autoescape) const
{
    Q_UNUSED(arg)
    Q_UNUSED(autoescape)

    const auto dt = input.value<QDateTime>();
    if (!dt.isValid()) {
        return {};
    }

    auto s = QLocale().toString(dt, QLocale::ShortFormat);
    if (dt.timeSpec() == Qt::TimeZone || dt.timeSpec() == Qt::OffsetFromUTC || dt.timeSpec() == Qt::UTC) {
        s += QLatin1Char(' ') + dt.timeZone().abbreviation(dt);
    }
    return s;
}

QVariant TimeFormatter::doFilter(const QVariant &input, const QVariant &arg, bool autoescape) const
{
    Q_UNUSED(arg)
    Q_UNUSED(autoescape)

    const auto dt = input.value<QDateTime>();
    if (!dt.isValid()) {
        return {};
    }

    auto s = QLocale().toString(dt.time(), QLocale::ShortFormat);
    if (dt.timeSpec() == Qt::TimeZone || dt.timeSpec() == Qt::OffsetFromUTC || dt.timeSpec() == Qt::UTC) {
        s += QLatin1Char(' ') + dt.timeZone().abbreviation(dt);
    }
    return s;
}

TagLibrary::TagLibrary(QObject *parent)
    : QObject(parent)
{
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QHash<QString, Grantlee::Filter *> TagLibrary::filters(const QString &name)
#else
QHash<QString, KTextTemplate::Filter *> TagLibrary::filters(const QString &name)
#endif
{
    Q_UNUSED(name)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QHash<QString, Grantlee::Filter *> filters;
#else
    QHash<QString, KTextTemplate::Filter *> filters;
#endif
    filters.insert(QStringLiteral("formatAddress"), new AddressFormatter());
    filters.insert(QStringLiteral("formatDate"), new DateFormatter());
    filters.insert(QStringLiteral("formatDateTime"), new DateTimeFormatter());
    filters.insert(QStringLiteral("formatTime"), new TimeFormatter());
    return filters;
}
