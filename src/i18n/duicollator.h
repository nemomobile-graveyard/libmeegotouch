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

#ifndef DUICOLLATOR_H
#define DUICOLLATOR_H

#include "duiexport.h"
#include "duilocale.h"

class QString;
class DuiCollatorPrivate;

class DUI_EXPORT DuiCollator
{
public:
    DuiCollator();
    DuiCollator(const DuiLocale &locale);
    DuiCollator(const DuiCollator &other);
    virtual ~DuiCollator();

    bool operator()(const QString &s1, const QString &s2) const;

    static DuiLocale::Comparison compare(const QString &first, const QString &second);

    static DuiLocale::Comparison compare(DuiLocale &locale, const QString &first,
                                         const QString &second);

private:
    // not implemented
    bool operator==(const DuiCollator &other) const;
    bool operator!=(const DuiCollator &other) const;
    DuiCollator &operator=(const DuiCollator &other);

    Q_DECLARE_PRIVATE(DuiCollator)
    DuiCollatorPrivate *const d_ptr;

    friend class DuiLocale;
};


#endif
