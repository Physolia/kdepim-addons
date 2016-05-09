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

#include "viewerpluginexternalscriptinfo.h"

ViewerPluginExternalScriptInfo::ViewerPluginExternalScriptInfo()
{

}

QString ViewerPluginExternalScriptInfo::name() const
{
    return mName;
}

void ViewerPluginExternalScriptInfo::setName(const QString &name)
{
    mName = name;
}

QString ViewerPluginExternalScriptInfo::commandLine() const
{
    return mCommandLine;
}

void ViewerPluginExternalScriptInfo::setCommandLine(const QString &commandLine)
{
    mCommandLine = commandLine;
}

QString ViewerPluginExternalScriptInfo::executable() const
{
    return mExecutable;
}

void ViewerPluginExternalScriptInfo::setExecutable(const QString &executable)
{
    mExecutable = executable;
}

QString ViewerPluginExternalScriptInfo::description() const
{
    return mDescription;
}

void ViewerPluginExternalScriptInfo::setDescription(const QString &description)
{
    mDescription = description;
}

bool ViewerPluginExternalScriptInfo::isValid() const
{
    return !mName.trimmed().isEmpty() &&
            !mExecutable.trimmed().isEmpty();
}
