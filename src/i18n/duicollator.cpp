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

#include "duicollator.h"
#include "duicollator_p.h"

#include <unicode/unistr.h>
#include <unicode/coll.h>

#include "duilocale.h"
#include "duiicuconversions.h"
#include "duilocale_p.h"


/////////////////////
// DuiCollatorPrivate

DuiCollatorPrivate::DuiCollatorPrivate()
    : _coll(0)
{
    // nothing
}


DuiCollatorPrivate::~DuiCollatorPrivate()
{
    delete _coll;
}


// allocates an icu collator based on locale
void DuiCollatorPrivate::initCollator(icu::Locale locale)
{
    UErrorCode status = U_ZERO_ERROR;
    _coll = icu::Collator::createInstance(locale, status);
}


//////////////////////
// Actual DuiCollator


/*!
  \class DuiCollator

  \brief DuiCollator is a DuiLocale dependant class that is used to do locale aware string comparisons.

  DuiCollator can be created using DuiLocale::collator(). It is used
  as a functor or with the comparison methods.

  Example:
  \verbatim
  DuiLocale loc; // default locale
  DuiCollator comp = loc.collator();

  QStringList stringList;
  stringList << "bb" << "da" << "aa" << "ab";

  qSort(stringList.begin(), stringList.end(), comp); // sorts the list
  \endverbatim

 */

//! Constructor, gets locale data from default locale
DuiCollator::DuiCollator()
    : d_ptr(new DuiCollatorPrivate)
{
    Q_D(DuiCollator);

    DuiLocale defaultLocale;

    icu::Locale icuLocale
    = defaultLocale.d_ptr->getCategoryLocale(DuiLocale::DuiLcCollate);
    d->initCollator(icuLocale);
}


//! Constructor, creates a collator based on given DuiLocale
DuiCollator::DuiCollator(const DuiLocale &locale)
    : d_ptr(new DuiCollatorPrivate)
{
    Q_D(DuiCollator);

    icu::Locale icuLocale
    = locale.d_ptr->getCategoryLocale(DuiLocale::DuiLcCollate);
    d->initCollator(icuLocale);
}


//! Copy constructor
DuiCollator::DuiCollator(const DuiCollator &other)
    : d_ptr(new DuiCollatorPrivate)
{
    Q_D(DuiCollator);

    d->_coll = other.d_ptr->_coll->safeClone();
}


DuiCollator::~DuiCollator()
{
    delete d_ptr;
}


//! operator () works as lessThan comparison.
//! Returns true if first string is less than the second
bool DuiCollator::operator()(const QString &s1, const QString &s2) const
{
    Q_D(const DuiCollator);

    const icu::UnicodeString us1 = DuiIcuConversions::qStringToUnicodeString(s1);
    const icu::UnicodeString us2 = DuiIcuConversions::qStringToUnicodeString(s2);
    icu::Collator::EComparisonResult result =  d->_coll->compare(us1, us2);

    if (result == Collator::LESS) {
        return true;
    } else {
        return false;
    }
}


//! Compares two strings with the default DuiLocale
DuiLocale::Comparison DuiCollator::compare(const QString &first,
        const QString &second)
{
    DuiLocale defaultLocale;
    return compare(defaultLocale, first, second);
}


//! Compares two string using given locale
DuiLocale::Comparison DuiCollator::compare(DuiLocale &locale, const QString &first,
        const QString &second)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale icuLocale
    = locale.d_ptr->getCategoryLocale(DuiLocale::DuiLcCollate);
    icu::Collator *collator = icu::Collator::createInstance(icuLocale, status);

    if (!U_SUCCESS(status)) {
        return DuiLocale::Equal; // ERROR
    }

    const icu::UnicodeString us1 = DuiIcuConversions::qStringToUnicodeString(first);
    const icu::UnicodeString us2 = DuiIcuConversions::qStringToUnicodeString(second);

    // do the comparison
    icu::Collator::EComparisonResult result = collator->compare(us1, us2);
    delete collator;

    if (result == icu::Collator::LESS) {
        return DuiLocale::LessThan;

    } else if (result == icu::Collator::EQUAL) {
        return DuiLocale::Equal;

    } else {
        return DuiLocale::GreaterThan;
    }
}
