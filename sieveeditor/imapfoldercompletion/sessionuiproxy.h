/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
    SPDX-FileContributor: Kevin Ottens <kevin@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KIO/SslUi>
#include <kimap/sessionuiproxy.h>

class SessionUiProxy : public KIMAP::SessionUiProxy
{
public:
    bool ignoreSslError(const KSslErrorUiData &errorData) override
    {
        if (KIO::SslUi::askIgnoreSslErrors(errorData, KIO::SslUi::RecallAndStoreRules)) {
            return true;
        } else {
            return false;
        }
    }
};
