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
#include "ut_mlocationdatabase.h"

#include "mlocationdatabase.h"
#include "mcity.h"
#include "mcountry.h"

void Ut_MLocationDatabase::initTestCase()
{
}

void Ut_MLocationDatabase::cleanupTestCase()
{
}

void Ut_MLocationDatabase::testHelsinki()
{
    MLocationDatabase db;
    QList<MCity> cities = db.cities();

    // do only run the tests, if we were able to load
    // some cities from the meegotouch-cities-*
    // package.
    if ( cities.count() < 10 )
    {
        qWarning( "loading of city list failed, skipping test" );
        return;
    }

    bool foundHelsinki = false;

    MCity helsinki;
    foreach( MCity city, cities )
    {
        if ( city.englishName() == "Helsinki" )
        {
            helsinki = city;
            foundHelsinki = true;
        }
    }

    QVERIFY( foundHelsinki );

    // 60.16449 latitude north / 24.94755 longitude east

    QVERIFY( helsinki.latitude() > 60.1 );
    QVERIFY( helsinki.latitude() < 60.2 );

    QVERIFY( helsinki.longitude() > 24.9 );
    QVERIFY( helsinki.longitude() < 25.0 );

    QVERIFY( helsinki.englishName() == "Helsinki" );

    QVERIFY( helsinki.country().englishName() == "Finland" );
}

QTEST_APPLESS_MAIN(Ut_MLocationDatabase);
