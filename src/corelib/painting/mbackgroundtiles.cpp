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

#include "mbackgroundtiles.h"
#include "mbackgroundtiles_p.h"
#include "mscalableimage.h"

#include <mtheme.h>

const QString TILE_POSTFIX[16] = {
    "",

    "-top-left",     "-top-center",    "-top-right",
    "-center-left",  "-center",       "-center-right",
    "-bottom-left",  "-bottom-center", "-bottom-right",

    "-horizontal-left", "-horizontal-center", "-horizontal-right",

    "-vertical-top", "-vertical-center", "-vertical-bottom"
};

MBackgroundTilesPrivate::MBackgroundTilesPrivate() :
    refCount(1),
    left(0),
    right(0),
    top(0),
    bottom(0)
{
    for(int i=0; i<16; ++i) {
        tiles[i] = 0;
    }
}

MBackgroundTilesPrivate::~MBackgroundTilesPrivate()
{
    for(int i=0; i!=16; ++i) {
        MTheme::releaseScalableImage(tiles[i]);
    }
}


MBackgroundTiles::MBackgroundTiles() :
    d_ptr(0)
{
    // constructs an invalid object
}

MBackgroundTiles::MBackgroundTiles(const MBackgroundTiles& other) :
    d_ptr(other.d_ptr)
{
    if(d_ptr)
        ++(d_ptr->refCount);
}

MBackgroundTiles::MBackgroundTiles(const QString& imageId, int left, int right, int top, int bottom) :
    d_ptr(new MBackgroundTilesPrivate)
{
    Q_D(MBackgroundTiles);
    d->imageId = imageId;
    d->left = left;
    d->right = right;
    d->top = top;
    d->bottom = bottom;
}

MBackgroundTiles::~MBackgroundTiles()
{
    if(d_ptr) {
        --(d_ptr->refCount);

        if(d_ptr->refCount == 0)
            delete d_ptr;
    }
}

const MScalableImage* MBackgroundTiles::operator [] (M::Position tile) const
{
    Q_D(const MBackgroundTiles);
    if(!d->tiles[tile]) {
        const_cast<MBackgroundTilesPrivate*&>(d)->tiles[tile] =
            MTheme::scalableImage(d->imageId+TILE_POSTFIX[tile], d->left, d->right, d->top, d->bottom);
    }
    return d->tiles[tile];
}

bool MBackgroundTiles::operator == (const MBackgroundTiles& other) const
{
    Q_D(const MBackgroundTiles);
    if(d == other.d_ptr)
        return true;

    if(d == NULL || other.d_ptr == NULL)
        return false;

    for(int i=0; i!=16; ++i) {
        if(d->tiles[i] != other.d_ptr->tiles[i])
            return false;
    }

    return true;
}

MBackgroundTiles& MBackgroundTiles::operator = (const MBackgroundTiles& other)
{
    if(this == &other)
        return *this;

    if(d_ptr) {
        --(d_ptr->refCount);
        if(d_ptr->refCount == 0)
            delete d_ptr;
    }

    d_ptr = other.d_ptr;
    if(d_ptr) {
        ++(d_ptr->refCount);
    }

    return *this;
}

bool MBackgroundTiles::isValid() const
{
    return d_ptr != NULL;
}

