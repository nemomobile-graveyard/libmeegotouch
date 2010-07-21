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

class TestLocationDatabase : public MLocationDatabase
{
};

class TestCity : public MCity
{
};

class TestCountry : public MCountry
{
};

void Ut_MLocationDatabase::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "ut_mlocationdatabase" };
    qap = new MApplication(dummyArgc, dummyArgv, "test");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Ut_MLocationDatabase::cleanupTestCase()
{
}

void Ut_MLocationDatabase::testConstructors()
{
    TestCountry *testCountryFoo = new TestCountry();
    testCountryFoo->setKey("qtn_clk_country_finland");
    testCountryFoo->setEnglishName("Finland");
    testCountryFoo->setLocalName("Suomi");
    TestCity *testCityFoo = new TestCity();
    testCityFoo->setKey("qtn_clk_city_fin_hki");
    testCityFoo->setEnglishName("Helsinki");
    testCityFoo->setLocalName("Helsinki");
    testCityFoo->setLatitude(60.1667);
    testCityFoo->setLongitude(24.9667);
    testCityFoo->setTimeZone("Europe/Helsinki");
    testCityFoo->setCountry(*testCountryFoo);

    // copy constructor
    MCity *cityBar = new MCity(*testCityFoo);
    QCOMPARE(cityBar->key(), testCityFoo->key());
    cityBar->setKey("something_which_is_not_"+testCityFoo->key());
    QVERIFY(cityBar->key() != testCityFoo->key());
    // assignment operator
    *cityBar = *testCityFoo;
    QVERIFY(cityBar->key() == testCityFoo->key());

    // copy constructor
    TestCity *testCityBar = new TestCity(*testCityFoo);
    QCOMPARE(testCityBar->key(), testCityFoo->key());
    testCityBar->setKey("something_which_is_not_"+testCityFoo->key());
    QVERIFY(testCityBar->key() != testCityFoo->key());
    // assignment operator
    *testCityBar = *testCityFoo;
    QVERIFY(testCityBar->key() == testCityFoo->key());

    // copy constructor
    MCountry *countryBar = new MCountry(*testCountryFoo);
    QCOMPARE(countryBar->key(), testCountryFoo->key());
    countryBar->setKey("something_which_is_not_"+testCountryFoo->key());
    QVERIFY(countryBar->key() != testCountryFoo->key());
    // assignment operator
    *countryBar = *testCountryFoo;
    QVERIFY(countryBar->key() == testCountryFoo->key());

    // copy constructor
    TestCountry *testCountryBar = new TestCountry(*testCountryFoo);
    QCOMPARE(testCountryBar->key(), testCountryFoo->key());
    testCountryBar->setKey("something_which_is_not_"+testCountryFoo->key());
    QVERIFY(testCountryBar->key() != testCountryFoo->key());
    // assignment operator
    *testCountryBar = *testCountryFoo;
    QVERIFY(testCountryBar->key() == testCountryFoo->key());
    
    delete testCityFoo;
    delete testCityBar;
    delete testCountryFoo;
    delete testCountryBar;
    delete countryBar;
    delete cityBar;

    MLocationDatabase *locationDatabase = new MLocationDatabase();
    TestLocationDatabase *testLocationDatabase = new TestLocationDatabase();
    delete locationDatabase;
    delete testLocationDatabase;
}

void Ut_MLocationDatabase::testNearestCity_data()
{
    QTest::addColumn<qreal>("latitude");
    QTest::addColumn<qreal>("longitude");
    QTest::addColumn<QString>("resultEnglishName");

    QTest::newRow("near Espoo")
        << qreal(60.205556)
        << qreal(24.655556)
        << "Helsinki";
}

void Ut_MLocationDatabase::testNearestCity()
{
    QFETCH(qreal, latitude);
    QFETCH(qreal, longitude);
    QFETCH(QString, resultEnglishName);

    MLocationDatabase db;
    QList<MCity> cities = db.cities();

    // do only run the tests, if we were able to load
    // some cities from the meegotouch-cities-*
    // package.
    if (cities.count() < 10) {
        qWarning( "loading of city list failed, skipping test" );
        return;
    }
    QCOMPARE(db.nearestCity(latitude, longitude).englishName(),
             resultEnglishName);
}

