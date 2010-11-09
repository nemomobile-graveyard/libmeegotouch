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

#include "pt_mlocationdatabase.h"

#include "mlocationdatabase.h"
#include "mcity.h"
#include "mcountry.h"

void Pt_MLocationDatabase::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Pt_MLocationDatabase::cleanupTestCase()
{
    delete qap;
}

void Pt_MLocationDatabase::init()
{
}

void Pt_MLocationDatabase::cleanup()
{
}

void Pt_MLocationDatabase::benchmarkMLocationDatabaseConstructorDestructor()
{
    QBENCHMARK {
        MLocationDatabase *db = new MLocationDatabase;
        delete db;
    }
}

void Pt_MLocationDatabase::benchmarkTimeZone()
{
    MLocationDatabase db;
    QList<MCity> cities = db.cities();

    // do only run the benchmark if we were able to load
    // some cities from the meegotouch-cities-*
    // package.
    if (cities.count() < 10) {
        qWarning( "loading of city list failed, skipping test" );
        return;
    }

    qDebug() << "number of cities = " << cities.count();
    QBENCHMARK {
        foreach (MCity city, cities) {
            city.timeZone();
        }
    }
}

void Pt_MLocationDatabase::benchmarkMatchingCities()
{
    QString pattern("ber");
    QStringList someExpectedMatches
        = (QStringList() << "Berlin" << "Bern");

    MLocationDatabase db;
    QList<MCity> cities = db.cities();

    // do only run the benchmark if we were able to load
    // some cities from the meegotouch-cities-*
    // package.
    if (cities.count() < 10) {
        qWarning( "loading of city list failed, skipping test" );
        return;
    }
    QList<MCity> matchingCities = db.matchingCities(pattern);
    QStringList matchingCitiesNames;
    foreach (MCity city, matchingCities) {
        matchingCitiesNames << city.englishName();
    }
    foreach (QString expectedMatch, someExpectedMatches) {
        QVERIFY2(matchingCitiesNames.contains(expectedMatch),
                 "expected match not found");
    }
    QBENCHMARK {
        db.matchingCities(pattern);
    }
}

QTEST_APPLESS_MAIN(Pt_MLocationDatabase);

