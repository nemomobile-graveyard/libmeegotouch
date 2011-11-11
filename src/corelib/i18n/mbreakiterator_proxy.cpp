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

#include "mbreakiterator.h"

#include "mlocale/mbreakiterator.h"

#include "mlocale_proxy_p.h"

static bool g_mLocaleInited = false;

class MBreakIteratorPrivate
{
public:
    MBreakIteratorPrivate() : pIterator( 0 )
        {
            if ( ! g_mLocaleInited )
            {
                MLocale::initMLocale();
                g_mLocaleInited = true;
            }
        }
    ~MBreakIteratorPrivate()
        {
            delete pIterator;
        }

    ML10N::MBreakIterator *pIterator;
};

MBreakIterator::MBreakIterator(const MLocale &locale, const QString &text, Type type)
    : d_ptr(new MBreakIteratorPrivate)
{
    d_ptr->pIterator = new ML10N::MBreakIterator(
            *locale.d_ptr->pLocale, text, (ML10N::MBreakIterator::Type)type );
}


MBreakIterator::MBreakIterator(const QString &text, Type type)
    : d_ptr(new MBreakIteratorPrivate)
{
    d_ptr->pIterator = new ML10N::MBreakIterator(
            text, (ML10N::MBreakIterator::Type)type );
}


MBreakIterator::~MBreakIterator()
{
    delete d_ptr;
}


bool MBreakIterator::hasNext() const
{
    return d_ptr->pIterator->hasNext();
}


bool MBreakIterator::hasPrevious() const
{
    return d_ptr->pIterator->hasPrevious();
}


int MBreakIterator::next()
{
    return d_ptr->pIterator->next();
}


int MBreakIterator::next(int index)
{
    return d_ptr->pIterator->next(index);
}


int MBreakIterator::peekNext()
{
    return d_ptr->pIterator->peekNext();
}


int MBreakIterator::peekPrevious()
{
    return d_ptr->pIterator->peekPrevious();
}


int MBreakIterator::previous()
{
    return d_ptr->pIterator->previous();
}


int MBreakIterator::previous(int index)
{
    return d_ptr->pIterator->previous(index);
}


int MBreakIterator::previousInclusive()
{
    return d_ptr->pIterator->previousInclusive();
}


int MBreakIterator::previousInclusive(int index)
{
    return d_ptr->pIterator->previousInclusive(index);
}


int MBreakIterator::index() const
{
    return d_ptr->pIterator->index();
}


void MBreakIterator::setIndex(int index)
{
    d_ptr->pIterator->setIndex(index);
}


void MBreakIterator::toBack()
{
    d_ptr->pIterator->toBack();
}


void MBreakIterator::toFront()
{
    d_ptr->pIterator->toFront();
}


bool MBreakIterator::isBoundary()
{
    return d_ptr->pIterator->isBoundary();
}


bool MBreakIterator::isBoundary(int index)
{
    return d_ptr->pIterator->isBoundary(index);
}
