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

#include <QTimeLine>
#include <QSize>
#include <QPoint>
#include <QMetaType>
#include <QDebug>
#include <QVariant>
#include <mphysics2dpanning.h>
#include "../../src/corelib/widgets/mphysics2dpanning_p.h"
#include <mapplication.h>
#include <mpannablewidget.h>

#include "ut_mphysics2dpanning.h"

#define QFLOATCOMPARE(x,y)  QCOMPARE(x+1.0,y+1.0)

QList<QString> Ut_MPhysics2DPanning::animationActions;
QAbstractAnimation::State Ut_MPhysics2DPanning::animationState;

void QVariantAnimation::setDuration(int duration)
{
    QString str = "setDuration";
    str.append("_");
    str.append(QString::number(duration));
    Ut_MPhysics2DPanning::animationActions.push_back(str);
}

void QVariantAnimation::setStartValue(const QVariant &value)
{
    QString str = "setStartValue";
    str.append("_");
    str.append(QString::number(value.toFloat()));
    Ut_MPhysics2DPanning::animationActions.push_back(str);
}

void QVariantAnimation::setEndValue(const QVariant &value)
{
    QString str = "setEndValue";
    str.append("_");
    str.append(QString::number(value.toFloat()));
    Ut_MPhysics2DPanning::animationActions.push_back(str);
}

void QAbstractAnimation::start(QAbstractAnimation::DeletionPolicy)
{
    QString str = "start";
    Ut_MPhysics2DPanning::animationActions.push_back(str);
}

void QAbstractAnimation::stop()
{
    QString str = "stop";
    Ut_MPhysics2DPanning::animationActions.push_back(str);
}

QAbstractAnimation::State QAbstractAnimation::state() const
{
    return Ut_MPhysics2DPanning::animationState;
}


void Ut_MPhysics2DPanning::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_pannablewidget" };
    app = new MApplication(argc, app_name);
    Q_UNUSED(app);
}


void Ut_MPhysics2DPanning::cleanupTestCase()
{
    delete app;
}


void Ut_MPhysics2DPanning::init()
{
    MPannableWidget *pannable = new MPannableWidget();
    pannable->setPanDirection(Qt::Vertical | Qt::Horizontal);

    physics = new MPhysics2DPanning(pannable);

    // no style object available, just set some default values
    physics->d_ptr->slideFrictionC = 0.95;
    physics->d_ptr->borderSpringK = 0.06;
    physics->d_ptr->borderFrictionC = 0.66;
    physics->d_ptr->panDirection = Qt::Vertical | Qt::Horizontal;
    physics->d_ptr->maxVel = 100.0;

    Ut_MPhysics2DPanning::animationActions.clear();
    Ut_MPhysics2DPanning::animationState = QAbstractAnimation::Stopped;
}


void Ut_MPhysics2DPanning::cleanup()
{
    delete physics;
}


void Ut_MPhysics2DPanning::initValues()
{
    QCOMPARE(physics->d_ptr->range, QRectF(0, 0, 0, 0));
    QCOMPARE(physics->d_ptr->posX, 0.0);
    QCOMPARE(physics->d_ptr->posY, 0.0);
    QCOMPARE(physics->d_ptr->velX, 0.0);
    QCOMPARE(physics->d_ptr->velY, 0.0);
    QCOMPARE(physics->d_ptr->slideFrictionC, 0.95);
    QCOMPARE(physics->d_ptr->borderSpringK, 0.06);
    QCOMPARE(physics->d_ptr->borderFrictionC, 0.66);
    QCOMPARE(physics->d_ptr->pointerPressed, false);
    QCOMPARE(physics->d_ptr->pointerSpringX, 0.0);
    QCOMPARE(physics->d_ptr->pointerSpringY, 0.0);
    QCOMPARE(physics->d_ptr->sceneLastPos, QPointF());
    QCOMPARE(physics->d_ptr->panningAnimation != NULL, true);
    QCOMPARE(physics->d_ptr->panDirection, Qt::Vertical | Qt::Horizontal);
}


