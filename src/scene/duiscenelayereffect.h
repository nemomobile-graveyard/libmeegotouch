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

// Make doxygen skip this internal class
//! \cond
#ifndef DUISCENELAYEREFFECT_H
#define DUISCENELAYEREFFECT_H

#include "duiscenewindow.h"
#include "duiscenelayereffectmodel.h"

class DuiSceneLayerEffectPrivate;

class DuiSceneLayerEffect : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiSceneLayerEffect)

public:

    DuiSceneLayerEffect(const QString &effectType);
    virtual ~DuiSceneLayerEffect();

public Q_SLOTS:
    void enableEffect();
    void disableEffect();

protected:

    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(DuiSceneLayerEffect)
};

#endif
//! \endcond
