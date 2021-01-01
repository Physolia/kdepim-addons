/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VIEWERPLUGINCREATETODOINTERFACE_H
#define VIEWERPLUGINCREATETODOINTERFACE_H

#include <MessageViewer/ViewerPluginInterface>
#include <KCalendarCore/Todo>
#include <AkonadiCore/Collection>
class KActionCollection;
namespace MessageViewer {
class TodoEdit;
class ViewerPluginCreateTodoInterface : public ViewerPluginInterface
{
    Q_OBJECT
public:
    explicit ViewerPluginCreateTodoInterface(KActionCollection *ac, QWidget *parent = nullptr);
    ~ViewerPluginCreateTodoInterface() override;

    void setText(const QString &text) override;
    QList<QAction *> actions() const override;
    void setMessage(const KMime::Message::Ptr &value) override;
    void closePlugin() override;
    void showWidget() override;
    void setMessageItem(const Akonadi::Item &item) override;
    ViewerPluginInterface::SpecificFeatureTypes featureTypes() const override;
    void setCurrentCollection(const Akonadi::Collection &col) override;

private:
    void slotCreateTodo(const KCalendarCore::Todo::Ptr &todoPtr, const Akonadi::Collection &collection);
    void createAction(KActionCollection *ac);
    TodoEdit *widget();

    TodoEdit *mTodoEdit = nullptr;
    QList<QAction *> mAction;
    Akonadi::Item mMessageItem;
};
}
#endif // VIEWERPLUGINCREATETODOINTERFACE_H
