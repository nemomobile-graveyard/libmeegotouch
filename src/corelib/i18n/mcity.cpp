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

#include "mcity.h"


class MCityPrivate
{
public:
    MCityPrivate()
        : latitude( 0.0 ),
          longitude( 0.0 )
        {}

    QString key;
    QString englishName;
    QString localName;
    qreal latitude;
    qreal longitude;
    QString timeZone;
    MCountry country;
};


MCity::MCity()
    : d_ptr( new MCityPrivate )
{
}


MCity::~MCity()
{
    delete d_ptr;
}


MCity::MCity( const MCity& other )
    : d_ptr( new MCityPrivate )
{
    *d_ptr = *other.d_ptr;
}


MCity& MCity::operator=( const MCity& other )
{
    *d_ptr = *other.d_ptr;

    return *this;
}


QString MCity::key() const
{
    Q_D( const MCity );
    return d->key;
}


QString MCity::englishName() const
{
    Q_D( const MCity );
    return d->englishName;
}


QString MCity::localName() const
{
    Q_D( const MCity );
    return d->localName;
}


qreal MCity::latitude() const
{
    Q_D( const MCity );
    return d->latitude;
}


qreal MCity::longitude() const
{
    Q_D( const MCity );
    return d->longitude;
}


QString MCity::timeZone() const
{
    Q_D( const MCity );
    return d->timeZone;
}


MCountry MCity::country() const
{
    Q_D( const MCity );
    return d->country;
}


void MCity::setKey( const QString& val )
{
    Q_D( MCity );
    d->key = val;
}


void MCity::setEnglishName( const QString& val )
{
    Q_D( MCity );
    d->englishName = val;
}


void MCity::setLocalName( const QString& val )
{
    Q_D( MCity );
    d->localName = val;
}


void MCity::setLatitude( qreal val )
{
    Q_D( MCity );
    d->latitude = val;
}


void MCity::setLongitude( qreal val )
{
    Q_D( MCity );
    d->longitude = val;
}


void MCity::setTimeZone( const QString& val )
{
    Q_D( MCity );
    d->timeZone = val;
}


void MCity::setCountry( const MCountry& val )
{
    Q_D( MCity );
    d->country = val;
}
