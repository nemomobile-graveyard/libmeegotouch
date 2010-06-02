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

#ifndef MSCENE_P_H
#define MSCENE_P_H

#include <QTime>
#include <QFile>
#include <QTextStream>

class MSceneManager;
class MOnDisplayChangeEvent;

class MScenePrivate
{
    Q_DECLARE_PUBLIC(MScene)

    struct Fps
    {
        Fps() : frameCount(-1), fps(0.0f) {}
        int frameCount;
        QTime time;
        float fps;
    };
    struct FpsLog
    {
        QTextStream stream;
        QFile output;
    };

public:
    MScenePrivate();
    virtual ~MScenePrivate();
    void setSceneManager(MSceneManager *sceneManager);

    void onDisplayChangeEvent(MOnDisplayChangeEvent *event);
    QList<MWidget *> filterMWidgets(QList<QGraphicsItem *> itemsList);
    void sendEventToMWidgets(QList<MWidget *> itemsList, QEvent *event);

    QBrush fpsBackgroundBrush;
    QPen   boundingRectLinePen;
    QBrush boundingRectFillBrush;

    void showFpsCounter(QPainter *painter, float fps);
    void logFpsCounter(const QTime *timestamp, float fps);

    void touchPointCopyPosToLastPos(QTouchEvent::TouchPoint &point);
    void touchPointCopyMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointCopyMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    bool eventEmulatePinch(QEvent* event);
    bool eventEmulatePan(QEvent* event);
    void fillMarginRectWithPattern(QPainter * painter, const QRectF& rect, int thickness);

protected:
    MScene *q_ptr;
    MSceneManager *manager;

    //Two finger gestures emulation variables
    QTouchEvent::TouchPoint emuPoint1, emuPoint2;
    bool panEmulationEnabled;
    bool pinchEmulationEnabled;

    Fps fps;
    FpsLog fpsLog;
};

#endif // MSCENE_P_H
