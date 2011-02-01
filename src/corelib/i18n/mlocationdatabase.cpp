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

#include "mlocationdatabase.h"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QStringMatcher>
#include <QStringList>
#include <QDebug>

#ifdef HAVE_ICU
#include <unicode/timezone.h>
#endif

static const QString path = "/usr/share/meegotouch/locationdatabase/";
static const QString zoneAliasFile = "/usr/share/tzdata-timed/zone.alias";
static const QString zoneAliasFileFallback = ":/zone.alias.fallback";

class MLocationDatabasePrivate
{
public:
    MLocationDatabasePrivate();
    bool loadCountries();
    bool loadCities();
    bool loadTimeZoneData();
    QString canonicalizeTimeZoneId(QString timeZoneId);

    QHash<QString, MCity> cities;
    QHash<QString, MCountry> countries;
    QHash<QString, QString> canonicalTimeZoneIds;
};

MLocationDatabasePrivate::MLocationDatabasePrivate()
{
}

bool MLocationDatabasePrivate::loadCountries()
{
    QFile file( path + "countries.xml" );
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug( "loadCountries file open failed" );
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qDebug( "loadCountries setContent failed" );
        file.close();
        return false;
    }
    
    file.close();

    QDomElement docElem = doc.documentElement();
    
    QDomNode n = docElem.firstChild();

    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if ( e.isNull() )
        {
            continue;
        }
        
        if ( e.tagName() != "country" )
        {
            continue;
        }

        // now we found a country key
        MCountry country;
        
        QDomElement tmpEl;

        tmpEl = e.elementsByTagName( "key" ).at( 0 ).toElement();
        country.setKey( tmpEl.text() );

        tmpEl = e.elementsByTagName( "englishname" ).at( 0 ).toElement();
        country.setEnglishName( tmpEl.text() );

        tmpEl = e.elementsByTagName( "localname" ).at( 0 ).toElement();
        country.setLocalName( tmpEl.text() );

        countries[ country.key() ] = country;

        // QTextStream s( stdout );
        // s.setCodec( "UTF-8" );

        // s        << "country: key: " << country.key()
        //          << "\nen:  " << country.englishName()
        //          << "\nloc: " << country.localName() << "\n";

        n = n.nextSibling();
    }

    return true;
}


bool MLocationDatabasePrivate::loadCities()
{
    QFile file( path + "cities.xml" );
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug( "loadCitiess file open failed" );
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qDebug( "loadCities setContent failed" );
        file.close();
        return false;
    }
    
    file.close();

    QDomElement docElem = doc.documentElement();
    
    QDomNode n = docElem.firstChild();

    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if ( e.isNull() )
        {
            continue;
        }
        
        if ( e.tagName() != "city" )
        {
            continue;
        }

        // now we found a country key
        MCity city;
        
        QDomElement tmpEl;

        tmpEl = e.elementsByTagName( "key" ).at( 0 ).toElement();
        city.setKey( tmpEl.text() );

        tmpEl = e.elementsByTagName( "englishname" ).at( 0 ).toElement();
        city.setEnglishName( tmpEl.text() );

        tmpEl = e.elementsByTagName( "localname" ).at( 0 ).toElement();
        city.setLocalName( tmpEl.text() );

        tmpEl = e.elementsByTagName( "timezone" ).at( 0 ).toElement();
        QString timeZoneId = canonicalizeTimeZoneId(tmpEl.text());
        if(timeZoneId.isEmpty()) {
            qWarning() << __PRETTY_FUNCTION__ << "Time zone id"
                       << tmpEl.text() << "cannot be canonicalized. Using it as it is.";
            timeZoneId = tmpEl.text();
        }
        else if(timeZoneId != tmpEl.text()) {
            qWarning() << __PRETTY_FUNCTION__ << "Time zone id"
                       << tmpEl.text() << "canonicalized to" << timeZoneId;
        }
        city.setTimeZone (timeZoneId);

        tmpEl = e.elementsByTagName( "countrykey" ).at( 0 ).toElement();
        QString countryKey = tmpEl.text();

        if ( countries.contains( countryKey ) )
        {
            city.setCountry( countries[ countryKey ] );
        }

        tmpEl = e.elementsByTagName( "latitude" ).at( 0 ).toElement();
        QString latString = tmpEl.text();
        city.setLatitude( latString.toDouble() );

        tmpEl = e.elementsByTagName( "longitude" ).at( 0 ).toElement();
        QString lonString = tmpEl.text();
        city.setLongitude( lonString.toDouble() );

        cities[ city.key() ] = city;

        n = n.nextSibling();
    }

    return true;
}

