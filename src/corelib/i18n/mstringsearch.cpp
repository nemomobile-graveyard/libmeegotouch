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
#include "mstringsearch_p.h"

#include <unicode/utypes.h>
#include <unicode/uversion.h>
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
#include <unicode/localpointer.h>
#endif
#include <unicode/uenum.h>
#include <unicode/ucsdet.h>

#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QDebug>

#include "micuconversions.h"

MStringSearchPrivate::MStringSearchPrivate()
    : _currentIndex(0),
      _collatorStrength(MLocale::CollatorStrengthPrimary),
      _caseSensitivity(Qt::CaseInsensitive),
      _alternateHandlingShifted(true),
      _status(U_ZERO_ERROR),
      _icuCollator(0),
      _icuStringSearch(0),
      q_ptr(0)
{
}

MStringSearchPrivate::~MStringSearchPrivate()
{
    delete _icuCollator;
    delete _icuStringSearch;
}

bool MStringSearchPrivate::hasError() const
{
    return(!U_SUCCESS(_status));
}

void MStringSearchPrivate::clearError()
{
    _status = U_ZERO_ERROR;
}

QString MStringSearchPrivate::errorString() const
{
    if (hasError())
        return QString(u_errorName(_status));
    else
        return QString();
}

bool MStringSearchPrivate::containsHani(const QString &text) const
{
    for(int i = 0; i < text.size(); ++i) {
        if(text.at(i).isHighSurrogate()) {
            // not exact, only some ranges
            // above the BMP like 0x20000 - 0x2A6DF
            // and 0x2F800 - 0x2FA1F contain Hani,
            // but “good enough” for the moment ...
            return true;
        }
        ushort cp = text.at(i).unicode();
        if((cp >= 0x4E00 && cp <= 0x9FFF) ||
           (cp >= 0x3400 && cp <= 0x4DFF) ||
           (cp >= 0xF900 && cp <= 0xFAFF)) {
            return true;
        }
    }
    return false;
}

QString MStringSearchPrivate::searchCollatorLocaleName(const QString &pattern, const MLocale &locale) const
{
    QString categoryCollateName = locale.categoryName(MLocale::MLcCollate);
    if(!categoryCollateName.startsWith("zh")) {
        return categoryCollateName;
    }
    else {
        QString categoryCollateCountry
            = locale.categoryCountry(MLocale::MLcCollate);
        if((locale.collation() == MLocale::PinyinCollation
            ||
            ((categoryCollateCountry == "CN"
              || categoryCollateCountry == "SG")
             && locale.collation() == MLocale::DefaultCollation))
           && !containsHani(pattern))
            return QString("zh_CN@collation=pinyinsearch");
        else
            return categoryCollateName;
    }
}

void MStringSearchPrivate::setIcuCollatorOptions()
{
    switch(_collatorStrength) {
    case MLocale::CollatorStrengthPrimary:
        _icuCollator->setStrength(icu::Collator::PRIMARY);
        break;
    case MLocale::CollatorStrengthSecondary:
        _icuCollator->setStrength(icu::Collator::SECONDARY);
        break;
    case MLocale::CollatorStrengthTertiary:
        _icuCollator->setStrength(icu::Collator::TERTIARY);
        break;
    case MLocale::CollatorStrengthQuaternary:
        _icuCollator->setStrength(icu::Collator::QUATERNARY);
        break;
    case MLocale::CollatorStrengthIdentical:
        _icuCollator->setStrength(icu::Collator::IDENTICAL);
        break;
    default:
        _icuCollator->setStrength(icu::Collator::QUATERNARY);
        break;
    }
    // unfortunately this attempt to set the case sensitivity does not
    // have any effect. The documentation says:
    //
    // UCOL_CASE_LEVEL
    //
    // Controls whether an extra case level (positioned before the
    // third level) is generated or not.
    //
    // Acceptable values are UCOL_OFF (default), when case level is not
    // generated, and UCOL_ON which causes the case level to be
    // generated. Contents of the case level are affected by the value of
    // UCOL_CASE_FIRST attribute. A simple way to ignore accent
    // differences in a string is to set the strength to UCOL_PRIMARY and
    // enable case level.
    //
    // But this just doesn’t seem to work.
    switch(_caseSensitivity) {
    case Qt::CaseSensitive:
        clearError();
        _icuCollator->setAttribute(UCOL_CASE_FIRST, UCOL_LOWER_FIRST, _status);
        if(hasError())
            qWarning() << __PRETTY_FUNCTION__
                       << "icu::Collator::setAttribute(UCOL_CASE_FIRST, UCOL_LOWER_FIRST) failed with error"
                       << errorString();
        _icuCollator->setAttribute(UCOL_CASE_LEVEL, UCOL_ON, _status);
        if(hasError())
            qWarning() << __PRETTY_FUNCTION__
                       << "icu::Collator::setAttribute(UCOL_CASE_LEVEL, UCOL_ON) failed with error"
                       << errorString();
        break;
    case Qt::CaseInsensitive:
    default:
        clearError();
        _icuCollator->setAttribute(UCOL_CASE_FIRST, UCOL_OFF, _status);
        if(hasError())
            qWarning() << __PRETTY_FUNCTION__
                       << "icu::Collator::setAttribute(UCOL_CASE_FIRST, UCOL_OFF, UCOL_OFF) failed with error"
                       << errorString();
        clearError();
        _icuCollator->setAttribute(UCOL_CASE_LEVEL, UCOL_OFF, _status);
        if(hasError())
            qWarning() << __PRETTY_FUNCTION__
                       << "icu::Collator::setAttribute(UCOL_CASE_LEVEL, UCOL_OFF) failed with error"
                       << errorString();
        break;
    }
    if(_alternateHandlingShifted) {
        // ignore space and punctuation characters (simplified, real explanation is longer ...)
        clearError();
        _icuCollator->setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_SHIFTED, _status);
        if(hasError())
            qWarning() << __PRETTY_FUNCTION__
                       << "icu::Collator::setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_SHIFTED) failed with error"
                       << errorString();
    }
    else {
        // don’t ignore space and punctuation characters
        clearError();
        _icuCollator->setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_NON_IGNORABLE, _status);
        if(hasError())
            qWarning() << __PRETTY_FUNCTION__
                       << "icu::Collator::setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_NON_IGNORABLE) failed with error"
                       << errorString();
    }
    // force normalization:
    clearError();
    _icuCollator->setAttribute(UCOL_NORMALIZATION_MODE, UCOL_ON, _status);
    if(hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "icu::Collator::setAttribute(UCOL_NORMALIZATION_MODE, UCOL_ON) failed with error"
                   << errorString();
}

