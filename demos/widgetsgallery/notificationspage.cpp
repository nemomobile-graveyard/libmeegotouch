/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <MBanner>
#include <MLayout>
#include <MButton>
#include <MLinearLayoutPolicy>
#include <MSeparator>
#include <MPannableViewport>

#include <QGraphicsLinearLayout>

#ifdef HAVE_DBUS
#include <MNotification>
#endif //HAVE_DBUS

NotificationsPage::NotificationsPage()
    : TemplatePage(TemplatePage::DialogsSheetsAndBanners),
      policy(0),
      labelPageTitle(0),
      labelTitleNotification(0),
      labelSubtitleNotification(0),
      textEditTitle(0),
      textEditSubTitle(0),
      labelIconEnable(0),
      checkBoxIconEnable(0),
      labelPreviewEnable(0),
      checkBoxPreview(0)
{
}

void NotificationsPage::createContent()
{
    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    setStyleName(inv("CommonApplicationPage"));

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);


    labelPageTitle = new MLabel(centralWidget());
    labelPageTitle->setStyleName(inv("CommonHeader"));
    policy->addItem(labelPageTitle);

    MSeparator *separator = new MSeparator();
    separator->setStyleName(inv("CommonHeaderDivider"));
    policy->addItem(separator);

    labelTitleNotification = new MLabel(centralWidget());
    labelTitleNotification->setStyleName(inv("CommonFieldLabel"));
    labelTitleNotification->setObjectName("labelTitleNotification");
    policy->addItem(labelTitleNotification);

    textEditTitle = new MTextEdit(MTextEditModel::SingleLine, "Example of source label", centralWidget());
    textEditTitle->setStyleName(inv("CommonSingleInputFieldLabeled"));
    policy->addItem(textEditTitle);

    labelSubtitleNotification = new MLabel(centralWidget());
    labelSubtitleNotification->setStyleName(inv("CommonFieldLabel"));
    policy->addItem(labelSubtitleNotification);

    textEditSubTitle = new MTextEdit(MTextEditModel::SingleLine, "Example of information label", centralWidget());
    textEditSubTitle->setStyleName(inv("CommonSingleInputFieldLabeled"));
    policy->addItem(textEditSubTitle);

    QGraphicsLinearLayout *l1;
    l1 = new QGraphicsLinearLayout(Qt::Horizontal, layout);
    l1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    l1->setContentsMargins(0,0,0,0);
    l1->setSpacing(0);

    MSeparator *separator2 = new MSeparator();
    separator2->setStyleName(inv("CommonHorizontalSeparator"));
    policy->addItem(separator2);

    labelIconEnable = new MLabel(centralWidget());
    labelIconEnable->setStyleName(inv("CommonBodyText"));
    labelIconEnable->setAlignment(Qt::AlignTop);
    labelIconEnable->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    l1->addItem(labelIconEnable);

    checkBoxIconEnable = new MButton(centralWidget());
    checkBoxIconEnable->setStyleName(inv("CommonRightCheckBox"));
    checkBoxIconEnable->setViewType(MButton::checkboxType);
    checkBoxIconEnable->setCheckable(true);
    l1->addItem(checkBoxIconEnable);

    policy->addItem(l1);


    QGraphicsLinearLayout *l2;
    l2 = new QGraphicsLinearLayout(Qt::Horizontal, layout);
    l2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    l2->setContentsMargins(0,0,0,0);
    l2->setSpacing(0);

    labelPreviewEnable = new MLabel(centralWidget());
    labelPreviewEnable->setStyleName(inv("CommonBodyText"));
    labelPreviewEnable->setAlignment(Qt::AlignTop);
    labelPreviewEnable->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    l2->addItem(labelPreviewEnable);

    checkBoxPreview = new MButton(centralWidget());
    checkBoxPreview->setStyleName(inv("CommonRightCheckBox"));
    checkBoxPreview->setViewType(MButton::checkboxType);
    checkBoxPreview->setCheckable(true);
    checkBoxPreview->setChecked(true);
    l2->addItem(checkBoxPreview);
    policy->addItem(l2);
    connect(checkBoxPreview, SIGNAL(clicked()), this, SLOT(toggleNotificationPreviewEnabled()));

    MButton * createNotification = new MButton(centralWidget());
    createNotification->setStyleName(inv("CommonSingleButton"));
    createNotification->setText("Send notification");

    connect(createNotification, SIGNAL(clicked()), this, SLOT(sendNotification()));
    policy->addItem(createNotification, Qt::AlignCenter);

    this->footnote = new MLabel(centralWidget());
    footnote->setStyleName(inv("CommonBodyText"));
    footnote->setWordWrap(true);
    footnote->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    policy->addItem(footnote);
    policy->addStretch();

    retranslateUi();
}


QString NotificationsPage::timedemoTitle()
{
    return "NotificationsPage";
}

void NotificationsPage::sendNotification()
{
#ifdef HAVE_DBUS
    MNotification *notification = new MNotification("widgetsgalleryeventtype.conf", textEditTitle->text(), textEditSubTitle->text());

    if (checkBoxIconEnable->isChecked()) {

        QStringList iconids;

        iconids << "icon-l-activities" << "icon-l-backup" << "icon-l-browser" << "icon-l-calculator"
                << "icon-l-calendar-reminder" << "icon-l-calendar" << "icon-l-camera" << "icon-l-clock"
                << "icon-l-common-video-playback" << "icon-l-contacts" << "icon-l-content-manager"
                << "icon-l-conversation" << "icon-l-documents"
                << "icon-l-drive" << "icon-l-email" << "icon-l-extras" << "icon-l-gallery"
                << "icon-l-games" << "icon-l-maps" << "icon-l-me" << "icon-l-mms" << "icon-l-music"
                << "icon-l-notes" << "icon-l-office-tools" << "icon-l-ovi-store" << "icon-l-ovi"
                << "icon-l-rss" << "icon-l-search" << "icon-l-settings";

        notification->setImage(iconids.at(qrand () % iconids.count()));
    }
    notification->setObjectName("notification");
    notification->publish();
    delete notification;
#endif //HAVE_DBUS
}

void NotificationsPage::toggleNotificationPreviewEnabled()
{
    MApplication::activeWindow()->setNotificationPreviewsVisible(checkBoxPreview->isChecked());
}

void NotificationsPage::retranslateUi()
{
    //% "Notifications"
    setTitle(qtTrId("xx_notificationspage_title"));
    //% "Notifications generator"
    labelPageTitle->setText(qtTrId("xx_notificationspage_page_title"));
    //% "Source label"
    labelTitleNotification->setText(qtTrId("xx_notificationspage_notification_title"));
    //% "Information label"
    labelSubtitleNotification->setText(qtTrId("xx_notificationspage_notification_subtitle"));
    //% "Enable icon"
    labelIconEnable->setText(qtTrId("xx_notificationspage_notification_icon_label"));
    //% "Enable previews"
    labelPreviewEnable->setText(qtTrId("xx_notificationspage_notification_previews_label"));
   //% "Note: the source label and  the information label will appear on inverted position at the events screen"
    footnote->setText(qtTrId("xx_notificationspage_notification_footnote"));

    if (!isContentCreated())
        return;
}
