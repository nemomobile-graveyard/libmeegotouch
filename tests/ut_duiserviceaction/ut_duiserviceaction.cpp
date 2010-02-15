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

#include "ut_duiserviceaction.h"
#include <duiserviceaction.h>
#include <duiserviceinvoker.h>
#include <duiaction_stub.h>
#include <duiserviceinvoker_stub.h>
#include "duiaction_p.h"

// DuiActionPrivate stubs
DuiActionPrivate::DuiActionPrivate()
{
}

DuiActionPrivate::~DuiActionPrivate()
{
}

void Ut_DuiServiceAction::init()
{
    m_subject = new DuiServiceAction(NULL);
}

void Ut_DuiServiceAction::cleanup()
{
    delete m_subject;
}

void Ut_DuiServiceAction::initTestCase()
{
}

void Ut_DuiServiceAction::cleanupTestCase()
{
}

void Ut_DuiServiceAction::testAction()
{
    m_subject->trigger();
    QCOMPARE(gDefaultDuiServiceInvokerStub.stubCallCount("invoke"), 1);
}

QTEST_APPLESS_MAIN(Ut_DuiServiceAction)
