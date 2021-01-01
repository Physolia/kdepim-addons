/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VIEWERPLUGINCREATEEVENTINTERFACE_H
#define VIEWERPLUGINCREATEEVENTINTERFACE_H

#include <MessageViewer/ViewerPluginInterface>
#include <KCalendarCore/Event>
class KActionCollection;
namespace MessageViewer {
class EventEdit;
class ViewerPluginCreateEventInterface : public ViewerPluginInterface
{
    Q_OBJECT
public:
    explicit ViewerPluginCreateEventInterface(KActionCollection *ac, QWidget *parent = nullptr);
    ~ViewerPluginCreateEventInterface() override;

    void setText(const QString &text) override;
    QList<QAction *> actions() const override;
    void setMessage(const KMime::Message::Ptr &value) override;
    void closePlugin() override;
    void showWidget() override;
    void setMessageItem(const Akonadi::Item &item) override;
    ViewerPluginInterface::SpecificFeatureTypes featureTypes() const override;

private:
    void slotCreateEvent(const KCalendarCore::Event::Ptr &eventPtr, const Akonadi::Collection &collection);
    void createAction(KActionCollection *ac);
    EventEdit *widget();

    Akonadi::Item mMessageItem;
    EventEdit *mEventEdit = nullptr;
    QList<QAction *> mAction;
};
}
#endif // VIEWERPLUGINCREATEEVENTINTERFACE_H
