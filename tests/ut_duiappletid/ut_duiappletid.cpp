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

#include <duiappletid.h>
#include "ut_duiappletid.h"

void Ut_DuiAppletId::init()
{
    m_subject = new DuiAppletId();
}

void Ut_DuiAppletId::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiAppletId::initTestCase()
{
}

void Ut_DuiAppletId::cleanupTestCase()
{
}

void Ut_DuiAppletId::testAppletInstanceIdentifier()
{
    QCOMPARE(m_subject->toString(), QString("//0"));
    QCOMPARE(m_subject->instanceId(), uint(0));

    delete m_subject;
    m_subject = new DuiAppletId("Foo", "FooCanvas", 1);
    QCOMPARE(m_subject->toString(), QString("Foo/FooCanvas/1"));
    QCOMPARE(m_subject->instanceId(), uint(1));
}

QTEST_MAIN(Ut_DuiAppletId)
