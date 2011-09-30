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
#include "mbuttonexpandingbackgroundtransition.h"
#include "mdebug.h"
#include "mbutton.h"
#include "mbuttonview.h"
#include "mbuttonview_p.h"

#include <QPainter>

MButtonExpandingBackgroundTransition::MButtonExpandingBackgroundTransition(MButtonStyleContainer& s,
                                                                           MButtonModel* m,
                                                                           MWidgetController* c,
                                                                           MButtonViewPrivate* p)
    :style(s),
     model(m),
     controller(c),
     priv(p),
     timeline(200),
     scale(1.0),
     opacity(1.0),
     scaleCurve(QEasingCurve::OutExpo),
     opacityCurve(QEasingCurve::InQuint)
{
    connect(&timeline, SIGNAL(valueChanged(qreal)), SLOT(valueChanged(qreal)));
    connect(&timeline, SIGNAL(finished()), SLOT(finished()));
}

MButtonExpandingBackgroundTransition::~MButtonExpandingBackgroundTransition()
{
}

void MButtonExpandingBackgroundTransition::onPress()
{
    if (controller->isEnabled()) {
        if (timeline.state() == QTimeLine::Running) {
            timeline.stop();
            timeline.setCurrentTime(0);
        }
        style.setModePressed();
        priv->updateItemsAfterModeChange();
    }
}

void MButtonExpandingBackgroundTransition::onRelease()
{
    if (controller->isEnabled()) {
        timeline.start();
    }
}

void MButtonExpandingBackgroundTransition::onCancel()
{
    if (controller->isEnabled()) {
        if (model->checked())
            style.setModeSelected();
        else
            style.setModeDefault();
        priv->updateItemsAfterModeChange();
    }
}

void MButtonExpandingBackgroundTransition::modifyBackgroundPainter(QPainter *painter) const
{
    qreal w2 = (controller->size().width() - (style->marginLeft()+style->marginRight()))/2.0;
    qreal h2 = (controller->size().height() - (style->marginTop()+style->marginBottom()))/2.0;

    painter->translate(w2, h2);
    painter->scale(scale,scale);
    painter->translate(-w2, -h2);

    qreal op = painter->opacity();
    painter->setOpacity(op*opacity);
}

void MButtonExpandingBackgroundTransition::refreshStyle()
{
    if (controller->isEnabled()) {
        if (model->down()) {
            style.setModePressed();
        } else if (model->checked()) {
            style.setModeSelected();
        } else {
            style.setModeDefault();
        }
    }
    priv->updateItemsAfterModeChange();
}

void MButtonExpandingBackgroundTransition::valueChanged(qreal value)
{
    // opacity 200ms 1 => 0
    // scale 350ms 100% => 125%
    opacity = 1.0f - opacityCurve.valueForProgress(value);

    value *= 0.57142f; // 200.0/350.0
    scale = 1.0f + scaleCurve.valueForProgress(value)*0.25f;
    controller->update();
}

void MButtonExpandingBackgroundTransition::finished()
{
    if (controller->isEnabled()) {
        if (model->checked())
            style.setModeSelected();
        else
            style.setModeDefault();
        timeline.setCurrentTime(0);
        priv->updateItemsAfterModeChange();
        controller->update();
    }
}
