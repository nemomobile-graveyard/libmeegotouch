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


#include "mbubbleitem.h"
#include "mbubbleitem_p.h"
#include "mbubbleitemmodel.h"

#include "mimagewidget.h"
#include "mlabel.h"
#include "mwidgetcreator.h"

M_REGISTER_WIDGET(MBubbleItem)

MBubbleItemPrivate::MBubbleItemPrivate()
    : MWidgetControllerPrivate(),
    commentsLabel(NULL),
    thumbsUpLabel(NULL),
    commentsIcon(NULL),
    thumbsUpIcon(NULL)
{
}

MBubbleItemPrivate::~MBubbleItemPrivate()
{
}

void MBubbleItemPrivate::init()
{
    Q_Q(MBubbleItem);
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    q->setSizePolicy(policy);
}

void MBubbleItemPrivate::createCommentsInfo()
{
    Q_Q(MBubbleItem);

    commentsLabel = new MLabel;
    commentsLabel->setParent(q);

    commentsIcon = new MImageWidget;
    commentsIcon->setParent(q);
    commentsIcon->setObjectName("InformationalIcon");
    commentsIcon->setImage("icon-s-common-comments", commentsIcon->minimumSize().toSize());

    q->addInformationWidget(commentsIcon);
    q->addInformationWidget(commentsLabel);

    refreshStyles();
}
void MBubbleItemPrivate::destroyCommentsInfo()
{
    Q_Q(MBubbleItem);

    q->removeInformationWidget(commentsLabel);
    q->removeInformationWidget(commentsIcon);
    if(commentsLabel->parent() == q)
        delete commentsLabel;
    if(commentsIcon->parent() == q)
        delete commentsIcon;
    commentsLabel = NULL;
    commentsIcon = NULL;
}

void MBubbleItemPrivate::createThumbsUpInfo()
{
    Q_Q(MBubbleItem);

    thumbsUpLabel = new MLabel;
    thumbsUpLabel->setParent(q);

    thumbsUpIcon = new MImageWidget;
    thumbsUpIcon->setParent(q);
    thumbsUpIcon->setObjectName("InformationalIcon");
    thumbsUpIcon->setImage("icon-s-common-like", thumbsUpIcon->minimumSize().toSize());

    q->addInformationWidget(thumbsUpIcon);
    q->addInformationWidget(thumbsUpLabel);

    refreshStyles();
}
void MBubbleItemPrivate::destroyThumbsUpInfo()
{
    Q_Q(MBubbleItem);

    q->removeInformationWidget(thumbsUpLabel);
    q->removeInformationWidget(thumbsUpIcon);
    if(thumbsUpLabel->parent() == q)
        delete thumbsUpLabel;
    if(thumbsUpIcon->parent() == q)
        delete thumbsUpIcon;

    thumbsUpLabel = NULL;
    thumbsUpIcon = NULL;
}

void MBubbleItemPrivate::refreshStyles()
{
    Q_Q(MBubbleItem);

    if (q->messageType() == MBubbleItem::Incoming) {
        if (thumbsUpLabel)
            thumbsUpLabel->setObjectName("InformationalLabelIncoming");
        if (commentsLabel)
            commentsLabel->setObjectName("InformationalLabelIncoming");
    } else {
        if (thumbsUpLabel)
            thumbsUpLabel->setObjectName("InformationalLabelOutgoing");
        if (commentsLabel)
            commentsLabel->setObjectName("InformationalLabelOutgoing");
    }
}

MBubbleItem::MBubbleItem(QGraphicsItem *parent)
    : MWidgetController(new MBubbleItemPrivate, new MBubbleItemModel, parent)
{
    Q_D(MBubbleItem);
    
    d->init();
}

MBubbleItem::MBubbleItem(MBubbleItemPrivate *dd, MBubbleItemModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MBubbleItem);
    
    d->init();
}

MBubbleItem::~MBubbleItem()
{
}

MImageWidget* MBubbleItem::avatar() const
{
    return model()->avatar();
}

void MBubbleItem::setAvatar(MImageWidget* avatar)
{
    MImageWidget *oldAvatar = model()->avatar();
    if(oldAvatar == avatar)
        return;

    model()->setAvatar(avatar);
    if(oldAvatar && oldAvatar->parent() == this)
        delete oldAvatar;
}

