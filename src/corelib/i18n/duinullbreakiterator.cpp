/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duinullbreakiterator.h"

/*! \internal
 *  internally used by DuiBreakIterator
 */
class DuiNullBreakIteratorPrivate
{
public:
    DuiNullBreakIteratorPrivate();
    virtual ~DuiNullBreakIteratorPrivate();

    int current;
    QString string;
};


DuiNullBreakIteratorPrivate::DuiNullBreakIteratorPrivate()
    : current(-1)
{
    // nothing
}


DuiNullBreakIteratorPrivate::~DuiNullBreakIteratorPrivate()
{
}


DuiNullBreakIterator::DuiNullBreakIterator(const DuiLocale &locale,
        const QString &text,
        DuiBreakIterator::Type type)
    : d_ptr(new DuiNullBreakIteratorPrivate)
{
    Q_UNUSED(locale);
    Q_UNUSED(type);
    Q_D(DuiNullBreakIterator);
    d->string = text;
}


DuiNullBreakIterator::DuiNullBreakIterator(const QString &text,
        DuiBreakIterator::Type type)
    : d_ptr(new DuiNullBreakIteratorPrivate)
{
    Q_UNUSED(type);
    Q_D(DuiNullBreakIterator);
    d->string = text;
}


DuiNullBreakIterator::~DuiNullBreakIterator()
{
    delete d_ptr;
}


bool DuiNullBreakIterator::hasNext() const
{
    Q_D(const DuiNullBreakIterator);

    int next = d->current + 1;

    if (next >= d->string.length()) {
        return false;
    }

    return true;
}


bool DuiNullBreakIterator::hasPrevious() const
{
    Q_D(const DuiNullBreakIterator);

    int prev = d->current - 1;

    if (prev < 0) {
        return false;
    }

    return true;
}


int DuiNullBreakIterator::next()
{
    Q_D(DuiNullBreakIterator);

    int next = d->current + 1;

    if (next >= d->string.length()) {
        next = -1;
        toBack();
    } else {
        d->current = next;
    }

    return next;
}


int DuiNullBreakIterator::next(int index)
{
    Q_D(DuiNullBreakIterator);

    int next = index + 1;

    if (next >= d->string.length()) {
        next = -1;
        toBack();
    } else {
        d->current = next;
    }

    return next;
}


int DuiNullBreakIterator::peekNext()
{
    Q_D(DuiNullBreakIterator);

    int next = d->current + 1;

    if (next >= d->string.length()) {
        next = -1;
    }

    return next;
}


int DuiNullBreakIterator::peekPrevious()
{
    Q_D(DuiNullBreakIterator);

    int previous = d->current - 1;

    if (previous < 0) {
        previous = -1;
    }

    return previous;
}


int DuiNullBreakIterator::previous()
{
    Q_D(DuiNullBreakIterator);

    int previous = d->current - 1;

    if (previous < 0) {
        previous = -1;
        toFront();
    }

    d->current = previous;

    return previous;
}


int DuiNullBreakIterator::previous(int index)
{
    Q_D(DuiNullBreakIterator);

    int previous = index - 1;

    if (previous < 0) {
        previous = -1;
        toFront();
    }

    d->current = previous;

    return previous;
}


int DuiNullBreakIterator::previousInclusive()
{
    return previous();
}


int DuiNullBreakIterator::previousInclusive(int index)
{
    return previous(index);
}


void DuiNullBreakIterator::setIndex(int index)
{
    Q_D(DuiNullBreakIterator);
    d->current = index;
}


void DuiNullBreakIterator::toBack()
{
    Q_D(DuiNullBreakIterator);
    d->current = d->string.length();
}


void DuiNullBreakIterator::toFront()
{
    Q_D(DuiNullBreakIterator);
    d->current = -1;
}


bool DuiNullBreakIterator::isBoundary()
{
    Q_D(DuiNullBreakIterator);
    return isBoundary(d->current);
}


bool DuiNullBreakIterator::isBoundary(int index)
{
    Q_UNUSED(index);
    return true;
}
/*! \internal_end */
