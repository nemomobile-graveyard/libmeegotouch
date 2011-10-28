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
     * \sa hasError()
     * \sa clearError()
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
     * \brief checks whether there is a “next” (>=) match
     *
     * returns “true” if there is match at a position greater or
     * equal than the current search index position, “false” if not.
     *
     * The current search index is <b>not</b> changed.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa toBack()
     * \sa toFront()
     * \sa hasPrevious()
     * \sa index()
     * \sa setIndex()
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    bool hasNext();

    /*!
     * \brief moves to the “next” (>=) match
     *
     * If there is a match at a position greater or equal than the
     * current search index position, returns the index of that match,
     * returns “-1” if there no such match
     *
     * The current search index is changed to the position of the
     * match, toBack() is called if there is no match.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa previous()
     * \sa toBack()
     * \sa toFront()
     * \sa hasPrevious()
     * \sa index()
     * \sa setIndex()
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    int next();

    /*!
     * \brief moves to the “next” (>=) match starting from a given index
     * \param index the position in the text where to start
     *
     * like next(), only that the current search index where
     * where to start from is given as a parameter.
     *
     * \sa next()
     * \sa previous()
     * \sa previous(int index)
     */
    int next(int index);

    /*!
     * \brief gets the “next” (>=) match without updating the index
     *
     * like next(), only that the current search index is not
     * changed.
     *
     * \sa next()
     * \sa previous()
     * \sa peekPrevious()
     */
    int peekNext();

    /*!
     * \brief checks whether there is a “previous” (<) match
     *
     * returns “true” if there is match at a position smaller than
     * the current search index position, “false” if not.
     *
     * The current search index is <b>not</b> changed.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa toBack()
     * \sa toFront()
     * \sa hasNext()
     * \sa index()
     * \sa setIndex()
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    bool hasPrevious();

    /*!
     * \brief moves to the “previous” (<) match
     *
     * If there is a match at a position smaller than the
     * current search index position, returns the index of that match,
     * returns “-1” if there no such match
     *
     * The current search index is changed to the position of the
     * match, toFront() is called if there is no match.
     *
     * If there is a match, matchedStart(), matchedLength(), and
     * matchedText() can be used to find out what was matched.
     *
     * \sa previous()
     * \sa toBack()
     * \sa toFront()
     * \sa hasPrevious()
     * \sa index()
     * \sa setIndex()
     * \sa matchedStart()
     * \sa matchedLength()
     * \sa matchedText()
     */
    int previous();

    /*!
     * \brief moves to the “previous” (<) match starting from a given index
     * \param index the position in the text where to start
     *
     * like previous(), only that the current search index where
     * where to start from is given as a parameter.
     *
     * \sa next()
     * \sa previous()
     * \sa next(int index)
     */
    int previous(int index);

    /*!
     * \brief gets the “previous” (<) match without updating the index
     *
     * like previous(), only that the current search index is not
     * changed.
     *
     * \sa next()
     * \sa previous()
     * \sa peekNext()
     */
    int peekPrevious();

    /*!
     * \brief moves the current search index to the end of the string
     *
     * moves the current search index to text.size(), i.e. one position
     * <b>after</b> the last index in the text in which the pattern
     * is searched.
     *
     * Calling previous() after toBack() goes to the last match, i.e.
     * it achieves the same as last().
     *
     * \sa toFront()
     * \sa previous()
     * \sa last()
     */
    void toBack();

    /*!
     * \brief moves the current search index to the beginning of the string
     *
     * moves the current search index to 0, i.e. the first index
     * in the text in which the pattern is searched.
     *
     * Calling next() after toFront() goes to the first match, i.e.
     * it achieves the same as first().
     *
     * \sa toFront()
     * \sa previous()
     * \sa last()
     */
    void toFront();

    /*!
     * \brief returns the current search index position
     *
     * \sa setIndex(int index)
     */
    int index() const;

    /*!
     * \brief sets the current search index position
     *
     * \sa index()
     */
    void setIndex(int index);

    /*!
     * \brief returns the start index of a match found by a previous command
     *
     * Returns the start index of the text that was matched by the
     * most recent call to first(), last(), hasNext(), next(),
     * peekNext(), hasPrevious(), previous(), and peekPrevious().
     *
     * \sa first()
     * \sa last()
     * \sa hasNext()
     * \sa next()
     * \sa next(int index)
     * \sa peekNext()
     * \sa hasPrevious()
     * \sa previous()
     * \sa previous(int index)
     * \sa peekPrevious()
     */
    int matchedStart();

    /*!
     * \brief returns the start index of a match found by a previous command
     *
     * Returns the length of the text that was matched by the
     * most recent call to first(), last(), hasNext(), next(),
     * peekNext(), hasPrevious(), previous(), and peekPrevious().
     *
     * \sa first()
     * \sa last()
     * \sa hasNext()
     * \sa next()
     * \sa next(int index)
     * \sa peekNext()
     * \sa hasPrevious()
     * \sa previous()
     * \sa previous(int index)
     * \sa peekPrevious()
     */
    int matchedLength();

    /*!
     * \brief returns the start index of a match found by a previous command
     *
     * Returns the text that was matched by the most recent call to
     * first(), last(), hasNext(), next(), peekNext(), hasPrevious(),
     * previous(), and peekPrevious().
     *
     * \sa first()
     * \sa last()
     * \sa hasNext()
     * \sa next()
     * \sa next(int index)
     * \sa peekNext()
     * \sa hasPrevious()
     * \sa previous()
     * \sa previous(int index)
     * \sa peekPrevious()
     */
    QString matchedText();

private:
    Q_DISABLE_COPY(MStringSearch)
    MStringSearchPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MStringSearch)
};

#endif
