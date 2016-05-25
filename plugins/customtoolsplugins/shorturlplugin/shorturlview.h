/*
    Copyright (c) 2015-2016 Montel Laurent <montel@kde.org>

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

#ifndef SHORTURLVIEW_H
#define SHORTURLVIEW_H

#include <pimcommon/customtoolsviewinterface.h>
class KActionCollection;
class ShortUrlWidgetNg;
class ShorturlView : public PimCommon::CustomToolsViewInterface
{
    Q_OBJECT
public:
    explicit ShorturlView(KActionCollection *ac, QWidget *parent = Q_NULLPTR);
    ~ShorturlView();
    KToggleAction *action() const Q_DECL_OVERRIDE;

private Q_SLOTS:
    void slotActivateShorturl(bool state);

private:
    void createAction(KActionCollection *ac);
    KToggleAction *mAction;
    ShortUrlWidgetNg *mShorturl;
};
#endif // SHORTURLVIEW_H