void MBubbleItem::setAvatar(const QPixmap &avatar)
{
    model()->beginTransaction();

    if (!model()->avatar()) {
        model()->setAvatar(new MImageWidget);
        model()->avatar()->setParent(this);
    }

    model()->avatar()->setPixmap(avatar);
    model()->commitTransaction();
}

QString MBubbleItem::senderName()
{
    return model()->senderName();
}

void MBubbleItem::setSenderName(const QString &senderName)
{
    model()->setSenderName(senderName);
}

QString MBubbleItem::timeStamp()
{
    return model()->timeStamp();
}

void MBubbleItem::setTimeStamp(const QString &timeStamp)
{
    model()->setTimeStamp(timeStamp);
}

QString MBubbleItem::message()
{
    return model()->message();
}

void MBubbleItem::setMessage(const QString &message)
{
    model()->setMessage(message);
}

MBubbleItem::MessageType MBubbleItem::messageType() const
{
    return static_cast<MBubbleItem::MessageType>(model()->messageType());
}

void MBubbleItem::setMessageType(MessageType messageType)
{
    Q_D(MBubbleItem);

    model()->beginTransaction();
    model()->setMessageType(messageType);

    d->refreshStyles();

    model()->commitTransaction();
}

void MBubbleItem::setCentralWidget(QGraphicsWidget* centralWidget)
{
    QGraphicsWidget *oldCentralWidget = model()->centralWidget();
    if(oldCentralWidget == centralWidget)
        return;

    model()->setCentralWidget(centralWidget);
    if(oldCentralWidget && oldCentralWidget->parent() == this)
        delete oldCentralWidget;
}

QGraphicsWidget* MBubbleItem::centralWidget()
{
    return model()->centralWidget();
}

QStack<QGraphicsWidget*> MBubbleItem::informationWidgets()
{
    return model()->informationWidgets();
}

void MBubbleItem::addInformationWidget(QGraphicsWidget *widget)
{
    QStack<QGraphicsWidget*> stack = model()->informationWidgets();
    stack.push(widget);
    model()->setInformationWidgets(stack);
}

void MBubbleItem::removeInformationWidget(QGraphicsWidget *widget)
{
    widget->setParentItem(0);
    widget->setParent(0);
    if (scene())
        scene()->removeItem(widget);

    QStack<QGraphicsWidget*> stack = model()->informationWidgets();
    int index = stack.indexOf(widget);
    if (index >= 0) {
        stack.remove(index);
        model()->setInformationWidgets(stack);
    }
}

QString MBubbleItem::commentsString()
{
    Q_D(MBubbleItem);

    if (d->commentsLabel)
        return model()->commentsString();

    return QString();
}

void MBubbleItem::setCommentsString(const QString &comments)
{
    Q_D(MBubbleItem);

    if(!comments.isEmpty()) {
        if (!d->commentsLabel)
            d->createCommentsInfo();
        Q_ASSERT(d->commentsLabel);
        Q_ASSERT(d->commentsIcon);
        d->commentsLabel->setText(comments);
    } else if (d->commentsLabel) {
        Q_ASSERT(d->commentsIcon);
        d->destroyCommentsInfo();
        Q_ASSERT(!d->commentsLabel);
        Q_ASSERT(!d->commentsIcon);
    }

    model()->setCommentsString(comments);
}

QString MBubbleItem::thumbsUpString()
{
    Q_D(MBubbleItem);

    if (d->thumbsUpLabel)
        return model()->thumbsUpString();

    return QString();
}

void MBubbleItem::setThumbsUpString(const QString &thumbsUp)
{
    Q_D(MBubbleItem);

    if(!thumbsUp.isEmpty()) {
        if (!d->thumbsUpLabel) {
            Q_ASSERT(!d->thumbsUpIcon);
            d->createThumbsUpInfo();
            Q_ASSERT(d->thumbsUpLabel);
            Q_ASSERT(d->thumbsUpIcon);
        }
        d->thumbsUpLabel->setText(thumbsUp);
    } else if (d->thumbsUpLabel != NULL) {
        Q_ASSERT(d->thumbsUpIcon);
        d->destroyThumbsUpInfo();
    }

    model()->setThumbsUpString(thumbsUp);
}

