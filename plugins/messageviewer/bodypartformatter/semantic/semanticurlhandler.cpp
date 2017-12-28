/*
   Copyright (c) 2017 Volker Krause <vkrause@kde.org>

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

#include "semanticurlhandler.h"
#include "calendarhandler.h"
#include "datatypes.h"
#include "jsonlddocument.h"
#include "semanticmemento.h"
#include "semantic_debug.h"

#include <MimeTreeParser/BodyPart>
#include <MimeTreeParser/NodeHelper>

#include <CalendarSupport/CalendarSingleton>

#include <KMime/Content>

#include <KDBusServiceStarter>
#include <KLocalizedString>

#include <QDate>
#include <QDBusInterface>
#include <QDesktopServices>
#include <QIcon>
#include <QMenu>
#include <QUrlQuery>

#include <memory>

bool SemanticUrlHandler::handleClick(MessageViewer::Viewer *viewerInstance, MimeTreeParser::Interface::BodyPart *part, const QString &path) const
{
    Q_UNUSED(viewerInstance);
    Q_UNUSED(part);
    return path == QLatin1String("semanticAction");
}

static void addGoToMapAction(QMenu *menu, const QVariant &place)
{
    if (place.isNull()) {
        return;
    }

    const auto addr = JsonLdDocument::readProperty(place, "address");
    if (!addr.isNull()) {
        auto action = menu->addAction(QIcon::fromTheme(QStringLiteral("map-globe")), i18n("Show \'%1\' On Map", JsonLdDocument::readProperty(place, "name").toString()));
        QObject::connect(action, &QAction::triggered, menu, [addr]() {
            QUrl url;
            url.setScheme(QStringLiteral("https"));
            url.setHost(QStringLiteral("www.openstreetmap.org"));
            url.setPath(QStringLiteral("/search"));
            const QString queryString = JsonLdDocument::readProperty(addr, "streetAddress").toString() + QLatin1String(", ")
                                        + JsonLdDocument::readProperty(addr, "postalCode").toString() + QLatin1Char(' ')
                                        + JsonLdDocument::readProperty(addr, "addressLocality").toString() + QLatin1String(", ")
                                        + JsonLdDocument::readProperty(addr, "addressCountry").toString();
            QUrlQuery query;
            query.addQueryItem(QStringLiteral("query"), queryString);
            url.setQuery(query);
            QDesktopServices::openUrl(url);
        });
        return;
    }

    const auto geo = JsonLdDocument::readProperty(place, "geo");
    if (!geo.isNull()) {
        auto action = menu->addAction(QIcon::fromTheme(QStringLiteral("map-globe")), i18n("Show \'%1\' On Map", JsonLdDocument::readProperty(place, "name").toString()));
        // zoom out further from airports, they are larger and you usually want to go further away from them
        const auto zoom = place.userType() == qMetaTypeId<Airport>() ? 12 : 16;
        QObject::connect(action, &QAction::triggered, menu, [geo, zoom]() {
            QUrl url;
            url.setScheme(QStringLiteral("https"));
            url.setHost(QStringLiteral("www.openstreetmap.org"));
            url.setPath(QStringLiteral("/"));
            const QString fragment = QLatin1String("map=") + QString::number(zoom)
                                     + QLatin1Char('/') + JsonLdDocument::readProperty(geo, "latitude").toString()
                                     + QLatin1Char('/') + JsonLdDocument::readProperty(geo, "longitude").toString();
            url.setFragment(fragment);
            QDesktopServices::openUrl(url);
        });
    }
}

bool SemanticUrlHandler::handleContextMenuRequest(MimeTreeParser::Interface::BodyPart *part, const QString &path, const QPoint &p) const
{
    Q_UNUSED(part);
    if (path != QLatin1String("semanticAction")) {
        return false;
    }

    const auto m = memento(part);
    if (!m || m->isEmpty()) {
        return false;
    }
    const auto date = dateForReservation(m);

    QMenu menu;
    QAction *action = nullptr;
    if (date.isValid()) {
        action = menu.addAction(QIcon::fromTheme(QStringLiteral("view-calendar")), i18n("Show Calendar"));
        QObject::connect(action, &QAction::triggered, this, [this, date](){
            showCalendar(date);
        });
    }

    action = menu.addAction(QIcon::fromTheme(QStringLiteral("appointment-new")), i18n("Add To Calendar"));
    QObject::connect(action, &QAction::triggered, this, [this, m](){
        addToCalendar(m);
    });

    QSet<QString> places;
    for (const auto &r : m->data()) {
        if (r.userType() == qMetaTypeId<LodgingReservation>()) {
            addGoToMapAction(&menu, JsonLdDocument::readProperty(r, "reservationFor"));
        } else if (r.userType() == qMetaTypeId<FlightReservation>()) {
            const auto flight = JsonLdDocument::readProperty(r, "reservationFor");

            auto airport = JsonLdDocument::readProperty(flight, "departureAirport");
            auto iataCode = JsonLdDocument::readProperty(airport, "iataCode").toString();
            if (!places.contains(iataCode)) {
                addGoToMapAction(&menu, airport);
                places.insert(iataCode);
            }

            airport = JsonLdDocument::readProperty(flight, "arrivalAirport");
            iataCode = JsonLdDocument::readProperty(airport, "iataCode").toString();
            if (!places.contains(iataCode)) {
                addGoToMapAction(&menu, airport);
                places.insert(iataCode);
            }
        } else if (r.userType() == qMetaTypeId<TrainReservation>()) {
            const auto trip = JsonLdDocument::readProperty(r, "reservationFor");

            auto station = JsonLdDocument::readProperty(trip, "departureStation");
            auto name = JsonLdDocument::readProperty(station, "name").toString();
            if (!places.contains(name)) {
                addGoToMapAction(&menu, station);
                places.insert(name);
            }

            station = JsonLdDocument::readProperty(trip, "arrivalStation");
            name = JsonLdDocument::readProperty(station, "name").toString();
            if (!places.contains(name)) {
                addGoToMapAction(&menu, station);
                places.insert(name);
            }
        }
    }

    menu.exec(p);
    return true;
}

QString SemanticUrlHandler::statusBarMessage(MimeTreeParser::Interface::BodyPart *part, const QString &path) const
{
    Q_UNUSED(part);
    if (path == QLatin1String("semanticAction")) {
        return i18n("Add reservation to your calendar.");
    }
    return {};
}

SemanticMemento *SemanticUrlHandler::memento(MimeTreeParser::Interface::BodyPart *part) const
{
    const auto node = part->content()->topLevel();
    const auto nodeHelper = part->nodeHelper();
    if (!nodeHelper || !node) {
        return nullptr;
    }
    return dynamic_cast<SemanticMemento *>(nodeHelper->bodyPartMemento(node->topLevel(), "org.kde.messageviewer.semanticData"));
}

QDate SemanticUrlHandler::dateForReservation(SemanticMemento *memento) const
{
    for (const auto &r : memento->data()) {
        if (r.userType() == qMetaTypeId<FlightReservation>()) {
            const auto v = JsonLdDocument::readProperty(r, "reservationFor");
            const auto d = JsonLdDocument::readProperty(v, "departureTime").toDate();
            if (d.isValid()) {
                return d;
            }
        } else if (r.userType() == qMetaTypeId<LodgingReservation>()) {
            const auto d = JsonLdDocument::readProperty(r, "checkinDate").toDate();
            if (d.isValid()) {
                return d;
            }
        } else if (r.userType() == qMetaTypeId<TrainReservation>()) {
            const auto trip = JsonLdDocument::readProperty(r, "reservationFor");
            const auto d = JsonLdDocument::readProperty(trip, "departureTime").toDate();
            if (d.isValid()) {
                return d;
            }
        }
    }
    return {};
}

void SemanticUrlHandler::showCalendar(const QDate &date) const
{
    // ensure KOrganizer or Kontact are running
    QString error, dbusService;
    const auto result = KDBusServiceStarter::self()->findServiceFor(QStringLiteral("DBUS/Organizer"), {}, &error, &dbusService) == 0;
    if (!result) {
        qCWarning(SEMANTIC_LOG) << "Failed to start KOrganizer" << error << dbusService;
    }

    // switch to KOrganizer if we are using Kontact
    std::unique_ptr<QDBusInterface> kontactIface(
        new QDBusInterface(QStringLiteral("org.kde.kontact"), QStringLiteral("/KontactInterface"),
                           QStringLiteral("org.kde.kontact.KontactInterface"), QDBusConnection::sessionBus()));
    if (kontactIface->isValid()) {
        kontactIface->call(QStringLiteral("selectPlugin"), QStringLiteral("kontact_korganizerplugin"));
    }

    // select the date of the reservation
    std::unique_ptr<QDBusInterface> korgIface(
        new QDBusInterface(QStringLiteral("org.kde.korganizer"), QStringLiteral("/Calendar"),
                           QStringLiteral("org.kde.Korganizer.Calendar"), QDBusConnection::sessionBus()));
    if (!korgIface->isValid()) {
        qCWarning(SEMANTIC_LOG) << "Calendar interface is not valid! " << korgIface->lastError().message();
        return;
    }
    korgIface->call(QStringLiteral("showEventView"));
    korgIface->call(QStringLiteral("showDate"), date);
}

void SemanticUrlHandler::addToCalendar(SemanticMemento *memento) const
{
    using namespace KCalCore;

    const auto calendar = CalendarSupport::calendarSingleton(true);
    for (const auto &r : memento->data()) {
        auto event = CalendarHandler::findEvent(calendar, r);
        if (!event) {
            event.reset(new Event);
            CalendarHandler::fillEvent(r, event);
            if (!event->dtStart().isValid() || !event->dtEnd().isValid() || event->summary().isEmpty()) {
                continue;
            }
            calendar->addEvent(event);
        } else {
            CalendarHandler::fillEvent(r, event);
            calendar->modifyIncidence(event);
        }
    }
}
