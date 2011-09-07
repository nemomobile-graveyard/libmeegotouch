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

#include "mcollator.h"
#include "mlocale/mcollator.h"
#include "mlocale_proxy_p.h"
#include "mcollator_proxy_p.h"

MCollatorPrivate::MCollatorPrivate() : pCollator( 0 )
{
}

MCollatorPrivate::~MCollatorPrivate()
{
    delete pCollator;
}


MCollator::MCollator()
    : d_ptr(new MCollatorPrivate)
{
    d_ptr->pCollator = new ML10N::MCollator;
}

MCollator::MCollator(const MLocale &locale)
    : d_ptr(new MCollatorPrivate)
{
    d_ptr->pCollator = new ML10N::MCollator( *locale.d_ptr->pLocale );
}

MCollator::MCollator(const MCollator &other)
    : d_ptr(new MCollatorPrivate)
{
    d_ptr->pCollator = new ML10N::MCollator( *other.d_ptr->pCollator );
}

MCollator &MCollator::operator =(const MCollator &other)
{
    *d_ptr->pCollator = *other.d_ptr->pCollator;
    return *this;
}

MCollator::~MCollator()
{
    delete d_ptr;
}

MLocale::CollatorStrength MCollator::strength() const
{
    return (MLocale::CollatorStrength)d_ptr->pCollator->strength();
}

void MCollator::setStrength(MLocale::CollatorStrength collatorStrength)
{
    d_ptr->pCollator->setStrength( (ML10N::MLocale::CollatorStrength)collatorStrength );
}

bool MCollator::operator()(const QString &s1, const QString &s2) const
{
    return d_ptr->pCollator->operator()( s1, s2 );
}

MLocale::Comparison MCollator::compare(const QString &first, const QString &second)
{
    return (MLocale::Comparison)ML10N::MCollator::compare( first, second );
}

MLocale::Comparison MCollator::compare(MLocale &locale, const QString &first, const QString &second)
{
    return (MLocale::Comparison)ML10N::MCollator::compare( *locale.d_ptr->pLocale, first, second );
}
