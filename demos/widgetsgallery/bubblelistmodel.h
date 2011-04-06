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

#ifndef BUBBLELISTMODEL_H
#define BUBBLELISTMODEL_H

#include <QAbstractItemModel>
#include <QList>

struct ChatMessageEntry {
    QString message;
    QString sendername;
    QString avatar;
    QString timestamp;

    ChatMessageEntry( QString _message, QString _sendername, QString _avatar, QString _timestamp )
    : message(_message), sendername(_sendername), avatar(_avatar), timestamp(_timestamp)
    {}
};

class BubbleListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    BubbleListModel(QObject *parent = 0);
    virtual ~BubbleListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QList<ChatMessageEntry*> chatMessages;
};

#endif // MODEL_H
