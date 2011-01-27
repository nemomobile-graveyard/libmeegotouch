/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "notificationspage.h"

#include <MApplication>
#include <MAbstractCellCreator>
#include <MBasicListItem>
#include <MBanner>
#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

#include <QStringListModel>
#include <QTimer>
#include <MDialog>
#include <QGraphicsLinearLayout>

#ifdef HAVE_DBUS
#include <MNotification>
#endif //HAVE_DBUS

class NotificationsPageCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    NotificationsPageCellCreator() : MAbstractCellCreator<MBasicListItem>() {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MBasicListItem *cell = dynamic_cast<MBasicListItem *>(recycler.take(MBasicListItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new MBasicListItem(MBasicListItem::SingleTitle);
            cell->initLayout();
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MBasicListItem *item = qobject_cast<MBasicListItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());
    }
};

NotificationsPage::NotificationsPage()
    : TemplatePage(TemplatePage::DialogsSheetsAndBanners),
      policy(0),
      list(0),
      notificationPreviewsEnabled(false),
      notificationPreviewEnabledLabel(0)
{
}

void NotificationsPage::createContent()
{
    MApplicationPage::createContent();

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);

    populateLayout();

    retranslateUi();
}

void NotificationsPage::populateLayout()
{    
    notificationPreviewEnabledLabel = new MLabel(previewEnabledString(notificationPreviewsEnabled), centralWidget());
    policy->addItem(notificationPreviewEnabledLabel);

    list = new MList(centralWidget());
    list->setCellCreator(new NotificationsPageCellCreator());
    list->setItemModel(new QStringListModel(list));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void NotificationsPage::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0)
        sendNotification();
    else if (index.row() == 1)
        toggleNotificationPreviewEnabled();
}

QString NotificationsPage::timedemoTitle()
{
    return "NotificationsPage";
}

void NotificationsPage::sendNotification()
{
#ifdef HAVE_DBUS
    MNotification *notification = new MNotification("widgetsgalleryeventtype.conf", "This is a notification", "This is a notification");
    notification->publish();
    delete notification;
#endif //HAVE_DBUS
}

void NotificationsPage::toggleNotificationPreviewEnabled()
{
    notificationPreviewsEnabled = !notificationPreviewsEnabled;
    MApplication::activeWindow()->setNotificationPreviewsVisible(notificationPreviewsEnabled);

    notificationPreviewEnabledLabel->setText(previewEnabledString(notificationPreviewsEnabled));
}

void NotificationsPage::retranslateUi()
{
    //% "Notifications"
    setTitle(qtTrId("xx_notificationspage_title"));
    if (!isContentCreated())
        return;

    QStringList bannerTypes;
    //% "Send notification"
    bannerTypes << qtTrId("xx_wg_notifications_page_send_notification");
    //% "Toggle preview banner shown"
    bannerTypes << qtTrId("xx_wg_notifications_page_toggle_preview_banner_show");

    static_cast<QStringListModel *>(list->itemModel())->setStringList(bannerTypes);
}

QString NotificationsPage::previewEnabledString(const bool &enabled) const
{
    return (enabled ? "Previews shown" : "Previews not shown");
}
