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

#ifndef MCOLLATOR_H
#define MCOLLATOR_H

#include "mexport.h"
#include "mlocale.h"

class QString;
class MCollatorPrivate;

class M_CORE_EXPORT MCollator
{
public:
    MCollator();
    MCollator(const MLocale &locale);
    MCollator(const MCollator &other);
    virtual ~MCollator();

    bool operator()(const QString &s1, const QString &s2) const;

    static MLocale::Comparison compare(const QString &first, const QString &second);

    static MLocale::Comparison compare(MLocale &locale, const QString &first,
                                         const QString &second);

    MCollator &operator=(const MCollator &other);

private:
    // not implemented
    bool operator==(const MCollator &other) const;
    bool operator!=(const MCollator &other) const;

    Q_DECLARE_PRIVATE(MCollator)
    MCollatorPrivate *const d_ptr;

    friend class MLocale;
};


#endif
