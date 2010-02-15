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

#include "duipalette.h"
#include "duipalette_p.h"
#include "duilogicalvalues.h"

DuiPalettePrivate::DuiPalettePrivate(const DuiLogicalValues &values) : values(values)
{
}

DuiPalette::DuiPalette(const DuiLogicalValues &values) :
    d_ptr(new DuiPalettePrivate(values))
{
}

DuiPalette::~DuiPalette()
{
    delete d_ptr;
}

void DuiPalette::refresh()
{
    Q_D(DuiPalette);

    const QString PALETTE_GROUP_NAME("Palette");

    d->foregroundColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_FOREGROUND");
    d->secondaryForegroundColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_SECONDARY_FOREGROUND");
    d->backgroundColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_BACKGROUND");
    d->invertedForegroundColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_INVERTED_FOREGROUND");
    d->invertedSecondaryForegroundColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_INVERTED_SECONDARY_FOREGROUND");
    d->invertedBackgroundColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_INVERTED_BACKGROUND");
    d->selectionColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_SELECT");
    d->warningColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_WARNING");
    d->attentionColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_ATTENTION");
    d->notificationColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_NOTIFICATION");
    d->linkColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_LINK");
    d->activeLinkColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_LINK_ACTIVE");
    d->firstAccentColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_ACCENT1");
    d->secondAccentColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_ACCENT2");
    d->thirdAccentColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_ACCENT3");
    d->fourthAccentColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_ACCENT4");
    d->fifthAccentColor = d->values.color(PALETTE_GROUP_NAME, "COLOR_ACCENT5");

}

const QColor &DuiPalette::foregroundColor() const
{
    Q_D(const DuiPalette);
    return d->foregroundColor;
}

const QColor &DuiPalette::secondaryForegroundColor() const
{
    Q_D(const DuiPalette);
    return d->secondaryForegroundColor;
}

const QColor &DuiPalette::backgroundColor() const
{
    Q_D(const DuiPalette);
    return d->backgroundColor;
}

const QColor &DuiPalette::invertedForegroundColor() const
{
    Q_D(const DuiPalette);
    return d->invertedForegroundColor;
}

const QColor &DuiPalette::invertedSecondaryForegroundColor() const
{
    Q_D(const DuiPalette);
    return d->invertedSecondaryForegroundColor;
}

const QColor &DuiPalette::invertedBackgroundColor() const
{
    Q_D(const DuiPalette);
    return d->invertedBackgroundColor;
}

const QColor &DuiPalette::selectionColor() const
{
    Q_D(const DuiPalette);
    return d->selectionColor;
}

const QColor &DuiPalette::warningColor() const
{
    Q_D(const DuiPalette);
    return d->warningColor;
}

const QColor &DuiPalette::attentionColor() const
{
    Q_D(const DuiPalette);
    return d->attentionColor;
}

const QColor &DuiPalette::notificationColor() const
{
    Q_D(const DuiPalette);
    return d->notificationColor;
}

const QColor &DuiPalette::linkColor() const
{
    Q_D(const DuiPalette);
    return d->linkColor;
}

const QColor &DuiPalette::activeLinkColor() const
{
    Q_D(const DuiPalette);
    return d->activeLinkColor;
}

const QColor &DuiPalette::firstAccentColor() const
{
    Q_D(const DuiPalette);
    return d->firstAccentColor;
}

const QColor &DuiPalette::secondAccentColor() const
{
    Q_D(const DuiPalette);;
    return d->secondAccentColor;
}

const QColor &DuiPalette::thirdAccentColor() const
{
    Q_D(const DuiPalette);;
    return d->thirdAccentColor;
}

const QColor &DuiPalette::fourthAccentColor() const
{
    Q_D(const DuiPalette);
    return d->fourthAccentColor;
}

const QColor &DuiPalette::fifthAccentColor() const
{
    Q_D(const DuiPalette);
    return d->fifthAccentColor;
}

