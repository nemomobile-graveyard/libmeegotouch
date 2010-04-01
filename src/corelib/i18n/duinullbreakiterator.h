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

#ifndef DUINULLBREAKITERATOR_H
#define DUINULLBREAKITERATOR_H

#include "duiexport.h"
#include "duibreakiterator.h"
#include "duibreakiteratorif.h"

#include <QString>

class DuiNullBreakIteratorPrivate;

//! \internal Used by DuiBreakIterator
class DUI_EXPORT DuiNullBreakIterator : public DuiBreakIteratorIf
{
public:
    DuiNullBreakIterator(const DuiLocale &locale,
                         const QString &text,
                         DuiBreakIterator::Type type = DuiBreakIterator::WordIterator);
    explicit DuiNullBreakIterator(const QString &text,
                                  DuiBreakIterator::Type type = DuiBreakIterator::WordIterator);
    virtual ~DuiNullBreakIterator();

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
    Q_DISABLE_COPY(DuiNullBreakIterator)
    Q_DECLARE_PRIVATE(DuiNullBreakIterator)
    DuiNullBreakIteratorPrivate *const d_ptr;
};
//! \internal_end

#endif
