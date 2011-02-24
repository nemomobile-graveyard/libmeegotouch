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

#include <QtTest/QtTest>
#include <QPainter>
#include <QImage>
#include <QGraphicsSceneMouseEvent>
#include <QGesture>
#include <QGestureEvent>
#include "mwidget.h"
#include "mbutton.h"

#include "ut_mscene.h"
#include "mscene_p.h"
#include "mdeviceprofile.h"

#define MAX_PARAMS 10

class EventTester : public MWidget
{
public:
    EventTester() {
        mousePressReceived = false;
        mouseMoveReceived = false;
        mouseReleaseReceived = false;
        cancelReceived = false;
        touchBeginReceived = false;
        touchUpdateReceived = false;
        touchEndReceived = false;

        setAcceptTouchEvents(true);
    }

    bool mousePressReceived;
    bool mouseMoveReceived;
    bool mouseReleaseReceived;
    bool cancelReceived;
    bool touchBeginReceived;
    bool touchUpdateReceived;
    bool touchEndReceived;

    void mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
    {
        mousePressReceived = true;
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent */*event*/)
    {
        mouseMoveReceived = true;
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/)
    {
        mouseReleaseReceived = true;
    }

    void cancelEvent(MCancelEvent */*event*/)
    {
        cancelReceived = true;
    }

    bool event(QEvent *event)
    {
        switch (event->type())
        {
        case QEvent::TouchBegin:
            touchBeginReceived = true;
            event->accept();
            return true;
        case QEvent::TouchUpdate:
            touchUpdateReceived = true;
            event->accept();
            return true;
        case QEvent::TouchEnd:
            touchEndReceived = true;
            event->accept();
            return true;
        default:
            return MWidget::event(event);
        }
    }
};

EventTester *eventTester = 0;

Qt::GestureState currentPanState = Qt::NoGesture;
Qt::GestureState QGesture::state() const
{
    return currentPanState;
}

void Ut_MScene::initTestCase()
{
    QChar sep(' ');
    static char *argv[MAX_PARAMS];
    static int x = 0;
    QString params("./ut_mscene -software -show-br -show-fps -show-size -show-position");

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }
    x = 6;
    app = new MApplication(x, argv);
}

void Ut_MScene::cleanupTestCase()
{
    delete app;
    app = 0;
}

void Ut_MScene::init()
{
    eventTester = new EventTester;
    m_subject = new MScene();

    m_subject->addItem(eventTester);
    eventTester->resize(1000,1000);
}

void Ut_MScene::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MScene::drawForeground()
{
    QPixmap *p = new QPixmap(300, 300);
    p->fill(QColor(255, 255, 255, 0));
    QPainter *myPainter = new QPainter(p);

    m_subject->drawForeground(myPainter, QRect(0, 0, 300, 300));
    QImage img1 = p->toImage();

    m_subject->addItem(new MButton("foobar"));
    p->fill(QColor(255, 255, 255, 0));
    m_subject->drawForeground(myPainter, QRect(0, 0, 300, 300));
    QImage img2 = p->toImage();
    QVERIFY(img1 != img2);

    QVERIFY(img1 == img1);
}

void Ut_MScene::mousePressAndReleaseAreDeliveredToGrabber()
{
    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setButton(Qt::LeftButton);
    mousePress.setPos(QPointF(100,100));
    mousePress.setScreenPos(QPoint(100,100));
    mousePress.setScenePos(QPointF(100,100));

    QGraphicsSceneMouseEvent mouseRelease(QEvent::GraphicsSceneMouseRelease);
    mouseRelease.setButton(Qt::LeftButton);
    mouseRelease.setPos(QPointF(100,100));
    mouseRelease.setScreenPos(QPoint(100,100));
    mouseRelease.setScenePos(QPointF(100,100));

    QApplication::sendEvent(m_subject,&mousePress);

    QCOMPARE(eventTester->mousePressReceived, true);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, false);
    eventTester->mousePressReceived = false;

    QApplication::sendEvent(m_subject,&mouseRelease);

    QCOMPARE(eventTester->mousePressReceived, false);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, true);

}

