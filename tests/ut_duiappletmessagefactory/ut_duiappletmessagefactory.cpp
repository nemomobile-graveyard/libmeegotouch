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

#include <QtTest/QtTest>
#include <QDataStream>

#include "duiappletmessagefactory.h"
#include "duiappletmessage.h"
#include "ut_duiappletmessagefactory.h"

void Ut_DuiAppletMessageFactory::init()
{
}

void Ut_DuiAppletMessageFactory::cleanup()
{
}

void Ut_DuiAppletMessageFactory::testCreation()
{
    // Try instantiating an object of unknown type
    DuiAppletMessage *m = DuiAppletMessageFactory::create(DuiAppletMessage::INVALID);
    QVERIFY(m == NULL);

    int i = (int)DuiAppletMessage::INVALID;
    ++i;
    for (; i < (int)DuiAppletMessage::NUM_TYPES; ++i) {
        m = DuiAppletMessageFactory::create((DuiAppletMessage::DuiAppletMessageType)i);
        QVERIFY(m != NULL);
        QCOMPARE(m->type(), (DuiAppletMessage::DuiAppletMessageType)i);
        delete m;
    }
}

QTEST_APPLESS_MAIN(Ut_DuiAppletMessageFactory)
