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

#include "ut_mdynamicpropertywatcher.h"
#include <mdynamicpropertywatcher.h>

void Ut_MDynamicPropertyWatcher::testDontEmitSignalIfPropertySetToSameValue()
{
    MDynamicPropertyWatcher propertyWatcher;
    QObject object;
    QSignalSpy propertyChangedSpy(&propertyWatcher, SIGNAL(propertyChanged()));

    propertyWatcher.watch(&object);
    propertyWatcher.setPropertyName("foo");

    object.setProperty("foo", true);
    QCOMPARE(propertyChangedSpy.count(), 1);

    object.setProperty("foo", true);
    QCOMPARE(propertyChangedSpy.count(), 1); // no new signal emitted
}

void Ut_MDynamicPropertyWatcher::testEmitSignalIfPropertySetToDifferentValue()
{
    MDynamicPropertyWatcher propertyWatcher;
    QObject object;
    QSignalSpy propertyChangedSpy(&propertyWatcher, SIGNAL(propertyChanged()));

    propertyWatcher.watch(&object);
    propertyWatcher.setPropertyName("foo");

    object.setProperty("foo", true);
    QCOMPARE(propertyChangedSpy.count(), 1);

    object.setProperty("foo", false);
    QCOMPARE(propertyChangedSpy.count(), 2);
}

QTEST_MAIN(Ut_MDynamicPropertyWatcher)