bool MLocationDatabasePrivate::loadTimeZoneData()
{
    QFile file;
    file.setFileName(zoneAliasFile);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "file" << zoneAliasFile  << "is missing."
                   << "Using fallback, aliases might have problems.";
        file.setFileName(zoneAliasFileFallback);
        if(!file.open(QIODevice::ReadOnly)) {
            qWarning() << "fallback cannot be opened either.";
            return false;
        }
    }
    QTextStream stream(&file);
    QString line;
    do {
        line = stream.readLine();
        QStringList timeZoneIds(line.split(QLatin1Char(' '),
                                           QString::SkipEmptyParts));
        if(!timeZoneIds.isEmpty()) {
            QString canonicalTimeZoneId = timeZoneIds.first();
            foreach(const QString &alias, timeZoneIds) {
                canonicalTimeZoneIds[alias] = canonicalTimeZoneId;
            }
        }
    } while (!line.isNull());
    file.close();
    return true;
}

QString MLocationDatabasePrivate::canonicalizeTimeZoneId(QString timeZoneId)
{
    // returns an empty string if the hash does not contain the key:
    return canonicalTimeZoneIds[timeZoneId];
}

MLocationDatabase::MLocationDatabase()
    : d_ptr( new MLocationDatabasePrivate )
{
    if ( ! d_ptr->loadTimeZoneData() )
    {
        qWarning( "loading of time zone data failed." );
    }
    if ( ! d_ptr->loadCountries() )
    {
        qWarning( "loading of country list failed." );
    }

    if ( ! d_ptr->loadCities() )
    {
        qWarning( "loading of city list failed." );
    }
}


MLocationDatabase::~MLocationDatabase()
{
    delete d_ptr;
}


QList<MCountry> MLocationDatabase::countries()
{
    Q_D(MLocationDatabase);

    QList<MCountry> list;

    foreach( const MCountry& country, d->countries )
    {
        list.append( country );
    }
    return list;
}


QList<MCity> MLocationDatabase::cities()
{
    Q_D(MLocationDatabase);

    QList<MCity> list;

    foreach( const MCity& city, d->cities )
    {
        list.append( city );
    }
    return list;
}


QList<MCity> MLocationDatabase::citiesInCountry( const QString& countryKey )
{
    Q_D(MLocationDatabase);

    QList<MCity> list;

    foreach( const MCity& city, d->cities )
    {
        if ( city.country().key() == countryKey )
        {
            list.append( city );
        }
    }

    return list;
}

QList<MCity> MLocationDatabase::citiesInTimeZone(const QString& timeZoneId)
{
    Q_D(MLocationDatabase);
    QList<MCity> list;
    QString canonicalTimeZoneId = d->canonicalizeTimeZoneId(timeZoneId);
    if(canonicalTimeZoneId.isEmpty())
        return list;
    // Cut out last section of timezone id, for example cut out
    // “Tell_City” out of “America/Indiana/Tell_City” In case of
    // canonical time zone ids, the part after the last / seems to be
    // a city in most cases, although there are exceptions (For
    // example “Indian/Mahe” is a canonical id but “Mahe” is an
    // island, not a city. There are many non-canonical time zone ids
    // which do not have a city name in the last part, for example
    // “US/Pacific”.
    QString canonicalCity = canonicalTimeZoneId.section('/', -1);
    canonicalCity.replace('_', ' ');

    foreach(const MCity& city, d->cities) {
        // city.timeZone is already canonical
        if (city.timeZone() == canonicalTimeZoneId) {
            QString englishName = city.englishName();
            if(englishName.contains(canonicalCity))
                list.prepend(city);
            else
                list.append(city);
        }
    }
    return list;
}

QList<MCity> MLocationDatabase::matchingCities(const QString& searchString)
{
    Q_D(MLocationDatabase);

    QList<MCity> list;
    QStringMatcher *matcher = new QStringMatcher(searchString, Qt::CaseInsensitive);
    foreach (const MCity &city, d->cities) {
        if (matcher->indexIn(city.englishName()) != -1
            || matcher->indexIn(city.localName()) != -1)
        {
            list.append( city );
        }
    }
    delete matcher;
    return list;
}

MCity MLocationDatabase::nearestCity(qreal latitude, qreal longitude)
{
    Q_D(MLocationDatabase);

    MCity bestCity;

    qreal bestDistance = 1000000;

    QList<MCity> list;
    foreach(const MCity &city, d->cities) {
        qreal distance =
            (latitude - city.latitude()) * (latitude - city.latitude())
            + (longitude - city.longitude()) * (longitude - city.longitude());
        if ( distance < bestDistance ) {
            bestDistance = distance;
            bestCity = city;
        }
    }
    return bestCity;
}
