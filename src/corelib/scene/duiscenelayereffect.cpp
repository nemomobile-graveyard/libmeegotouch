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

#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include "duiscenelayereffect.h"
#include "duiscenelayereffect_p.h"
#include "duiscenelayereffectmodel.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET_NO_CREATE(DuiSceneLayerEffect)

DuiSceneLayerEffectPrivate::DuiSceneLayerEffectPrivate()
{
    layerPressedDirectly = false;
}

DuiSceneLayerEffectPrivate::~DuiSceneLayerEffectPrivate()
{
}

DuiSceneLayerEffect::DuiSceneLayerEffect(const QString &effectType)
    : DuiSceneWindow(new DuiSceneLayerEffectPrivate(),
                     new DuiSceneLayerEffectModel(), DuiSceneWindow::LayerEffect,
                     effectType)
{
}

DuiSceneLayerEffect::~DuiSceneLayerEffect()
{
}

void DuiSceneLayerEffect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiSceneLayerEffect);
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

void DuiSceneLayerEffect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiSceneLayerEffect);
    QList<QGraphicsItem *> itemList = childItems();
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
                if (item->isWidget()) {
                    QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);
                    DuiSceneWindow *sceneWindow = qobject_cast<DuiSceneWindow *>(widget);
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

void DuiSceneLayerEffect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}

void DuiSceneLayerEffect::enableEffect()
{
    model()->setEnabled(true);
}

void DuiSceneLayerEffect::disableEffect()
{
    model()->setEnabled(false);
}
