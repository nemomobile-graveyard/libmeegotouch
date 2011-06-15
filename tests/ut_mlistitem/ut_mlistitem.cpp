/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <MApplication>
#include "ut_mlistitem.h"

void Ut_MListItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MGraphicsClipEffect" };
    app = new MApplication(argc, app_name);
}

void Ut_MListItem::cleanupTestCase()
{
    delete app;
}

void Ut_MListItem::init()
{
    listItem = new MListItem ();
}

void Ut_MListItem::cleanup()
{
    delete listItem;
}

void Ut_MListItem::testClick()
{
    listItem->click();
}

void Ut_MListItem::testLongTap()
{
    QPointF point (0, 0);
    listItem->longTap(point);
}

void Ut_MListItem::testClicked()
{
    listItem->clicked();
}

void Ut_MListItem::testLongTapped()
{
    QPointF point (0, 0);
    listItem->longTapped(point);
}

QTEST_APPLESS_MAIN(Ut_MListItem)
