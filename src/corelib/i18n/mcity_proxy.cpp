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

#include "mcity.h"
#include "mlocale/mcity.h"
#include "mcity_proxy_p.h"
#include "mcountry_proxy_p.h"

MCityPrivate::MCityPrivate() : pCity( 0 )
{
}

MCityPrivate::~MCityPrivate()
{
    delete pCity;
}


MCity::MCity()
    : d_ptr( new MCityPrivate )
{
    d_ptr->pCity = new ML10N::MCity;
}


MCity::MCity( const MCity& other )
    : d_ptr( new MCityPrivate )
{
    d_ptr->pCity = new ML10N::MCity;
    *d_ptr->pCity = *other.d_ptr->pCity;
}


MCity::~MCity()
{
    delete d_ptr;
}


MCity& MCity::operator=( const MCity& other )
{
    if (this == &other) {
        return *this;
    }

    *d_ptr->pCity = *other.d_ptr->pCity;

    return *this;
}


QString MCity::key() const
{
    return d_ptr->pCity->key();
}


QString MCity::englishName() const
{
    return d_ptr->pCity->englishName();
}


QString MCity::localName() const
{
    return d_ptr->pCity->localName();
}


qreal MCity::latitude() const
{
    return d_ptr->pCity->latitude();
}


qreal MCity::longitude() const
{
    return d_ptr->pCity->longitude();
}


QString MCity::timeZone() const
{
    return d_ptr->pCity->timeZone();
}

#ifdef HAVE_ICU
qint32 MCity::timeZoneRawOffset() const
{
    return d_ptr->pCity->timeZoneRawOffset();
}


qint32 MCity::timeZoneDstOffset(QDateTime dateTime) const
{
    return d_ptr->pCity->timeZoneDstOffset( dateTime );
}


qint32 MCity::timeZoneTotalOffset(QDateTime dateTime) const
{
    return d_ptr->pCity->timeZoneTotalOffset( dateTime );
}
#endif

MCountry MCity::country() const
{
    MCountry country;
    *country.d_ptr->pCountry = d_ptr->pCity->country();
    return country;
}


void MCity::setKey( const QString& val )
{
    d_ptr->pCity->setKey( val );
}


void MCity::setEnglishName( const QString& val )
{
    d_ptr->pCity->setEnglishName( val );
}


void MCity::setLocalName( const QString& val )
{
    d_ptr->pCity->setLocalName( val );
}


void MCity::setLatitude( qreal val )
{
    d_ptr->pCity->setLatitude( val );
}


void MCity::setLongitude( qreal val )
{
    d_ptr->pCity->setLongitude( val );
}


void MCity::setTimeZone( const QString& val )
{
    d_ptr->pCity->setTimeZone( val );
}


void MCity::setCountry( const MCountry& val )
{
    d_ptr->pCity->setCountry( *val.d_ptr->pCountry );
}
