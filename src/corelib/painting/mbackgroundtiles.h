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

#ifndef MBACKGROUNDTILES_H
#define MBACKGROUNDTILES_H

#include <mexport.h>
#include <mnamespace.h>

class MScalableImage;
class MBackgroundTilesPrivate;

/*!
  \class MBackgroundTiles
  \brief This class holds 16 pieces of scalable images which represent every possible position in a layout

  M::Position documents all the variations.
  \sa M::Position
*/
class M_CORE_EXPORT MBackgroundTiles
{
public:
    MBackgroundTiles();
    MBackgroundTiles(const MBackgroundTiles& other);
    MBackgroundTiles(const QString& imageId, int left, int right, int top, int bottom);
    virtual ~MBackgroundTiles();

    const MScalableImage* operator [] (M::Position tile) const;

    bool operator == (const MBackgroundTiles& other) const;
    MBackgroundTiles& operator = (const MBackgroundTiles& other);

    bool isValid() const;

protected:
    MBackgroundTilesPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(MBackgroundTiles)
};

Q_DECLARE_METATYPE(MBackgroundTiles)

#endif