void Ut_MLocationDatabase::testMatchingCities_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QStringList>("someExpectedMatches");

    QTest::newRow("ber")
        << "ber"
        << (QStringList() << "Berlin" << "Bern");
}

void Ut_MLocationDatabase::testMatchingCities()
{
    QFETCH(QString, pattern);
    QFETCH(QStringList, someExpectedMatches);

    MLocationDatabase db;
    QList<MCity> cities = db.cities();

    // do only run the tests, if we were able to load
    // some cities from the meegotouch-cities-*
    // package.
    if (cities.count() < 10) {
        qWarning( "loading of city list failed, skipping test" );
        return;
    }
    QList<MCity> matchingCities = db.matchingCities(pattern);
    qDebug() << "number of matching cities" << matchingCities.size();
    QStringList matchingCitiesNames;
    foreach (MCity city, matchingCities) {
        qDebug() << "matching city:" << city.englishName();
        matchingCitiesNames << city.englishName();
    }
    foreach (QString expectedMatch, someExpectedMatches) {
        qDebug() << "expected match:" << expectedMatch;
        QVERIFY2(matchingCitiesNames.contains(expectedMatch),
                 "expected match not found");
    }
}

void Ut_MLocationDatabase::testCitiesInCountry_data()
{
    QTest::addColumn<QString>("countryKey");
    QTest::addColumn<QStringList>("someExpectedMatches");

    QTest::newRow("Mexico")
        << "qtn_clk_country_mexico"
        << (QStringList() << "Chihuahua" << "Tijuana");
}

void Ut_MLocationDatabase::testCitiesInCountry()
{
    QFETCH(QString, countryKey);
    QFETCH(QStringList, someExpectedMatches);

    MLocationDatabase db;
    QList<MCountry> countries = db.countries();

    // do only run the tests, if we were able to load
    // some countries from the meegotouch-cities-*
    // package.
    if (countries.count() < 10) {
        qWarning( "loading of country list failed, skipping test" );
        return;
    }
    QList<MCity> citiesInCountry = db.citiesInCountry(countryKey);
    qDebug() << "number of cities in country" << citiesInCountry.size();
    QStringList citiesInCountryNames;
    foreach (MCity city, citiesInCountry) {
        qDebug() << "city in country:" << city.englishName();
        citiesInCountryNames << city.englishName();
    }
    foreach (QString expectedMatch, someExpectedMatches) {
        qDebug() << "expected match:" << expectedMatch;
        QVERIFY2(citiesInCountryNames.contains(expectedMatch),
                 "expected match not found");
    }
}

void Ut_MLocationDatabase::testCities_data()
{
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("englishName");
    QTest::addColumn<QString>("localName");
    QTest::addColumn<qreal>("latitude");
    QTest::addColumn<qreal>("longitude");
    QTest::addColumn<QString>("timeZone");
    QTest::addColumn<QString>("countryEnglishName");
    QTest::addColumn<QString>("countryLocalName");
    QTest::addColumn<QDateTime>("dateTime");
    QTest::addColumn<qint32>("timeZoneRawOffset");
    QTest::addColumn<qint32>("timeZoneDstOffset");
    QTest::addColumn<qint32>("timeZoneTotalOffset");

    QTest::newRow("Helsinki, Winter 2010-03-28 3:59:59 local")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 3, 28), QTime(3, 59, 59, 0), Qt::LocalTime)
        << 2 * 3600 * 1000  // 2 hours always
        << 0 * 3600 * 1000  // 0 hours in winter
        << 2 * 3600 * 1000; // 2 hours in winter
    QTest::newRow("Helsinki, Summer 2010-03-28 4:00:00 local")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 3, 28), QTime(4, 0, 0, 0), Qt::LocalTime)
        << 2 * 3600 * 1000  // 2 hours always
        << 1 * 3600 * 1000  // 1 hours in summer
        << 3 * 3600 * 1000; // 3 hours in summer
    QTest::newRow("Helsinki, Winter 2010-03-28 0:59:59 utc")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 3, 28), QTime(0, 59, 59, 0), Qt::UTC)
        << 2 * 3600 * 1000  // 2 hours always
        << 0 * 3600 * 1000  // 0 hours in winter
        << 2 * 3600 * 1000; // 2 hours in winter
    QTest::newRow("Helsinki, Summer 2010-03-28 1:00:00 utc")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 3, 28), QTime(1, 0, 0, 0), Qt::UTC)
        << 2 * 3600 * 1000  // 2 hours always
        << 1 * 3600 * 1000  // 1 hours in summer
        << 3 * 3600 * 1000; // 3 hours in summer
    //--------------------------------------------------
    QTest::newRow("Helsinki, Summer 2010-10-31 0:59:59 local")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 10, 31), QTime(2, 59, 59, 0), Qt::LocalTime)
        << 2 * 3600 * 1000  // 2 hours always
        << 1 * 3600 * 1000  // 1 hours in summer
        << 3 * 3600 * 1000; // 3 hours in summer
    QTest::newRow("Helsinki, Winter 2010-10-31 1:00:00 local")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 10, 31), QTime(3, 0, 0, 0), Qt::LocalTime)
        << 2 * 3600 * 1000  // 2 hours always
        << 0 * 3600 * 1000  // 0 hours in winter
        << 2 * 3600 * 1000; // 2 hours in winter
    QTest::newRow("Helsinki, Summer 2010-10-31 0:59:59 utc")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 10, 31), QTime(0, 59, 59, 0), Qt::UTC)
        << 2 * 3600 * 1000  // 2 hours always
        << 1 * 3600 * 1000  // 1 hours in summer
        << 3 * 3600 * 1000; // 3 hours in summer
    QTest::newRow("Helsinki, Winter 2010-10-31 1:00:00 utc")
        << "qtn_clk_city_fin_hki"
        << "Helsinki"
        << "" // local name currently always empty
        << qreal(60.1667) << qreal(24.9667)
        << "Europe/Helsinki"
        << "Finland"
        << "" // local name currently always empty
        << QDateTime(QDate(2010, 10, 31), QTime(1, 0, 0, 0), Qt::UTC)
        << 2 * 3600 * 1000  // 2 hours always
        << 0 * 3600 * 1000  // 0 hours in winter
        << 2 * 3600 * 1000; // 2 hours in winter
}

