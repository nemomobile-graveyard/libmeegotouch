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

#include "ut_mpannablewidget.h"
#include "mpannablewidget.h"
#include "mpannablewidget_p.h"
#include "mcancelevent.h"

#include <QGestureEvent>
#include <QPanGesture>

#include <QDebug>

class DummyGraphicsItem : public MWidget
{
public:
    DummyGraphicsItem() {
        mousePressReceived = false;
        mouseMoveReceived = false;
        mouseReleaseReceived = false;
        cancelReceived = false;
    }

    bool mousePressReceived;
    bool mouseMoveReceived;
    bool mouseReleaseReceived;
    bool cancelReceived;
};
DummyGraphicsItem *dummyItem = 0;

struct PhysicsState {

    PhysicsState() {
        pointerPressed = false;
        pointerMoved = false;
        pointerReleased = false;
        physicsStopped = false;
    }

    bool pointerPressed;
    bool pointerMoved;
    bool pointerReleased;
    bool physicsStopped;
};
PhysicsState *physicsState = 0;

struct MouseGrabberState {

    MouseGrabberState() {
        grabbedMouse = false;
        ungrabbedMouse = false;
    }

    bool grabbedMouse;
    bool ungrabbedMouse;
};
MouseGrabberState *mouseGrabberState = 0;

//Stubs

//Physics stubs:
void MPhysics2DPanning::start()
{
}

void MPhysics2DPanning::stop()
{
    physicsState->physicsStopped = true;
}

void MPhysics2DPanning::pointerPress(const QPointF& /*pos*/)
{
    physicsState->pointerPressed = true;
}

void MPhysics2DPanning::pointerMove(const QPointF& /*pos*/)
{
    physicsState->pointerMoved = true;
}

void MPhysics2DPanning::pointerRelease()
{
    physicsState->pointerReleased = true;
}

//QGraphicsObject stubs:
void QGraphicsObject::grabGesture(Qt::GestureType, Qt::GestureFlags)
{
}

//QGraphicsItem stubs:
void QGraphicsItem::grabMouse()
{
    mouseGrabberState->grabbedMouse = true;
}

void QGraphicsItem::ungrabMouse()
{
    mouseGrabberState->ungrabbedMouse = true;
}

QGraphicsScene *QGraphicsItem::scene() const
{
    return (QGraphicsScene*)1;
}

//QGraphiscScene stubs:
 QGraphicsItem* QGraphicsScene::mouseGrabberItem() const
{
    return dummyItem;
}

 QList<QGraphicsItem*> QGraphicsScene::items() const
{
    QList<QGraphicsItem*> itemList;
    if (dummyItem) {
        itemList.append(dummyItem);
    }
    return itemList;
}

 QList<QGraphicsView *> QGraphicsScene::views() const
 {
     QList<QGraphicsView *> viewList;
     viewList.append(0);
     return viewList;
 }
 
 bool QGraphicsScene::sendEvent(QGraphicsItem *item, QEvent *event)
 {
     if (item == dummyItem) {

         if (event->type() == MCancelEvent::eventNumber()) {
             dummyItem->cancelReceived = true;
         }

         switch (event->type()) {
         case QEvent::GraphicsSceneMousePress:
             dummyItem->mousePressReceived = true;
             break;

         case QEvent::GraphicsSceneMouseRelease:
             dummyItem->mouseReleaseReceived = true;
             break;

         case QEvent::GraphicsSceneMouseMove:
             dummyItem->mouseMoveReceived = true;
             break;

         default:
             break;
         }
     }
     return true;
 }

 QPointF QGraphicsItem::mapFromItem(const QGraphicsItem */*item*/, const QPointF &/*point*/) const
 {
     return QPointF();
 }

 QPoint QGraphicsView::mapFromScene(const QPointF& /*point*/) const
 {
     return QPoint();
 }

 QWidget *QAbstractScrollArea::viewport() const
 {
     return 0;
 }

 void QCoreApplication::postEvent(QObject* /*receiver*/, QEvent */*event*/)
 {
     qDebug("QCoreApplication::postEvent() - called");
 }

 Qt::GestureState currentPanState = Qt::NoGesture;
 Qt::GestureState QGesture::state() const
 {
     return currentPanState;
 }

 // End of stubs.

void Ut_MPannableWidget::initTestCase()
{
}

void Ut_MPannableWidget::cleanupTestCase()
{
}

void Ut_MPannableWidget::init()
{
    dummyItem = new DummyGraphicsItem;
    physicsState = new PhysicsState;
    mouseGrabberState = new MouseGrabberState;
    widget = new MPannableWidget;
}

