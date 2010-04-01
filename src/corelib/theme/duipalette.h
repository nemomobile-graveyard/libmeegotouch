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

#ifndef DUIPALETTE_H
#define DUIPALETTE_H

#include <QColor>
#include "duiexport.h"

class DuiPalettePrivate;
class DuiLogicalValues;

class DUI_EXPORT DuiPalette
{
public:
    DuiPalette(const DuiLogicalValues &);
    ~DuiPalette();

    const QColor &foregroundColor() const;
    const QColor &secondaryForegroundColor() const;
    const QColor &backgroundColor() const;

    const QColor &invertedForegroundColor() const;
    const QColor &invertedSecondaryForegroundColor() const;
    const QColor &invertedBackgroundColor() const;

    const QColor &selectionColor() const;

    const QColor &warningColor() const;
    const QColor &attentionColor() const;
    const QColor &notificationColor() const;

    const QColor &linkColor() const;
    const QColor &activeLinkColor() const;

    const QColor &firstAccentColor() const;
    const QColor &secondAccentColor() const;
    const QColor &thirdAccentColor() const;
    const QColor &fourthAccentColor() const;
    const QColor &fifthAccentColor() const;

    void refresh();

protected:
    DuiPalettePrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(DuiPalette)
    Q_DISABLE_COPY(DuiPalette)
};

#endif

