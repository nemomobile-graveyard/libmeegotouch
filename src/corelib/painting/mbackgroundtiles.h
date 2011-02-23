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

  All 16 pieces are fetched from MTheme with different suffixes. For example if you use imageId 'bg'
  this class will fetch the following images corresponding to M::Position:
  \li M::DefaultPosition => 'bg'

  \li M::TopLeftPosition => 'bg-top-left'
  \li M::TopCenterPosition => 'bg-top-center'
  \li M::TopRightPosition => 'bg-top-right'
  \li M::CenterLeftPosition => 'bg-center-left'
  \li M::CenterPosition => 'bg-center'
  \li M::CenterRightPosition => 'bg-center-right'
  \li M::BottomLeftPosition => 'bg-bottom-left'
  \li M::BottomCenterPosition => 'bg-bottom-center'
  \li M::BottomRightPosition => 'bg-bottom-right'

  \li M::HorizontalLeftPosition => 'bg-horizontal-left'
  \li M::HorizontalCenterPosition => 'bg-horizontal-center'
  \li M::HorizontalRightPosition => 'bg-horizontal-right'

  \li M::VerticalTopPosition => 'bg-vertical-top'
  \li M::VerticalCenterPosition => 'bg-vertical-center'
  \li M::VerticalBottomPosition => 'bg-vertical-bottom'

  This class is mainly meant to be used with the style system, so that the imageId can be given from css,
  and the M::Position from application code. A working example is the background of any widget.
  MWidgetStyle::backgroundTiles defines the id and the margins while MWidgetController::setLayoutPosition can be
  used for defining which tile is the one to be rendered. Some layout policies have this functionality to automatically
  set the correct position for each widget, e.g. MLinearLayoutPolicy::setNotifyWidgetsOfLayoutPositionEnabled.

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
