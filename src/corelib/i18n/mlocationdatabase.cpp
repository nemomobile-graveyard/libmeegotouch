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
#include <QDomDocument>
#include <QStringMatcher>
#include <QDebug>

const QString path = "/usr/share/meegotouch/locationdatabase/";

class MLocationDatabasePrivate
{
public:
    MLocationDatabasePrivate();
    bool loadCountries();
    bool loadCities();

    QHash<QString, MCity> cities;
    QHash<QString, MCountry> countries;
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
        city.setTimeZone( tmpEl.text() );

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


MLocationDatabase::MLocationDatabase()
    : d_ptr( new MLocationDatabasePrivate )
{
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

    foreach( MCountry country, d->countries )
    {
        list.append( country );
    }
    return list;
}


QList<MCity> MLocationDatabase::cities()
{
    Q_D(MLocationDatabase);

    QList<MCity> list;

    foreach( MCity city, d->cities )
    {
        list.append( city );
    }
    return list;
}


QList<MCity> MLocationDatabase::citiesInCountry( const QString& countryKey )
{
    Q_D(MLocationDatabase);

    QList<MCity> list;

    foreach( MCity city, d->cities )
    {
        if ( city.country().key() == countryKey )
        {
            list.append( city );
        }
    }

    return list;
}

QList<MCity> MLocationDatabase::matchingCities(const QString& searchString)
{
    Q_D(MLocationDatabase);

    QList<MCity> list;
    QStringMatcher matcher(searchString, Qt::CaseInsensitive);
    foreach (const MCity &city, d->cities) {
        if (matcher.indexIn(city.englishName()) != -1
            || matcher.indexIn(city.localName()) != -1)
        {
            list.append( city );
        }
    }
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
