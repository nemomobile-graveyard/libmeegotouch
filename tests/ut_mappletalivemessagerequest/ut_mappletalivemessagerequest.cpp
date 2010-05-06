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

#include "ut_mappletalivemessagerequest.h"

#include <QtTest/QtTest>
#include "mappletalivemessagerequest.h"

void Ut_MAppletAliveMessageRequest::init()
{
    message = new MAppletAliveMessageRequest();
}

void Ut_MAppletAliveMessageRequest::cleanup()
{
    delete message;
}

void Ut_MAppletAliveMessageRequest::testType()
{
    QCOMPARE(message->type(), MAppletMessage::AppletAliveMessageRequest);
}

QTEST_APPLESS_MAIN(Ut_MAppletAliveMessageRequest)
