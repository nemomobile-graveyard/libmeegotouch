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
#include "mtapstatemachine.h"
#include "mtapstatemachine_p.h"
#include "mtapstatemachinestyle.h"

#include <MTheme>

#include <QStateMachine>
#include <QTimer>
#include <QEventTransition>
#include <QSignalTransition>

MCancelEventTransition::MCancelEventTransition(QObject *target)
{
    target->installEventFilter(this);
}

bool MCancelEventTransition::eventTest(QEvent *e) {
    return (e->type() == MCancelEvent::eventNumber());
}

void MCancelEventTransition::onTransition(QEvent *)
{
}

bool MCancelEventTransition::eventFilter(QObject *, QEvent *event) {
    if (event->type() == MCancelEvent::eventNumber())
        machine()->postEvent(new MCancelEvent());
    return false;
}

MTapStateMachinePrivate::MTapStateMachinePrivate() :
        style(0),

        highlightMachine(0),
        initialWaitState(0),
        pressHighlightedState(0),
        timedHighlightedState(0),
        shortBlinkState(0),
        unhighlightedState(0),

        initialMousePressTransition(0),
        initCancelEventTransition(0),
        timerHighlightTransition(0),
        releaseUnhighlightTransition(0),
        releaseTimedHighlightTransition(0),
        timerUnhighlightTransition(0),
        pressedCancelEventTransition(0),

        initialWaitTimer(0),
        minimumHighlightTimer(0),
        q_ptr(0)
{

}

MTapStateMachinePrivate::~MTapStateMachinePrivate()
{

}

MTapStateMachine::MTapStateMachine(QObject *parent)
    :   QObject(parent),
        d_ptr(new MTapStateMachinePrivate)
{
    Q_D(MTapStateMachine);

    d->style = static_cast<const MTapStateMachineStyle *>(MTheme::style("MTapStateMachineStyle", ""));

    d->highlightMachine = new QStateMachine(this);

    d->unhighlightedState = new QState(d->highlightMachine);
    d->initialWaitState = new QState(d->highlightMachine);
    d->pressHighlightedState = new QState(d->highlightMachine);
    d->timedHighlightedState = new QState(d->highlightMachine);

    d->initialWaitTimer = new QTimer(this);
    d->initialWaitTimer->setSingleShot(true);
    d->initialWaitTimer->setInterval(d->style->initialWaitTime());

    d->minimumHighlightTimer = new QTimer(this);
    d->minimumHighlightTimer->setSingleShot(true);
    d->minimumHighlightTimer->setInterval(d->style->blinkDuration());

    d->highlightMachine->setInitialState(d->unhighlightedState);

    //Unhighlighted -> initialWaitState (on mouse press)
    d->initialMousePressTransition = new QEventTransition(parent, QEvent::GraphicsSceneMousePress);
    d->initialMousePressTransition->setTargetState(d->initialWaitState);
    d->unhighlightedState->addTransition(d->initialMousePressTransition);

    //InitialWaitState -> unhighlightedState (on Cancel Event)
    d->initCancelEventTransition = new MCancelEventTransition(parent);
    d->initCancelEventTransition->setTargetState(d->unhighlightedState);
    d->initialWaitState->addTransition(d->initCancelEventTransition);

    //InitialWaitState -> timedHighlight (on mouse release)
    d->releaseTimedHighlightTransition = new QEventTransition(parent, QEvent::GraphicsSceneMouseRelease);
    d->releaseTimedHighlightTransition->setTargetState(d->timedHighlightedState);
    d->initialWaitState->addTransition(d->releaseTimedHighlightTransition);

    //InitialWaitState -> pressHighlightedState (on timer)
    d->timerHighlightTransition = new QSignalTransition(d->initialWaitTimer, SIGNAL(timeout()));
    d->timerHighlightTransition->setTargetState(d->pressHighlightedState);
    d->initialWaitState->addTransition(d->timerHighlightTransition);

    //PressHighlightedState -> unhighlightedState (on mouse release)
    d->releaseUnhighlightTransition = new QEventTransition(parent, QEvent::GraphicsSceneMouseRelease);
    d->releaseUnhighlightTransition->setTargetState(d->unhighlightedState);
    d->pressHighlightedState->addTransition(d->releaseUnhighlightTransition);

    //pressHighlightedState -> unhighlightedState (on Cancel Event)
    d->pressedCancelEventTransition = new MCancelEventTransition(parent);
    d->pressedCancelEventTransition->setTargetState(d->unhighlightedState);
    d->pressHighlightedState->addTransition(d->pressedCancelEventTransition);

    //TimedHighlightState -> UnhighlightedState (on timer)
    d->timerUnhighlightTransition = new QSignalTransition(d->minimumHighlightTimer, SIGNAL(timeout()));
    d->timerUnhighlightTransition->setTargetState(d->unhighlightedState);
    d->timedHighlightedState->addTransition(d->timerUnhighlightTransition);

    d->highlightMachine->start();

    connect(d->initialWaitState, SIGNAL(entered()), d->initialWaitTimer, SLOT(start()));
    connect(d->timedHighlightedState, SIGNAL(entered()), d->minimumHighlightTimer, SLOT(start()));
    connect(d->initialWaitState, SIGNAL(exited()), d->initialWaitTimer, SLOT(stop()));
    connect(d->timedHighlightedState, SIGNAL(exited()), d->minimumHighlightTimer, SLOT(stop()));

    connect(d->initCancelEventTransition, SIGNAL(triggered()), this, SIGNAL(release()));
    connect(d->releaseUnhighlightTransition, SIGNAL(triggered()), this, SIGNAL(release()));
    connect(d->pressedCancelEventTransition, SIGNAL(triggered()), this, SIGNAL(release()));
    connect(d->timerUnhighlightTransition, SIGNAL(triggered()), this, SIGNAL(release()));

    connect(d->pressHighlightedState, SIGNAL(entered()), this, SIGNAL(delayedPress()));
    connect(d->timedHighlightedState, SIGNAL(entered()), this, SIGNAL(delayedPress()));
}

MTapStateMachine::~MTapStateMachine()
{
    Q_D(MTapStateMachine);
    MTheme::releaseStyle(d->style);
    delete d_ptr;
}
