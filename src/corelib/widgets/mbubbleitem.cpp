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
#include "mwidgetcreator.h"

M_REGISTER_WIDGET(MBubbleItem)

MBubbleItemPrivate::MBubbleItemPrivate() 
    : MWidgetControllerPrivate()
{
}

MBubbleItemPrivate::~MBubbleItemPrivate()
{
}

void MBubbleItemPrivate::init()
{
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
    model()->beginTransaction();
    
    if (messageType == MBubbleItem::Outgoing)
        model()->setAvatar(NULL);
    
    model()->setMessageType(messageType);
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

void MBubbleItem::addInformationWidget(QGraphicsWidget* item)
{
    QStack<QGraphicsWidget*> stack = model()->informationWidgets();
    stack.push(item);
    model()->setInformationWidgets( stack );
}

QString MBubbleItem::commentsString()
{
    return model()->commentsString();
}

void MBubbleItem::setCommentsString(const QString &comments)
{
    model()->setCommentsString(comments);
}

QString MBubbleItem::thumbsUpString()
{
    return model()->thumbsUpString();
}

void MBubbleItem::setThumbsUpString(const QString &thumbsUp)
{
    model()->setThumbsUpString(thumbsUp);
}

