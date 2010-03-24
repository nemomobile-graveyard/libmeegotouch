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

#include "ut_duiappletcancelmessage.h"
#include "duiappletcancelmessage.h"

#include <QtTest/QtTest>

void Ut_DuiAppletCancelMessage::init()
{
    message = new DuiAppletCancelMessage();
}

void Ut_DuiAppletCancelMessage::cleanup()
{
    delete message;
}

void Ut_DuiAppletCancelMessage::testDefaultAppletCancelMessageType()
{
    QCOMPARE(message->type(), DuiAppletMessage::CancelMessage);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletCancelMessage)

