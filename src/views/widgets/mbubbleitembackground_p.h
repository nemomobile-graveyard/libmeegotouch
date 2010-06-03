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
    virtual bool event(QEvent* event);
    void applyStyle();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void cancelEvent(MCancelEvent *event);

    void setupModel();

protected Q_SLOTS:
    void refreshStyleMode();

private:
  bool mirrored;
  bool down;
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

    void setMessageType(MBubbleItem::MessageType mt);
    MBubbleItem::MessageType messageType();

public Q_SLOTS:
    void click();

Q_SIGNALS:
    void clicked();
    void messageTypeChanged();

private:
    MBubbleItem::MessageType _messageType;
};

#endif
