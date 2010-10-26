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

#include "mpalette.h"
#include "mpalette_p.h"
#include "mlogicalvalues.h"

MPalettePrivate::MPalettePrivate(const MLogicalValues &values) : values(values)
{
}

MPalette::MPalette(const MLogicalValues &values) :
    d_ptr(new MPalettePrivate(values))
{
}

MPalette::~MPalette()
{
    delete d_ptr;
}

void MPalette::refresh()
{
    Q_D(MPalette);

    const QByteArray PALETTE_GROUP_NAME("Palette");

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

const QColor &MPalette::foregroundColor() const
{
    Q_D(const MPalette);
    return d->foregroundColor;
}

const QColor &MPalette::secondaryForegroundColor() const
{
    Q_D(const MPalette);
    return d->secondaryForegroundColor;
}

const QColor &MPalette::backgroundColor() const
{
    Q_D(const MPalette);
    return d->backgroundColor;
}

const QColor &MPalette::invertedForegroundColor() const
{
    Q_D(const MPalette);
    return d->invertedForegroundColor;
}

const QColor &MPalette::invertedSecondaryForegroundColor() const
{
    Q_D(const MPalette);
    return d->invertedSecondaryForegroundColor;
}

const QColor &MPalette::invertedBackgroundColor() const
{
    Q_D(const MPalette);
    return d->invertedBackgroundColor;
}

const QColor &MPalette::selectionColor() const
{
    Q_D(const MPalette);
    return d->selectionColor;
}

const QColor &MPalette::warningColor() const
{
    Q_D(const MPalette);
    return d->warningColor;
}

const QColor &MPalette::attentionColor() const
{
    Q_D(const MPalette);
    return d->attentionColor;
}

const QColor &MPalette::notificationColor() const
{
    Q_D(const MPalette);
    return d->notificationColor;
}

const QColor &MPalette::linkColor() const
{
    Q_D(const MPalette);
    return d->linkColor;
}

const QColor &MPalette::activeLinkColor() const
{
    Q_D(const MPalette);
    return d->activeLinkColor;
}

const QColor &MPalette::firstAccentColor() const
{
    Q_D(const MPalette);
    return d->firstAccentColor;
}

const QColor &MPalette::secondAccentColor() const
{
    Q_D(const MPalette);;
    return d->secondAccentColor;
}

const QColor &MPalette::thirdAccentColor() const
{
    Q_D(const MPalette);;
    return d->thirdAccentColor;
}

const QColor &MPalette::fourthAccentColor() const
{
    Q_D(const MPalette);
    return d->fourthAccentColor;
}

const QColor &MPalette::fifthAccentColor() const
{
    Q_D(const MPalette);
    return d->fifthAccentColor;
}

