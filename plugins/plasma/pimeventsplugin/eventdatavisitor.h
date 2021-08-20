/*
 * SPDX-FileCopyrightText: 2016 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include <CalendarEvents/CalendarEventsPlugin>
#include <KCalendarCore/Visitor>
#include <QMultiHash>

class PimDataSource;
class BaseEventDataVisitor : public KCalendarCore::Visitor
{
public:
    ~BaseEventDataVisitor() override;

    Q_REQUIRED_RESULT bool act(const KCalendarCore::Incidence::Ptr &incidence);
    Q_REQUIRED_RESULT bool act(const KCalendarCore::Event::List &events);
    Q_REQUIRED_RESULT bool act(const KCalendarCore::Todo::List &todos);

protected:
    BaseEventDataVisitor(PimDataSource *dataSource, QDate start, QDate end);

    Q_REQUIRED_RESULT QString generateUid(const KCalendarCore::Incidence::Ptr &incidence, const QDateTime &recurrenceId = {}) const;
    Q_REQUIRED_RESULT bool isInRange(QDate start, QDate end) const;

    Q_REQUIRED_RESULT QVector<CalendarEvents::EventData>
    explodeIncidenceOccurences(const CalendarEvents::EventData &ed, const KCalendarCore::Incidence::Ptr &incidence, bool &ok);

protected:
    PimDataSource *const mDataSource;
    const QDate mStart;
    const QDate mEnd;
};

class EventDataVisitor : public BaseEventDataVisitor
{
public:
    EventDataVisitor(PimDataSource *dataSource, const QDate &start, const QDate &end);
    ~EventDataVisitor() override;

    const QMultiHash<QDate, CalendarEvents::EventData> &results() const;

protected:
    bool visit(const KCalendarCore::Event::Ptr &event) override;
    bool visit(const KCalendarCore::Todo::Ptr &todo) override;
    bool visit(const KCalendarCore::Journal::Ptr &) override
    {
        return false;
    }

    bool visit(const KCalendarCore::FreeBusy::Ptr &) override
    {
        return false;
    }

private:
    void insertResult(const CalendarEvents::EventData &result);

    Q_REQUIRED_RESULT bool visit(const KCalendarCore::Incidence::Ptr &incidence, CalendarEvents::EventData::EventType eventType);
    CalendarEvents::EventData incidenceData(const KCalendarCore::Incidence::Ptr &incidence) const;

    QMultiHash<QDate, CalendarEvents::EventData> mResults;
};

class EventDataIdVisitor : public BaseEventDataVisitor
{
public:
    explicit EventDataIdVisitor(PimDataSource *dataSource, QDate start, QDate end);

    const QStringList &results() const;

protected:
    bool visit(const KCalendarCore::Event::Ptr &event) override;
    bool visit(const KCalendarCore::Todo::Ptr &todo) override;
    bool visit(const KCalendarCore::Journal::Ptr &) override
    {
        return false;
    }

    bool visit(const KCalendarCore::FreeBusy::Ptr &) override
    {
        return false;
    }

private:
    bool visit(const KCalendarCore::Incidence::Ptr &incidence);

    QStringList mResults;
};

