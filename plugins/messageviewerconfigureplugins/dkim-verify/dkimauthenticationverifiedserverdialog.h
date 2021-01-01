/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef DKIMAUTHENTICATIONVERIFIEDSERVERDIALOG_H
#define DKIMAUTHENTICATIONVERIFIEDSERVERDIALOG_H

#include <QDialog>
#include "dkimconfigure_private_export.h"
class DKIMAuthenticationVerifiedServerWidget;
class LIBDKIMVERIFYCONFIGURE_TESTS_EXPORT DKIMAuthenticationVerifiedServerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DKIMAuthenticationVerifiedServerDialog(QWidget *parent = nullptr);
    ~DKIMAuthenticationVerifiedServerDialog() override;

private:
    void slotAccepted();
    void readConfig();
    void writeConfig();
    DKIMAuthenticationVerifiedServerWidget *mAuthenticationVerifiedWidget = nullptr;
};

#endif // DKIMAUTHENTICATIONVERIFIEDSERVERDIALOG_H
