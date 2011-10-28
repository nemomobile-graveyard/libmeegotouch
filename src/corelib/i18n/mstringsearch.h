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

#ifndef MSTRINGSEARCH_H
#define MSTRINGSEARCH_H

#include "mexport.h"
#include "mlocale.h"
#include "mbreakiterator.h"

class MStringSearchPrivate;

/*!
 * \class MStringSearch
 *
 * \brief implements language-sensitive text searching
 *
 * By default, MStringSearch tries to be very lenient, i.e.
 * by default it ignores accent differences, case differences,
 * white space and punctuation and matches substrings anywhere
 * in the text. But there are options to select stricter matching
 * as well.
 *
 * Examples:
 *
 * In English locale, the search pattern “aa” matches by default all
 * uppercase and lowercase variants and accent variants of “aa”,
 * i.e. it matches “Aa”, “aÁ”, ...:
 *
 * \code
 * QString pattern("aa");
 * QString text("Aaland Åland Ááland");
 * MLocale locale("en_US");
 * int start, length;
 * QString match;
 * MStringSearch stringSearch(pattern, text, locale);
 * if (stringSearch.first() != -1) {
 *    start = stringSearch.matchedStart();   // now contains “0”
 *    length = stringSearch.matchedLength(); // now contains “2”
 *    match = stringSearch.matchedText();    // now contains “Aa”
 * }
 * if (stringSearch.last() != -1) {
 *    start = stringSearch.matchedStart();   // now contains “13”
 *    length = stringSearch.matchedLength(); // now contains “2”
 *    match = stringSearch.matchedText();    // now contains “Áá”
 * }
 * \endcode
 *
 * In Danish locale, “aa” is considered as a character on its own
 * with only a tertiary difference to “å”, therefore
 * searching for “aa” matches “aa”, “Aa”, “aA”, “å”, and “Å”.
 * But it does not match “Áá”
 *
 * \code
 * QString pattern("aa");
 * QString text("Aaland Åland Ááland");
 * MLocale locale("da_DK");
 * int start, length;
 * QString match;
 * MStringSearch stringSearch(pattern, text, locale);
 * if (stringSearch.first() != -1) {
 *    start = stringSearch.matchedStart();   // now contains “0”
 *    length = stringSearch.matchedLength(); // now contains “2”
 *    match = stringSearch.matchedText();    // now contains “Aa”
 * }
 * if (stringSearch.last() != -1) {
 *    start = stringSearch.matchedStart();   // now contains “7”
 *    length = stringSearch.matchedLength(); // now contains “1”
 *    match = stringSearch.matchedText();    // now contains “Å”
 * }
 * \endcode
 *
 * In a Chinese locale which uses pinyin sorting, e.g.
 * “zh_CN”, “zh_SG”, “zh_CN@collation=pinyin”, “zh_TW@collation=pinyin”, ...
 * Chinese characters can be matched via their pinyin transcription.
 *
 * For example “liu” will match “liu”, “líu”, “liǔ”, “刘”,
 * “柳”, ..  and all other Chinese characters which have a pinyin
 * transcription of “liu” (any tone).  But if the pattern contains
 * Chinese, it will only match Chinese characters exactly, e.g. the
 * pattern “刘” will <b>only</b> match “刘”, it will <b>not</b>
 * match “líu”, “liǔ”, “柳”, ...
 *
 * Mixed input will only match the Chinese exactly, i.e. the pattern
 * “liu 刘” will match “liu 刘”, “liu刘”, “líu刘”, ... but
 * will <b>not</b> match “liu 柳”.
 */

class M_CORE_EXPORT MStringSearch
{
public:
    /*!
     * \brief constructs a MStringSearch
     * \param pattern: the search string to search for
     * \param text: the text in which the search string is searched
     * \param locale: the locale which determines the language-specific rules
     * \param breakIteratorType: the  break iterator type to use
     *
     * The default for the break iterator type is
     * MBreakIterator::CharacterIterator.
     * Possible choices for the break iterator type are:
     *
     * <ul>
     * <li>MBreakIterator::CharacterIterator<br>
     * Matches are limited to character boundaries
     * <li>
     * <li>MBreakIterator::WordIterator<br>
     * Matches are limited to word boundaries. For example
     * “abc” would match in “ abc ” but it would not match in “ abcd ”.
     * <li>
     * <li>MBreakIterator::LineIterator<br>
     * Matches are limited to good line breaking positions.
     * <li>
     * <li>MBreakIterator::SentenceIterator<br>
     * Matches are limited to the beginnings and  ends of sentences.
     * <li>
     * <li>MBreakIterator::TitleIterator<br>
     * Matches are limited to title-casing breaks.
     * <li>
     * </ul>
     *
     * \sa setPattern(const QString &pattern)
     * \sa setText(const QString &text)
     * \sa setLocale(const MLocale &locale)
     * \sa setCollatorStrength(MLocale::CollatorStrength collatorStrength)
     * \sa setAlternateHandlingShifted(bool isShifted)
     */
    MStringSearch(const QString &pattern, const QString &text, const MLocale &locale, MBreakIterator::Type breakIteratorType = MBreakIterator::CharacterIterator);

