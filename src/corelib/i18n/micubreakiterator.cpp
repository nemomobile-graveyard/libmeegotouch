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

#include "micubreakiterator.h"
#include <MDebug>

#ifdef HAVE_ICU
#include <unicode/brkiter.h>
#include <unicode/uchriter.h>
#include "mlocale_p.h"
#endif

/*
 *  internally used by MBreakIterator
 */
class MIcuBreakIteratorPrivate
{
public:
    MIcuBreakIteratorPrivate();
    virtual ~MIcuBreakIteratorPrivate();

    void init(const MLocale &locale, const QString &text, MBreakIterator::Type type);

    int current;

    icu::BreakIterator *icuIterator;
};

MIcuBreakIteratorPrivate::MIcuBreakIteratorPrivate()
    : current(-1), icuIterator(0)
{
    // nothing
}

MIcuBreakIteratorPrivate::~MIcuBreakIteratorPrivate()
{
    delete icuIterator;
}

void MIcuBreakIteratorPrivate::init(const MLocale &locale, const QString &text,
                                      MBreakIterator::Type type)
{
    UErrorCode status = U_ZERO_ERROR;

    icu::Locale msgLocale
    = locale.d_ptr->getCategoryLocale(MLocale::MLcMessages);

    switch (type) {
    case MBreakIterator::LineIterator:
        icuIterator = icu::BreakIterator::createLineInstance(msgLocale, status);
        break;

    case MBreakIterator::WordIterator:
        icuIterator = icu::BreakIterator::createWordInstance(msgLocale, status);
        break;
    }

    if (U_FAILURE(status)) {
        mWarning("MIcuBreakIteratorPrivate") << "failed creating iterator:" << u_errorName(status);
        return;
    }

    UCharCharacterIterator *it = new UCharCharacterIterator(text.utf16(), text.length());
    icuIterator->adoptText(it);
}

MIcuBreakIterator::MIcuBreakIterator(const MLocale &locale,
        const QString &text,
        MBreakIterator::Type type)
    : d_ptr(new MIcuBreakIteratorPrivate)
{
    Q_D(MIcuBreakIterator);
    d->init(locale, text, type);
}

///! Creates a MIcuBreakIterator using default locale
MIcuBreakIterator::MIcuBreakIterator(const QString &text,
        MBreakIterator::Type type)
    : d_ptr(new MIcuBreakIteratorPrivate)
{
    Q_D(MIcuBreakIterator);
    MLocale locale; // get default
    d->init(locale, text, type);
}

//! Destructor
MIcuBreakIterator::~MIcuBreakIterator()
{
    delete d_ptr;
}

//! returns true if a boundary exists after current index
bool MIcuBreakIterator::hasNext() const
{
    Q_D(const MIcuBreakIterator);

    int next = d->icuIterator->following(d->current);

    if (next != BreakIterator::DONE) {
        return true;
    } else {
        return false;
    }
}

//! returns true if boundary exists before current index
bool MIcuBreakIterator::hasPrevious() const
{
    Q_D(const MIcuBreakIterator);

    int previous = d->icuIterator->preceding(d->current);

    if (previous != BreakIterator::DONE) {
        return true;
    } else {
        return false;
    }
}

//! returns the next boundary index after the current index or -1 if none exists.
//! current index is updated to the resulting value.
int MIcuBreakIterator::next()
{
    Q_D(MIcuBreakIterator);

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
int MIcuBreakIterator::next(int index)
{
    Q_D(MIcuBreakIterator);

    int result = d->icuIterator->following(index);

    if (result == BreakIterator::DONE) {
        result = -1;
        toBack();
    }

    return result;
}

//! returns the next boundary index value. The current index is not updated.
int MIcuBreakIterator::peekNext()
{
    Q_D(MIcuBreakIterator);

    int next = d->icuIterator->following(d->current);

    if (next == BreakIterator::DONE) {
        next = -1;
    }

    return next;
}

//! returns the previous boundary index value. The current index is not updated.
int MIcuBreakIterator::peekPrevious()
{
    Q_D(MIcuBreakIterator);

    int previous = d->icuIterator->preceding(d->current);

    if (previous == BreakIterator::DONE) {
        previous = -1;
    }

    return previous;
}

//! returns the previous boundary index or -1 if none exists.
//! The current index is updated to the resulting value.
int MIcuBreakIterator::previous()
{
    Q_D(MIcuBreakIterator);

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
int MIcuBreakIterator::previous(int index)
{
    Q_D(MIcuBreakIterator);

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
int MIcuBreakIterator::previousInclusive()
{
    Q_D(MIcuBreakIterator);
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
int MIcuBreakIterator::previousInclusive(int index)
{
    Q_D(MIcuBreakIterator);
    if (isBoundary(index)) {
        d->current = index - 1;
        return index;
    } else {
        return previous(index);
    }
}

//! returns the current index
int MIcuBreakIterator::index() const
{
    Q_D(const MIcuBreakIterator);
    return d->current;
}

//! Sets the current index to the given value
void MIcuBreakIterator::setIndex(int index)
{
    Q_D(MIcuBreakIterator);
    d->current = index;
}

//! Sets the current index to the end of the string
void MIcuBreakIterator::toBack()
{
    Q_D(MIcuBreakIterator);
    // API reference says last() gives index _beyond_ the last character. not so.
    d->current = d->icuIterator->last() + 1;
}

//! Sets the current index to the beginning of the string
void MIcuBreakIterator::toFront()
{
    Q_D(MIcuBreakIterator);
    d->current = -1;
}

//! Checks if current index is a boundary.
bool MIcuBreakIterator::isBoundary()
{
    Q_D(MIcuBreakIterator);
    return isBoundary(d->current);
}

//! Checks if given index is a boundary.
bool MIcuBreakIterator::isBoundary(int index)
{
    Q_D(MIcuBreakIterator);
    return d->icuIterator->isBoundary(index);
}
