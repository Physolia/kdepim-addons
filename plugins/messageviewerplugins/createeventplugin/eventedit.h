/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EVENTEDIT_H
#define EVENTEDIT_H

#include <QWidget>

#include <AkonadiCore/Collection>
#include <KMime/Message>
#include <KCalendarCore/Event>

class QLineEdit;
class QPushButton;
namespace Akonadi {
class CollectionComboBox;
}

namespace MessageViewer {
class EventDateTimeWidget;
class EventEdit : public QWidget
{
    Q_OBJECT
public:
    explicit EventEdit(QWidget *parent = nullptr);
    ~EventEdit() override;

    Akonadi::Collection collection() const;
    void setCollection(const Akonadi::Collection &value);

    KMime::Message::Ptr message() const;
    void setMessage(const KMime::Message::Ptr &value);

    void writeConfig();
    void showEventEdit();

public Q_SLOTS:
    void slotCloseWidget();

private Q_SLOTS:
    void slotReturnPressed();
    void slotCollectionChanged(int);
    void slotOpenEditor();
    void slotStartDateTimeChanged(const QDateTime &newDateTime);
    void slotUpdateButtons(const QString &subject);

    void slotEndDateTimeChanged(const QDateTime &newDateTime);
Q_SIGNALS:
    void createEvent(const KCalendarCore::Event::Ptr &event, const Akonadi::Collection &collection);
    void collectionChanged(const Akonadi::Collection &col);
    void messageChanged(const KMime::Message::Ptr &msg);

protected:
    bool eventFilter(QObject *object, QEvent *e) override;
private:
    KCalendarCore::Event::Ptr createEventItem();
    void readConfig();
    void comboboxRowInserted();
    Akonadi::Collection mCollection;
    KMime::Message::Ptr mMessage;
    QLineEdit *mEventEdit = nullptr;
    Akonadi::CollectionComboBox *mCollectionCombobox = nullptr;
    EventDateTimeWidget *mStartDateTimeEdit = nullptr;
    EventDateTimeWidget *mEndDateTimeEdit = nullptr;
    QPushButton *mSaveButton = nullptr;
    QPushButton *mOpenEditorButton = nullptr;
};
}

#endif // EVENTEDIT_H