    /*!
     * \brief destructor for MStringSearch
     */
    virtual ~MStringSearch();

    /*!
     * \brief text describing the error which occurred during the last action
     *
     * Example:
     *
     * \code
     * QString pattern("World");
     * QString text("Hello world!");
     * MLocale locale("en_US");
     * MStringSearch stringSearch(pattern, text, locale);
     * if(!stringSearch.errorString().isEmpty())
     *   qWarning() << stringSearch.errorString();
     * \endcode
     *
     */
    QString errorString() const;

    /*!
     * \brief sets the locale used for the language-sensitive text searching
     */
    void setLocale(const MLocale &locale);

    /*!
     * \brief sets text in which the pattern is searched
     *
     * \sa text()
     * \sa setPattern(const QString &pattern)
     * \sa pattern()
     */
    void setText(const QString &text);

    /*!
     * \brief get the text in which the pattern is searched
     *
     * \sa setText(const QString &text)
     * \sa setPattern(const QString &pattern)
     * \sa pattern()
     */
    QString text() const;

    /*!
     * \brief sets the string to search in the text
     *
     * \sa text()
     * \sa setText(const QString &text)
     * \sa pattern()
     */
    void setPattern(const QString &pattern);

    /*!
     * \brief get the string which is searched for
     *
     * \sa setText(const QString &text)
     * \sa text()
     * \sa setPattern(const QString &pattern)
     */
    QString pattern() const;

    /*!
     * \brief set the strength of the collator used  for searching
     *
     * The default strength of MStringSearch is
     * MLocale::CollatorStrengthPrimary. Therefore, accent
     * differences and case differences are ignored by default.
     *
     * Accent differences usually have secondary strength and
     * case differences usually have tertiary strength.
     * Therefore, setting secondary strengths would not ignore
     * accent differences anymore but still ignore case differences,
     * setting tertiary strength would neither ignore accent
     * differences nor case differences.
     *
     * \sa collatorStrength()
     */
    void setCollatorStrength(MLocale::CollatorStrength collatorStrength);

    /*!
     * \brief gets the strength of the collator currently used for searching
     *
     * \sa setCollatorStrength(MLocale::CollatorStrength collatorStrength)
     */
    MLocale::CollatorStrength collatorStrength() const;

    /*!
     * \brief sets whether the alternate characters are handled shifted or not
     *
     * The default is true.
     *
     * “true”, i.e. “shifted” basically means that white space
     * and punctuation is ignore when matching.
     *
     * \sa alternateHandlingShifted()
     */
    void setAlternateHandlingShifted(bool isShifted);

    /*!
     * \brief gets whether alternate characters are handled shifted or not
     *
     * \sa setAlternateHandlingShifted(bool isShifted)
     */
    bool alternateHandlingShifted() const;

    /*!
     * \brief returns the first index at which the search pattern matches in the text
     * returns “-1” if there is no match.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa last()
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    int first();

    /*!
     * \brief returns the last index at which the search pattern matches in the text
     * returns “-1” if there is no match.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa first()
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    int last();

    /*!
     * \brief moves to the next match
     *
     * Returns the index of the next point at which the text matches
     * the search pattern, starting from the current position.
     *
     * The iterator is adjusted so that its current index (as returned
     * by offset()) is the match position if one was found.
     *
     * Returns “-1” if there no next match is found.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa previous()
     * \sa offset()
     * \sa setOffset(int offset)
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    int next();

    /*!
     * \brief moves to the previous < match
     *
     * Returns the index of the previous point at which the string
     * text matches the search pattern, starting at the current
     * position.
     *
     * The iterator is adjusted so that its current index (as returned
     * by offset()) is the match position if one was found.
     *
     * Returns “-1” if there no previous match is found.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa previous()
     * \sa offset()
     * \sa setOffset(int offset)
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    int previous();

    /*!
     * \brief returns the current search index position
     *
     * \sa setOffset(int offset)
     */
    int offset() const;

    /*!
     * \brief sets the current search index position
     *
     * \sa offset()
     */
    void setOffset(int offset);

    /*!
     * \brief returns the start index of a match found by a previous command
     *
     * Returns the start index of the text that was matched by the
     * most recent call to first(), last(), next(), previous().
     *
     * \sa first()
     * \sa last()
     * \sa next()
     * \sa previous()
     */
    int matchedStart() const;

    /*!
     * \brief returns the start index of a match found by a previous command
     *
     * Returns the length of the text that was matched by the most
     * recent call to first(), last(), next(), previous().
     *
     * \sa first()
     * \sa last()
     * \sa next()
     * \sa previous()
     */
    int matchedLength() const;

    /*!
     * \brief returns the start index of a match found by a previous command
     *
     * Returns the text that was matched by the most recent call to
     * first(), last(), next(), previous().
     *
     * \sa first()
     * \sa last()
     * \sa next()
     * \sa previous()
     */
    QString matchedText() const;

private:
    Q_DISABLE_COPY(MStringSearch)
    MStringSearchPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MStringSearch)
};

#endif
