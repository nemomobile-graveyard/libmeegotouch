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

#ifndef MBREAKITERATORIF_H
#define MBREAKITERATORIF_H

#include "mexport.h"
#include "mlocale.h"

#include <QString>

/*! \internal
 * This class describes the interface that both MIcuIterator
 * and MNullIterator implement. It is used to have the possibility
 * to choose one of both backend implementations at compile time
 * and to keep the number of needed "#ifdef" small.
 */
class M_CORE_EXPORT MBreakIteratorIf
{
public:
    virtual ~MBreakIteratorIf() {};

    // java-style iterator interface:
    virtual bool hasNext() const = 0;
    virtual bool hasPrevious() const = 0;
    virtual int next() = 0;
    virtual int next(int index) = 0; // searching from explicit index
    virtual int peekNext() = 0;
    virtual int peekPrevious() = 0;
    virtual int previous() = 0;
    virtual int previous(int index) = 0;
    virtual int previousInclusive() = 0;
    virtual int previousInclusive(int index) = 0;
    virtual void toBack() = 0;
    virtual void toFront() = 0;
    virtual int index() const = 0;
    virtual void setIndex(int index) = 0;
    virtual bool isBoundary() = 0;
    virtual bool isBoundary(int index) = 0;
};
//! \internal_end

#endif
