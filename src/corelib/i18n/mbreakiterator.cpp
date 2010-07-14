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

#include "mbreakiterator.h"

#include "mlocale.h"

#include "mbreakiteratorif.h"

#ifdef HAVE_ICU
#include "micubreakiterator.h"
#else
#include "mnullbreakiterator.h"
#endif

class MBreakIteratorPrivate
{
public:
    MBreakIteratorPrivate();
    virtual ~MBreakIteratorPrivate();

    void init(const MLocale &locale, const QString &text, MBreakIterator::Type type);
    void init(const QString &text, MBreakIterator::Type type);

    MBreakIteratorIf *impl;
};

MBreakIteratorPrivate::MBreakIteratorPrivate()
    : impl(0)
{
    // nothing
}


MBreakIteratorPrivate::~MBreakIteratorPrivate()
{
    delete impl;
}

void MBreakIteratorPrivate::init(const MLocale &locale, const QString &text,
                                 MBreakIterator::Type type)
{
    if (impl)
        delete impl;
#ifdef HAVE_ICU
    impl = new MIcuBreakIterator(locale, text, type);
#else
    impl = new MNullBreakIterator(locale, text, type);
#endif
}

void MBreakIteratorPrivate::init(const QString &text,
                                 MBreakIterator::Type type)
{
    if (impl)
        delete impl;
#ifdef HAVE_ICU
    impl = new MIcuBreakIterator(text, type);
#else
    impl = new MNullBreakIterator(text, type);
#endif
}
///////////////////////////////////


/*!
   \class MBreakIterator

   \brief MBreakIterator allows to iterate strings in order to get the indexes of word boundaries
   and possible line breaks.

   Example to iterate string:
   \verbatim
   MLocale locale; // default
   QString text("This is a string to iterate");
   MBreakIterator it(locale, text, MBreakIterator::WordIterator);
   while (it.hasNext()) {
     int index = it.next();
     mDebug() << "index at " << i;
   }
   \endverbatim

   Another common use is to get indexes around cursor:
   \verbatim
   MLocale locale; // default
   QString text("This is a string to iterate");
   MBreakIterator it(locale, text, MBreakIterator::WordIterator);
   int cursor = 12;
   int previous = it.previousInclusive(cursor);
   int next = it.next(cursor);
   \endverbatim

 */


//! Creates a MBreakIterator for a string based on rules derived from the given locale.
//! The index is set to be before the actual string so next() returns the first index.
//! \param locale used locale
//! \param text text to iterate. SHOULD NOT be changed while iterating.
//! \param type (optional) type of iterator. Default is word iterator
MBreakIterator::MBreakIterator(const MLocale &locale, const QString &text, Type type)
    : d_ptr(new MBreakIteratorPrivate)
{
    Q_D(MBreakIterator);
    d->init(locale, text, type);
}

///! Creates a MBreakIterator using the default locale
MBreakIterator::MBreakIterator(const QString &text, Type type)
    : d_ptr(new MBreakIteratorPrivate)
{
    Q_D(MBreakIterator);
    d->init(text, type);
}

//! Destructor
MBreakIterator::~MBreakIterator()
{
    delete d_ptr;
}

//! returns true if a boundary exists after current index
bool MBreakIterator::hasNext() const
{
    Q_D(const MBreakIterator);
    return d->impl->hasNext();
}

//! returns true if boundary exists before current index
bool MBreakIterator::hasPrevious() const
{
    Q_D(const MBreakIterator);
    return d->impl->hasPrevious();
}

//! returns the next boundary index after the current index or -1 if none exists.
//! current index is updated to the resulting value.
int MBreakIterator::next()
{
    Q_D(const MBreakIterator);
    return d->impl->next();
}

//! returns the next boundary after the given index. Returns the boundary or -1
//! if none exists. Updates the current index to the resulting value.
int MBreakIterator::next(int index)
{
    Q_D(const MBreakIterator);
    return d->impl->next(index);
}

//! returns the next boundary index value. The current index is not updated.
int MBreakIterator::peekNext()
{
    Q_D(const MBreakIterator);
    return d->impl->peekNext();
}

//! returns the previous boundary index value. The current index is not updated.
int MBreakIterator::peekPrevious()
{
    Q_D(const MBreakIterator);
    return d->impl->peekPrevious();
}

//! returns the previous boundary index or -1 if none exists.
//! The current index is updated to the resulting value.
int MBreakIterator::previous()
{
    Q_D(const MBreakIterator);
    return d->impl->previous();
}

//! returns the previous boundary from the given index value or -1 if none exists.
//! The current index is updated to the resulting value.
int MBreakIterator::previous(int index)
{
    Q_D(const MBreakIterator);
    return d->impl->previous(index);
}


//! returns the previous boundary including the current index in the search.
//! If current index is a boundary, it is returned and current is decreased by one,
//! otherwise works as previous()
int MBreakIterator::previousInclusive()
{
    Q_D(const MBreakIterator);
    return d->impl->previousInclusive();
}

//! returns the previous boundary from given index, including the index in the search.
//! If index is a boundary, it is returned and current is decreased by one,
//! otherwise works as previous()
int MBreakIterator::previousInclusive(int index)
{
    Q_D(const MBreakIterator);
    return d->impl->previousInclusive(index);
}

//! returns the current index
int MBreakIterator::index() const
{
    Q_D(const MBreakIterator);
    return d->impl->index();
}

//! Sets the current index to the given value
void MBreakIterator::setIndex(int index)
{
    Q_D(MBreakIterator);
    d->impl->setIndex(index);
}

//! Sets the current index to the end of the string
void MBreakIterator::toBack()
{
    Q_D(MBreakIterator);
    d->impl->toBack();
}

//! Sets the current index to the beginning of the string
void MBreakIterator::toFront()
{
    Q_D(MBreakIterator);
    d->impl->toFront();
}

//! Checks if current index is a boundary.
bool MBreakIterator::isBoundary()
{
    Q_D(MBreakIterator);
    return d->impl->isBoundary();
}

//! Checks if given index is a boundary.
bool MBreakIterator::isBoundary(int index)
{
    Q_D(MBreakIterator);
    return d->impl->isBoundary(index);
}
