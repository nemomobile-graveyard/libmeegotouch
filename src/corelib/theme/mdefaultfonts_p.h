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

#ifndef MDEFAULTFONTS_P_H
#define MDEFAULTFONTS_P_H

class MDefaultFontsPrivate
{
public:
    MDefaultFontsPrivate(const MLogicalValues &values);

    const MLogicalValues &values;

    mutable QFont extraSmallFont;
    mutable QFont smallFont;
    mutable QFont defaultFont;
    mutable QFont largeFont;
    mutable QFont extraLargeFont;

    mutable bool refresh;
    void reloadAllValues() const;
};

#endif
