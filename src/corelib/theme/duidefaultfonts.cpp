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

#include "duidefaultfonts.h"
#include "duidefaultfonts_p.h"
#include "duilogicalvalues.h"

DuiDefaultFontsPrivate::DuiDefaultFontsPrivate(const DuiLogicalValues &values) : values(values), refresh(true)
{
}

void DuiDefaultFontsPrivate::reloadAllValues() const
{
    const QString FONT_GROUP_NAME = "Fonts";

    extraSmallFont = values.font(FONT_GROUP_NAME, "FONT_XSMALL");
    smallFont = values.font(FONT_GROUP_NAME, "FONT_SMALL");
    defaultFont = values.font(FONT_GROUP_NAME, "FONT_DEFAULT");
    largeFont = values.font(FONT_GROUP_NAME, "FONT_LARGE");
    extraLargeFont = values.font(FONT_GROUP_NAME, "FONT_XLARGE");
    refresh = false;
}

DuiDefaultFonts::DuiDefaultFonts(const DuiLogicalValues &values) :
    d_ptr(new DuiDefaultFontsPrivate(values))
{
}

DuiDefaultFonts::~DuiDefaultFonts()
{
    delete d_ptr;
}

void DuiDefaultFonts::refresh()
{
    // TODO: Fix cyclic dependency between theming/styling & DuideviceProfile (DuiDeviceProfileStyle)
    // We can't load values directly because we need DuiDeviceProfile for dpi conversions but it may not exist yet.
    Q_D(DuiDefaultFonts);
    d->refresh = true;
}

const QFont &DuiDefaultFonts::extraSmallFont() const
{
    Q_D(const DuiDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->extraSmallFont;
}

const QFont &DuiDefaultFonts::smallFont() const
{
    Q_D(const DuiDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->smallFont;
}

const QFont &DuiDefaultFonts::defaultFont() const
{
    Q_D(const DuiDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->defaultFont;
}

const QFont &DuiDefaultFonts::largeFont() const
{
    Q_D(const DuiDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->largeFont;
}

const QFont &DuiDefaultFonts::extraLargeFont() const
{
    Q_D(const DuiDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->extraLargeFont;
}