void Ut_MScene::touchBeginAndEndAreDeliveredToGrabber()
{
    QList<QTouchEvent::TouchPoint> touchBeginList;
    QTouchEvent::TouchPoint touchPointBegin;
    touchPointBegin.setState(Qt::TouchPointPressed);
    touchPointBegin.setPos(QPointF(100,100));
    touchPointBegin.setScenePos(QPointF(100,100));
    touchPointBegin.setScreenPos(QPoint(100,100));
    touchBeginList.append(touchPointBegin);
    QTouchEvent touchBeginEvent(QEvent::TouchBegin, QTouchEvent::TouchPad, Qt::NoModifier, Qt::TouchPointPressed, touchBeginList);

    QList<QTouchEvent::TouchPoint> touchEndList;
    QTouchEvent::TouchPoint touchPointEnd;
    touchPointEnd.setState(Qt::TouchPointReleased);
    touchPointEnd.setPos(QPointF(100,100));
    touchPointEnd.setScenePos(QPointF(100,100));
    touchPointEnd.setScreenPos(QPoint(100,100));
    touchEndList.append(touchPointEnd);
    QTouchEvent touchEndEvent(QEvent::TouchEnd, QTouchEvent::TouchPad, Qt::NoModifier, Qt::TouchPointReleased, touchEndList);

    QApplication::sendEvent(m_subject,&touchBeginEvent);

    QCOMPARE(eventTester->touchBeginReceived, true);
    QCOMPARE(eventTester->touchUpdateReceived, false);
    QCOMPARE(eventTester->touchEndReceived, false);
    eventTester->touchBeginReceived = false;

    QApplication::sendEvent(m_subject,&touchEndEvent);

    QCOMPARE(eventTester->touchBeginReceived, false);
    QCOMPARE(eventTester->touchUpdateReceived, false);
    QCOMPARE(eventTester->touchEndReceived, true);
}

void Ut_MScene::mouseMoveIsDelieveredToGrabberIfNoGestureIsRecognized()
{
    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setButton(Qt::LeftButton);
    mousePress.setPos(QPointF(100,100));
    mousePress.setScreenPos(QPoint(100,100));
    mousePress.setScenePos(QPointF(100,100));

    QGraphicsSceneMouseEvent mouseMove(QEvent::GraphicsSceneMouseMove);
    mouseMove.setButton(Qt::LeftButton);
    mouseMove.setPos(QPointF(150,100));
    mouseMove.setScreenPos(QPoint(150,100));
    mouseMove.setScenePos(QPointF(150,100));

    QApplication::sendEvent(m_subject,&mousePress);

    QCOMPARE(eventTester->mousePressReceived, true);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, false);
    eventTester->mousePressReceived = false;

    QApplication::sendEvent(m_subject,&mouseMove);

    QCOMPARE(eventTester->mousePressReceived, false);
    QCOMPARE(eventTester->mouseMoveReceived, true);
    QCOMPARE(eventTester->mouseReleaseReceived, false);
}

void Ut_MScene::mouseReleaseResetsStateOfEventEater()
{
    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setButton(Qt::LeftButton);
    mousePress.setPos(QPointF(100,100));
    mousePress.setScreenPos(QPoint(100,100));
    mousePress.setScenePos(QPointF(100,100));

    QGraphicsSceneMouseEvent mouseRelease(QEvent::GraphicsSceneMouseRelease);
    mouseRelease.setButton(Qt::LeftButton);
    mouseRelease.setPos(QPointF(100,100));
    mouseRelease.setScreenPos(QPoint(100,100));
    mouseRelease.setScenePos(QPointF(100,100));

    QApplication::sendEvent(m_subject,&mousePress);

    QCOMPARE(eventTester->mousePressReceived, true);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, false);
    eventTester->mousePressReceived = false;

    QPanGesture panGesture;
    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent gestureEvent(gestureList);
    currentPanState = Qt::GestureStarted;

    QApplication::sendEvent(m_subject,&gestureEvent);
    m_subject->d_func()->notifyChildRequestedMouseCancel();

    currentPanState = Qt::GestureFinished;
    QApplication::sendEvent(m_subject,&gestureEvent);

    QCOMPARE(m_subject->mouseGrabberItem(), m_subject->d_func()->eventEater);

    QApplication::sendEvent(m_subject,&mouseRelease);

    QVERIFY(m_subject->mouseGrabberItem() != m_subject->d_func()->eventEater);
}

