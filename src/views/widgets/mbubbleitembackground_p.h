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

#ifndef MBUBBLEITEMBACKGROUND_H
#define MBUBBLEITEMBACKGROUND_H

#include <MWidgetController>
#include <MWidgetView>
#include "mbubbleitem.h"
#include <MScalableImage>

#include "mbubbleitembackgroundstyle.h"

/*!
 * \internal
 */

class MBubbleItemBackground;

class MBubbleItemBackgroundView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, MBubbleItemBackgroundStyle)

public:
    MBubbleItemBackgroundView(MBubbleItemBackground *controller);
    ~MBubbleItemBackgroundView(){}

protected:
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    virtual bool event(QEvent* event);
    void applyStyle();

private:
  bool rtl;

  MBubbleItemBackground* controller;
  Q_DISABLE_COPY(MBubbleItemBackgroundView)
};

/*
 * Controller
 */

class MBubbleItemBackground : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MWidget) // uses MWidgetModel

public:
    MBubbleItemBackground(QGraphicsItem *parent = 0);
    virtual ~MBubbleItemBackground() {}

    void setMessageType(MBubbleItem::MessageType mt)
    {
        _messageType = mt;
    }

    MBubbleItem::MessageType messageType()
    {
        return _messageType;
    }

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  virtual void cancelEvent(MCancelEvent *event);

Q_SIGNALS:
    void clicked();
    void canceled();

private:
    MBubbleItem::MessageType _messageType;
};

#endif
