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
#include "mapplication.h"
#include <mtheme.h>
#include <mpannablewidget.h>
#include "../../src/corelib/widgets/mpannablewidget_p.h"
#include "../../src/corelib/widgets/mphysics2dpanning_p.h"
#include <mapplication.h>

#include <QMetaType>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

/** Events that the test widget emits */
typedef enum {
//    START_TIMER,
//    STOP_TIMER,
    PHYS_STOP,
    PHYS_MOUSE_PRESS,
    PHYS_MOUSE_MOVE,
    PHYS_MOUSE_RELEASE,
    MOUSE_BUTTON_PRESS,
    MOUSE_BUTTON_RELEASE,
    MOUSE_MOVE,
    UNGRAB_MOUSE
} StateTransitionEvent;

/** Class indicating state of the pannable widget after a state transition. */
class StateTransitionResult
{
public:
    StateTransitionResult(int targetState = 0) :
        _targetState(targetState), _outputEvents(QList<StateTransitionEvent>()) {}
    StateTransitionResult(int targetState, QList<StateTransitionEvent> expectedEvents) :
        _targetState(targetState), _outputEvents(expectedEvents) {}

    /** Compares this result with another result using QTestLib compare. */
    void compare(const StateTransitionResult &result) const;

    /** State where the pannable widget should be. */
    int _targetState;
    /** List of output events from test widget. */
    QList<StateTransitionEvent> _outputEvents;
};

void StateTransitionResult::compare(const StateTransitionResult &result) const
{
    // Iterate through the output events and see that they match
    QCOMPARE(_outputEvents.size(), result._outputEvents.size());
    QListIterator<StateTransitionEvent> outputEvent(_outputEvents);
    QListIterator<StateTransitionEvent> resultEvent(result._outputEvents);
    while (outputEvent.hasNext()) {
        QCOMPARE(outputEvent.next(), resultEvent.next());
    }
}

// Required to insert StateTransitionResult instances to the test data.
Q_DECLARE_METATYPE(StateTransitionResult);

static Ut_MPannableWidget *gTester = 0;

/* Using the QT timer methods rather than the stubbed ones.
 *
// Mock methods of the services required by pannable widget.
void QTimer::start()
{
    qDebug("QTimer::start() - called.");
    gTester->_currentResult->_outputEvents.push_back(START_TIMER);
}


void QTimer::stop()
{
    qDebug("QTimer::stop() - called.");
    gTester->_currentResult->_outputEvents.push_back(STOP_TIMER);
}
*/

void MPhysics2DPanning::start()
{
    qDebug("Physics2DPanning::start() - called.");
}

void QTimeLine::start()
{
    qDebug("QTimeLine::start() - called.");
}

QWidget::QWidget(QWidget* /*parent*/, Qt::WindowFlags /*f*/)
{
    qDebug("QWidget::QWidget() - called.");
}

QFrame::QFrame(QWidget* /*parent*/, Qt::WindowFlags /*f*/)
{
    qDebug("QFrame::QFrame() - called.");
}

QAbstractScrollArea::QAbstractScrollArea(QWidget* /*parent*/)
{
    qDebug("QAbstractScrollArea::QAbstractScrollArea() - called.");
}

QGraphicsView::QGraphicsView(QWidget* /*parent*/)
{
    qDebug("QGraphicsView::QGraphicsView() - called.");
}

QGraphicsScene *QGraphicsItem::scene() const
{
    qDebug("QGraphicsItem::scene() - called.");
    return 0;
}

QGraphicsItem *QGraphicsScene::mouseGrabberItem() const
{
    qDebug("QGraphicsScene::mouseGrabberItem() - called");
    return 0;
}

QList<QGraphicsItem *> QGraphicsScene::items() const
{
    qDebug("QGraphicsScene::items() - called");
    QList<QGraphicsItem *> list;
    return list;
}

QPointF QGraphicsItem::mapFromItem(const QGraphicsItem */*item*/, const QPointF &/*point*/) const
{
    qDebug("QGraphicsScene::mapFromItem() - called");
    return QPointF();
}

QList<QGraphicsView *> QGraphicsScene::views() const
{
    qDebug("QGraphicsScene::views() - called.");
    return QList<QGraphicsView *>();
}

QPoint QGraphicsView::mapFromScene(const QPointF& /*point*/) const
{
    qDebug("QGraphicsView::mapFromScene() - called.");
    return QPoint();
}

QWidget *QAbstractScrollArea::viewport() const
{
    qDebug("QAbstractScrollArea::viewport() - called.");
    return 0;
}

bool QGraphicsScene::sendEvent(QGraphicsItem *item, QEvent *event)
{
    Q_UNUSED(item);
    Q_UNUSED(event);
    qDebug("QGraphicsScene::sendEvent() - called.");
    return true;
}