void Ut_MPhysics2DPanning::timeLineInStart1()
{
    // Checks that timeline is initialized correctly in case it was not running

    Ut_MPhysics2DPanning::animationState = QAbstractAnimation::Stopped;

    physics->d_ptr->velX = -1.0;
    physics->d_ptr->velY = -1.0;
    physics->start();

    QCOMPARE(physics->d_ptr->velX, 0.0);
    QCOMPARE(physics->d_ptr->velY, 0.0);
    QCOMPARE(animationActions.size(), 4);
    QCOMPARE(animationActions[0], QString("setDuration_1000000"));
    QCOMPARE(animationActions[1], QString("setStartValue_0"));
    QCOMPARE(animationActions[2], QString("setEndValue_1"));
    QCOMPARE(animationActions[3], QString("start"));
}


void Ut_MPhysics2DPanning::timeLineInStart2()
{
    // Checks that timeline is not initialized in case it was already running

    Ut_MPhysics2DPanning::animationState = QAbstractAnimation::Running;

    physics->d_ptr->velX = -1.0;
    physics->d_ptr->velY = -1.0;
    physics->start();

    QCOMPARE(physics->d_ptr->velX, -1.0);
    QCOMPARE(physics->d_ptr->velY, -1.0);
    QCOMPARE(animationActions.size(), 0);
}


void Ut_MPhysics2DPanning::timeLineInStop()
{
    physics->stop();

    QCOMPARE(animationActions.size(), 1);
    QCOMPARE(animationActions[0], QString("stop"));
}


void Ut_MPhysics2DPanning::setAndGetQSizeF_data()
{
    QTest::addColumn<QSizeF>("value");

    QTest::newRow("value 1") << QSizeF(300.003, -40122112);
    QTest::newRow("value 2") << QSizeF(-300.003, 0.0003000003);
    QTest::newRow("value 3") << QSizeF(0.0000000001, -0.003273218);
    QTest::newRow("value 4") << QSizeF(7654321000.9, 2130.0);
    QTest::newRow("value 5") << QSizeF(-12121212.5, -1010101.11);
    QTest::newRow("value 6") << QSizeF(0.0, -40123112);
    QTest::newRow("value 7") << QSizeF(-300.003, 0.0);
}


void Ut_MPhysics2DPanning::setAndGetQSizeF()
{
    QSizeF readvalue;

    QFETCH(QSizeF, value);

    physics->setRange(QRectF(QPointF(), value));
    readvalue = physics->range().size();
    QCOMPARE(readvalue, value);
}


void Ut_MPhysics2DPanning::setAndGetQPointF_data()
{
    QTest::addColumn<QPointF>("value");

    QTest::newRow("value 1") << QPointF(300.003, -40102112);
    QTest::newRow("value 2") << QPointF(-300.003, 0.0003000003);
    QTest::newRow("value 3") << QPointF(0.0000000001, -0.003273218);
    QTest::newRow("value 4") << QPointF(7654321000.9, 2130.0);
    QTest::newRow("value 5") << QPointF(-12121212.5, -1010101.11);
    QTest::newRow("value 6") << QPointF(0.0, -40122112);
    QTest::newRow("value 7") << QPointF(-300.003, 0.0);
}


void Ut_MPhysics2DPanning::setAndGetQPointF()
{
    QPointF readvalue;

    QFETCH(QPointF, value);

    physics->setPosition(value);
    readvalue = physics->position();
    QCOMPARE(readvalue, value);

    physics->start();
}


void Ut_MPhysics2DPanning::animationStateQuery()
{
    Ut_MPhysics2DPanning::animationState = QAbstractAnimation::Stopped;
    QCOMPARE(physics->inMotion(), false);

    Ut_MPhysics2DPanning::animationState = QAbstractAnimation::Paused;
    QCOMPARE(physics->inMotion(), false);

    Ut_MPhysics2DPanning::animationState = QAbstractAnimation::Running;
    QCOMPARE(physics->inMotion(), true);
}


void Ut_MPhysics2DPanning::pointerPress()
{
    physics->pointerPress(QPointF(-5.0, 10.5));

    QCOMPARE(physics->d_ptr->pointerPressed, true);
    QCOMPARE(physics->d_ptr->sceneLastPos, QPointF(-5.0, 10.5));
    QFLOATCOMPARE(physics->d_ptr->pointerSpringX, 0.0);
    QFLOATCOMPARE(physics->d_ptr->pointerSpringY, 0.0);
}


