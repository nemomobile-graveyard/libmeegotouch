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

#include <QtTest/QtTest>
#include <QDataStream>

#include "mappletmessagefactory.h"
#include "mappletmessage.h"
#include "ut_mappletmessagefactory.h"

void Ut_MAppletMessageFactory::init()
{
}

void Ut_MAppletMessageFactory::cleanup()
{
}

void Ut_MAppletMessageFactory::testCreation()
{
    // Try instantiating an object of unknown type
    MAppletMessage *m = MAppletMessageFactory::create(MAppletMessage::InvalidMessage);
    QVERIFY(m == NULL);

    int i = (int)MAppletMessage::InvalidMessage;
    ++i;
    for (; i < (int)MAppletMessage::NumMessages; ++i) {
        m = MAppletMessageFactory::create((MAppletMessage::MAppletMessageType)i);
        QVERIFY(m != NULL);
        QCOMPARE(m->type(), (MAppletMessage::MAppletMessageType)i);
        delete m;
    }
}

QTEST_APPLESS_MAIN(Ut_MAppletMessageFactory)