void Ut_MLocationDatabase::testCities()
{
    QFETCH(QString, key);
    QFETCH(QString, englishName);
    QFETCH(QString, localName);
    QFETCH(qreal, latitude);
    QFETCH(qreal, longitude);
    QFETCH(QString, timeZone);
    QFETCH(QString, countryEnglishName);
    QFETCH(QString, countryLocalName);
    QFETCH(QDateTime, dateTime);
    QFETCH(qint32, timeZoneRawOffset);
    QFETCH(qint32, timeZoneDstOffset);
    QFETCH(qint32, timeZoneTotalOffset);

    MLocationDatabase db;
    QList<MCity> cities = db.cities();

    // do only run the tests, if we were able to load
    // some cities from the meegotouch-cities-*
    // package.
    if (cities.count() < 10) {
        qWarning( "loading of city list failed, skipping test" );
        return;
    }

    bool found = false;
    MCity foundCity;
    foreach(MCity city, cities) {
        if (city.key() == key) {
            foundCity = city;
            found = true;
        }
    }
    QVERIFY(found);
    QCOMPARE(foundCity.key(), key);
    QCOMPARE(foundCity.englishName(), englishName);
    QCOMPARE(foundCity.localName(), localName);
    QCOMPARE(foundCity.country().englishName(), countryEnglishName);
    QCOMPARE(foundCity.country().localName(), countryLocalName);
    qDebug() << "found latitude" << foundCity.latitude()
             << "expected latitude" << latitude;
    QVERIFY(qAbs(foundCity.latitude() - latitude)   < 0.01);
    qDebug() << "found longitude" << foundCity.longitude()
             << "expected longitude" << longitude;
    QVERIFY(qAbs(foundCity.longitude() - longitude) < 0.01);
    QCOMPARE(foundCity.timeZone(), timeZone);
    QCOMPARE(foundCity.timeZoneRawOffset(), timeZoneRawOffset);
    QCOMPARE(foundCity.timeZoneDstOffset(dateTime), timeZoneDstOffset);
    QCOMPARE(foundCity.timeZoneTotalOffset(dateTime), timeZoneTotalOffset);
}

