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

#ifndef MCOUNTRY_H
#define MCOUNTRY_H

#include <QString>

#include "mexport.h"

class MCountryPrivate;

class M_EXPORT MCountry
{
  public:
    MCountry();
    MCountry(const MCountry&);
    virtual ~MCountry();
    MCountry& operator=(const MCountry&);


    /**
     * \brief returns the unique key for the country
     */
    QString key() const;

    /**
     * \brief returns the english name for the country
     */
    QString englishName() const;

    /**
     * \brief returns the local name for the country
     */
    QString localName() const;

    // the setters
    void setKey( const QString& val );
    void setEnglishName( const QString& val );
    void setLocalName( const QString& val );

  private:
    MCountryPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MCountry)
};

#endif
