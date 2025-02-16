/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "vcardimportexportplugininterface.h"
#include <KActionCollection>
#include <KAddressBookImportExport/ContactSelectionDialog>
#include <KContacts/VCardConverter>
#include <KJobWidgets>
#include <KLocalizedString>
#include <KMessageBox>
#include <PimCommon/RenameFileDialog>
#include <QAction>
#include <QFileDialog>
#include <QPointer>
#include <QUrl>
#include <kio/filecopyjob.h>
#include <kio/storedtransferjob.h>

#ifdef QGPGME_FOUND
#include <KAddressBookImportExport/ImportExportEngine>
#include <QTemporaryFile>
#include <gpgme++/context.h>
#include <gpgme++/data.h>
#include <gpgme++/key.h>
#include <qgpgme/dataprovider.h>
#endif // QGPGME_FOUND

using namespace KAddressBookImportExport;

VCardImportExportPluginInterface::VCardImportExportPluginInterface(QObject *parent)
    : PluginInterface(parent)
    , mExportVCardType(VCard3)
{
}

VCardImportExportPluginInterface::~VCardImportExportPluginInterface() = default;

void VCardImportExportPluginInterface::createAction(KActionCollection *ac)
{
    QAction *action = ac->addAction(QStringLiteral("file_import_vcard"));
    action->setText(i18n("Import vCard..."));
    action->setWhatsThis(i18n("Import contacts from a vCard file."));
    connect(action, &QAction::triggered, this, &VCardImportExportPluginInterface::slotImportVCard);
    setImportActions(QList<QAction *>() << action);

    QList<QAction *> exportActionList;

    action = ac->addAction(QStringLiteral("file_export_vcard40"));
    action->setWhatsThis(i18n("Export contacts to a vCard 4.0 file."));
    action->setText(i18n("Export vCard 4.0..."));
    connect(action, &QAction::triggered, this, &VCardImportExportPluginInterface::slotExportVCard4);
    exportActionList << action;

    action = ac->addAction(QStringLiteral("file_export_vcard30"));
    action->setText(i18n("Export vCard 3.0..."));
    action->setWhatsThis(i18n("Export contacts to a vCard 3.0 file."));
    connect(action, &QAction::triggered, this, &VCardImportExportPluginInterface::slotExportVCard3);
    exportActionList << action;

    action = ac->addAction(QStringLiteral("file_export_vcard21"));
    action->setText(i18n("Export vCard 2.1..."));
    action->setWhatsThis(i18n("Export contacts to a vCard 2.1 file."));
    connect(action, &QAction::triggered, this, &VCardImportExportPluginInterface::slotExportVCard2);
    exportActionList << action;
    setExportActions(exportActionList);
}

void VCardImportExportPluginInterface::slotExportVCard4()
{
    mImportExportAction = Export;
    mExportVCardType = VCard4;
    Q_EMIT emitPluginActivated(this);
}

void VCardImportExportPluginInterface::slotExportVCard3()
{
    mImportExportAction = Export;
    mExportVCardType = VCard3;
    Q_EMIT emitPluginActivated(this);
}

void VCardImportExportPluginInterface::slotExportVCard2()
{
    mImportExportAction = Export;
    mExportVCardType = VCard2_1;
    Q_EMIT emitPluginActivated(this);
}

void VCardImportExportPluginInterface::exec()
{
    switch (mImportExportAction) {
    case Import:
        importVCard();
        break;
    case Export:
        exportVCard();
        break;
    }
}

void VCardImportExportPluginInterface::slotImportVCard()
{
    mImportExportAction = Import;
    Q_EMIT emitPluginActivated(this);
}

