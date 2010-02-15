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

#ifndef DUIBREAKITERATOR_H
#define DUIBREAKITERATOR_H

#include "duiexport.h"
#include "duilocale.h"

#include <QString>

class DuiBreakIteratorPrivate;

class DUI_EXPORT DuiBreakIterator
{
public:
    enum Type {LineIterator, WordIterator}; // would also be supported: character, sentence, title

    DuiBreakIterator(const DuiLocale &locale, const QString &text, Type type = WordIterator);
    explicit DuiBreakIterator(const QString &text, Type type = WordIterator);
    virtual ~DuiBreakIterator();

    // java-style iterator interface:
    bool hasNext() const;
    bool hasPrevious() const;
    int next();
    int next(int index); // searching from explicit index
    int peekNext();
    int peekPrevious();
    int previous();
    int previous(int index);
    int previousInclusive();
    int previousInclusive(int index);
    void toBack();
    void toFront();
    void setIndex(int index);
    bool isBoundary();
    bool isBoundary(int index);

private:
    Q_DISABLE_COPY(DuiBreakIterator)
    Q_DECLARE_PRIVATE(DuiBreakIterator)
    DuiBreakIteratorPrivate *const d_ptr;
};


#endif
