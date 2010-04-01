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

#ifndef DUIDEFAULTFONTS_H
#define DUIDEFAULTFONTS_H

#include <QFont>
#include "duiexport.h"

class DuiDefaultFontsPrivate;
class DuiLogicalValues;

class DUI_EXPORT DuiDefaultFonts
{
public:
    DuiDefaultFonts(const DuiLogicalValues &);
    ~DuiDefaultFonts();

    const QFont &extraSmallFont() const;
    const QFont &smallFont() const;
    const QFont &defaultFont() const;
    const QFont &largeFont() const;
    const QFont &extraLargeFont() const;

    void refresh();

protected:
    DuiDefaultFontsPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(DuiDefaultFonts)
    Q_DISABLE_COPY(DuiDefaultFonts)
};

#endif

