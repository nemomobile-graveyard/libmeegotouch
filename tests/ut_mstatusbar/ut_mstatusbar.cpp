/****************************************************************************
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
#include <QGraphicsSceneMouseEvent>
#include <QtTest/QtTest>

#include "ut_mstatusbar.h"
#include "mstatusbar.h"
#include "mscenewindowprivate_stub.h"
#include "mwidgetcontroller_stub.h"
#include <MApplication>

MWidgetControllerPrivate::MWidgetControllerPrivate()
{
}

MWidgetControllerPrivate::~MWidgetControllerPrivate()
{
}

MWidgetPrivate::MWidgetPrivate()
{
}

MWidgetPrivate::~MWidgetPrivate()
{
}

QPointF mouseDownPos;
QPointF mouseMovePos;
bool mouseDown;
bool mouseMove;
QPointF QGraphicsSceneMouseEvent::pos() const
{
    if (mouseDown)
        return mouseDownPos;
    else if (mouseMove)
        return mouseMovePos;
    else
        return QPointF();
}

void Ut_MStatusBar::init()
{
    statusBar = new MStatusBar();
    mouseDown = false;
    mouseMove = false;
}

void Ut_MStatusBar::cleanup()
{
    delete statusBar;
}

void Ut_MStatusBar::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mstatusbar";
    app = new MApplication(argc, &app_name);
}

void Ut_MStatusBar::cleanupTestCase()
{
    delete app;
}

void Ut_MStatusBar::testWhenSwipeStatusIndicatorMenuAppears()
{
    mouseDownPos = QPointF(12.24, 20.20); // Arbitary point
    mouseDown = true;
    QGraphicsSceneMouseEvent mouseDownEvent(QEvent::GraphicsSceneMousePress);
    statusBar->sceneEvent(&mouseDownEvent);

    mouseMovePos = QPointF(15.20, 51.10); // Point with y greater than mouseDownPos + SWIPE_THRESHOLD
    mouseMove = true;
    mouseDown = false;
    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    statusBar->sceneEvent(&mouseMoveEvent);

    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    statusBar->sceneEvent(&mouseReleaseEvent);
}

void Ut_MStatusBar::testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear()
{
    mouseDownPos = QPointF(12.24, 20.20); // Arbitary point
    mouseDown = true;
    QGraphicsSceneMouseEvent mouseDownEvent(QEvent::GraphicsSceneMousePress);
    statusBar->sceneEvent(&mouseDownEvent);

    mouseMovePos = QPointF(15.20, 49.10); // Point with y less than mouseDownPos + SWIPE_THRESHOLD
    mouseMove = true;
    mouseDown = false;
    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    statusBar->sceneEvent(&mouseMoveEvent);

    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    statusBar->sceneEvent(&mouseReleaseEvent);
}

QTEST_APPLESS_MAIN(Ut_MStatusBar)
