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

#include "duiicubreakiterator.h"
#include <DuiDebug>

#ifdef HAVE_ICU
#include <unicode/brkiter.h>
#include <unicode/uchriter.h>
#include "duilocale_p.h"
#endif

/*
 *  internally used by DuiBreakIterator
 */
class DuiIcuBreakIteratorPrivate
{
public:
    DuiIcuBreakIteratorPrivate();
    virtual ~DuiIcuBreakIteratorPrivate();

    void init(const DuiLocale &locale, const QString &text, DuiBreakIterator::Type type);

    int current;

    icu::BreakIterator *icuIterator;
};

DuiIcuBreakIteratorPrivate::DuiIcuBreakIteratorPrivate()
    : current(-1), icuIterator(0)
{
    // nothing
}

DuiIcuBreakIteratorPrivate::~DuiIcuBreakIteratorPrivate()
{
    delete icuIterator;
}

void DuiIcuBreakIteratorPrivate::init(const DuiLocale &locale, const QString &text,
                                      DuiBreakIterator::Type type)
{
    UErrorCode status = U_ZERO_ERROR;

    icu::Locale msgLocale
    = locale.d_ptr->getCategoryLocale(DuiLocale::DuiLcMessages);

    switch (type) {
    case DuiBreakIterator::LineIterator:
        icuIterator = icu::BreakIterator::createLineInstance(msgLocale, status);
        break;

    case DuiBreakIterator::WordIterator:
        icuIterator = icu::BreakIterator::createWordInstance(msgLocale, status);
        break;
    }

    if (U_FAILURE(status)) {
        duiWarning("DuiIcuBreakIteratorPrivate") << "failed creating iterator:" << u_errorName(status);
        return;
    }

    UCharCharacterIterator *it = new UCharCharacterIterator(text.utf16(), text.length());
    icuIterator->adoptText(it);
}

DuiIcuBreakIterator::DuiIcuBreakIterator(const DuiLocale &locale,
        const QString &text,
        DuiBreakIterator::Type type)
    : d_ptr(new DuiIcuBreakIteratorPrivate)
{
    Q_D(DuiIcuBreakIterator);
    d->init(locale, text, type);
}

///! Creates a DuiIcuBreakIterator using default locale
DuiIcuBreakIterator::DuiIcuBreakIterator(const QString &text,
        DuiBreakIterator::Type type)
    : d_ptr(new DuiIcuBreakIteratorPrivate)
{
    Q_D(DuiIcuBreakIterator);
    DuiLocale locale; // get default
    d->init(locale, text, type);
}

//! Destructor
DuiIcuBreakIterator::~DuiIcuBreakIterator()
{
    delete d_ptr;
}

//! returns true if a boundary exists after current index
bool DuiIcuBreakIterator::hasNext() const
{
    Q_D(const DuiIcuBreakIterator);

    int next = d->icuIterator->following(d->current);

    if (next != BreakIterator::DONE) {
        return true;
    } else {
        return false;
    }
}

//! returns true if boundary exists before current index
bool DuiIcuBreakIterator::hasPrevious() const
{
    Q_D(const DuiIcuBreakIterator);

    int previous = d->icuIterator->preceding(d->current);

    if (previous != BreakIterator::DONE) {
        return true;
    } else {
        return false;
    }
}

//! returns the next boundary index after the current index or -1 if none exists.
//! current index is updated to the resulting value.
int DuiIcuBreakIterator::next()
{
    Q_D(DuiIcuBreakIterator);

    int next = d->icuIterator->following(d->current);

    if (next == BreakIterator::DONE) {
        next = -1;
        toBack();
    } else {
        d->current = next;
    }

    return next;
}

//! returns the next boundary after the given index. Returns the boundary or -1
//! if none exists. Updates the current index to the resulting value.
int DuiIcuBreakIterator::next(int index)
{
    Q_D(DuiIcuBreakIterator);

    int result = d->icuIterator->following(index);

    if (result == BreakIterator::DONE) {
        result = -1;
        toBack();
    }

    return result;
}

//! returns the next boundary index value. The current index is not updated.
int DuiIcuBreakIterator::peekNext()
{
    Q_D(DuiIcuBreakIterator);

    int next = d->icuIterator->following(d->current);

    if (next == BreakIterator::DONE) {
        next = -1;
    }

    return next;
}

//! returns the previous boundary index value. The current index is not updated.
int DuiIcuBreakIterator::peekPrevious()
{
    Q_D(DuiIcuBreakIterator);

    int previous = d->icuIterator->preceding(d->current);

    if (previous == BreakIterator::DONE) {
        previous = -1;
    }

    return previous;
}

//! returns the previous boundary index or -1 if none exists.
//! The current index is updated to the resulting value.
int DuiIcuBreakIterator::previous()
{
    Q_D(DuiIcuBreakIterator);

    int result = d->icuIterator->preceding(d->current);

    if (result == BreakIterator::DONE) {
        result = -1;
        toFront();
    }

    d->current = result;

    return result;
}

//! returns the previous boundary from the given index value or -1 if none exists.
//! The current index is updated to the resulting value.
int DuiIcuBreakIterator::previous(int index)
{
    Q_D(DuiIcuBreakIterator);

    int result = d->icuIterator->preceding(index);

    if (result == BreakIterator::DONE) {
        result = -1;
        toFront();
    }

    d->current = result;
    return result;
}

//! returns the previous boundary including the current index in the search.
//! If current index is a boundary, it is returned and current is decreased by one,
//! otherwise works as previous()
int DuiIcuBreakIterator::previousInclusive()
{
    Q_D(DuiIcuBreakIterator);
    if (isBoundary()) {
        int result = d->current;
        --d->current;
        return result;

    } else {
        return previous();
    }
}

//! returns the previous boundary from given index, including the index in the search.
//! If index is a boundary, it is returned and current is decreased by one,
//! otherwise works as previous()
int DuiIcuBreakIterator::previousInclusive(int index)
{
    Q_D(DuiIcuBreakIterator);
    if (isBoundary(index)) {
        d->current = index - 1;
        return index;
    } else {
        return previous(index);
    }
}

//! Sets the current index to the given value
void DuiIcuBreakIterator::setIndex(int index)
{
    Q_D(DuiIcuBreakIterator);
    d->current = index;
}

//! Sets the current index to the end of the string
void DuiIcuBreakIterator::toBack()
{
    Q_D(DuiIcuBreakIterator);
    // API reference says last() gives index _beyond_ the last character. not so.
    d->current = d->icuIterator->last() + 1;
}

//! Sets the current index to the beginning of the string
void DuiIcuBreakIterator::toFront()
{
    Q_D(DuiIcuBreakIterator);
    d->current = -1;
}

//! Checks if current index is a boundary.
bool DuiIcuBreakIterator::isBoundary()
{
    Q_D(DuiIcuBreakIterator);
    return isBoundary(d->current);
}

//! Checks if given index is a boundary.
bool DuiIcuBreakIterator::isBoundary(int index)
{
    Q_D(DuiIcuBreakIterator);
    return d->icuIterator->isBoundary(index);
}
