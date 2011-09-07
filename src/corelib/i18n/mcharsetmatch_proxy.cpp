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

#include "mcharsetmatch.h"
#include "mlocale/mcharsetmatch.h"
#include "mcharsetmatch_proxy_p.h"

MCharsetMatchPrivate::MCharsetMatchPrivate() : pMatch( 0 )
{
}

MCharsetMatchPrivate::~MCharsetMatchPrivate()
{
    delete pMatch;
}

MCharsetMatch::MCharsetMatch()
    : d_ptr(new MCharsetMatchPrivate)
{
    d_ptr->pMatch = new ML10N::MCharsetMatch();
}

MCharsetMatch::MCharsetMatch( const QString name,
                              const QString language,
                              const qint32 confidence )
    : d_ptr(new MCharsetMatchPrivate)
{
    d_ptr->pMatch = new ML10N::MCharsetMatch( name, language, confidence );
}

MCharsetMatch::MCharsetMatch(const MCharsetMatch &other)
    : d_ptr(new MCharsetMatchPrivate)
{
    d_ptr->pMatch = new ML10N::MCharsetMatch( *other.d_ptr->pMatch );
}

MCharsetMatch::~MCharsetMatch()
{
    delete d_ptr;
}

MCharsetMatch &MCharsetMatch::operator=(const MCharsetMatch &other)
{
    if (this == &other) {
        return *this;
    }
    *d_ptr->pMatch = *other.d_ptr->pMatch;
    return *this;
}

bool MCharsetMatch::operator<(const MCharsetMatch &other) const
{
    return d_ptr->pMatch->operator<( *other.d_ptr->pMatch );
}

bool MCharsetMatch::operator>(const MCharsetMatch &other) const
{
    return d_ptr->pMatch->operator>( *other.d_ptr->pMatch );
}

QString MCharsetMatch::name() const
{
    return d_ptr->pMatch->name();
}

void MCharsetMatch::setName(QString name)
{
    d_ptr->pMatch->setName( name );
}

QString MCharsetMatch::language() const
{
    return d_ptr->pMatch->language();
}

void MCharsetMatch::setLanguage(QString language)
{
    d_ptr->pMatch->setLanguage( language );
}

qint32 MCharsetMatch::confidence() const
{
    return d_ptr->pMatch->confidence();
}

void MCharsetMatch::setConfidence(qint32 confidence)
{
    d_ptr->pMatch->setConfidence( confidence );
}
