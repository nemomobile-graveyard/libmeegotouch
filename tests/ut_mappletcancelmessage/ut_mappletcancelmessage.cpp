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

#include "ut_mappletcancelmessage.h"
#include "mappletcancelmessage.h"

#include <QtTest/QtTest>

void Ut_MAppletCancelMessage::init()
{
    message = new MAppletCancelMessage();
}

void Ut_MAppletCancelMessage::cleanup()
{
    delete message;
}

void Ut_MAppletCancelMessage::testDefaultAppletCancelMessageType()
{
    QCOMPARE(message->type(), MAppletMessage::CancelMessage);
}

QTEST_APPLESS_MAIN(Ut_MAppletCancelMessage)