void VCardImportExportPluginInterface::importVCard()
{
    KContacts::Addressee::List addrList;
    const QString filter = i18n("vCard (*.vcf *.vcard *.vct *.gcrd);;All files (*)");
    const QList<QUrl> urls = QFileDialog::getOpenFileUrls(parentWidget(), i18nc("@title:window", "Select vCard to Import"), QUrl(), filter);

    if (urls.isEmpty()) {
        return;
    }

    const QString caption(i18nc("@title:window", "vCard Import Failed"));

    const int numberOfUrl(urls.count());
    for (int i = 0; i < numberOfUrl; ++i) {
        const QUrl url = urls.at(i);

        auto job = KIO::storedGet(url);
        KJobWidgets::setWindow(job, parentWidget());
        if (job->exec()) {
            const QByteArray data = job->data();
            if (!data.isEmpty()) {
                addrList += parseVCard(data);
            }
        } else {
            const QString msg = xi18nc("@info", "<para>Unable to access vCard:</para><para>%1</para>", job->errorString());
            KMessageBox::error(parentWidget(), msg, caption);
        }
    }
    ContactList contactList;
    contactList.setAddressList(addrList);
    auto engine = new ImportExportEngine(this);
    engine->setContactList(contactList);
    engine->setDefaultAddressBook(defaultCollection());
    engine->importContacts();
}

KContacts::Addressee::List VCardImportExportPluginInterface::parseVCard(const QByteArray &data) const
{
    KContacts::VCardConverter converter;
    return converter.parseVCards(data);
}

