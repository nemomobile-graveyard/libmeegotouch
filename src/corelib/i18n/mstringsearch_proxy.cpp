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

#include "mstringsearch.h"
#include "mlocale/mstringsearch.h"

#include "mlocale_proxy_p.h"

class MStringSearchPrivate
{
public:
    MStringSearchPrivate() : pStringSearch( 0 )
        {
        }
    ~MStringSearchPrivate()
        {
            delete pStringSearch;
        }

    ML10N::MStringSearch *pStringSearch;
};



MStringSearch::MStringSearch( QString &pattern, QString &text, const MLocale &locale, MBreakIterator::Type breakIteratorType )
    : d_ptr ( new MStringSearchPrivate )
{
    d_ptr->pStringSearch = new ML10N::MStringSearch(
             pattern, text, *locale.d_ptr->pLocale,
             (ML10N::MBreakIterator::Type)breakIteratorType );
}

MStringSearch::~MStringSearch()
{
    delete d_ptr;
}

bool MStringSearch::hasError() const
{
    return d_ptr->pStringSearch->hasError();
}

void MStringSearch::clearError()
{
    d_ptr->pStringSearch->clearError();
}

QString MStringSearch::errorString() const
{
    return d_ptr->pStringSearch->errorString();
}

void MStringSearch::setLocale(const MLocale &locale)
{
    d_ptr->pStringSearch->setLocale( *locale.d_ptr->pLocale );
}

void MStringSearch::setText(const QString &text)
{
    d_ptr->pStringSearch->setText( text );
}

QString MStringSearch::text() const
{
    return d_ptr->pStringSearch->text();
}

void MStringSearch::setPattern(const QString &pattern)
{
    d_ptr->pStringSearch->setPattern( pattern );
}

QString MStringSearch::pattern() const
{
    return d_ptr->pStringSearch->pattern();
}

void MStringSearch::setCollatorStrength(MLocale::CollatorStrength collatorStrength)
{
    d_ptr->pStringSearch->setCollatorStrength( (ML10N::MLocale::CollatorStrength)collatorStrength );
}

MLocale::CollatorStrength MStringSearch::collatorStrength() const
{
    return (MLocale::CollatorStrength)d_ptr->pStringSearch->collatorStrength();
}

void MStringSearch::setAlternateHandlingShifted(bool isShifted)
{
    d_ptr->pStringSearch->setAlternateHandlingShifted( isShifted );
}

bool MStringSearch::alternateHandlingShifted() const
{
    return d_ptr->pStringSearch->alternateHandlingShifted();
}

int MStringSearch::first()
{
    return d_ptr->pStringSearch->first();
}

int MStringSearch::last()
{
    return d_ptr->pStringSearch->last();
}

bool MStringSearch::hasNext()
{
    return d_ptr->pStringSearch->hasNext();
}

int MStringSearch::next()
{
    return d_ptr->pStringSearch->next();
}

int MStringSearch::next(int index)
{
    return d_ptr->pStringSearch->next( index );
}

int MStringSearch::peekNext()
{
    return d_ptr->pStringSearch->peekNext();
}

bool MStringSearch::hasPrevious()
{
    return d_ptr->pStringSearch->hasPrevious();
}

int MStringSearch::previous()
{
    return d_ptr->pStringSearch->previous();
}

int MStringSearch::previous(int index)
{
    return d_ptr->pStringSearch->previous( index );
}

int MStringSearch::peekPrevious()
{
    return d_ptr->pStringSearch->peekPrevious();
}

void MStringSearch::toBack()
{
    d_ptr->pStringSearch->toBack();
}

void MStringSearch::toFront()
{
    d_ptr->pStringSearch->toFront();
}

int MStringSearch::index() const
{
    return d_ptr->pStringSearch->index();
}

void MStringSearch::setIndex(int index)
{
    d_ptr->pStringSearch->setIndex( index );
}

int MStringSearch::matchedStart()
{
    return d_ptr->pStringSearch->matchedStart();
}

int MStringSearch::matchedLength()
{
    return d_ptr->pStringSearch->matchedLength();
}

QString MStringSearch::matchedText()
{
    return d_ptr->pStringSearch->matchedText();
}
