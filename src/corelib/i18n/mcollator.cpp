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

#include "mcollator.h"
#include "mcollator_p.h"

#include <unicode/unistr.h>
#include <unicode/coll.h>

#include "mlocale.h"
#include "micuconversions.h"
#include "mlocale_p.h"


/////////////////////
// MCollatorPrivate

MCollatorPrivate::MCollatorPrivate()
    : _coll(0)
{
    // nothing
}

MCollatorPrivate::~MCollatorPrivate()
{
    delete _coll;
}

// allocates an icu collator based on locale
void MCollatorPrivate::initCollator(const icu::Locale &locale)
{
    UErrorCode status = U_ZERO_ERROR;
    _coll = icu::Collator::createInstance(locale, status);
    _coll->setStrength(icu::Collator::QUATERNARY);
    // This is default already in Japanese locales:
    // _coll->setAttribute(UCOL_HIRAGANA_QUATERNARY_MODE, UCOL_ON, status);
}

//////////////////////
// Actual MCollator

/*!
  \class MCollator

  \brief MCollator is a MLocale dependant class that is used to do locale aware string comparisons.

  MCollator can be created using MLocale::collator(). It is used
  as a functor or with the comparison methods.

  Example:
  \verbatim
  MLocale loc; // default locale
  MCollator comp = loc.collator();

  QStringList stringList;
  stringList << "bb" << "da" << "aa" << "ab";

  qSort(stringList.begin(), stringList.end(), comp); // sorts the list
  \endverbatim

 */

//! Constructor, gets locale data from default locale
MCollator::MCollator()
    : d_ptr(new MCollatorPrivate)
{
    Q_D(MCollator);

    MLocale defaultLocale;

    icu::Locale icuLocale
    = defaultLocale.d_ptr->getCategoryLocale(MLocale::MLcCollate);
    d->initCollator(icuLocale);
}

//! Constructor, creates a collator based on given MLocale
MCollator::MCollator(const MLocale &locale)
    : d_ptr(new MCollatorPrivate)
{
    Q_D(MCollator);

    icu::Locale icuLocale
    = locale.d_ptr->getCategoryLocale(MLocale::MLcCollate);
    d->initCollator(icuLocale);
}

//! Copy constructor
MCollator::MCollator(const MCollator &other)
    : d_ptr(new MCollatorPrivate)
{
    Q_D(MCollator);

    d->_coll = other.d_ptr->_coll->safeClone();
}

MCollator::~MCollator()
{
    delete d_ptr;
}

//! operator () works as lessThan comparison.
//! Returns true if first string is less than the second
bool MCollator::operator()(const QString &s1, const QString &s2) const
{
    Q_D(const MCollator);

    const icu::UnicodeString us1 = MIcuConversions::qStringToUnicodeString(s1);
    const icu::UnicodeString us2 = MIcuConversions::qStringToUnicodeString(s2);
    icu::Collator::EComparisonResult result =  d->_coll->compare(us1, us2);

    if (result == Collator::LESS) {
        return true;
    } else {
        return false;
    }
}

//! Compares two strings with the default MLocale
MLocale::Comparison MCollator::compare(const QString &first,
        const QString &second)
{
    MLocale defaultLocale;
    return compare(defaultLocale, first, second);
}

//! Compares two strings using the given locale
MLocale::Comparison MCollator::compare(MLocale &locale, const QString &first,
        const QString &second)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale icuLocale
    = locale.d_ptr->getCategoryLocale(MLocale::MLcCollate);
    icu::Collator *collator = icu::Collator::createInstance(icuLocale, status);
    if (!U_SUCCESS(status)) {
        return MLocale::Equal; // ERROR
    }
    collator->setStrength(icu::Collator::QUATERNARY);
    // This is default already in Japanese locales:
    // collator->setAttribute(UCOL_HIRAGANA_QUATERNARY_MODE, UCOL_ON, status);

    const icu::UnicodeString us1 = MIcuConversions::qStringToUnicodeString(first);
    const icu::UnicodeString us2 = MIcuConversions::qStringToUnicodeString(second);

    // do the comparison
    icu::Collator::EComparisonResult result = collator->compare(us1, us2);
    delete collator;

    if (result == icu::Collator::LESS) {
        return MLocale::LessThan;

    } else if (result == icu::Collator::EQUAL) {
        return MLocale::Equal;

    } else {
        return MLocale::GreaterThan;
    }
}

MCollator &MCollator::operator =(const MCollator &other)
{
    Q_D(MCollator);

    delete d->_coll;
    d->_coll = other.d_ptr->_coll->safeClone();
    return *this;
}
