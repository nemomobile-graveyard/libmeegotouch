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

#ifndef MTAPSTATEMACHINE_P_H
#define MTAPSTATEMACHINE_P_H

#include "mtapstatemachine.h"

#include "mcancelevent.h"

#include <QAbstractTransition>
#include <MCancelEvent>

class QStateMachine;
class QSignalTransition;
class QEventTransition;
class QState;
class QTimer;
class MTapStateMachineStyle;

class MCancelEventTransition : public QAbstractTransition
{
public:
    MCancelEventTransition(QObject *target);

protected:
    virtual bool eventTest(QEvent *e);
    virtual void onTransition(QEvent *);
    bool eventFilter(QObject *, QEvent *event);
};

class MTapStateMachinePrivate
{
public:
    MTapStateMachinePrivate();
    virtual ~MTapStateMachinePrivate();

protected:
    const MTapStateMachineStyle* style;

    QStateMachine *highlightMachine;
    QState *initialWaitState;
    QState *pressHighlightedState;
    QState *timedHighlightedState;
    QState *shortBlinkState;
    QState *unhighlightedState;

    QEventTransition *initialMousePressTransition;
    MCancelEventTransition *initCancelEventTransition;
    QSignalTransition *timerHighlightTransition;
    QEventTransition *releaseUnhighlightTransition;
    QEventTransition *releaseTimedHighlightTransition;
    QSignalTransition *timerUnhighlightTransition;
    MCancelEventTransition *pressedCancelEventTransition;

    QTimer *initialWaitTimer;
    QTimer *minimumHighlightTimer;

protected:
    MTapStateMachine *q_ptr;

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_MTapStateMachine;
#endif

private:
    Q_DECLARE_PUBLIC(MTapStateMachine)
};

#endif // MTAPSTATEMACHINE_P_H
