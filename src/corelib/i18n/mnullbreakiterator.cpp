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

#include "mnullbreakiterator.h"

/*! \internal
 *  internally used by MBreakIterator
 */
class MNullBreakIteratorPrivate
{
public:
    MNullBreakIteratorPrivate();
    virtual ~MNullBreakIteratorPrivate();

    int current;
    QString string;
};

MNullBreakIteratorPrivate::MNullBreakIteratorPrivate()
    : current(-1)
{
    // nothing
}

MNullBreakIteratorPrivate::~MNullBreakIteratorPrivate()
{
}

MNullBreakIterator::MNullBreakIterator(const MLocale &locale,
        const QString &text,
        MBreakIterator::Type type)
    : d_ptr(new MNullBreakIteratorPrivate)
{
    Q_UNUSED(locale);
    Q_UNUSED(type);
    Q_D(MNullBreakIterator);
    d->string = text;
}

MNullBreakIterator::MNullBreakIterator(const QString &text,
        MBreakIterator::Type type)
    : d_ptr(new MNullBreakIteratorPrivate)
{
    Q_UNUSED(type);
    Q_D(MNullBreakIterator);
    d->string = text;
}

MNullBreakIterator::~MNullBreakIterator()
{
    delete d_ptr;
}

bool MNullBreakIterator::hasNext() const
{
    Q_D(const MNullBreakIterator);
    int next = d->current + 1;
    if (next >= d->string.length()) {
        return false;
    }
    return true;
}

bool MNullBreakIterator::hasPrevious() const
{
    Q_D(const MNullBreakIterator);
    int prev = d->current - 1;
    if (prev < 0) {
        return false;
    }
    return true;
}

int MNullBreakIterator::next()
{
    Q_D(MNullBreakIterator);
    int next = d->current + 1;
    if (next >= d->string.length()) {
        next = -1;
        toBack();
    } else {
        d->current = next;
    }
    return next;
}

int MNullBreakIterator::next(int index)
{
    Q_D(MNullBreakIterator);
    int next = index + 1;
    if (next >= d->string.length()) {
        next = -1;
        toBack();
    } else {
        d->current = next;
    }
    return next;
}

int MNullBreakIterator::peekNext()
{
    Q_D(MNullBreakIterator);
    int next = d->current + 1;
    if (next >= d->string.length()) {
        next = -1;
    }
    return next;
}

int MNullBreakIterator::peekPrevious()
{
    Q_D(MNullBreakIterator);
    int previous = d->current - 1;
    if (previous < 0) {
        previous = -1;
    }
    return previous;
}

int MNullBreakIterator::previous()
{
    Q_D(MNullBreakIterator);
    int previous = d->current - 1;
    if (previous < 0) {
        previous = -1;
        toFront();
    }
    d->current = previous;
    return previous;
}

int MNullBreakIterator::previous(int index)
{
    Q_D(MNullBreakIterator);
    int previous = index - 1;
    if (previous < 0) {
        previous = -1;
        toFront();
    }
    d->current = previous;
    return previous;
}

int MNullBreakIterator::previousInclusive()
{
    return previous();
}

int MNullBreakIterator::previousInclusive(int index)
{
    return previous(index);
}

int MNullBreakIterator::index() const
{
    Q_D(const MNullBreakIterator);
    return d->current;
}

void MNullBreakIterator::setIndex(int index)
{
    Q_D(MNullBreakIterator);
    d->current = index;
}

void MNullBreakIterator::toBack()
{
    Q_D(MNullBreakIterator);
    d->current = d->string.length();
}

void MNullBreakIterator::toFront()
{
    Q_D(MNullBreakIterator);
    d->current = -1;
}

bool MNullBreakIterator::isBoundary()
{
    Q_D(MNullBreakIterator);
    return isBoundary(d->current);
}

bool MNullBreakIterator::isBoundary(int index)
{
    Q_UNUSED(index);
    return true;
}
/*! \internal_end */