void Ut_MScene::panGestureCancelsMouseEvents()
{
    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setButton(Qt::LeftButton);
    mousePress.setPos(QPointF(100,100));
    mousePress.setScreenPos(QPoint(100,100));
    mousePress.setScenePos(QPointF(100,100));

    QGraphicsSceneMouseEvent mouseRelease(QEvent::GraphicsSceneMouseRelease);
    mouseRelease.setButton(Qt::LeftButton);
    mouseRelease.setPos(QPointF(100,100));
    mouseRelease.setScreenPos(QPoint(100,100));
    mouseRelease.setScenePos(QPointF(100,100));

    QApplication::sendEvent(m_subject,&mousePress);

    QCOMPARE(eventTester->mousePressReceived, true);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, false);
    eventTester->mousePressReceived = false;

    QPanGesture panGesture;
    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent gestureEvent(gestureList);
    currentPanState = Qt::GestureStarted;

    QApplication::sendEvent(m_subject,&gestureEvent);

    m_subject->d_func()->notifyChildRequestedMouseCancel();

    currentPanState = Qt::GestureFinished;
    QApplication::sendEvent(m_subject,&gestureEvent);

    QCOMPARE(m_subject->mouseGrabberItem(), m_subject->d_func()->eventEater);

    QApplication::sendEvent(m_subject,&mouseRelease);
    QCOMPARE(eventTester->mousePressReceived, false);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, false);
    QCOMPARE(eventTester->cancelReceived, true);
}

void Ut_MScene::ignoredGestureShouldNotCancelMouseEvents()
{
    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setButton(Qt::LeftButton);
    mousePress.setPos(QPointF(100,100));
    mousePress.setScreenPos(QPoint(100,100));
    mousePress.setScenePos(QPointF(100,100));

    QGraphicsSceneMouseEvent mouseRelease(QEvent::GraphicsSceneMouseRelease);
    mouseRelease.setButton(Qt::LeftButton);
    mouseRelease.setPos(QPointF(100,100));
    mouseRelease.setScreenPos(QPoint(100,100));
    mouseRelease.setScenePos(QPointF(100,100));

    QApplication::sendEvent(m_subject,&mousePress);

    QCOMPARE(eventTester->mousePressReceived, true);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, false);
    eventTester->mousePressReceived = false;

    QPanGesture panGesture;
    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent gestureEvent(gestureList);
    currentPanState = Qt::GestureStarted;

    QApplication::sendEvent(m_subject,&gestureEvent);

    currentPanState = Qt::GestureFinished;
    QApplication::sendEvent(m_subject,&gestureEvent);

    QApplication::sendEvent(m_subject,&mouseRelease);
    QCOMPARE(eventTester->mousePressReceived, false);
    QCOMPARE(eventTester->mouseMoveReceived, false);
    QCOMPARE(eventTester->mouseReleaseReceived, true);
    QCOMPARE(eventTester->cancelReceived, false);
}

void Ut_MScene::touchPointCopyPosToLastPos()
{
    qreal  xy1 = 0.1,xy2 = 0.2,xy3 = 0.3;
    QPointF  pos(xy1,xy1),scenePos(xy2,xy2),screenPos(xy3,xy3);

    QTouchEvent::TouchPoint  touchPoint;

    touchPoint.setPos(pos);
    touchPoint.setScenePos(scenePos);
    touchPoint.setScreenPos(screenPos);  

    MScenePrivate  mscenePrivate;
    mscenePrivate.touchPointCopyPosToLastPos(touchPoint);

    QVERIFY(qFuzzyCompare ( touchPoint.lastPos().rx(), xy1 ));
    QVERIFY(qFuzzyCompare ( touchPoint.lastPos().ry(), xy1 ));

    QVERIFY(qFuzzyCompare ( touchPoint.lastScenePos().rx(), xy2 ));
    QVERIFY(qFuzzyCompare ( touchPoint.lastScenePos().ry(), xy2 ));

    QVERIFY(qFuzzyCompare ( touchPoint.lastScreenPos().rx(), xy3 ));
    QVERIFY(qFuzzyCompare ( touchPoint.lastScreenPos().ry(), xy3 ));
}

void Ut_MScene::touchPointCopyMousePosToPointPos()
{
    qreal  xy1 = 1,xy2 = 2,xy3 = 3;
    QPoint  pos(xy1,xy1),scenePos(xy2,xy2),screenPos(xy3,xy3);

    QTouchEvent::TouchPoint  touchPoint;

    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setPos(pos);
    mousePress.setScenePos(scenePos);
    mousePress.setScreenPos(screenPos);


    MScenePrivate  mscenePrivate;
    mscenePrivate.touchPointCopyMousePosToPointPos(touchPoint,&mousePress);

    QVERIFY(qFuzzyCompare ( touchPoint.pos().rx(), xy1 ));
    QVERIFY(qFuzzyCompare ( touchPoint.pos().ry(), xy1 ));

    QVERIFY(qFuzzyCompare ( touchPoint.scenePos().rx(), xy2 ));
    QVERIFY(qFuzzyCompare ( touchPoint.scenePos().ry(), xy2 ));

    QVERIFY(qFuzzyCompare ( touchPoint.screenPos().rx(), xy3 ));
    QVERIFY(qFuzzyCompare ( touchPoint.screenPos().ry(), xy3 ));      
}

