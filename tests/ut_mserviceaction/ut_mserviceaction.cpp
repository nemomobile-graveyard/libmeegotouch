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

#include "ut_mserviceaction.h"
#include <mserviceaction.h>
#include <mserviceinvoker.h>
#include <maction_stub.h>
#include <mserviceinvoker_stub.h>
#include "maction_p.h"

// MActionPrivate stubs
MActionPrivate::MActionPrivate()
  : location(MAction::EveryLocation), styleAction(false), q_ptr(0)
{
}

MActionPrivate::~MActionPrivate()
{
}

void Ut_MServiceAction::init()
{
    m_subject = new MServiceAction(NULL);
}

void Ut_MServiceAction::cleanup()
{
    delete m_subject;
}

void Ut_MServiceAction::initTestCase()
{
}

void Ut_MServiceAction::cleanupTestCase()
{
}

void Ut_MServiceAction::testAction()
{
    m_subject->trigger();
    QCOMPARE(gDefaultMServiceInvokerStub.stubCallCount("invoke"), 1);
}

QTEST_APPLESS_MAIN(Ut_MServiceAction)
