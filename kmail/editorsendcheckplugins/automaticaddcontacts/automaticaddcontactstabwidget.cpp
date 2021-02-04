/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "automaticaddcontactstabwidget.h"
#include <AkonadiCore/Collection>
#include <AkonadiWidgets/CollectionComboBox>
#include <KConfigGroup>
#include <KContacts/Addressee>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>

namespace
{
QString configGroupName()
{
    return QStringLiteral("Automatic Add Contacts %1");
}
}
AutomaticAddContactsTabWidget::AutomaticAddContactsTabWidget(QWidget *parent, QAbstractItemModel *model)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mEnabled = new QCheckBox(i18n("Automatic Add Contacts"), this);
    mEnabled->setObjectName(QStringLiteral("enabled"));
    connect(mEnabled, &QCheckBox::clicked, this, &AutomaticAddContactsTabWidget::configureChanged);
    mainLayout->addWidget(mEnabled);

    auto hlay = new QHBoxLayout;
    hlay->setContentsMargins({});
    hlay->setObjectName(QStringLiteral("folderlayout"));
    mainLayout->addLayout(hlay);

    QLabel *lab = new QLabel(i18n("Select the addressbook in which to store contacts:"), this);
    lab->setObjectName(QStringLiteral("labelfolder"));
    hlay->addWidget(lab);

    mCollectionCombobox = new Akonadi::CollectionComboBox(model, this);
    mCollectionCombobox->setAccessRightsFilter(Akonadi::Collection::CanCreateItem);
    mCollectionCombobox->setMinimumWidth(250);
    mCollectionCombobox->setMimeTypeFilter(QStringList() << KContacts::Addressee::mimeType());
    mCollectionCombobox->setObjectName(QStringLiteral("akonadicombobox"));
    hlay->addWidget(mCollectionCombobox);
    hlay->addStretch(1);
    connect(mCollectionCombobox, qOverload<int>(&Akonadi::CollectionComboBox::currentIndexChanged), this, &AutomaticAddContactsTabWidget::configureChanged);
    connect(mCollectionCombobox, qOverload<int>(&Akonadi::CollectionComboBox::activated), this, &AutomaticAddContactsTabWidget::configureChanged);

    mainLayout->addStretch(1);
}

AutomaticAddContactsTabWidget::~AutomaticAddContactsTabWidget()
{
}

void AutomaticAddContactsTabWidget::resetSettings()
{
    mEnabled->setChecked(false);
}

void AutomaticAddContactsTabWidget::loadSettings()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup grp = config->group(configGroupName().arg(mIdentity));
    mEnabled->setChecked(grp.readEntry("Enabled", false));
    mCollectionCombobox->setDefaultCollection(Akonadi::Collection(grp.readEntry("Collection", -1)));
}

void AutomaticAddContactsTabWidget::setIdentity(uint identity)
{
    mIdentity = identity;
}

void AutomaticAddContactsTabWidget::saveSettings()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup grp = config->group(configGroupName().arg(mIdentity));
    grp.writeEntry("Enabled", mEnabled->isChecked());
    grp.writeEntry("Collection", mCollectionCombobox->currentCollection().id());
}
