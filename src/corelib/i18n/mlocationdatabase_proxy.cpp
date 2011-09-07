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

#include "mlocationdatabase.h"
#include "mlocale/mlocationdatabase.h"
#include "mcity_proxy_p.h"
#include "mcountry_proxy_p.h"

class MLocationDatabasePrivate
{
public:
    MLocationDatabasePrivate() : pDatabase( new ML10N::MLocationDatabase() )
        {
        }
    ~MLocationDatabasePrivate()
        {
            delete pDatabase;
        }

    QList<MCity> convertCities( const QList<ML10N::MCity>& cl );
    MCity nearestCity(qreal latitude, qreal longitude);

    QList<MCountry> countries();

    ML10N::MLocationDatabase *pDatabase;
};


QList<MCity> MLocationDatabasePrivate::convertCities( const QList<ML10N::MCity>& cl )
{
    QList<MCity> cityList;

    foreach (const ML10N::MCity &c, cl)
    {
        MCity city;
        *city.d_ptr->pCity = c;
        cityList.append( city );
    }

    return cityList;
}


MLocationDatabase::MLocationDatabase()
    : d_ptr( new MLocationDatabasePrivate )
{
}


MLocationDatabase::~MLocationDatabase()
{
    delete d_ptr;
}


QList<MCountry> MLocationDatabasePrivate::countries()
{
    QList<ML10N::MCountry> cl = pDatabase->countries();

    QList<MCountry> countryList;

    foreach (const ML10N::MCountry &c, cl)
    {
        MCountry country;
        *country.d_ptr->pCountry = c;
        countryList.append( country );
    }

    return countryList;
}

QList<MCountry> MLocationDatabase::countries()
{
    return d_ptr->countries();
}

QList<MCity> MLocationDatabase::cities()
{
    return d_ptr->convertCities( d_ptr->pDatabase->cities() );
}


QList<MCity> MLocationDatabase::citiesInCountry( const QString& countryKey )
{
    return d_ptr->convertCities( d_ptr->pDatabase->citiesInCountry( countryKey ) );
}


QList<MCity> MLocationDatabase::citiesInTimeZone(const QString& timeZoneId)
{
    return d_ptr->convertCities( d_ptr->pDatabase->citiesInTimeZone( timeZoneId ) );
}


QList<MCity> MLocationDatabase::matchingCities(const QString& searchString)
{
    return d_ptr->convertCities( d_ptr->pDatabase->matchingCities( searchString ) );
}


MCity MLocationDatabasePrivate::nearestCity(qreal latitude, qreal longitude)
{
    MCity city;
    *city.d_ptr->pCity = pDatabase->nearestCity( latitude, longitude );
    return city;
}


MCity MLocationDatabase::nearestCity(qreal latitude, qreal longitude)
{
    return d_ptr->nearestCity( latitude, longitude );
}