void Ut_MPhysics2DPanning::pointerMove()
{
    QPointF currSpring = QPointF(0.0, 0.0);
    QPointF delta;
    QPointF sceneLastPos = QPointF(0.0, 0.0);
    QPointF pos;

    physics->d_ptr->pointerSpringX = 0.0;
    physics->d_ptr->pointerSpringY = 0.0;

    physics->d_ptr->sceneLastPos = sceneLastPos;

    QFLOATCOMPARE(physics->d_ptr->pointerSpringX, currSpring.x());
    QFLOATCOMPARE(physics->d_ptr->pointerSpringY, currSpring.y());

    //First one is going to be ignored.
    physics->pointerPress(sceneLastPos);
    physics->pointerMove(sceneLastPos);

    pos = QPointF(100.0, -50.0);
    physics->pointerMove(pos);
    currSpring += (pos - sceneLastPos);
    sceneLastPos = pos;
    QFLOATCOMPARE(physics->d_ptr->pointerSpringX, currSpring.x());
    QFLOATCOMPARE(physics->d_ptr->pointerSpringY, currSpring.y());

    pos = QPointF(0.01, -0.0000001);
    physics->pointerMove(pos);
    currSpring += (pos - sceneLastPos);
    sceneLastPos = pos;
    QFLOATCOMPARE(physics->d_ptr->pointerSpringX, currSpring.x());
    QFLOATCOMPARE(physics->d_ptr->pointerSpringY, currSpring.y());

    pos = QPointF(-33.0, 51.0);
    physics->pointerMove(pos);
    currSpring += (pos - sceneLastPos);
    sceneLastPos = pos;
    QFLOATCOMPARE(physics->d_ptr->pointerSpringX, currSpring.x());
    QFLOATCOMPARE(physics->d_ptr->pointerSpringY, currSpring.y());

    pos = QPointF(10.0, 10.0);
    physics->pointerMove(pos);
    currSpring += (pos - sceneLastPos);
    sceneLastPos = pos;
    QFLOATCOMPARE(physics->d_ptr->pointerSpringX, currSpring.x());
    QFLOATCOMPARE(physics->d_ptr->pointerSpringY, currSpring.y());
}


void Ut_MPhysics2DPanning::pointerRelease()
{
    QCOMPARE(physics->d_ptr->pointerPressed, false);
}


enum actionType {
    tick,        // int argument
    press,       // QPointF argument
    move,        // QPointF argument
    release,     // no argument
    setRange     // QSizeF argument
};

class integratingAction
{
public:
    integratingAction(actionType type,
                      int value) :
        type(type),
        value_int(value) {}
    integratingAction(actionType type,
                      QPointF value) :
        type(type),
	value_int(0),
        value_QPointF(value) {}
    integratingAction(actionType type,
                      QSizeF value) :
        type(type),
	value_int(0),
        value_QSizeF(value) {}
    integratingAction(actionType type) :
        type(type),
	value_int(0) {}

    enum actionType type;
    int             value_int;
    QPointF         value_QPointF;
    QSizeF          value_QSizeF;
};

Q_DECLARE_METATYPE(QList<integratingAction>);

void Ut_MPhysics2DPanning::integrating_data()
{
    QList<integratingAction> actionList;

    QTest::addColumn< QList<integratingAction> >("actionList");
    QTest::addColumn< QPointF >("endPosition");

    actionList.clear();
    actionList.push_back(integratingAction(press,   QPointF(0.0, 0.0)));
    actionList.push_back(integratingAction(tick,    7));
    actionList.push_back(integratingAction(move,    QPointF(50.0, 10.0)));
    actionList.push_back(integratingAction(tick,    7));
    actionList.push_back(integratingAction(move,    QPointF(80.0, 10.0)));
    actionList.push_back(integratingAction(tick,    7));
    actionList.push_back(integratingAction(release));
    actionList.push_back(integratingAction(tick,    7));

    // End position is rounded to nearest 0.001
    QTest::newRow("actionList 1") << actionList << QPointF(-135.16, -16.944);

    actionList.clear();
    actionList.push_back(integratingAction(setRange, QSizeF(100, 100)));
    actionList.push_back(integratingAction(press,   QPointF(0.0, 0.0)));
    actionList.push_back(integratingAction(tick,    7));
    actionList.push_back(integratingAction(move,    QPointF(-59.0, -10.0)));
    actionList.push_back(integratingAction(tick,    7));
    actionList.push_back(integratingAction(move,    QPointF(-83.0, -20.0)));
    actionList.push_back(integratingAction(tick,    7));
    actionList.push_back(integratingAction(release));
    actionList.push_back(integratingAction(tick,    7));

    // End position is rounded to nearest 0.001
    QTest::newRow("actionList 2") << actionList << QPointF(225.894,67.316);
}