void QCoreApplication::postEvent(QObject* /*receiver*/, QEvent *event)
{
    qDebug("QCoreApplication::postEvent() - called");
    if (event->type() == QEvent::MouseButtonPress) {
        gTester->_currentResult->_outputEvents.push_back(MOUSE_BUTTON_PRESS);
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        gTester->_currentResult->_outputEvents.push_back(MOUSE_BUTTON_RELEASE);
    }
    if (event->type() == QEvent::MouseMove) {
        gTester->_currentResult->_outputEvents.push_back(MOUSE_MOVE);
    }
}

void MPhysics2DPanning::stop()
{
    gTester->_currentResult->_outputEvents.push_back(PHYS_STOP);
}

void MPhysics2DPanning::pointerPress(const QPointF& /*pos*/)
{
    gTester->_currentResult->_outputEvents.push_back(PHYS_MOUSE_PRESS);
}

void MPhysics2DPanning::pointerMove(const QPointF& /*pos*/)
{
    gTester->_currentResult->_outputEvents.push_back(PHYS_MOUSE_MOVE);
}

void MPhysics2DPanning::pointerRelease()
{
    gTester->_currentResult->_outputEvents.push_back(PHYS_MOUSE_RELEASE);
}

void MPhysics2DPanningPrivate::_q_integrator(int /*frame*/)
{
}

MApplication *app;

void QGraphicsItem::ungrabMouse()
{
    gTester->_currentResult->_outputEvents.push_back(UNGRAB_MOUSE);
}

void Ut_MPannableWidget::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_pannablewidget" };
    app = new MApplication(argc, app_name);
    Q_UNUSED(app);

    gTester = this;

}

void Ut_MPannableWidget::cleanupTestCase()
{
    gTester = 0;
    delete app;
}


void Ut_MPannableWidget::init()
{
    _widget = new MPannableWidget();
    _currentResult = new StateTransitionResult();
}

void Ut_MPannableWidget::cleanup()
{
    delete _currentResult;
    delete _widget;
    _widget = 0;
    _currentResult = 0;
}

void Ut_MPannableWidget::testWaitState_data()
{
    QList<StateTransitionEvent> expectedEvents;

    // Create a list of results that the widget should reach to
    // when receiving events.
    QList<StateTransitionResult> results;

    // Result after no events
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait)));

    // Result after mouse press event
    expectedEvents.push_back(UNGRAB_MOUSE);
    expectedEvents.push_back(PHYS_MOUSE_PRESS);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Evaluate), expectedEvents));
    expectedEvents.clear();

    // Result after mouse release event
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait), expectedEvents));

    // Result after mouse move event
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait), expectedEvents));

    // Result after move over passive threshold event
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait), expectedEvents));

    // Result after move over active threshold event
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait), expectedEvents));

    createTestData(results);
}

void Ut_MPannableWidget::testWaitState()
{
    // Set the pannable widget into wait state
    static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->state = MPannableWidgetPrivate::Wait;

    testStateTransitions();
}

void Ut_MPannableWidget::testEvaluateState_data()
{
    QList<StateTransitionEvent> expectedEvents;

    // Create a list of results that the widget should reach to
    // when receiving events.
    QList<StateTransitionResult> results;

    // Result after no events
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Evaluate)));

    // Result after mouse press event
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Evaluate)));

    // Result after mouse release event
    expectedEvents.push_back(UNGRAB_MOUSE);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait), expectedEvents));
    expectedEvents.clear();

    // Result after mouse move event
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Evaluate)));

    // Result after move over passive threshold event
    expectedEvents.push_back(PHYS_MOUSE_RELEASE);
    expectedEvents.push_back(UNGRAB_MOUSE);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait), expectedEvents));
    expectedEvents.clear();

    // Result after move over active threshold event
    expectedEvents.push_back(PHYS_MOUSE_MOVE);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Pan), expectedEvents));
    expectedEvents.clear();

    createTestData(results);
}

void Ut_MPannableWidget::testEvaluateState()
{
    // Set the pannable widget into evaluate state
    static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->state = MPannableWidgetPrivate::Evaluate;

    testStateTransitions();
}

void Ut_MPannableWidget::testPanState_data()
{
    QList<StateTransitionEvent> expectedEvents;

    // Create a list of results that the widget should reach to
    // when receiving events.
    QList<StateTransitionResult> results;

    // Result after no events
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Pan)));

    // Result after mouse press event
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Pan)));

    // Result after mouse release event
    expectedEvents.push_back(PHYS_MOUSE_RELEASE);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Wait), expectedEvents));
    expectedEvents.clear();

    // Result after mouse move event
    expectedEvents.push_back(PHYS_MOUSE_MOVE);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Pan), expectedEvents));
    expectedEvents.clear();

    // Result after move over passive threshold event
    expectedEvents.push_back(PHYS_MOUSE_MOVE);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Pan), expectedEvents));
    expectedEvents.clear();

    // Result after move over active threshold event
    expectedEvents.push_back(PHYS_MOUSE_MOVE);
    results.push_back(StateTransitionResult(int(MPannableWidgetPrivate::Pan), expectedEvents));
    expectedEvents.clear();

    createTestData(results);
}