KContacts::Addressee::List VCardImportExportPluginInterface::filterContacts(const KContacts::Addressee::List &addrList,
                                                                            ExportSelectionWidget::ExportFields exportFieldType) const
{
    KContacts::Addressee::List list;

    if (addrList.isEmpty()) {
        return addrList;
    }

    KContacts::Addressee::List::ConstIterator it;
    KContacts::Addressee::List::ConstIterator end(addrList.end());
    for (it = addrList.begin(); it != end; ++it) {
        KContacts::Addressee addr;

        addr.setUid(QString());
        addr.setFormattedName((*it).formattedName());

        bool addrDone = false;
        if (exportFieldType & ExportSelectionWidget::DiplayName) { // output display name as N field
            QString fmtName = (*it).formattedName();
            QStringList splitNames = fmtName.split(QLatin1Char(' '), Qt::SkipEmptyParts);
            if (splitNames.count() >= 2) {
                addr.setPrefix(QString());
                addr.setGivenName(splitNames.takeFirst());
                addr.setFamilyName(splitNames.takeLast());
                addr.setAdditionalName(splitNames.join(QLatin1Char(' ')));
                addr.setSuffix(QString());
                addrDone = true;
            }
        }

        if (!addrDone) { // not wanted, or could not be split
            addr.setPrefix((*it).prefix());
            addr.setGivenName((*it).givenName());
            addr.setAdditionalName((*it).additionalName());
            addr.setFamilyName((*it).familyName());
            addr.setSuffix((*it).suffix());
        }

        addr.setExtraNickNameList((*it).extraNickNameList());
        addr.setMailer((*it).mailer());
        addr.setTimeZone((*it).timeZone());
        addr.setGeo((*it).geo());
        addr.setProductId((*it).productId());
        addr.setSortString((*it).sortString());
        addr.setUrl((*it).url());
        addr.setExtraUrlList((*it).extraUrlList());
        addr.setSecrecy((*it).secrecy());
        addr.setSound((*it).sound());
        addr.setEmailList((*it).emailList());
        addr.setCategories((*it).categories());
        addr.setExtraSoundList((*it).extraSoundList());
        addr.setGender((*it).gender());
        addr.setLangs((*it).langs());
        addr.setKind((*it).kind());
        addr.setMembers((*it).members());
        addr.setRelationships((*it).relationships());
        addr.setSourcesUrlList((*it).sourcesUrlList());
        addr.setImppList((*it).imppList());
        addr.setFieldGroupList((*it).fieldGroupList());

        if (exportFieldType & ExportSelectionWidget::Private) {
            addr.setBirthday((*it).birthday());
            addr.setNote((*it).note());
        }

        if (exportFieldType & ExportSelectionWidget::Picture) {
            if (exportFieldType & ExportSelectionWidget::Private) {
                addr.setPhoto((*it).photo());
                addr.setExtraPhotoList((*it).extraPhotoList());
            }

            if (exportFieldType & ExportSelectionWidget::Business) {
                addr.setLogo((*it).logo());
                addr.setExtraLogoList((*it).extraLogoList());
            }
        }

        if (exportFieldType & ExportSelectionWidget::Business) {
            addr.setExtraTitleList((*it).extraTitleList());
            addr.setExtraRoleList((*it).extraRoleList());
            addr.setExtraOrganizationList((*it).extraOrganizationList());
            addr.setDepartment((*it).department());

            KContacts::PhoneNumber::List phones = (*it).phoneNumbers(KContacts::PhoneNumber::Work);
            KContacts::PhoneNumber::List::Iterator phoneIt;
            KContacts::PhoneNumber::List::Iterator phoneEnd(phones.end());
            for (phoneIt = phones.begin(); phoneIt != phoneEnd; ++phoneIt) {
                addr.insertPhoneNumber(*phoneIt);
            }

            KContacts::Address::List addresses = (*it).addresses(KContacts::Address::Work);
            KContacts::Address::List::Iterator addrIt;
            KContacts::Address::List::Iterator addrEnd(addresses.end());
            for (addrIt = addresses.begin(); addrIt != addrEnd; ++addrIt) {
                addr.insertAddress(*addrIt);
            }
        }

        KContacts::PhoneNumber::List phones = (*it).phoneNumbers();
        KContacts::PhoneNumber::List::Iterator phoneIt;
        KContacts::PhoneNumber::List::Iterator phoneEnd(phones.end());
        for (phoneIt = phones.begin(); phoneIt != phoneEnd; ++phoneIt) {
            int phoneType = (*phoneIt).type();

            if ((phoneType & KContacts::PhoneNumber::Home) && (exportFieldType & ExportSelectionWidget::Private)) {
                addr.insertPhoneNumber(*phoneIt);
            } else if ((phoneType & KContacts::PhoneNumber::Work) && (exportFieldType & ExportSelectionWidget::Business)) {
                addr.insertPhoneNumber(*phoneIt);
            } else if ((exportFieldType & ExportSelectionWidget::Other)) {
                addr.insertPhoneNumber(*phoneIt);
            }
        }

        KContacts::Address::List addresses = (*it).addresses();
        KContacts::Address::List::Iterator addrIt;
        KContacts::Address::List::Iterator addrEnd(addresses.end());
        for (addrIt = addresses.begin(); addrIt != addrEnd; ++addrIt) {
            int addressType = (*addrIt).type();

            if ((addressType & KContacts::Address::Home) && exportFieldType & ExportSelectionWidget::Private) {
                addr.insertAddress(*addrIt);
            } else if ((addressType & KContacts::Address::Work) && (exportFieldType & ExportSelectionWidget::Business)) {
                addr.insertAddress(*addrIt);
            } else if (exportFieldType & ExportSelectionWidget::Other) {
                addr.insertAddress(*addrIt);
            }
        }

        if (exportFieldType & ExportSelectionWidget::Other) {
            QStringList exportFields;
            const QStringList customs = (*it).customs();
            for (const QString &customStr : customs) {
                if (!customStr.startsWith(QLatin1String("X-GCALENDAR-groupMembershipInfo"))) {
                    exportFields.append(customStr);
                }
            }
            addr.setCustoms(exportFields);
        }

        if (exportFieldType & ExportSelectionWidget::Encryption) {
            addKey(addr, KContacts::Key::PGP);
            addKey(addr, KContacts::Key::X509);
        }

        list.append(addr);
    }

    return list;
}

