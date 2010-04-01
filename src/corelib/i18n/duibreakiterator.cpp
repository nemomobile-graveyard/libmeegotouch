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

#include "duibreakiterator.h"

#include "duilocale.h"

#include "duibreakiteratorif.h"

#ifdef HAVE_ICU
#include "duiicubreakiterator.h"
#else
#include "duinullbreakiterator.h"
#endif

class DuiBreakIteratorPrivate
{
public:
    DuiBreakIteratorPrivate();
    virtual ~DuiBreakIteratorPrivate();

    void init(const DuiLocale &locale, const QString &text, DuiBreakIterator::Type type);

    DuiBreakIteratorIf *impl;
};


DuiBreakIteratorPrivate::DuiBreakIteratorPrivate()
    : impl(0)
{
    // nothing
}


DuiBreakIteratorPrivate::~DuiBreakIteratorPrivate()
{
    delete impl;
}


void DuiBreakIteratorPrivate::init(const DuiLocale &locale, const QString &text,
                                   DuiBreakIterator::Type type)
{
    if (impl) {
        delete impl;
    }

#ifdef HAVE_ICU
    impl = new DuiIcuBreakIterator(locale, text, type);
#else
    impl = new DuiNullBreakIterator(locale, text, type);
#endif
}


///////////////////////////////////


/*!
   \class DuiBreakIterator

   \brief DuiBreakIterator allows to iterate strings in order to get the indexes of word boundaries
   and possible line breaks.

   Example to iterate string:
   \verbatim
   DuiLocale locale; // default
   QString text("This is a string to iterate");
   DuiBreakIterator it(locale, text, DuiBreakIterator::WordIterator);
   while (it.hasNext()) {
     int index = it.next();
     duiDebug() << "index at " << i;
   }
   \endverbatim

   Another common use is to get indexes around cursor:
   \verbatim
   DuiLocale locale; // default
   QString text("This is a string to iterate");
   DuiBreakIterator it(locale, text, DuiBreakIterator::WordIterator);
   int cursor = 12;
   int previous = it.previousInclusive(cursor);
   int next = it.next(cursor);
   \endverbatim

 */


//! Creates a DuiBreakIterator for a string based on rules derived from the given locale.
//! The index is set to be before the actual string so next() returns the first index.
//! \param locale used locale
//! \param text text to iterate. SHOULD NOT be changed while iterating.
//! \param type (optional) type of iterator. Default is word iterator
DuiBreakIterator::DuiBreakIterator(const DuiLocale &locale, const QString &text, Type type)
    : d_ptr(new DuiBreakIteratorPrivate)
{
    Q_D(DuiBreakIterator);
    d->init(locale, text, type);
}


///! Creates a DuiBreakIterator using default locale
DuiBreakIterator::DuiBreakIterator(const QString &text, Type type)
    : d_ptr(new DuiBreakIteratorPrivate)
{
    Q_D(DuiBreakIterator);
    DuiLocale locale; // get default
    d->init(locale, text, type);
}


//! Destructor
DuiBreakIterator::~DuiBreakIterator()
{
    delete d_ptr;
}


//! returns true if a boundary exists after current index
bool DuiBreakIterator::hasNext() const
{
    Q_D(const DuiBreakIterator);

    return d->impl->hasNext();
}


//! returns true if boundary exists before current index
bool DuiBreakIterator::hasPrevious() const
{
    Q_D(const DuiBreakIterator);

    return d->impl->hasPrevious();
}


//! returns the next boundary index after the current index or -1 if none exists.
//! current index is updated to the resulting value.
int DuiBreakIterator::next()
{
    Q_D(const DuiBreakIterator);

    return d->impl->next();
}


//! returns the next boundary after the given index. Returns the boundary or -1
//! if none exists. Updates the current index to the resulting value.
int DuiBreakIterator::next(int index)
{
    Q_D(const DuiBreakIterator);

    return d->impl->next(index);
}


//! returns the next boundary index value. The current index is not updated.
int DuiBreakIterator::peekNext()
{
    Q_D(const DuiBreakIterator);

    return d->impl->peekNext();
}


//! returns the previous boundary index value. The current index is not updated.
int DuiBreakIterator::peekPrevious()
{
    Q_D(const DuiBreakIterator);

    return d->impl->peekPrevious();
}


//! returns the previous boundary index or -1 if none exists.
//! The current index is updated to the resulting value.
int DuiBreakIterator::previous()
{
    Q_D(const DuiBreakIterator);

    return d->impl->previous();
}


//! returns the previous boundary from the given index value or -1 if none exists.
//! The current index is updated to the resulting value.
int DuiBreakIterator::previous(int index)
{
    Q_D(const DuiBreakIterator);

    return d->impl->previous(index);
}


//! returns the previous boundary including the current index in the search.
//! If current index is a boundary, it is returned and current is decreased by one,
//! otherwise works as previous()
int DuiBreakIterator::previousInclusive()
{
    Q_D(const DuiBreakIterator);

    return d->impl->previousInclusive();
}

//! returns the previous boundary from given index, including the index in the search.
//! If index is a boundary, it is returned and current is decreased by one,
//! otherwise works as previous()
int DuiBreakIterator::previousInclusive(int index)
{
    Q_D(const DuiBreakIterator);

    return d->impl->previousInclusive(index);
}


//! Sets the current index to the given value
void DuiBreakIterator::setIndex(int index)
{
    Q_D(DuiBreakIterator);

    d->impl->setIndex(index);
}


//! Sets the current index to the end of the string
void DuiBreakIterator::toBack()
{
    Q_D(DuiBreakIterator);

    d->impl->toBack();
}


//! Sets the current index to the beginning of the string
void DuiBreakIterator::toFront()
{
    Q_D(DuiBreakIterator);

    d->impl->toFront();
}


//! Checks if current index is a boundary.
bool DuiBreakIterator::isBoundary()
{
    Q_D(DuiBreakIterator);

    return d->impl->isBoundary();
}


//! Checks if given index is a boundary.
bool DuiBreakIterator::isBoundary(int index)
{
    Q_D(DuiBreakIterator);

    return d->impl->isBoundary(index);
}
