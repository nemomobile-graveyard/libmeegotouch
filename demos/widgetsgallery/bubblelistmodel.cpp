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

#include "bubblelistmodel.h"

#include <QStringList>
#include <QTime>
#include <QDir>

BubbleListModel::BubbleListModel( QObject* parent )
    : QAbstractListModel(parent),
      chatMessages()
{

    QString avatar_1(QDir(CONTACTS_DIR).canonicalPath() + QDir::separator() +"avatar-jets.png");
    QString avatar_2(QDir(CONTACTS_DIR).canonicalPath() + QDir::separator() +"avatar-butterfly.png");

    chatMessages
    << new ChatMessageEntry( "Hi!", "Manager", avatar_1,"Yesterday, "+QTime::currentTime().toString() )
    << new ChatMessageEntry( "Hi, what's up.", "", avatar_2,QTime::currentTime().toString() )
    << new ChatMessageEntry( "Where is the layout for the speech bubble?", "Manager", avatar_1,QTime::currentTime().toString() )
    << new ChatMessageEntry( "I am working on it. It will be ready on Friday ... seriously, I promise!", "", avatar_2,QTime::currentTime().toString() )
    << new ChatMessageEntry( "Just make sure it's the best speech bubble ever, OK?", "Manager", avatar_1,QTime::currentTime().toString() )
    << new ChatMessageEntry( "Sure. This will be better than sliced bread. I'll just finish it really quick and add it to git. If there are problems, call me at <a href=\"#35802298784\">+35802298784</a>.", "", avatar_2,QTime::currentTime().toString() )
    ;
}

BubbleListModel::~BubbleListModel()
{
    qDeleteAll(chatMessages);
    chatMessages.clear();
}

int BubbleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return chatMessages.size();
}

QVariant BubbleListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant::fromValue( (void*) chatMessages.at( index.row() ) );

    return QVariant();
}
