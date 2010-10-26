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

#include "mdefaultfonts.h"
#include "mdefaultfonts_p.h"
#include "mlogicalvalues.h"

MDefaultFontsPrivate::MDefaultFontsPrivate(const MLogicalValues &values) : values(values), refresh(true)
{
}

void MDefaultFontsPrivate::reloadAllValues() const
{
    const QByteArray FONT_GROUP_NAME = "Fonts";

    extraSmallFont = values.font(FONT_GROUP_NAME, "FONT_XSMALL");
    smallFont = values.font(FONT_GROUP_NAME, "FONT_SMALL");
    defaultFont = values.font(FONT_GROUP_NAME, "FONT_DEFAULT");
    largeFont = values.font(FONT_GROUP_NAME, "FONT_LARGE");
    extraLargeFont = values.font(FONT_GROUP_NAME, "FONT_XLARGE");
    refresh = false;
}

MDefaultFonts::MDefaultFonts(const MLogicalValues &values) :
    d_ptr(new MDefaultFontsPrivate(values))
{
}

MDefaultFonts::~MDefaultFonts()
{
    delete d_ptr;
}

void MDefaultFonts::refresh()
{
    // TODO: Fix cyclic dependency between theming/styling & MdeviceProfile (MDeviceProfileStyle)
    // We can't load values directly because we need MDeviceProfile for dpi conversions but it may not exist yet.
    Q_D(MDefaultFonts);
    d->refresh = true;
}

const QFont &MDefaultFonts::extraSmallFont() const
{
    Q_D(const MDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->extraSmallFont;
}

const QFont &MDefaultFonts::smallFont() const
{
    Q_D(const MDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->smallFont;
}

const QFont &MDefaultFonts::defaultFont() const
{
    Q_D(const MDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->defaultFont;
}

const QFont &MDefaultFonts::largeFont() const
{
    Q_D(const MDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->largeFont;
}

const QFont &MDefaultFonts::extraLargeFont() const
{
    Q_D(const MDefaultFonts);
    if (d->refresh)
        d->reloadAllValues();
    return d->extraLargeFont;
}

