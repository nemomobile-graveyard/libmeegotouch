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

#include "mcountry.h"

class MCountryPrivate
{
public:
    QString key;
    QString englishName;
    QString localName;
};


MCountry::MCountry()
    : d_ptr( new MCountryPrivate )
{
}


MCountry::~MCountry()
{
    delete d_ptr;
}


MCountry::MCountry( const MCountry& other )
    : d_ptr( new MCountryPrivate )
{
    *d_ptr = *other.d_ptr;
}


MCountry& MCountry::operator=( const MCountry& other )
{
    *d_ptr = *other.d_ptr;

    return *this;
}


QString MCountry::key() const
{
    Q_D( const MCountry );
    return d->key;
}


QString MCountry::englishName() const
{
    Q_D( const MCountry );
    return d->englishName;
}


QString MCountry::localName() const
{
    Q_D( const MCountry );
    return d->localName;
}


void MCountry::setKey( const QString& val )
{
    Q_D( MCountry );
    d->key = val;
}


void MCountry::setEnglishName( const QString& val )
{
    Q_D( MCountry );
    d->englishName = val;
}


void MCountry::setLocalName( const QString& val )
{
    Q_D( MCountry );
    d->localName = val;
}
