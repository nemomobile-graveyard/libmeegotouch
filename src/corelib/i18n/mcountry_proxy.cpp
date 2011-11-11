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

#include "mcountry.h"
#include "mlocale/mcountry.h"
#include "mcountry_proxy_p.h"
#include "mlocale.h"

static bool g_mLocaleInited = false;

MCountryPrivate::MCountryPrivate() : pCountry( 0 )
{
    if ( ! g_mLocaleInited )
    {
        MLocale::initMLocale();
        g_mLocaleInited = true;
    }
}

MCountryPrivate::~MCountryPrivate()
{
    delete pCountry;
}


MCountry::MCountry()
    : d_ptr( new MCountryPrivate )
{
    d_ptr->pCountry = new ML10N::MCountry;
}


MCountry::MCountry( const MCountry& other )
    : d_ptr( new MCountryPrivate )
{
    d_ptr->pCountry = new ML10N::MCountry;
    *d_ptr->pCountry = *other.d_ptr->pCountry;
}


MCountry::~MCountry()
{
    delete d_ptr;
}


MCountry& MCountry::operator=( const MCountry& other )
{
    if (this == &other) {
        return *this;
    }

    *d_ptr->pCountry = *other.d_ptr->pCountry;
    return *this;
}


QString MCountry::key() const
{
    return d_ptr->pCountry->key();
}


QString MCountry::englishName() const
{
    return d_ptr->pCountry->englishName();
}


QString MCountry::localName() const
{
    return d_ptr->pCountry->localName();
}


void MCountry::setKey( const QString& val )
{
    d_ptr->pCountry->setKey( val );
}


void MCountry::setEnglishName( const QString& val )
{
    d_ptr->pCountry->setEnglishName( val );
}


void MCountry::setLocalName( const QString& val )
{
    d_ptr->pCountry->setLocalName( val );
}
