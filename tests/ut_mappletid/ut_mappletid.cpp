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

#include <mappletid.h>
#include "ut_mappletid.h"

void Ut_MAppletId::init()
{
    m_subject = new MAppletId();
}

void Ut_MAppletId::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MAppletId::initTestCase()
{
}

void Ut_MAppletId::cleanupTestCase()
{
}

void Ut_MAppletId::testAppletInstanceIdentifier()
{
    QCOMPARE(m_subject->toString(), QString("//0"));
    QCOMPARE(m_subject->instanceId(), uint(0));

    delete m_subject;
    m_subject = new MAppletId("Foo", "FooCanvas", 1);
    QCOMPARE(m_subject->toString(), QString("Foo/FooCanvas/1"));
    QCOMPARE(m_subject->instanceId(), uint(1));
}

QTEST_MAIN(Ut_MAppletId)
