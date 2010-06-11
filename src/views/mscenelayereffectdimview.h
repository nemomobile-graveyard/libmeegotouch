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

#ifndef MSCENELAYEREFFECTDIMVIEW_H
#define MSCENELAYEREFFECTDIMVIEW_H

#include <mscenewindowview.h>
#include <mscenelayereffectmodel.h>
#include <mscenelayereffectdimstyle.h>

class MSceneLayerEffect;
class MSceneLayerEffectDimViewPrivate;

//! \internal
class MSceneLayerEffectDimView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneLayerEffectModel, MSceneLayerEffectDimStyle)

public:
    MSceneLayerEffectDimView(MSceneLayerEffect *controller);
    virtual ~MSceneLayerEffectDimView();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual QRectF boundingRect() const;

protected Q_SLOTS:
    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MSceneLayerEffectDimView)
};
//! \internal_end

#endif
