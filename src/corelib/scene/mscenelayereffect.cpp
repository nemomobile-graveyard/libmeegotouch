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

#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include "mscenelayereffect.h"
#include "mscenelayereffect_p.h"
#include "mscenelayereffectmodel.h"
#include "mwidgetfadeanimation.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(MSceneLayerEffect)

MSceneLayerEffectPrivate::MSceneLayerEffectPrivate()
{
    layerPressedDirectly = false;

    MWidgetFadeAnimation *fadeInAnimation = new MWidgetFadeAnimation;
    fadeInAnimation->setTransitionDirection(MWidgetFadeAnimation::In);

    MWidgetFadeAnimation *fadeOutAnimation = new MWidgetFadeAnimation;
    fadeOutAnimation->setTransitionDirection(MWidgetFadeAnimation::Out);

    appearanceAnimation = fadeInAnimation;
    disappearanceAnimation = fadeOutAnimation;
}

MSceneLayerEffectPrivate::~MSceneLayerEffectPrivate()
{
}

MSceneLayerEffect::MSceneLayerEffect(const QString &effectType)
    : MSceneWindow(new MSceneLayerEffectPrivate(),
                     new MSceneLayerEffectModel(), MSceneWindow::LayerEffect,
                     effectType)
{
    Q_D(MSceneLayerEffect);
    setFlag(QGraphicsItem::ItemStopsClickFocusPropagation, true);

    d->appearanceAnimation->setTargetWidget(this);
    d->appearanceAnimation->setParent(this);
    d->disappearanceAnimation->setTargetWidget(this);
    d->disappearanceAnimation->setParent(this);
}

MSceneLayerEffect::~MSceneLayerEffect()
{
}

void MSceneLayerEffect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSceneLayerEffect);
    event->accept();

    if (scene()->itemAt(event->scenePos()) == this) {
        // The layer was pressed directly
        d->layerPressedDirectly = true;
    } else {
        // The layer wasn't pressed. We got the event just because
        // our child scene window didn't accept it.
        d->layerPressedDirectly = false;
    }
}

void MSceneLayerEffect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MSceneLayerEffect);
    QList<QGraphicsItem *> itemList = parentItem()->childItems(); // get our siblings
    QGraphicsItem *item;

    event->accept();

    if (d->layerPressedDirectly) {
        // reset value
        d->layerPressedDirectly = false;
        if (scene()->itemAt(event->scenePos()) == this) {
            // Mouse was released on top of the layer effect. That constitutes a
            // proper click event.
            // Let's dismiss our associated scene window.
            foreach(item, itemList) {
                if (item != this && item->isWidget()) {
                    QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);
                    MSceneWindow *sceneWindow = qobject_cast<MSceneWindow *>(widget);
                    if (sceneWindow) {
                        sceneWindow->dismiss();
                    } else {
                        widget->close();
                    }
                }
            }
            //By closing the associated scene window, this may in turn be delete this,
            //so we can no longer rely on d_ptr from this point on.
        }
    }
}

void MSceneLayerEffect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}
