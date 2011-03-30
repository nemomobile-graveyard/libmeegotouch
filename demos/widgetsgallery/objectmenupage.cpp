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

#include "objectmenupage.h"

#include <QDir>
#include <MLocale>
#include <MLabel>
#include <MContainer>
#include <MLinearLayoutPolicy>
#include <MImageWidget>
#include <MAction>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <MBasicListItem>

#include "utils.h"

ObjectMenuPage::ObjectMenuPage() :
    TemplatePage(TemplatePage::ListsGridsAndPopups),
    containerFriends(0),
    containerAlbums(0)

{
}

ObjectMenuPage::~ObjectMenuPage()
{
}

QString ObjectMenuPage::timedemoTitle()
{
    return "ObjectMenu";
}

void ObjectMenuPage::createContent()
{
    TemplatePage::createContent();

    containerFriends = new MContainer(centralWidget());
    QGraphicsLinearLayout *friendsLayout = new QGraphicsLinearLayout(containerFriends->centralWidget());
    friendsLayout->setContentsMargins(0, 0, 0, 0);
    friendsLayout->setOrientation(Qt::Vertical);
    containerFriends->centralWidget()->setLayout(friendsLayout);
    containerPolicy->addItem(containerFriends);

    containerAlbums = new MContainer(centralWidget());
    QGraphicsLinearLayout *albumLayout = new QGraphicsLinearLayout(Qt::Horizontal, containerAlbums->centralWidget());
    albumLayout->setContentsMargins(0, 0, 0, 0);
    containerAlbums->centralWidget()->setLayout(albumLayout);
    containerPolicy->addItem(containerAlbums);


    QString contactsDir = Utils::contactsDir();

    QDir imagesDir(contactsDir);
    imagesDir.setNameFilters(QStringList() << "*.png");

    QStringList imageContacts = imagesDir.entryList(QDir::Files);

    const char *names[] = { "Susanna Holmes", "Karl Markus", "Jessica Blom", "Karl Markus" };
    const char *numbers[] = { "+345 223 2444567", "+05 2344 455 677", "+05 2344 455 677", "+05 2344 455 677" };

    // Fill the friends
    for (int i = 0; i < 3; ++i) {
        MBasicListItem *item = new MBasicListItem(MBasicListItem::IconWithTitleAndSubtitle, containerFriends->centralWidget());

        // set content uri to dummy contact.
        item->setProperty("contentURI", QString("a.contact"));

        item->setProperty("objectMenuTitle", QString(names[i]));
        if( i == 0 )
            item->setProperty("objectMenuIconId", QString(contactsDir + QDir::separator() + imageContacts[i % imageContacts.size()]));
        else if( i == 1 )
            item->setProperty("objectMenuIconId", QString("icon-m-messaging-smiley-sarcastic"));

        if (imageContacts.size() > 0) {
            MImageWidget *icon = new MImageWidget(new QPixmap(contactsDir + QDir::separator() + imageContacts[i % imageContacts.size()]));
            item->setImageWidget(icon);
        }
        item->setTitle(names[i]);
        item->setSubtitle(numbers[i]);

        //% "Call"
        MAction *action = new MAction(qtTrId("xx_objectmenu_call"), item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        //% "Send message"
        action = new MAction(qtTrId("xx_objectmenu_send_message"), item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        //% "Open contact card"
        action = new MAction(qtTrId("xx_objectmenu_open_contact_card"), item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        //% "Remove"
        action = new MAction(qtTrId("xx_objectmenu_remove"), item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        friendsLayout->addItem(item);
    }


    // Fill the albums
    for (int i = 0; i < 4; ++i) {
        MImageWidget *image = new MImageWidget(containerAlbums->centralWidget());

        // set content uri to dummy album
        image->setProperty("contentURI", QString("a.album"));

        image->setProperty("objectMenuTitle", QString("Album%1").arg(i));

        if (imageContacts.size() > 0) {
            image->setImage(QImage(contactsDir + QDir::separator() + imageContacts[(i+5) % imageContacts.size()]));
        } else {
            QImage im(92, 92, QImage::Format_RGB32);
            im.fill(0xffffffff);
            image->setImage(im);
        }
        image->setPreferredSize(92, 92);

        //% "Play"
        MAction *action = new MAction(qtTrId("xx_objectmenu_Play"), image);
        action->setLocation(MAction::ObjectMenuLocation);
        image->addAction(action);

        //% "Add to playlist"
        action = new MAction(qtTrId("xx_objectmenu_add_to_playlist"), image);
        action->setLocation(MAction::ObjectMenuLocation);
        image->addAction(action);

        //% "Change album art"
        action = new MAction(qtTrId("xx_objectmenu_change_album_art"), image);
        action->setLocation(MAction::ObjectMenuLocation);
        image->addAction(action);

        //% "Remove"
        action = new MAction(qtTrId("xx_objectmenu_remove"), image);
        action->setLocation(MAction::ObjectMenuLocation);
        image->addAction(action);

        albumLayout->addItem(image);
    }

    retranslateUi();
}

void ObjectMenuPage::retranslateUi()
{
    //% "Object menu"
    setTitle(qtTrId("xx_objectmenu_page_title"));
    if (!isContentCreated())
        return;

    //%  "Object Menu "
    //% "is a popup menu of commands opened by long "
    //% "tapping an element. It is used for providing "
    //% "shortcuts to functions related to that "
    //% "particular element.\n\n"
    //% "The primary (single short tap) action of the "
    //% "element should not be duplicated in the Object "
    //% "Menu."
    infoLabel->setText("<a></a>" + qtTrId("xx_objectmenu_page_info_label").replace('\n', "<br>"));

    //% "Friends"
    containerFriends->setTitle(qtTrId("xx_objectmenu_page_container_friends"));
    //% "Favorite Albums"
    containerAlbums->setTitle(qtTrId("xx_objectmenu_page_container_albums"));
}
