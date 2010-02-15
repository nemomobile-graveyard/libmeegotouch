/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "ut_duiserviceinvoker.h"

#include <duiserviceinvoker.h>

void DuiServiceAction::executeService()
{
}

MockServiceAction::MockServiceAction() :
    DuiServiceAction(NULL),
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
void Ut_DuiServiceInvoker::initTestCase()
{
}

// Called after the last testfunction was executed
void Ut_DuiServiceInvoker::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_DuiServiceInvoker::init()
{
}

// Called after every testfunction
void Ut_DuiServiceInvoker::cleanup()
{
}

void Ut_DuiServiceInvoker::testInvoke()
{
    MockServiceAction *action = new MockServiceAction();
    action->executeServiceCalled = false;
    action->trigger();
    delete action;
    QVERIFY(action->executeServiceCalled);
}

QTEST_MAIN(Ut_DuiServiceInvoker)