void Ut_MPhysics2DPanning::integrating()
{
    int i, j;
    qreal x, y;
    int frame = 0;

    QFETCH(QList<integratingAction>, actionList);

    for (i = 0; i < actionList.size(); i++) {
        switch (actionList[i].type) {
        case tick:
            for (j = 0; j < actionList[i].value_int; j++) {
                physics->d_ptr->_q_integrator(frame++);
            }
            break;
        case press:
            physics->pointerPress(actionList[i].value_QPointF);
            break;
        case move:
            physics->pointerMove(actionList[i].value_QPointF);
            break;
        case release:
            physics->pointerRelease();
            break;
        case setRange:
            physics->setRange(QRectF(QPointF(), actionList[i].value_QSizeF));
            break;
        default:
            break;
        }
    }

    QFETCH(QPointF, endPosition);

    x = physics->position().x();
    y = physics->position().y();

    // Rounds to nearest 0.001

    x *= 1000.0;
    x  = qRound(x);
    x /= 1000.0;
    y *= 1000.0;
    y  = qRound(y);
    y /= 1000.0;

    QCOMPARE(QPointF(x, y), endPosition);
}

void Ut_MPhysics2DPanning::kinetic_disabled_integrating_data()
{
    QList<integratingAction> actionList;

    QTest::addColumn< QList<integratingAction> >("actionList");
    QTest::addColumn< QPointF >("endPosition");

    actionList.clear();
    actionList.push_back(integratingAction(setRange, QSizeF(0, 0)));
    actionList.push_back(integratingAction(press,   QPointF(0.0, 0.0)));
    actionList.push_back(integratingAction(move,    QPointF(50.0, 10.0)));
    actionList.push_back(integratingAction(move,    QPointF(80.0, 10.0)));
    actionList.push_back(integratingAction(release));

    QTest::newRow("actionList 1") << actionList << QPointF(0.0, 0.0);

    actionList.clear();
    actionList.push_back(integratingAction(setRange, QSizeF(100, 100)));
    actionList.push_back(integratingAction(press,   QPointF(0.0, 0.0)));
    actionList.push_back(integratingAction(move,    QPointF(-83.0, -20.0)));
    actionList.push_back(integratingAction(release));

    // End position is rounded to nearest 0.001
    QTest::newRow("actionList 2") << actionList << QPointF(83, 20);
}


void Ut_MPhysics2DPanning::kinetic_disabled_integrating()
{
    int i;
    qreal x, y;

    physics->setPosition(QPointF(0,0));
    physics->setEnabled(false);

    QFETCH(QList<integratingAction>, actionList);


    for (i = 0; i < actionList.size(); i++) {
        switch (actionList[i].type) {
        case press:
            physics->pointerPress(actionList[i].value_QPointF);
            break;
        case move:
            physics->pointerMove(actionList[i].value_QPointF);
            break;
        case release:
            physics->pointerRelease();
            break;
        case setRange:
            physics->setRange(QRectF(QPointF(), actionList[i].value_QSizeF));
            break;
        default:
            break;
        }
    }

    QFETCH(QPointF, endPosition);

    x = physics->position().x();
    y = physics->position().y();

    // Rounds to nearest 0.001

    x *= 1000.0;
    x  = qRound(x);
    x /= 1000.0;
    y *= 1000.0;
    y  = qRound(y);
    y /= 1000.0;

    QCOMPARE(QPointF(x, y), endPosition);

    physics->setEnabled(true);
}

void Ut_MPhysics2DPanning::positionShouldReturnToStartRangeAfterMovingViewportBeyondStartRange()
{
    physics->setRange(QRectF(10, 10, 100, 100));
    physics->setPosition(QPointF(10, -30));

    QCOMPARE(physics->range(), QRectF(10, 10, 100, 100));
    QCOMPARE(physics->position(), QPointF(10, -30));

    for (int i = 1; i < 500; i++) {
        physics->d_ptr->_q_integrator(i);
    }


    QVERIFY2(abs(physics->position().y() - 10) < 1, "Position (rounded to 1px) should be equal 10");

    physics->setPosition(QPointF(10, -30));
    QCOMPARE(physics->range(), QRectF(10, 10, 100, 100));
    QCOMPARE(physics->position(), QPointF(10, -30));

    for (int i = 1; i < 500; i++) {
        physics->d_ptr->_q_integrator(i);
    }

    QVERIFY2(abs(physics->position().y() - 10) < 1, "Position (rounded to 1px) should be equal 10");
}