void Ut_MScene::touchPointCopyMousePosToPointStartPos()
{
    qreal  xy1 = 1,xy2 = 2,xy3 = 3;
    QPoint  pos(xy1,xy1),scenePos(xy2,xy2),screenPos(xy3,xy3);

    QTouchEvent::TouchPoint  touchPoint;

    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setPos(pos);
    mousePress.setScenePos(scenePos);
    mousePress.setScreenPos(screenPos);


    MScenePrivate  mscenePrivate;
    mscenePrivate.touchPointCopyMousePosToPointStartPos(touchPoint,&mousePress);

    QVERIFY(qFuzzyCompare ( touchPoint.startPos().rx(), xy1 ));
    QVERIFY(qFuzzyCompare ( touchPoint.startPos().ry(), xy1 ));

    QVERIFY(qFuzzyCompare ( touchPoint.startScenePos().rx(), xy2 ));
    QVERIFY(qFuzzyCompare ( touchPoint.startScenePos().ry(), xy2 ));

    QVERIFY(qFuzzyCompare ( touchPoint.startScreenPos().rx(), xy3 ));
    QVERIFY(qFuzzyCompare ( touchPoint.startScreenPos().ry(), xy3 ));      
}

void Ut_MScene::touchPointMirrorMousePosToPointPos()
{
    qreal  xy1 = 300;
    QPoint  screenPos(xy1,xy1);

    QTouchEvent::TouchPoint  touchPoint;

    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setScreenPos(screenPos);

    MWindow * mwindow= new MWindow;
    mwindow->setScene(m_subject);

    QSize resolution = MDeviceProfile::instance()->resolution();
    qreal mirror_x = resolution.width(),mirror_y = resolution.height();
    mirror_x -= xy1;
    mirror_y -= xy1;

    MScenePrivate * mscenePrivate= m_subject->d_func();
    mscenePrivate->touchPointMirrorMousePosToPointPos(touchPoint,&mousePress);

    QVERIFY(qFuzzyCompare ( touchPoint.pos().rx(), mirror_x ));
    QVERIFY(qFuzzyCompare ( touchPoint.pos().ry(), mirror_y ));

    QVERIFY(qFuzzyCompare ( touchPoint.scenePos().rx(), mirror_x ));
    QVERIFY(qFuzzyCompare ( touchPoint.scenePos().ry(), mirror_y ));

    QVERIFY(qFuzzyCompare ( touchPoint.screenPos().rx(), mirror_x ));
    QVERIFY(qFuzzyCompare ( touchPoint.screenPos().ry(), mirror_y ));
}

void Ut_MScene::touchPointMirrorMousePosToPointStartPos()
{
    qreal  xy1 = 300;
    QPoint  screenPos(xy1,xy1);

    QTouchEvent::TouchPoint  touchPoint;

    QGraphicsSceneMouseEvent mousePress(QEvent::GraphicsSceneMousePress);
    mousePress.setScreenPos(screenPos);

    MWindow * mwindow= new MWindow;
    mwindow->setScene(m_subject);

    QSize resolution = MDeviceProfile::instance()->resolution();
    qreal mirror_x = resolution.width(),mirror_y = resolution.height();
    mirror_x -= xy1;
    mirror_y -= xy1;

    MScenePrivate * mscenePrivate= m_subject->d_func();
    mscenePrivate->touchPointMirrorMousePosToPointStartPos(touchPoint,&mousePress);

    QVERIFY(qFuzzyCompare ( touchPoint.startPos().rx(), mirror_x ));
    QVERIFY(qFuzzyCompare ( touchPoint.startPos().ry(), mirror_y ));

    QVERIFY(qFuzzyCompare ( touchPoint.startScenePos().rx(), mirror_x ));
    QVERIFY(qFuzzyCompare ( touchPoint.startScenePos().ry(), mirror_y ));

    QVERIFY(qFuzzyCompare ( touchPoint.startScreenPos().rx(), mirror_x ));
    QVERIFY(qFuzzyCompare ( touchPoint.startScreenPos().ry(), mirror_y ));
}

QTEST_APPLESS_MAIN(Ut_MScene)