void VCardImportExportPluginInterface::addKey(KContacts::Addressee &addr, KContacts::Key::Type type) const
{
#ifdef QGPGME_FOUND
    const QString fingerprint =
        addr.custom(QStringLiteral("KADDRESSBOOK"), (type == KContacts::Key::PGP ? QStringLiteral("OPENPGPFP") : QStringLiteral("SMIMEFP")));
    if (fingerprint.isEmpty()) {
        return;
    }

    GpgME::Context *context = GpgME::Context::createForProtocol(GpgME::OpenPGP);
    if (!context) {
        qCritical() << "No context available";
        return;
    }

    context->setArmor(false);
    context->setTextMode(false);

    QGpgME::QByteArrayDataProvider dataProvider;
    GpgME::Data dataObj(&dataProvider);
    GpgME::Error error = context->exportPublicKeys(fingerprint.toLatin1().constData(), dataObj);
    delete context;

    if (error) {
        qCritical() << error.asString();
        return;
    }

    KContacts::Key key;
    key.setType(type);
    key.setBinaryData(dataProvider.data());

    addr.insertKey(key);
#else
    Q_UNUSED(addr)
    Q_UNUSED(type)
#endif
}

bool VCardImportExportPluginInterface::doExport(const QUrl &url, const QByteArray &data) const
{
    QUrl newUrl(url);
    if (newUrl.isLocalFile() && QFileInfo::exists(newUrl.toLocalFile())) {
        auto dialog = new PimCommon::RenameFileDialog(newUrl, false, parentWidget());
        auto result = static_cast<PimCommon::RenameFileDialog::RenameFileDialogResult>(dialog->exec());
        if (result == PimCommon::RenameFileDialog::RENAMEFILE_RENAME) {
            newUrl = dialog->newName();
        } else if (result == PimCommon::RenameFileDialog::RENAMEFILE_IGNORE) {
            delete dialog;
            return true;
        }
        delete dialog;
    }

    QTemporaryFile tmpFile;
    tmpFile.open();

    tmpFile.write(data);
    tmpFile.flush();
    auto job = KIO::file_copy(QUrl::fromLocalFile(tmpFile.fileName()), newUrl, -1, KIO::Overwrite);
    KJobWidgets::setWindow(job, parentWidget());
    return job->exec();
}

