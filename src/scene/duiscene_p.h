/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUISCENE_P_H
#define DUISCENE_P_H

class DuiSceneManager;
class DuiOnDisplayChangeEvent;

class DuiScenePrivate
{
    Q_DECLARE_PUBLIC(DuiScene)

public:
    DuiScenePrivate();
    virtual ~DuiScenePrivate();
    void setSceneManager(DuiSceneManager *sceneManager);

    void onDisplayChangeEvent(DuiOnDisplayChangeEvent *event);
    QList<DuiWidget *> filterDuiWidgets(QList<QGraphicsItem *> itemsList);
    void sendEventToDuiWidgets(QList<DuiWidget *> itemsList, QEvent *event);

    QBrush fpsBackgroundBrush;
    QPen   boundingRectLinePen;
    QBrush boundingRectFillBrush;

    void touchPointCopyPosToLastPos(QTouchEvent::TouchPoint &point);
    void touchPointCopyMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointCopyMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    bool eventEmulatePinch(QEvent* event);
    bool eventEmulatePan(QEvent* event);

protected:
    DuiScene *q_ptr;
    DuiSceneManager *manager;

    //Two finger gestures emulation variables
    QTouchEvent::TouchPoint emuPoint1, emuPoint2;
    bool panEmulationEnabled;
    bool pinchEmulationEnabled;
};

#endif // DUISCENE_P_H
