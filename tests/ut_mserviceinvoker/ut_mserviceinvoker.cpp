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

#include "ut_mserviceinvoker.h"

#include <mserviceinvoker.h>

void MServiceAction::executeService()
{
}

MockServiceAction::MockServiceAction() :
    MServiceAction(NULL),
    executeServiceCalled(false)
{
}

void MockServiceAction::trigger()
{
    emit triggered();
}

void MockServiceAction::executeService()
{
    executeServiceCalled = true;
}


// Called before the first testfunction is executed
void Ut_MServiceInvoker::initTestCase()
{
}

// Called after the last testfunction was executed
void Ut_MServiceInvoker::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_MServiceInvoker::init()
{
}

// Called after every testfunction
void Ut_MServiceInvoker::cleanup()
{
}

void Ut_MServiceInvoker::testInvoke()
{
    MockServiceAction *action = new MockServiceAction();
    action->executeServiceCalled = false;
    action->trigger();
    QVERIFY(action->executeServiceCalled);
    delete action;
}

QTEST_MAIN(Ut_MServiceInvoker)
