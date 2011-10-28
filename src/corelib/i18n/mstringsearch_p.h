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

#ifndef MSTRINGSEARCH_P_H
#define MSTRINGSEARCH_P_H

#include <unicode/utypes.h>
#include <unicode/stsearch.h>
#include <unicode/brkiter.h>

class MLocale;

class MStringSearchPrivate
{
    Q_DECLARE_PUBLIC(MStringSearch)

public:
    MStringSearchPrivate();

    virtual ~MStringSearchPrivate();

    bool hasError() const;
    void clearError();
    QString errorString() const;

    bool containsHani(const QString &text) const;
    QString searchCollatorLocaleName(const QString &pattern, const MLocale &locale) const;
    void setIcuCollatorOptions();
    void updateOrInitIcuCollator();
    void icuStringSearchSetCollator();

    MLocale _locale;
    QString _pattern;
    QString _text;
    QString _searchCollatorLocaleName;
    MLocale::CollatorStrength _collatorStrength;
    Qt::CaseSensitivity _caseSensitivity;
    bool _alternateHandlingShifted;

    UErrorCode _status;
    icu::Collator *_icuCollator;
    icu::StringSearch *_icuStringSearch;

    MStringSearch *q_ptr;
private:
    Q_DISABLE_COPY(MStringSearchPrivate)
};

#endif