void Ut_MPannableWidget::testPanState()
{
    // Set the pannable widget into pan state
    static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->state = MPannableWidgetPrivate::Pan;

    testStateTransitions();
}

void Ut_MPannableWidget::createTestData(QList<StateTransitionResult> results)
{
    qRegisterMetaType< QList<QEvent *> >();
    qRegisterMetaType< StateTransitionResult >();

    Q_ASSERT(results.size() == 6);

    QTest::addColumn< QList<QEvent *> >("events");
    QTest::addColumn<StateTransitionResult>("result");

    // Sequence of events to be passed to the test widget.
    QList<QEvent *> sequence;

    // Iterator to results that the widget should reach when
    // input events are send to the test widget.
    QListIterator<StateTransitionResult> resIt(results);

    // No events sent to the widget
    QTest::newRow("initial") << sequence << resIt.next();

    QGraphicsSceneMouseEvent *mouseEvent;

    // Send a mouse press event
    mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    mouseEvent->setButton(Qt::LeftButton);
    sequence.push_back(mouseEvent);
    QTest::newRow("left mouse button pressed") << sequence << resIt.next();
    sequence.clear();

    // Send a mouse release event
    mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
    mouseEvent->setButton(Qt::LeftButton);
    sequence.push_back(mouseEvent);
    QTest::newRow("left mouse button released") << sequence << resIt.next();
    sequence.clear();

    // Send a mouse move event
    mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    mouseEvent->setButton(Qt::LeftButton);
    mouseEvent->setScreenPos(QPoint(0, 1));
    sequence.push_back(mouseEvent);
    QTest::newRow("mouse move") << sequence << resIt.next();
    sequence.clear();

    // Send a mouse move over passive threshold
    mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    mouseEvent->setButton(Qt::LeftButton);
    mouseEvent->setPos(QPoint(31, 0));
    sequence.push_back(mouseEvent);
    QTest::newRow("mouse move over passive") << sequence << resIt.next();
    sequence.clear();

    // Send a mouse move over active threshold
    mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    mouseEvent->setButton(Qt::LeftButton);
    mouseEvent->setPos(QPoint(0, 31));
    sequence.push_back(mouseEvent);
    QTest::newRow("mouse move over active") << sequence << resIt.next();
    sequence.clear();
}


void Ut_MPannableWidget::testStateTransitions()
{
    QFETCH(QList<QEvent *>, events);
    QFETCH(StateTransitionResult, result);

    // Iterate through the events in the event list
    QListIterator<QEvent *> i(events);
    while (i.hasNext()) {
        QEvent *event = i.next();
        switch (event->type()) {
        case QEvent::GraphicsSceneMousePress:
            _widget->glassMousePressEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
            break;
        case QEvent::GraphicsSceneMouseMove:
            _widget->glassMouseMoveEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
            break;
        case QEvent::GraphicsSceneMouseRelease:
            _widget->glassMouseReleaseEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
            break;
        default:
            break;
        }
    }

    QCOMPARE(static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->state, result._targetState);

    _currentResult->compare(result);
}

void Ut_MPannableWidget::testRightMouseButton()
{
    QGraphicsSceneMouseEvent *mouseEvent;
    mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    mouseEvent->setButton(Qt::RightButton);
    mouseEvent->setScenePos(QPoint(100, 100));
    _widget->glassMousePressEvent(mouseEvent);
    delete mouseEvent;
    // State should not change with right mousebutton
    QVERIFY(static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->state == MPannableWidgetPrivate::Wait);

    mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
    mouseEvent->setButton(Qt::RightButton);
    mouseEvent->setScenePos(QPoint(100, 100));
    _widget->glassMouseReleaseEvent(mouseEvent);
    delete mouseEvent;
    // State should stay the same after button release
    QVERIFY(static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->state == MPannableWidgetPrivate::Wait);
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
    CustomPhysics *customPhysics = new CustomPhysics(_widget);
    _widget->setPhysics(customPhysics);

    QVERIFY(static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->physics == customPhysics);

    _widget->setPhysics(new MPhysics2DPanning(_widget));
}

void Ut_MPannableWidget::settingNewPhysicsShouldEmitPhysicsChangeSignal()
{
    CustomPhysics *customPhysics = new CustomPhysics(_widget);

    QSignalSpy spyPhysicsChange(_widget, SIGNAL(physicsChanged()));

    _widget->setPhysics(customPhysics);

    QCOMPARE(spyPhysicsChange.count(),1);

    _widget->setPhysics(new MPhysics2DPanning(_widget));
}


void Ut_MPannableWidget::settingPhysicsToNULLShouldNotBreakTheWidget()
{
    _widget->setPhysics(NULL);
    QVERIFY(static_cast<MPannableWidgetPrivate *>(_widget->d_ptr)->physics != NULL);

}


QTEST_APPLESS_MAIN(Ut_MPannableWidget);
