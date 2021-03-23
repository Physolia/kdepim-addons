/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "dkimconfigure_private_export.h"
#include <QWidget>
class QCheckBox;
class QComboBox;
class LIBDKIMVERIFYCONFIGURE_TESTS_EXPORT DKIMGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DKIMGeneralWidget(QWidget *parent = nullptr);
    ~DKIMGeneralWidget() override;
    void loadSettings();
    void saveSettings();
    void resetSettings();

private:
    QCheckBox *mEnableDkimSupport = nullptr;
    QCheckBox *mSaveResult = nullptr;
    QComboBox *mSaveKey = nullptr;
    QCheckBox *mUseOnlyAuthenticationResult = nullptr;
};

