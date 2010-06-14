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

#include "mexport.h"
#include "mcountry.h"

class MCityPrivate;

class M_EXPORT MCity
{
  public:
    MCity();
    MCity(const MCity&);
    virtual ~MCity();
    MCity& operator=(const MCity&);

    /**
     * \brief returns the unique key for the city
     */
    QString key() const;

    /**
     * \brief returns the english name for the city
     */
    QString englishName() const;

    /**
     * \brief returns the local name for the city
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
     */
    QString timeZone() const;

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
