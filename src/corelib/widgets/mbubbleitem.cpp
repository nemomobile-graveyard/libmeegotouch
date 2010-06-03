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
}

void MBubbleItemPrivate::createCommentsInfo()
{
    Q_Q(MBubbleItem);

    commentsLabel = new MLabel(q);

    commentsIcon = new MImageWidget(q);
    commentsIcon->setObjectName("InformationalIcon");
    commentsIcon->setImage("icon-s-common-comments", commentsIcon->minimumSize().toSize());

    q->addInformationWidget(commentsIcon);
    q->addInformationWidget(commentsLabel);

    refreshStyles();
}

void MBubbleItemPrivate::createThumbsUpInfo()
{
    Q_Q(MBubbleItem);

    thumbsUpLabel = new MLabel(q);

    thumbsUpIcon = new MImageWidget(q);
    thumbsUpIcon->setObjectName("InformationalIcon");
    thumbsUpIcon->setImage("icon-s-common-like", thumbsUpIcon->minimumSize().toSize());

    q->addInformationWidget(thumbsUpIcon);
    q->addInformationWidget(thumbsUpLabel);

    refreshStyles();
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
    model()->setAvatar(avatar);
}

void MBubbleItem::setAvatar(const QPixmap &avatar)
{
    model()->beginTransaction();
    
    if (!model()->avatar())
        model()->setAvatar(new MImageWidget);

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
    model()->setCentralWidget( centralWidget );
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
    QStack<QGraphicsWidget*> stack = model()->informationWidgets();
    int index = stack.indexOf(widget);
    if (index >= 0)
        stack.remove(index);
    model()->setInformationWidgets(stack);
}

QString MBubbleItem::commentsString()
{
    Q_D(MBubbleItem);

    if (d->commentsLabel)
        return d->commentsLabel->text();
    return QString();
}

void MBubbleItem::setCommentsString(const QString &comments)
{
    Q_D(MBubbleItem);

    if (!d->commentsLabel || !d->commentsIcon)
        d->createCommentsInfo();

    d->commentsLabel->setText(comments);
}

QString MBubbleItem::thumbsUpString()
{
    Q_D(MBubbleItem);

    if (d->thumbsUpLabel)
        return d->thumbsUpLabel->text();

    return QString();
}

void MBubbleItem::setThumbsUpString(const QString &thumbsUp)
{
    Q_D(MBubbleItem);

    if (!d->thumbsUpLabel || !d->thumbsUpIcon)
        d->createThumbsUpInfo();

    d->thumbsUpLabel->setText(thumbsUp);
}

