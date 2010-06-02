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
#include <MContentItem>

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
    friendsLayout->setOrientation(Qt::Vertical);
    containerFriends->centralWidget()->setLayout(friendsLayout);
    containerPolicy->addItem(containerFriends);

    containerAlbums = new MContainer(centralWidget());
    QGraphicsLinearLayout *albumLayout = new QGraphicsLinearLayout(Qt::Horizontal, containerAlbums->centralWidget());
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
        MContentItem *item = new MContentItem(MContentItem::IconAndTwoTextLabels, containerFriends->centralWidget());
        item->setItemMode(MContentItem::Single);

        // set content uri to dummy contact.
        item->setProperty("contentURI", QString("a.contact"));

        if (imageContacts.size() > 0) {
            item->setPixmap(QPixmap(contactsDir + QDir::separator() + imageContacts[i % imageContacts.size()]));
        }
        item->setTitle(names[i]);
        item->setSubtitle(numbers[i]);

        MAction *action = new MAction("Call", item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        action = new MAction("Send message", item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        action = new MAction("Open contact card", item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        action = new MAction("Remove", item);
        action->setLocation(MAction::ObjectMenuLocation);
        item->addAction(action);

        friendsLayout->addItem(item);
    }


    // Fill the albums
    for (int i = 0; i < 4; ++i) {
        MImageWidget *image = new MImageWidget(containerAlbums->centralWidget());

        // set content uri to dummy album
        image->setProperty("contentURI", QString("a.album"));

        if (imageContacts.size() > 0) {
            image->setImage(QImage(contactsDir + QDir::separator() + imageContacts[(i+5) % imageContacts.size()]));
        } else {
            QImage im(92, 92, QImage::Format_RGB32);
            im.fill(0xffffffff);
            image->setImage(im);
        }
        image->setPreferredSize(92, 92);

        MAction *action = new MAction("Play", image);
        action->setLocation(MAction::ObjectMenuLocation);
        image->addAction(action);

        action = new MAction("Add to playlist", image);
        action->setLocation(MAction::ObjectMenuLocation);
        image->addAction(action);

        action = new MAction("Change album art", image);
        action->setLocation(MAction::ObjectMenuLocation);
        image->addAction(action);

        action = new MAction("Remove", image);
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
    infoLabel->setText("<a></a>" + qtTrId("xx_objectmenu_page_info_label"));

    //% "Friends"
    containerFriends->setTitle(qtTrId("xx_objectmenu_page_container_friends"));
    //% "Favorite Albums"
    containerAlbums->setTitle(qtTrId("xx_objectmenu_page_container_albums"));
}
