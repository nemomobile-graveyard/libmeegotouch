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

#ifndef MPALETTE_P_H
#define MPALETTE_P_H

class MPalettePrivate
{
public:
    MPalettePrivate(const MLogicalValues &values);

    const MLogicalValues &values;

    QColor foregroundColor;
    QColor secondaryForegroundColor;
    QColor backgroundColor;
    QColor invertedForegroundColor;
    QColor invertedSecondaryForegroundColor;
    QColor invertedBackgroundColor;
    QColor selectionColor;
    QColor warningColor;
    QColor attentionColor;
    QColor notificationColor;
    QColor linkColor;
    QColor activeLinkColor;
    QColor firstAccentColor;
    QColor secondAccentColor;
    QColor thirdAccentColor;
    QColor fourthAccentColor;
    QColor fifthAccentColor;
};

#endif
