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

#ifndef MCITY_H
#define MCITY_H

#include <QDateTime>

#include "mexport.h"
#include "mcountry.h"

class MCityPrivate;

class M_CORE_EXPORT MCity
{
  public:
    MCity();
    MCity(const MCity&);
    virtual ~MCity();
    MCity& operator=(const MCity&);

    /**
     * \brief returns the unique key for the city
     *
     * Example:
     * something like “qtn_clk_city_fin_hki” might be returned
     * for Helsinki. This key can be used in qtTrId() to get the
     * translation for “Helsinki”:
     *
     * \code
     * qtTrId("qtn_clk_city_fin_hki");
     * \endcode
     */
    QString key() const;

    /**
     * \brief returns the english name for the city
     *
     * Example:
     * might return something like “Helsinki”.
     */
    QString englishName() const;

    /**
     * \brief returns the local name for the city
     *
     * Example:
     * For a city with the English name “Rome” this
     * function might return “Roma”.
     */
    QString localName() const;

    /**
     * \brief returns the latitude for the city
     */
    qreal latitude() const;

    /**
     * \brief returns the longitude for the city
     */
    qreal longitude() const;

    /**
     * \brief returns the timezone string for the city
     *
     * Example:
     * returns something like “Europe/Helsinki”
     */
    QString timeZone() const;

    /*!
     * \brief returns the raw offset of the timezone
     *
     * returns the raw offset of the time zone in milliseconds.
     * The raw offset is the offset without considering daylight savings time.
     *
     * \sa timeZoneTotalOffset(QDateTime dateTime = QDateTime::currentDateTime()) const
     * \sa timeZoneDstOffset(QDateTime dateTime = QDateTime::currentDateTime()) const
     */
     qint32 timeZoneRawOffset() const;

    /*!
     * \brief returns the daylight savings time offset of the timezone
     *
     * \param dateTime the date and time to calculate the offset for
     *
     * returns the daylight savings time offset for the date and time given
     * in milliseconds.
     *
     * \sa timeZoneRawOffset() const
     * \sa timeZoneTotalOffset(QDateTime dateTime = QDateTime::currentDateTime()) const
     */
     qint32 timeZoneDstOffset(QDateTime dateTime = QDateTime::currentDateTime()) const;

    /*!
     * \brief returns the total offset of the timezone
     *
     * \param dateTime the date and time to calculate the offset for
     *
     * returns the total offset, i.e. the raw offset plus the
     * daylight savings time offset for the date and time given
     * in milliseconds.
     *
     * \sa timeZoneRawOffset() const
     * \sa timeZoneDstOffset(QDateTime dateTime = QDateTime::currentDateTime()) const
     */
     qint32 timeZoneTotalOffset(QDateTime dateTime = QDateTime::currentDateTime()) const;

    /**
     * \brief returns the country of the city
     */
    MCountry country() const;

    // setters
    void setKey( const QString& val );
    void setEnglishName( const QString& val );
    void setLocalName( const QString& val );
    void setLatitude( qreal val );
    void setLongitude( qreal val );
    void setTimeZone( const QString& val );
    void setCountry( const MCountry& val );

  private:
    MCityPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MCity)
};

#endif