void Ut_MPannableWidget::cleanup()
{
    delete widget;
    delete physicsState;
    delete mouseGrabberState;
    delete dummyItem;
}

void Ut_MPannableWidget::mousePressAndReleaseAreDeliveredToGrabber()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);

    QTimerEvent timerEvent(1);

    widget->d_func()->glass->mousePressEvent(&pressEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, (QGraphicsItem*)0);

    widget->d_func()->pressDeliveryTimerId = 1;    
    widget->d_func()->glass->timerEvent(&timerEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, dummyItem);
    QVERIFY2(widget->d_func()->resentList.at(0).type == QEvent::GraphicsSceneMousePress, "Mouse press was not sent");

    widget->d_func()->glass->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, (QGraphicsItem*)0);
    QCOMPARE(dummyItem->mouseReleaseReceived, true);
}

void Ut_MPannableWidget::mouseMoveIsDelieveredToGrabberIfNoPanningIsRecognized()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setButton(Qt::LeftButton);
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);

    QTimerEvent timerEvent(1);

    widget->d_func()->glass->mousePressEvent(&pressEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, (QGraphicsItem*)0);

    widget->d_func()->pressDeliveryTimerId = 1;
    widget->d_func()->glass->timerEvent(&timerEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, dummyItem);
    QVERIFY2(widget->d_func()->resentList.at(0).type == QEvent::GraphicsSceneMousePress, "Mouse press was not sent");

    widget->d_func()->glass->mouseMoveEvent(&moveEvent);
    QCOMPARE(widget->d_func()->mouseGrabber, dummyItem);
    QCOMPARE(dummyItem->mouseMoveReceived, true);
    QCOMPARE(dummyItem->mouseReleaseReceived, false);

    widget->d_func()->glass->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, (QGraphicsItem*)0);
    QCOMPARE(dummyItem->mouseReleaseReceived, true);
}

void Ut_MPannableWidget::mousePressWithNoLeftButtonIsIgnored()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::RightButton);

    widget->d_func()->glass->mousePressEvent(&pressEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, (QGraphicsItem*)0);
    QCOMPARE(widget->d_func()->pressDeliveryTimerId, 0);
    QVERIFY(pressEvent.isAccepted() == false);
}

void Ut_MPannableWidget::pannableViewportInsidePannableViewportIgnoresMousePress()
{
    MPannableWidget *innerWidget = new MPannableWidget(widget);

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);

    innerWidget->d_func()->glass->mousePressEvent(&pressEvent);

    QCOMPARE(innerWidget->d_func()->mouseGrabber, (QGraphicsItem*)0);
    QCOMPARE(innerWidget->d_func()->pressDeliveryTimerId, 0);
    QVERIFY(pressEvent.isAccepted() == false);
}

void Ut_MPannableWidget::cancelEventStopPhysicsAndResetsStateOfWidget()
{
    QPanGesture panGesture;

    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent event(gestureList);

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);

    QTimerEvent timerEvent(1);


    widget->d_func()->glass->mousePressEvent(&pressEvent);      //mousePress delivered to glass,
                                                                //delivery timer is started.
    widget->d_func()->pressDeliveryTimerId = 1;
    widget->d_func()->glass->timerEvent(&timerEvent);           //timer expired, mouse press should be delivered to
                                                                //the widget inside pannable widget.

    QCOMPARE(widget->d_func()->mouseGrabber, dummyItem);

    currentPanState = Qt::GestureStarted;
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(physicsState->pointerPressed, true);
    QCOMPARE(physicsState->pointerMoved, true);
    QCOMPARE(physicsState->pointerReleased, false);

    QCOMPARE(mouseGrabberState->ungrabbedMouse, true);          //pannable widget ungrabbed mouse;
    QCOMPARE(mouseGrabberState->grabbedMouse, true);            //and grabbed it again later.

    MCancelEvent cancelEvent;                                   //Sending cancel event
    widget->cancelEvent(&cancelEvent);

    mouseGrabberState->grabbedMouse = false;
    mouseGrabberState->ungrabbedMouse = false;

    QCOMPARE(physicsState->pointerReleased, true);              //Pannable widget should reset it's state
    QCOMPARE(physicsState->physicsStopped, true);               //after cancel event.
    QCOMPARE(mouseGrabberState->grabbedMouse, false);
}

