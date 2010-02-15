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

#include <QPropertyAnimation>

#include "duiscenelayereffectdimview.h"

#include "duiscenewindowview_p.h"
#include "duiviewcreator.h"
#include "duiscenelayereffect.h"
#include "duiscenelayereffectmodel.h"

//! \internal
class DuiSceneLayerEffectDimViewPrivate : public DuiSceneWindowViewPrivate
{
public:
    DuiSceneLayerEffect *controller;
    QPropertyAnimation *animation;
};
//! \internal_end

DuiSceneLayerEffectDimView::DuiSceneLayerEffectDimView(DuiSceneLayerEffect *controller) :
    DuiSceneWindowView(*new DuiSceneLayerEffectDimViewPrivate, controller)
{
    Q_D(DuiSceneLayerEffectDimView);
    d->controller = controller;
    d->controller->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);

    d->animation = new QPropertyAnimation(d->controller, "opacity", this);
    d->animation->setStartValue(0.0f);
}

DuiSceneLayerEffectDimView::~DuiSceneLayerEffectDimView()
{
}

void DuiSceneLayerEffectDimView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->fillRect(boundingRect(), QColor(0, 0, 0));
}

QRectF DuiSceneLayerEffectDimView::boundingRect() const
{
    Q_D(const DuiSceneLayerEffectDimView);
    return QRectF(QPointF(0, 0), d->controller->size());
}

void DuiSceneLayerEffectDimView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiSceneLayerEffectDimView);

    DuiSceneWindowView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiSceneLayerEffectModel::Enabled) {
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

DUI_REGISTER_VIEW_NEW(DuiSceneLayerEffectDimView, DuiSceneLayerEffect)