void MStringSearchPrivate::updateOrInitIcuCollator()
{
    QString newSearchCollatorLocaleName
        = searchCollatorLocaleName(_pattern, _locale);
    if(!_icuCollator || _searchCollatorLocaleName != newSearchCollatorLocaleName) {
        _searchCollatorLocaleName = newSearchCollatorLocaleName;
        if(_icuCollator)
            delete _icuCollator;
        clearError();
        _icuCollator = icu::Collator::createInstance(
            icu::Locale(qPrintable(_searchCollatorLocaleName)),
            _status);
        if(hasError())
            qWarning() << __PRETTY_FUNCTION__
                       << "icu::Collator::createInstance() failed with error"
                       << errorString();
    }
    setIcuCollatorOptions();
}

void MStringSearchPrivate::icuStringSearchSetCollator()
{
    clearError();
    _icuStringSearch->setCollator(
        static_cast<RuleBasedCollator *>(_icuCollator),
        _status);
    if(hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "new icu::StringSearch() failed with error"
                   << errorString();
}

MStringSearch::MStringSearch(const QString &pattern, const QString &text, const MLocale &locale, MBreakIterator::Type breakIteratorType)
    : d_ptr (new MStringSearchPrivate)
{
    Q_D(MStringSearch);
    d->q_ptr = this;
    d->_locale = locale;
    d->_pattern = pattern;
    d->_text = text;
    d->updateOrInitIcuCollator();
    icu::BreakIterator *icuBreakIterator = NULL;
    switch(breakIteratorType) {
    case MBreakIterator::SentenceIterator:
        icuBreakIterator = icu::BreakIterator::createSentenceInstance(
            icu::Locale(qPrintable(d->_searchCollatorLocaleName)),
            d->_status);
        break;
    case MBreakIterator::TitleIterator:
        icuBreakIterator = icu::BreakIterator::createTitleInstance(
            icu::Locale(qPrintable(d->_searchCollatorLocaleName)),
            d->_status);
        break;
    case MBreakIterator::LineIterator:
    icuBreakIterator = icu::BreakIterator::createLineInstance(
        icu::Locale(qPrintable(d->_searchCollatorLocaleName)),
        d->_status);
        break;
    case MBreakIterator::WordIterator:
        icuBreakIterator = icu::BreakIterator::createWordInstance(
            icu::Locale(qPrintable(d->_searchCollatorLocaleName)),
            d->_status);
        break;
    case MBreakIterator::CharacterIterator:
    default:
        icuBreakIterator = icu::BreakIterator::createCharacterInstance(
            icu::Locale(qPrintable(d->_searchCollatorLocaleName)),
            d->_status);
        break;
    }
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "breakIteratorType =" << breakIteratorType
                   << "icu::BreakIterator::create...Instance() failed with error"
                   << errorString();
    d->clearError();
    d->_icuStringSearch = new icu::StringSearch(
        MIcuConversions::qStringToUnicodeString(d->_pattern),
        MIcuConversions::qStringToUnicodeString(d->_text),
        static_cast<RuleBasedCollator *>(d->_icuCollator),
        icuBreakIterator,
        d->_status);
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "new icu::StringSearch() failed with error"
                   << errorString();
}

MStringSearch::~MStringSearch()
{
    delete d_ptr;
}

QString MStringSearch::errorString() const
{
    Q_D(const MStringSearch);
    return d->errorString();
}

void MStringSearch::setLocale(const MLocale &locale)
{
    Q_D(MStringSearch);
    d->clearError();
    d->_locale=locale;
    d->updateOrInitIcuCollator();
    d->icuStringSearchSetCollator();
}

void MStringSearch::setText(const QString &text)
{
    Q_D(MStringSearch);
    d->_text = text;
    d->_currentIndex = 0;
    d->clearError();
    if(d->_icuStringSearch)
        d->_icuStringSearch->setText(
            MIcuConversions::qStringToUnicodeString(d->_text),
            d->_status);
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "new icu::StringSearch::setText() failed with error"
                   << errorString();
}

QString MStringSearch::text() const
{
    Q_D(const MStringSearch);
    return d->_text;
}

void MStringSearch::setPattern(const QString &pattern)
{
    Q_D(MStringSearch);
    d->clearError();
    if(pattern == d->_pattern)
        return;
    d->_pattern = pattern;
    d->_icuStringSearch->setPattern(
        MIcuConversions::qStringToUnicodeString(d->_pattern),
        d->_status);
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "icu::StringSearch::setPattern() failed with error"
                   << errorString();
    d->updateOrInitIcuCollator();
    d->icuStringSearchSetCollator();
}

QString MStringSearch::pattern() const
{
    Q_D(const MStringSearch);
    return d->_pattern;
}

void MStringSearch::setCollatorStrength(MLocale::CollatorStrength collatorStrength)
{
    Q_D(MStringSearch);
    d->_collatorStrength = collatorStrength;
    d->setIcuCollatorOptions();
}

MLocale::CollatorStrength MStringSearch::collatorStrength() const
{
    Q_D(const MStringSearch);
    return d->_collatorStrength;
}

void MStringSearch::setAlternateHandlingShifted(bool isShifted)
{
    Q_D(MStringSearch);
    d->_alternateHandlingShifted = isShifted;
    d->setIcuCollatorOptions();
}

bool MStringSearch::alternateHandlingShifted() const
{
    Q_D(const MStringSearch);
    return d->_alternateHandlingShifted;
}

int MStringSearch::first()
{
    Q_D(MStringSearch);
    d->clearError();
    int first = d->_icuStringSearch->first(d->_status);
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "icu::StringSearch::first() failed with error"
                   << errorString();
    if (first == USEARCH_DONE) {
        first = -1;
        d->_currentIndex = d->_text.size();
    }
    else {
        d->_currentIndex = first;
    }
    return first;
}

int MStringSearch::last()
{
    Q_D(MStringSearch);
    d->clearError();
    int last = d->_icuStringSearch->last(d->_status);
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "icu::StringSearch::last() failed with error"
                   << errorString();
    if (last == USEARCH_DONE) {
        last = -1;
        d->_currentIndex = 0;
    }
    else {
        d->_currentIndex = last;
    }
    return last;
}

int MStringSearch::next()
{
    Q_D(MStringSearch);
    d->clearError();
    int next = d->_icuStringSearch->following(d->_currentIndex, d->_status);
    // using the above _icuStringSearch->following() behaves the same
    // as the two calls:
    //     d->_icuStringSearch->setOffset(d->_currentIndex, d->_status);
    //     next = d->_icuStringSearch->next(d->_status);
    // “next” is set to the position of the next match >= the current
    // position, i.e. if there is a match at the current position,
    // “next” is set to the current position.
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "icu::StringSearch::following("
                   << d->_currentIndex
                   <<") failed with error"
                   << errorString();
    if (next == USEARCH_DONE) {
        next = -1;
        d->_currentIndex = d->_text.size();
    }
    else {
        d->_currentIndex = next;
    }
    return next;
}

int MStringSearch::previous()
{
    Q_D(MStringSearch);
    d->clearError();
    int previous = d->_icuStringSearch->preceding(d->_currentIndex, d->_status);
    if(d->hasError())
        qWarning() << __PRETTY_FUNCTION__
                   << "icu::StringSearch::preceding("
                   << d->_currentIndex
                   <<") failed with error"
                   << errorString();
    if (previous == USEARCH_DONE) {
        previous = -1;
        d->_currentIndex = 0;
    }
    else {
        d->_currentIndex = previous;
    }
    return previous;
}

int MStringSearch::index() const
{
    Q_D(const MStringSearch);
    return d->_currentIndex;
}

void MStringSearch::setIndex(int index)
{
    Q_D(MStringSearch);
    if(index < 0)
        d->_currentIndex = 0;
    else if (index > d->_text.size())
        d->_currentIndex = d->_text.size();
    else
        d->_currentIndex = index;
}

int MStringSearch::matchedStart() const
{
    Q_D(const MStringSearch);
    int start = d->_icuStringSearch->getMatchedStart();
    if (start == USEARCH_DONE)
        return -1;
    else
        return start;
}

int MStringSearch::matchedLength() const
{
    Q_D(const MStringSearch);
    int length = d->_icuStringSearch->getMatchedLength();
    if (length == USEARCH_DONE)
        return -1;
    else
        return length;
}

QString MStringSearch::matchedText() const
{
    Q_D(const MStringSearch);
    icu::UnicodeString uString;
    d->_icuStringSearch->getMatchedText(uString);
    return MIcuConversions::unicodeStringToQString(uString);
}
