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

#ifndef MLOCATIONDATABASE_H
#define MLOCATIONDATABASE_H

#include "mexport.h"

#include <QList>

#include "mcity.h"
#include "mcountry.h"

class MLocationDatabasePrivate;

class M_CORE_EXPORT MLocationDatabase
{
  public:
    MLocationDatabase();
    virtual ~MLocationDatabase();

    /**
     * \brief returns a list with all known countries
     */
    QList<MCountry> countries();

    /**
     * \brief returns a list with all known cities
     */
    QList<MCity> cities();

    /**
     * \brief returns a list with all cities in a country
     */
    QList<MCity> citiesInCountry( const QString& countryKey );

    /**
     * \brief returns a list with all cities in a time zone
     * \param timeZoneId the Olson id of the timezone
     *
     * Time zone ids are “canonicalized” when comparing, i.e.
     * citiesInTimeZone("US/Pacific") and
     * citiesInTimeZone("America/Los_Angeles") will return the same
     * list.
     *
     * I.e. this lists all cities which have the same canonical
     * timezone id as the id given as parameter.
     *
     * It tries to put the “most important” city for this timezone id
     * first in the list of cities returned. Currently it does this
     * by putting a city first if it happens to have the same name
     * as the part of the canonical timezone id after the last “/”.
     *
     * For invalid time zone ids like "Oz/Emerald_City", an empty list
     * is returned.
     */
    QList<MCity> citiesInTimeZone(const QString& timeZoneId);

    /**
     * \brief returns a list with all cities that contain the given searchString
     */
    QList<MCity> matchingCities( const QString& searchString );

    /**
     * \brief returns the city with the nearest position to the given location.
     */
    MCity nearestCity( qreal latitude, qreal longitude );

  private:
    MLocationDatabasePrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MLocationDatabase)

};

#endif
