/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef VIEWERPLUGINEXTERNALSCRIPTINFO_H
#define VIEWERPLUGINEXTERNALSCRIPTINFO_H

#include <QString>
#include <QObject>
class ViewerPluginExternalScriptInfo
{
public:
    ViewerPluginExternalScriptInfo();
    QString name() const;
    void setName(const QString &name);

    QString commandLine() const;
    void setCommandLine(const QString &commandLine);

    QString executable() const;
    void setExecutable(const QString &executable);

    QString description() const;
    void setDescription(const QString &description);

    QString icon() const;
    void setIcon(const QString &icon);

    bool isValid() const;
private:
    QString mCommandLine;
    QString mName;
    QString mExecutable;
    QString mDescription;
    QString mIcon;
};
Q_DECLARE_TYPEINFO(ViewerPluginExternalScriptInfo, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(ViewerPluginExternalScriptInfo)

#endif // VIEWERPLUGINEXTERNALSCRIPTINFO_H