void VCardImportExportPluginInterface::exportVCard()
{
    QPointer<ContactSelectionDialog> dlg = new ContactSelectionDialog(itemSelectionModel(), true, parentWidget());
    dlg->setMessageText(i18n("Which contact do you want to export?"));
    dlg->setDefaultAddressBook(defaultCollection());
    if (!dlg->exec() || !dlg) {
        delete dlg;
        return;
    }

    const KContacts::AddresseeList contacts = dlg->selectedContacts().addressList();
    const ExportSelectionWidget::ExportFields exportFields = dlg->exportType();
    delete dlg;

    if (contacts.isEmpty()) {
        KMessageBox::error(nullptr, i18n("You have not selected any contacts to export."));
        return;
    }

    KContacts::VCardConverter converter;
    QUrl url;

    const KContacts::Addressee::List list = filterContacts(contacts, exportFields);
    if (list.isEmpty()) { // no contact selected
        return;
    }

    bool ok = true;
    if (list.count() == 1) {
        QFileDialog::Options options = QFileDialog::DontConfirmOverwrite;
        QString filename;
        const KContacts::Addressee addr = list.at(0);
        if (!addr.givenName().isEmpty()) {
            filename = addr.givenName();
        }
        if (!addr.familyName().isEmpty()) {
            if (filename.isEmpty()) {
                filename = addr.familyName();
            } else {
                filename += QLatin1Char('_') + addr.familyName();
            }
        }
        if (filename.isEmpty()) {
            if (!addr.emailList().isEmpty()) {
                filename = addr.emailList().at(0).mail();
            } else {
                filename = QStringLiteral("contact");
            }
        }
        filename += QStringLiteral(".vcf");
        url = QFileDialog::getSaveFileUrl(parentWidget(), QString(), QUrl::fromLocalFile(filename), QString(), nullptr, options);
        if (url.isEmpty()) { // user canceled export
            return;
        }

        switch (mExportVCardType) {
        case VCard2_1:
            ok = doExport(url, converter.exportVCards(list, KContacts::VCardConverter::v2_1));
            break;
        case VCard3:
            ok = doExport(url, converter.exportVCards(list, KContacts::VCardConverter::v3_0));
            break;
        case VCard4:
            ok = doExport(url, converter.exportVCards(list, KContacts::VCardConverter::v4_0));
            break;
        }
    } else {
        const int answer = KMessageBox::questionTwoActionsCancel(parentWidget(),
                                                                 i18nc("@info",
                                                                       "You have selected a list of contacts, "
                                                                       "shall they be exported to several files?"),
                                                                 QString(),
                                                                 KGuiItem(i18nc("@action:button", "Export to One File")),
                                                                 KGuiItem(i18nc("@action:button", "Export to Several Files")));

        switch (answer) {
        case KMessageBox::ButtonCode::SecondaryAction: {
            const QUrl baseUrl = QFileDialog::getExistingDirectoryUrl();
            if (baseUrl.isEmpty()) {
                return; // user canceled export
            }

            for (int i = 0; i < list.count(); ++i) {
                const KContacts::Addressee contact = list.at(i);

                url = QUrl::fromLocalFile(baseUrl.path() + QLatin1Char('/') + contactFileName(contact) + QStringLiteral(".vcf"));

                bool tmpOk = false;

                switch (mExportVCardType) {
                case VCard2_1:
                    tmpOk = doExport(url, converter.exportVCard(contact, KContacts::VCardConverter::v2_1));
                    break;
                case VCard3:
                    tmpOk = doExport(url, converter.exportVCard(contact, KContacts::VCardConverter::v3_0));
                    break;
                case VCard4:
                    tmpOk = doExport(url, converter.exportVCard(contact, KContacts::VCardConverter::v4_0));
                    break;
                }
                ok = ok && tmpOk;
            }
            break;
        }
        case KMessageBox::ButtonCode::PrimaryAction: {
            QFileDialog::Options options = QFileDialog::DontConfirmOverwrite;
            url = QFileDialog::getSaveFileUrl(parentWidget(), QString(), QUrl::fromLocalFile(QStringLiteral("addressbook.vcf")), QString(), nullptr, options);
            if (url.isEmpty()) {
                return; // user canceled export
            }

            switch (mExportVCardType) {
            case VCard2_1:
                ok = doExport(url, converter.exportVCards(list, KContacts::VCardConverter::v2_1));
                break;
            case VCard3:
                ok = doExport(url, converter.exportVCards(list, KContacts::VCardConverter::v3_0));
                break;
            case VCard4:
                ok = doExport(url, converter.exportVCards(list, KContacts::VCardConverter::v4_0));
                break;
            }
            break;
        }
        case KMessageBox::Cancel:
        default:
            return; // user canceled export
        }
    }
}

bool VCardImportExportPluginInterface::canImportFileType(const QUrl &url)
{
    return url.path().endsWith(QLatin1String(".vcf"));
}

QString VCardImportExportPluginInterface::contactFileName(const KContacts::Addressee &contact) const
{
    if (!contact.givenName().isEmpty() && !contact.familyName().isEmpty()) {
        return QStringLiteral("%1_%2").arg(contact.givenName(), contact.familyName());
    }

    if (!contact.familyName().isEmpty()) {
        return contact.familyName();
    }

    if (!contact.givenName().isEmpty()) {
        return contact.givenName();
    }

    if (!contact.organization().isEmpty()) {
        return contact.organization();
    }

    return contact.uid();
}

#include "moc_vcardimportexportplugininterface.cpp"
