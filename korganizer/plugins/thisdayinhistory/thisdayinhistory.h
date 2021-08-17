/*
  This file is part of KOrganizer.
  SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>
  SPDX-FileCopyrightText: 2007 Loïc Corbasson <loic.corbasson@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <EventViews/CalendarDecoration>

using namespace EventViews::CalendarDecoration;

class ThisDayInHistory : public Decoration
{
public:
    ThisDayInHistory(QObject *parent = nullptr, const QVariantList &args = {});

    Q_REQUIRED_RESULT Element::List createDayElements(const QDate &) override;
    Q_REQUIRED_RESULT Element::List createMonthElements(const QDate &) override;

    //    void configure( QWidget *parent );

    Q_REQUIRED_RESULT QString info() const override;
};
