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
#ifndef DUISCENELAYEREFFECTDIMVIEW_H
#define DUISCENELAYEREFFECTDIMVIEW_H

#include <views/duiscenewindowview.h>
#include <duiscenelayereffectmodel.h>
#include <duiscenelayereffectdimstyle.h>

class DuiSceneLayerEffect;
class DuiSceneLayerEffectDimViewPrivate;

class DuiSceneLayerEffectDimView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiSceneLayerEffectModel, DuiSceneLayerEffectDimStyle)

public:
    DuiSceneLayerEffectDimView(DuiSceneLayerEffect *controller);
    virtual ~DuiSceneLayerEffectDimView();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual QRectF boundingRect() const;

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(DuiSceneLayerEffectDimView)
};

#endif
//! \endcond
