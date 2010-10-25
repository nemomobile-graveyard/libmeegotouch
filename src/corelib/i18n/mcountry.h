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

class M_CORE_EXPORT MCountry
{
  public:
    MCountry();
    MCountry(const MCountry&);
    virtual ~MCountry();
    MCountry& operator=(const MCountry&);


    /**
     * \brief returns the unique key for the country
     *
     * Example:
     * might return something like “qtn_clk_country_finland”
     * for Finland. This key can be used in qtTrId() to get
     * the translation of “Finland”
     *
     * \code
     * qtTrId("qtn_clk_country_finland");
     * \endcode
     */
    QString key() const;

    /**
     * \brief returns the english name for the country
     *
     * Example:
     * might return something like “Finland”.
     */
    QString englishName() const;

    /**
     * \brief returns the local name for the country
     *
     * Example
     * might return something like “Suomi” in case of Finland
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
