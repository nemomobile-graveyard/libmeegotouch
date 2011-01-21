/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mscenelayereffectdimview.h"

#include "mscenewindowview_p.h"
#include "mviewcreator.h"
#include "mscenelayereffect.h"
#include "mscenelayereffectmodel.h"
#include <mdeviceprofile.h>
#include "mscenemanager.h"

//! \internal
class MSceneLayerEffectDimViewPrivate : public MSceneWindowViewPrivate
{
public:
    MSceneLayerEffect *controller;
};
//! \internal_end

MSceneLayerEffectDimView::MSceneLayerEffectDimView(MSceneLayerEffect *controller) :
    MSceneWindowView(*new MSceneLayerEffectDimViewPrivate, controller)
{
    Q_D(MSceneLayerEffectDimView);
    d->controller = controller;

    d->controller->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
}

MSceneLayerEffectDimView::~MSceneLayerEffectDimView()
{
}

void MSceneLayerEffectDimView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(MSceneLayerEffectDimView);
    Q_UNUSED(widget);
    Q_UNUSED(option);

    qreal oldOpacity = painter->opacity();
    qreal opacity = d_ptr->controller->effectiveOpacity() * style()->opacity();

    QTransform oldTransform = painter->transform();
    QTransform transform;
    M::Orientation layerEffectOrientation = M::Landscape;

    QRectF geometry = d->controller->geometry();
    if (geometry.height() > geometry.width()) {
        layerEffectOrientation = M::Portrait;
    }

    if (layerEffectOrientation != MDeviceProfile::instance()->orientationFromAngle(M::Angle0)) {
        // layer effect's orientation is different than display's native orientation.
        // Therefore we have to place our layer effect sideways.
        transform.translate(0, geometry.width());
        transform.rotate(-90);
    }

    painter->setTransform(transform);
    painter->setOpacity(opacity);
    painter->fillRect(boundingRect(), QColor(0, 0, 0));
    painter->setOpacity(oldOpacity);

    painter->setTransform(oldTransform);
}

QRectF MSceneLayerEffectDimView::boundingRect() const
{
    Q_D(const MSceneLayerEffectDimView);
    return QRectF(QPointF(0, 0), d->controller->size());
}

void MSceneLayerEffectDimView::applyStyle()
{
    MSceneWindowView::applyStyle();
}

M_REGISTER_VIEW_NEW(MSceneLayerEffectDimView, MSceneLayerEffect)
