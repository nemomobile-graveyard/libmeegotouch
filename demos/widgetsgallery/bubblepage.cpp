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

#include <QDir>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QTime>

#include <MImageWidget>
#include <MAbstractCellCreator>
#include <MLabel>
#include <MMessageBox>
#include <MBubbleItem>

#include "utils.h"
#include "bubblepage.h"
#include "bubblelistmodel.h"

static MBubbleItem *createBubbleItem(const QModelIndex& index)
{
    MBubbleItem* bi = new MBubbleItem;
    QVariant data = index.data(Qt::DisplayRole);
    ChatMessageEntry* me = static_cast<ChatMessageEntry*>(data.value<void *>());

    bi->setMessage(me->message);
    bi->setSenderName(me->sendername);
    bi->setTimeStamp(me->timestamp);

    if(me->sendername.isEmpty())
        bi->setMessageType(MBubbleItem::Outgoing);
    else
        bi->setAvatar(me->avatar);

    return bi;
}

BubblePage::BubblePage()
    : TemplatePage(TemplatePage::ListsGridsAndPopups),
      messageBox()
{
}

BubblePage::~BubblePage()
{
}

void BubblePage::createContent()
{
    MApplicationPage::createContent();

    QGraphicsWidget * panel = centralWidget();
    QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(layout);

    BubbleListModel * model = new BubbleListModel;

    for (int i = 0; i < model->rowCount(); ++i)
    {
        MBubbleItem *item =  createBubbleItem(model->index(i,0));
        connect(item, SIGNAL(linkActivated(QString)), this, SLOT(bubbleLinkActivated(QString)));
        connect(item, SIGNAL(bubbleClicked()), this, SLOT(speechBubbleClicked()));
        layout->addItem(item);
    }

    MBubbleItem* expandedItem = new MBubbleItem();
    connect(expandedItem, SIGNAL(bubbleClicked()), this, SLOT(speechBubbleClicked()));
    expandedItem->setMessage("Here are some nice pictures to enjoy while you're working.");
    expandedItem->setSenderName("Tester");
    MImageWidget *avatar = new MImageWidget(expandedItem);
    avatar->setPixmap(QPixmap(QString(MEDIA_DIR) + QDir::separator() + "avatar-cat.png"));
    expandedItem->setAvatar(avatar);
    expandedItem->setTimeStamp("5 min ago");

    MImageWidget *serviceIcon = new MImageWidget(expandedItem);
    serviceIcon->setObjectName("InformationalIcon");
    serviceIcon->setImage("icon-s-common-pending", serviceIcon->minimumSize().toSize());
    MLabel *pendingLabel = new MLabel(expandedItem);
    pendingLabel->setObjectName("InformationalLabelIncoming");
    pendingLabel->setText("pending...");
    expandedItem->addInformationWidget(serviceIcon);
    expandedItem->addInformationWidget(pendingLabel);
    expandedItem->setCommentsString("+3");

    QImage* p0 = new QImage(QString(MEDIA_DIR) + QDir::separator() + "photo_450x450_001.jpg");
    MImageWidget* i0 = new MImageWidget(p0);
    QImage* p1 = new QImage(QString(MEDIA_DIR) + QDir::separator() + "photo_450x450_002.jpg");
    MImageWidget* i1 = new MImageWidget(p1);
    QImage* p2 = new QImage(QString(MEDIA_DIR) + QDir::separator() + "photo_450x450_003.jpg");
    MImageWidget* i2 = new MImageWidget(p2);

    MWidget* widget = new MWidget;
    QGraphicsGridLayout* gl = new QGraphicsGridLayout(widget);
    gl->setSpacing(30);
    gl->addItem(i0, 0,0);
    gl->addItem(i1, 0,1);
    gl->addItem(i2, 0,2);

    expandedItem->setCentralWidget(widget);
    layout->addItem(expandedItem);

    MBubbleItem* secondExpandedItem = new MBubbleItem();
    connect(secondExpandedItem, SIGNAL(bubbleClicked()), this, SLOT(speechBubbleClicked()));
    secondExpandedItem->setMessage("And here are some of my favorites. Works great!");
    secondExpandedItem->setAvatar(QPixmap(QString(MEDIA_DIR) + QDir::separator() + "avatar-cat.png"));
    secondExpandedItem->setMessageType(MBubbleItem::Outgoing);
    secondExpandedItem->setSenderName("");
    secondExpandedItem->setTimeStamp("5 min ago");

    QImage* p4 = new QImage(QString(MEDIA_DIR) + QDir::separator() + "photo_450x450_004.jpg");
    MImageWidget* i4 = new MImageWidget(p4);
    QImage* p5 = new QImage(QString(MEDIA_DIR) + QDir::separator() + "photo_450x450_005.jpg");
    MImageWidget* i5 = new MImageWidget(p5);
    QImage* p6 = new QImage(QString(MEDIA_DIR) + QDir::separator() + "photo_450x450_006.jpg");
    MImageWidget* i6 = new MImageWidget(p6);
    MWidget* widget2 = new MWidget;

    QGraphicsGridLayout* gl2 = new QGraphicsGridLayout(widget2);
    gl2->setSpacing(30);
    gl2->addItem(i4, 0,0);
    gl2->addItem(i5, 0,1);
    gl2->addItem(i6, 0,2);

    secondExpandedItem->setCentralWidget(widget2);

    secondExpandedItem->setCommentsString("+3");
    secondExpandedItem->setThumbsUpString("+6");
    layout->addItem(secondExpandedItem);

    retranslateUi();
}

void BubblePage::retranslateUi()
{
    //% "Speech Bubble"
    setTitle(qtTrId("xx_bubblepage_title"));
    if (!isContentCreated())
        return;
}

QString BubblePage::timedemoTitle()
{
    return "SpeechBubble";
}

void BubblePage::bubbleLinkActivated(QString url)
{
    //% "Link activated"
    messageBox = new MMessageBox(qtTrId("xx_wg_bubblepage_linkactivated") + " " + url);
    messageBox->appear(MSceneWindow::DestroyWhenDone);
}

void BubblePage::speechBubbleClicked()
{
    //% "Speech buuble clicked"
    messageBox = new MMessageBox(qtTrId("xx_wg_bubblepage_bubbleclicked"));
    messageBox->appear(MSceneWindow::DestroyWhenDone);
}
