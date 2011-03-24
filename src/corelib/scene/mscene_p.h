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
#include <QList>

class MSceneManager;
class MOnDisplayChangeEvent;
class QGraphicsSceneMouseEvent;
class QTouchEvent;

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
    void drawObjectNames(QPainter *painter, QList<QGraphicsItem *>::iterator item);
    void drawStyleNames(QPainter *painter, QList<QGraphicsItem *>::iterator item);

    void touchPointCopyPosToLastPos(QTouchEvent::TouchPoint &point);
    void touchPointCopyMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointCopyMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointMirrorMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);
    void touchPointMirrorMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event);

    bool eventEmulateTwoFingerGestures(QEvent *event);
    bool eventEmulatePinch(QEvent *event);
    void fillMarginRectWithPattern(QPainter *painter, const QRectF& rect, int thickness);

    void resetMouseGrabber();
    void sendCancelEvent();

    void notifyChildRequestedMouseCancel();

    // These two methods are static so that they can be called from scenemanager-less MWindows
    // for which mscene presence is not guaranteed.
    static void sendEventToAllSubLayouts(QGraphicsLayout *layout, QEvent *event);
    static void notifySceneAboutOrientationChange(QGraphicsScene *scene, M::Orientation orientation);

protected:
    MScene *q_ptr;
    MSceneManager *manager;

    QGraphicsWidget* eventEater;
    bool cancelSent;

    //Two finger gestures emulation variables
    QTouchEvent::TouchPoint emuPoint1, emuPoint2;
    bool pinchEmulationEnabled;
    QFontMetrics metrics;

    Fps fps;
    FpsLog fpsLog;

#ifdef UNIT_TEST
    friend class Ut_MScene;
#endif // UNIT_TEST
};

#endif // MSCENE_P_H