void Ut_MPannableWidget::disabledWidgetShouldIgnoreMousePressAndPanEvents()
{
    widget->setVerticalPanningPolicy(MPannableWidget::PanningAlwaysOff);
    widget->setHorizontalPanningPolicy(MPannableWidget::PanningAlwaysOff);

    QCOMPARE(physicsState->pointerPressed, false);
    QCOMPARE(physicsState->pointerMoved, false);
    QCOMPARE(physicsState->pointerReleased, true);              //Disabling widget resets the state of physics.
    physicsState->pointerReleased = false;                      //Cleaning up the state tracker.

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);

    widget->d_func()->glass->mousePressEvent(&pressEvent);

    QCOMPARE(physicsState->pointerPressed, false);
    QCOMPARE(physicsState->pointerMoved, false);
    QCOMPARE(physicsState->pointerReleased, false);

    QCOMPARE(widget->d_func()->mouseGrabber, (QGraphicsItem*)0);
    QCOMPARE(widget->d_func()->pressDeliveryTimerId, 0);
    QVERIFY(pressEvent.isAccepted() == false);

    QPanGesture panGesture;

    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent event(gestureList);

    currentPanState = Qt::GestureStarted;
    panGesture.setOffset(QPointF(100,0));

    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(physicsState->pointerPressed, false);
    QCOMPARE(physicsState->pointerMoved, false);
    QCOMPARE(physicsState->pointerReleased, false);
    QCOMPARE(event.isAccepted(&panGesture), false);
}

void Ut_MPannableWidget::panGestureMovesPhysicsPointer()
{
    QPanGesture panGesture;

    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent event(gestureList);

    currentPanState = Qt::GestureStarted;
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(physicsState->pointerPressed, true);
    QCOMPARE(physicsState->pointerMoved, true);
    QCOMPARE(physicsState->pointerReleased, false);

    physicsState->pointerPressed = false;

    currentPanState = Qt::GestureUpdated;
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(physicsState->pointerPressed, false);
    QCOMPARE(physicsState->pointerMoved, true);
    QCOMPARE(physicsState->pointerReleased, false);

    physicsState->pointerMoved = false;

    currentPanState = Qt::GestureFinished;
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(physicsState->pointerPressed, false);
    QCOMPARE(physicsState->pointerMoved, false);
    QCOMPARE(physicsState->pointerReleased, true);

}

void Ut_MPannableWidget::panGestureAgainstPanningDirectionIsIgnored()
{
    QPanGesture panGesture;

    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent event(gestureList);

    currentPanState = Qt::GestureStarted;
    panGesture.setOffset(QPointF(100,0));

    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(physicsState->pointerPressed, false);
    QCOMPARE(physicsState->pointerMoved, false);
    QCOMPARE(physicsState->pointerReleased, false);
    QCOMPARE(event.isAccepted(&panGesture), false);
}

void Ut_MPannableWidget::panGestureCancelsMouseEvents()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    QTimerEvent timerEvent(1);

    widget->glassMousePressEvent(&pressEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, (QGraphicsItem*)0);

    widget->d_func()->pressDeliveryTimerId = 1;
    widget->glassTimerEvent(&timerEvent);

    QCOMPARE(widget->d_func()->mouseGrabber, dummyItem);
    QVERIFY2(widget->d_func()->resentList.at(0).type == QEvent::GraphicsSceneMousePress, "Mouse press was not sent");

    QPanGesture panGesture;

    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent event(gestureList);

    currentPanState = Qt::GestureStarted;
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(physicsState->pointerPressed, true);
    QCOMPARE(physicsState->pointerMoved, true);
    QCOMPARE(physicsState->pointerReleased, false);

    QCOMPARE(dummyItem->cancelReceived, true);
}

class CustomPhysics : public MPhysics2DPanning
{
public:
    CustomPhysics(MPannableWidget *parent) : MPhysics2DPanning(parent)
    {
    }
};

void Ut_MPannableWidget::usingCustomPhysics()
{
    CustomPhysics *customPhysics = new CustomPhysics(widget);
    widget->setPhysics(customPhysics);

    QVERIFY(static_cast<MPannableWidgetPrivate *>(widget->d_ptr)->physics == customPhysics);

    widget->setPhysics(new MPhysics2DPanning(widget));
}

void Ut_MPannableWidget::settingNewPhysicsShouldEmitPhysicsChangeSignal()
{
    CustomPhysics *customPhysics = new CustomPhysics(widget);

    QSignalSpy spyPhysicsChange(widget, SIGNAL(physicsChanged()));

    widget->setPhysics(customPhysics);

    QCOMPARE(spyPhysicsChange.count(),1);

    widget->setPhysics(new MPhysics2DPanning(widget));
}


void Ut_MPannableWidget::settingPhysicsToNULLShouldNotBreakTheWidget()
{
    widget->setPhysics(NULL);
    QVERIFY(static_cast<MPannableWidgetPrivate *>(widget->d_ptr)->physics != NULL);

}


QTEST_APPLESS_MAIN(Ut_MPannableWidget);
