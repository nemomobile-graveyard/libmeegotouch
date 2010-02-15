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

#include "ut_duiactionprovider.h"
#include <duiactionprovider.h>
#include <duiserviceaction_stub.h>

void Ut_DuiActionProvider::init()
{
}

void Ut_DuiActionProvider::cleanup()
{
}

void Ut_DuiActionProvider::initTestCase()
{
}

void Ut_DuiActionProvider::cleanupTestCase()
{
}

void Ut_DuiActionProvider::testGetDefaultAction()
{
    QUrl uri(QUrl::fromLocalFile("image.png"));
    DuiAction *action = DuiActionProvider::getDefaultAction(uri);
    QVERIFY(action != NULL);
    delete action;
}

QTEST_APPLESS_MAIN(Ut_DuiActionProvider)
