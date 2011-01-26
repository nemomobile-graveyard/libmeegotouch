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

#include <mscenemanager.h>
#include "mviewcreator.h"
#include "mscenewindow.h"
#include "mscenewindow_p.h"
#include "mscenewindowview.h"
#include "mscenewindowview_p.h"
#include "mclassfactory.h"

MSceneWindowViewPrivate::MSceneWindowViewPrivate() :
    controller(0)
{
}

MSceneWindowViewPrivate::~MSceneWindowViewPrivate()
{
}

void MSceneWindowViewPrivate::_q_playSceneWindowAppearingFeedback()
{
    Q_Q(MSceneWindowView);

    // Play a feedback when scene window starts to appear
    q->style()->appearFeedback().play();
}

MSceneWindowView::MSceneWindowView(MSceneWindow *controller) :
    MWidgetView(*new MSceneWindowViewPrivate, controller)
{
    Q_D(MSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
    connect(controller, SIGNAL(appearing()), this, SLOT(_q_playSceneWindowAppearingFeedback()));
}

MSceneWindowView::MSceneWindowView(MSceneWindowViewPrivate &dd, MSceneWindow *controller) :
    MWidgetView(dd, controller)
{
    Q_D(MSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
    connect(controller, SIGNAL(appearing()), this, SLOT(_q_playSceneWindowAppearingFeedback()));
}

MSceneWindowView::~MSceneWindowView()
{
}

void MSceneWindowView::applyStyle()
{
    Q_D(MSceneWindowView);

    /*!
     *  Setuping disapearTimeout, by default= 0
     */
    model()->setDisappearTimeout(style()->disappearTimeout());
    MWidgetView::applyStyle();

    // smuggle those style properties to our controller so that MSceneManager can use them.
    d->controller->setProperty("_m_appearanceAnimation", style()->appearanceAnimation());
    d->controller->setProperty("_m_disappearanceAnimation", style()->disappearanceAnimation());

    emit geometryAttributesChanged();
}

void MSceneWindowView::setupModel()
{
    MWidgetView::setupModel();
    emit geometryAttributesChanged();
}


void MSceneWindowView::notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    Q_D(MSceneWindowView);

    if (change == QGraphicsItem::ItemParentHasChanged) {
        // Do not notify MWidgetView that parent has changed, thus the styles of scene window
        // and all its children won't be reloaded.
        return;
    } else if (change == QGraphicsItem::ItemSceneHasChanged) {
        if (value.isValid()) {
            // new scene might mean new orientation. reapply styles.
            QVariant variantParentItem;
            variantParentItem.setValue<QGraphicsItem*>(d->controller->parentItem());
            MWidgetView::notifyItemChange(QGraphicsItem::ItemParentHasChanged,
                             variantParentItem);
        }
    }
    MWidgetView::notifyItemChange(change, value);
}

Qt::Alignment MSceneWindowView::alignment() const
{
    Qt::Alignment verticalAlign = style()->verticalAlign() & Qt::AlignVertical_Mask;
    Qt::Alignment horizontalAlign = style()->horizontalAlign() & Qt::AlignHorizontal_Mask;

    return verticalAlign | horizontalAlign;
}

QPointF MSceneWindowView::offset() const
{
    return style()->offset();
}

M_REGISTER_VIEW_NEW(MSceneWindowView, MSceneWindow)

#include "moc_mscenewindowview.cpp"
