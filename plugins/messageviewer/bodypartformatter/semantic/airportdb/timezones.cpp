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

#include "timezones.h"

#include <QDebug>
#include <QFile>

Timezones::Timezones()
{
    m_map.load(QStringLiteral("timezones.png"));

    QFile colorMap(QStringLiteral("timezones.colormap"));
    colorMap.open(QFile::ReadOnly);

    while (!colorMap.atEnd()) {
        const auto line = colorMap.readLine();
        const auto split = line.split(',');
        if (split.size() < 5)
            continue;
        QColor c;
        c.setRed(split.at(0).toInt());
        c.setGreen(split.at(1).toInt());
        c.setBlue(split.at(2).toInt());
        m_zones.insert(c.rgb(), split.at(4).trimmed());
    }
}

Timezones::~Timezones() = default;

QByteArray Timezones::timezoneForCoordinate(float longitude, float latitude) const
{
    const int x = m_map.width() * ((longitude + 180.0f)/ 360.0f);
    const int y = -m_map.height() * ((latitude - 90.0f) / 180.0f);

    //qDebug() << x << y << m_map.width() << m_map.height() << longitude << latitude << QColor(m_map.pixel(x, y)) << m_zones.value(m_map.pixel(x, y));
    return m_zones.value(m_map.pixel(x, y));
}
