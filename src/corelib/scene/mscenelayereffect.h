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

#ifndef MSCENELAYEREFFECT_H
#define MSCENELAYEREFFECT_H

#include "mscenewindow.h"
#include "mscenelayereffectmodel.h"

class MSceneLayerEffectPrivate;

//! \internal
class MSceneLayerEffect : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MSceneLayerEffect)

public:

    MSceneLayerEffect(const QString &effectType);
    virtual ~MSceneLayerEffect();

protected:

    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MSceneLayerEffect)
};

//! \internal_end
#endif
