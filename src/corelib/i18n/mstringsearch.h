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
 * explain language-sensitive text searching in detail here ...
 *
 * Example:
 *
 * \code
 * add code example here
 * \endcode
 */

class M_CORE_EXPORT MStringSearch
{
public:
    MStringSearch(QString &pattern, QString &text, const MLocale &locale, MBreakIterator::Type breakIteratorType = MBreakIterator::CharacterIterator);
    virtual ~MStringSearch();
    bool hasError() const;
    void clearError();
    QString errorString() const;
    void setLocale(const MLocale &locale);
    void setText(const QString &text);
    QString text() const;
    void setPattern(const QString &pattern);
    QString pattern() const;
    void setCollatorStrength(MLocale::CollatorStrength collatorStrength);
    MLocale::CollatorStrength collatorStrength() const;
    void setAlternateHandlingShifted(bool isShifted);
    bool alternateHandlingShifted() const;

    int first();
    int last();
    bool hasNext();
    int next();
    int next(int index);
    int peekNext();
    bool hasPrevious();
    int previous();
    int previous(int index);
    int peekPrevious();
    void toBack();
    void toFront();
    int index() const;
    void setIndex(int index);
    int matchedStart();
    int matchedLength();
    QString matchedText();

private:
    Q_DISABLE_COPY(MStringSearch)
    MStringSearchPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MStringSearch)
};

#endif