void Ut_MPhysics2DPanning::positionShouldReturnToEndRangeAfterMovingViewportBeyondEndRange()
{
    physics->setRange(QRectF(10, 10, 100, 100));
    physics->setPosition(QPointF(10, 130));

    QCOMPARE(physics->range(), QRectF(10, 10, 100, 100));
    QCOMPARE(physics->position(), QPointF(10, 130));

    for (int i = 1; i < 500; i++) {
        physics->d_ptr->_q_integrator(i);
    }

    QVERIFY2(abs(physics->position().y() - 110) < 1, "Position (rounded to 1px) should be equal 110");

    physics->setPosition(QPointF(10, 130));
    QCOMPARE(physics->range(), QRectF(10, 10, 100, 100));
    QCOMPARE(physics->position(), QPointF(10, 130));

    for (int i = 1; i < 500; i++) {
        physics->d_ptr->_q_integrator(i);
    }

    QVERIFY2(abs(physics->position().y() - 110) < 1, "Position (rounded to 1px) should be equal 110");
}

void Ut_MPhysics2DPanning::integrationShouldStopAfterReachingPositionInsideRange()
{
    physics->setRange(QRectF(10, 10, 100, 100));
    physics->setPosition(QPointF(10, -30));

    QCOMPARE(physics->range(), QRectF(10, 10, 100, 100));
    QCOMPARE(physics->position(), QPointF(10, -30));

    for (int i = 1; i < 500; i++) {
        physics->d_ptr->_q_integrator(i);
    }

    QVERIFY2(abs(physics->velocity().y()) < 1, "Velocity should be smaller than 1px");

    physics->setPosition(QPointF(10, -30));
    QCOMPARE(physics->position(), QPointF(10, -30));

    for (int i = 1; i < 500; i++) {
        physics->d_ptr->_q_integrator(i);
    }

    QVERIFY2(abs(physics->velocity().y()) < 1, "Velocity should be smaller than 1px");
}

void Ut_MPhysics2DPanning::maximumSlidingSpeedShouldNotExceedAllowedMaximumVelocity()
{
    physics->setRange(QRectF(10, 10, 1000, 1000));
    physics->setPosition(QPointF(10, 10));

    QPointF pos;

    pos = QPointF(100.0, 600.0);
    physics->pointerPress(pos);

    pos = QPointF(100.0, 0.0);
    physics->pointerMove(pos);

    physics->pointerRelease();

    QCOMPARE(physics->velocity().y(), qreal(100.0));
}

void Ut_MPhysics2DPanning::testSetGetEnabled()
{
    QCOMPARE(physics->enabled(), true);
    physics->setEnabled(false);
    QCOMPARE(physics->enabled(), false);
    physics->setEnabled(true);
}

void Ut_MPhysics2DPanning::testSetGetFriction()
{
    physics->setFriction(100);
    QCOMPARE(physics->friction(), 100.0);
}

void Ut_MPhysics2DPanning::testSetGetBorderFriction()
{
    physics->setBorderFriction(200);
    QCOMPARE(physics->borderFriction(), 200.0);
}

void Ut_MPhysics2DPanning::testSetGetBorderSpringK()
{
    physics->setBorderSpringK(300);
    QCOMPARE(physics->borderSpringK(), 300.0);
}

void Ut_MPhysics2DPanning::testSetGetSlidingFriction()
{
    physics->setSlidingFriction(400);
    QCOMPARE(physics->slidingFriction(),400.0);
}

void Ut_MPhysics2DPanning::testSetGetPointerSpringK()
{
    physics->setPointerSpringK(500);
    QCOMPARE(physics->pointerSpringK(), 500.0);
}

void Ut_MPhysics2DPanning::testSetGetPanDirection()
{
    physics->setPanDirection(0);
    QCOMPARE(physics->panDirection(), 0);
    physics->setPanDirection(Qt::Horizontal);
    QCOMPARE(physics->panDirection(), Qt::Horizontal);
    physics->setPanDirection(Qt::Vertical | Qt::Horizontal);
    QCOMPARE(physics->panDirection(), Qt::Vertical | Qt::Horizontal);
}

QTEST_APPLESS_MAIN(Ut_MPhysics2DPanning);
