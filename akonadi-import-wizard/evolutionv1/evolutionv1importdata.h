/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <ImportWizard/AbstractImporter>

class Evolutionv1ImportData : public LibImportWizard::AbstractImporter
{
public:
    explicit Evolutionv1ImportData(QObject *parent, const QList<QVariant> & = QList<QVariant>());
    ~Evolutionv1ImportData() override;

    Q_REQUIRED_RESULT TypeSupportedOptions supportedOption() override;
    Q_REQUIRED_RESULT bool foundMailer() const override;

    Q_REQUIRED_RESULT bool importMails() override;
    Q_REQUIRED_RESULT QString name() const override;
};

