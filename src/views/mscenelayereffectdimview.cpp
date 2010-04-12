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

#include <QPropertyAnimation>

#include "mscenelayereffectdimview.h"

#include "mscenewindowview_p.h"
#include "mviewcreator.h"
#include "mscenelayereffect.h"
#include "mscenelayereffectmodel.h"

//! \internal
class MSceneLayerEffectDimViewPrivate : public MSceneWindowViewPrivate
{
public:
    MSceneLayerEffect *controller;
    QPropertyAnimation *animation;
};
//! \internal_end

MSceneLayerEffectDimView::MSceneLayerEffectDimView(MSceneLayerEffect *controller) :
    MSceneWindowView(*new MSceneLayerEffectDimViewPrivate, controller)
{
    Q_D(MSceneLayerEffectDimView);
    d->controller = controller;
    d->controller->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);

    d->animation = new QPropertyAnimation(d->controller, "opacity", this);
    d->animation->setStartValue(0.0f);
}

MSceneLayerEffectDimView::~MSceneLayerEffectDimView()
{
}

void MSceneLayerEffectDimView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->fillRect(boundingRect(), QColor(0, 0, 0));
}

QRectF MSceneLayerEffectDimView::boundingRect() const
{
    Q_D(const MSceneLayerEffectDimView);
    return QRectF(QPointF(0, 0), d->controller->size());
}

void MSceneLayerEffectDimView::updateData(const QList<const char *>& modifications)
{
    Q_D(MSceneLayerEffectDimView);

    MSceneWindowView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MSceneLayerEffectModel::Enabled) {
            if (model()->enabled()) {
                d->animation->setDuration(style()->fadeDuration());
                d->animation->setEasingCurve(style()->easingCurve());
                d->animation->setEndValue(style()->opacity());
                d->animation->start();
            } else {
                d->animation->setStartValue(d->controller->opacity());
                d->animation->setEndValue(0.0f);
                d->animation->start();
            }
        }
    }
}

M_REGISTER_VIEW_NEW(MSceneLayerEffectDimView, MSceneLayerEffect)