void Ut_MLocationDatabase::testCitiesDumpInfo()
{
    // This test dumps lots of information from the database to detect
    // changes and serves as a simple benchmark whether all the
    // information can be gathered in reasonable time.
#if QT_VERSION >= 0x040700
    QElapsedTimer timer;
    timer.start();
#endif
    MLocationDatabase db;
    QList<MCity> cities = db.cities();

    // do only run the tests, if we were able to load
    // some cities from the meegotouch-cities-*
    // package.
    if (cities.count() < 10) {
        qWarning( "loading of city list failed, skipping test" );
        return;
    }

    QDateTime summerDateTime(QDate(2010, 6, 21), QTime(0, 0, 0, 0), Qt::LocalTime);
    QDateTime winterDateTime(QDate(2009, 12, 24), QTime(0, 0, 0, 0), Qt::LocalTime);
    QString ut_mlocationdatabaseTestOutput = "";
    foreach(MCity city, cities) {
        ut_mlocationdatabaseTestOutput
            += city.key() + "\tEnglish Name\t" + city.englishName()
            + '\n' + city.key() + "\tCountry English Name\t"
            + city.country().englishName()
            + '\n' + city.key() + "\tLatitude\t"
            + QString::number(city.latitude())
            + '\n' + city.key() + "\tLongitude\t"
            + QString::number(city.longitude())
            + '\n' + city.key() + "\tTime zone id\t"
            + city.timeZone()
            + '\n' + city.key() + "\tTime zone raw offset in hours\t"
            + QString::number(city.timeZoneRawOffset() / 1000 / 3600)
            + '\n' + city.key() + "\tTime zone dst offset summer in hours\t"
            + QString::number(city.timeZoneDstOffset(summerDateTime) / 1000 / 3600)
            + '\n' + city.key() + "\tTime zone dst offset winter in hours\t"
            + QString::number(city.timeZoneDstOffset(winterDateTime) / 1000 / 3600)
            + '\n' + city.key() + "\tTime zone total offset summer in hours\t"
            + QString::number(city.timeZoneTotalOffset(summerDateTime) / 1000 / 3600)
            + '\n' + city.key() + "\tTime zone total offset winter in hours\t"
            + QString::number(city.timeZoneTotalOffset(winterDateTime) / 1000 / 3600)
            + '\n';
    }
    QString ut_mlocationdatabaseTestOutputFileName =
        "/tmp/ut_mlocationdatabase.txt";
    QFile ut_mlocationdatabaseTestOutputFile(ut_mlocationdatabaseTestOutputFileName);
    if (!ut_mlocationdatabaseTestOutputFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        QFAIL(qPrintable("could not open file " + ut_mlocationdatabaseTestOutputFileName));
    int bytesWritten = ut_mlocationdatabaseTestOutputFile.write(ut_mlocationdatabaseTestOutput.toUtf8().constData());
    if (bytesWritten == -1)
        QFAIL(qPrintable("could not write to file" + ut_mlocationdatabaseTestOutputFileName));
    QCOMPARE(uint(bytesWritten), qstrlen(ut_mlocationdatabaseTestOutput.toUtf8().constData()));
    ut_mlocationdatabaseTestOutputFile.close();

    QString ut_mlocationdatabaseTestInputFileName =
        qApp->applicationDirPath() + QDir::separator() + "ut_mlocationdatabase-test-input.txt";
    QFile ut_mlocationdatabaseTestInputFile(ut_mlocationdatabaseTestInputFileName);
    if (!ut_mlocationdatabaseTestInputFile.open(QIODevice::ReadOnly))
        QFAIL(qPrintable("could not open file " + ut_mlocationdatabaseTestInputFileName));
    QString ut_mlocationdatabaseTestInput = QString::fromUtf8(ut_mlocationdatabaseTestInputFile.readAll().constData());
    ut_mlocationdatabaseTestInputFile.close();

    // QTextStream debugStream(stderr);
    // debugStream.setCodec("UTF-8");
    // debugStream << ut_mlocationdatabaseTestInput;

    if (ut_mlocationdatabaseTestOutput != ut_mlocationdatabaseTestInput) {
        // don’t fail if there is a difference, there can easily
        // be differences due to changes in the database.
        // Just show the difference on standard output for easy checking
        // what has changed:
        QProcess::execute("diff -u " + ut_mlocationdatabaseTestInputFileName + ' ' + ut_mlocationdatabaseTestOutputFileName);
    }
#if QT_VERSION >= 0x040700
    qDebug() << __PRETTY_FUNCTION__ << "took" << timer.restart() << "milliseconds";
#endif
}

QTEST_APPLESS_MAIN(Ut_MLocationDatabase);
