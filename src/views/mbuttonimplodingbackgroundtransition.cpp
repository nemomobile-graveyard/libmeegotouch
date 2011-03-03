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


#include "mbuttonimplodingbackgroundtransition.h"
#include "mdebug.h"
#include "mbutton.h"
#include "mbuttonview.h"
#include "mbuttonview_p.h"

#include <QPainter>

static const int forward_duration = 50;
static const int backward_duration = 150;

MButtonImplodingBackgroundTransition::MButtonImplodingBackgroundTransition(MButtonStyleContainer& s,
                                                                           MButtonModel* m,
                                                                           MWidgetController* c,
                                                                           MButtonViewPrivate* p)
    :style(s),
     model(m),
     controller(c),
     priv(p),
     timeline(forward_duration),
     scale(1.0),
     opacity(1.0),
     inScaleCurve(QEasingCurve::OutCubic),
     inOpacityCurve(QEasingCurve::OutCubic),
     outScaleCurve(QEasingCurve::InExpo),
     outOpacityCurve(QEasingCurve::InExpo),
     released(false)
{
    connect(&timeline, SIGNAL(valueChanged(qreal)), SLOT(valueChanged(qreal)));
    connect(&timeline, SIGNAL(finished()), SLOT(finished()));
}

MButtonImplodingBackgroundTransition::~MButtonImplodingBackgroundTransition()
{
}

void MButtonImplodingBackgroundTransition::onPress()
{
    if (controller->isEnabled()) {
        released = false;
        timeline.setDirection(QTimeLine::Forward);
        if (timeline.state() == QTimeLine::Running) {
            timeline.stop();
            timeline.setCurrentTime(0);
        }
        opacity = 0.0f;
        scale = 0.75f;
        timeline.setDuration(forward_duration);
        timeline.start();
        style.setModePressed();
        priv->updateItemsAfterModeChange();
    }
}

void MButtonImplodingBackgroundTransition::onRelease()
{
    if (controller->isEnabled()) {
        if (timeline.state() == QTimeLine::Running) {
            released = true;
        } else {
            timeline.setDirection(QTimeLine::Backward);
            timeline.setDuration(backward_duration);
            timeline.start();
            released = false;
        }
    }
}

void MButtonImplodingBackgroundTransition::onCancel()
{
    if (controller->isEnabled()) {
        opacity = 1.0f;
        scale = 1.0f;
        if (timeline.state() == QTimeLine::Running) {
            timeline.stop();
        }
        if (model->checked())
            style.setModeSelected();
        else
            style.setModeDefault();
        priv->updateItemsAfterModeChange();
    }
}

void MButtonImplodingBackgroundTransition::modifyBackgroundPainter(QPainter *painter) const
{
    qreal w2 = (controller->size().width() - (style->marginLeft()+style->marginRight()))/2.0;
    qreal h2 = (controller->size().height() - (style->marginTop()+style->marginBottom()))/2.0;

    painter->translate(w2, h2);
    painter->scale(scale,scale);
    painter->translate(-w2, -h2);

    qreal op = painter->opacity();
    painter->setOpacity(op*opacity);
}

void MButtonImplodingBackgroundTransition::refreshStyle()
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

void MButtonImplodingBackgroundTransition::valueChanged(qreal value)
{
    if (timeline.direction() == QTimeLine::Forward) {
        // animating released -> pressed
        // opacity 25ms 0 => 1
        // scale 50ms 75% => 100%
        if (value <= 0.5f)
            opacity = inOpacityCurve.valueForProgress(value*2.0f);
        else
            opacity = 1.0f;

        scale = 0.75f + inScaleCurve.valueForProgress(value)*0.25f;
    } else {
        // animating pressed -> released
        // opacity 150ms 1 => 0
        // scale 150ms 100% => 75%
        opacity = 1.0f - outOpacityCurve.valueForProgress(1.0f-value);
        scale = 1.0f - outScaleCurve.valueForProgress(1.0f-value)*0.25f;
    }
    controller->update();
}

void MButtonImplodingBackgroundTransition::finished()
{
    if (released) {
        timeline.setDirection(QTimeLine::Backward);
        timeline.setDuration(backward_duration);
        timeline.start();
        released = false;
        return;
    }

    if (timeline.direction() == QTimeLine::Backward && controller->isEnabled()) {
        opacity = 1.0f;
        scale = 1.0f;
        if (model->checked())
            style.setModeSelected();
        else
            style.setModeDefault();
        priv->updateItemsAfterModeChange();
        controller->update();
    }
}
