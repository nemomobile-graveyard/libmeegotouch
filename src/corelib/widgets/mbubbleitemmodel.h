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

#ifndef MBUBBLEITEMMODEL_H__
#define MBUBBLEITEMMODEL_H__

#include <mwidgetmodel.h>
#include <QStack>

class QPixmap;
class QGraphicsWidget;
class MImageWidget;

class M_CORE_EXPORT MBubbleItemModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MBubbleItemModel)

public:
    /*!
        \property MBubbleItemModel::avatar
        \brief Avatar.
    */
    M_MODEL_PTR_PROPERTY(MImageWidget *, avatar, Avatar, true, 0)

    /*!
        \property MBubbleItemModel::senderName
        \brief Sender name.
    */
    M_MODEL_PROPERTY(QString, senderName, SenderName, true, QString())

    /*!
        \property MBubbleItemModel::timeStamp
        \brief Timestamp.
    */
    M_MODEL_PROPERTY(QString, timeStamp, TimeStamp, true, QString())

    /*!
      \property MBubbleItemModel::message
      \brief Message.
      */
    M_MODEL_PROPERTY(QString, message, Message, true, QString())

    /*!
      \property MBubbleItemModel::centralWidget
      \brief CentralWidget.
      */
    M_MODEL_PTR_PROPERTY(QGraphicsWidget *, centralWidget, CentralWidget, true, NULL)

    /*!
      \property MBubbleItemModel::informationWidgets
      \brief Stack of items shown in the header, for example icons
      */
    M_MODEL_PROPERTY(QStack<QGraphicsWidget*>, informationWidgets, InformationWidgets, true, QStack<QGraphicsWidget*>())

    /*!
      \property MBubbleItemModel::type
      \brief centralWidget.
      */
    M_MODEL_PTR_PROPERTY(int, messageType, MessageType, true, 0)

    /*!
      \property MBubbleItemModel::commentsString
      \brief commentsString.
      */
    M_MODEL_PROPERTY(QString, commentsString, CommentsString, true, QString())

    /*!
      \property MBubbleItemModel::thumbsUpString
      \brief thumbsUpString.
      */
    M_MODEL_PROPERTY(QString, thumbsUpString, ThumbsUpString, true, QString())
};

#endif
