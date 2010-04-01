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

#include "duiwidgetanimation.h"

#if QT_VERSION >= 0x040600

#include "duiwidgetanimation_p.h"

#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QGraphicsWidget>

DuiWidgetAnimationPrivate::DuiWidgetAnimationPrivate()
    : duration(500),
      easingCurve()
{
}

DuiWidgetAnimationPrivate::~DuiWidgetAnimationPrivate()
{
}

QPropertyAnimation *DuiWidgetAnimationPrivate::getAnimation(QGraphicsWidget *widget, const QString &property)
{
    Q_Q(DuiWidgetAnimation);

    if (!animations.contains(widget))
        animations.insert(widget, Animations());

    Animations &animationList = animations[widget];
    QPropertyAnimation *animation = animationList.value(property, NULL);
    if (animation) {
        return animation;
    } else {
        QPropertyAnimation *a = new QPropertyAnimation();
        a->setTargetObject(widget);
        a->setPropertyName(property.toAscii());
        a->setDuration(duration);
        a->setEasingCurve(easingCurve);
        animationList.insert(property, a);
        q->group()->addAnimation(a);
        return a;
    }
}

void DuiWidgetAnimationPrivate::setTargetForAllWidgets(const QString &property, const QVariant &value)
{
    QMapIterator<QGraphicsWidget *, Animations> i(animations);
    while (i.hasNext()) {
        QGraphicsWidget  *widget = i.key();
        QPropertyAnimation *a = getAnimation(widget, property);
        if (a) {
            a->setStartValue(widget->property(property.toAscii()));
            a->setEndValue(value);
        }
    }
}

// protected constructor
DuiWidgetAnimation::DuiWidgetAnimation(DuiWidgetAnimationPrivate *dd, QObject *parent) :
    DuiGroupAnimation(dd, DuiGroupAnimation::Parallel, parent)
{
}

// public constructor
DuiWidgetAnimation::DuiWidgetAnimation(QObject *parent) :
    DuiGroupAnimation(new DuiWidgetAnimationPrivate, DuiGroupAnimation::Parallel, parent)
{
}

// destructor
DuiWidgetAnimation::~DuiWidgetAnimation()
{
}

void DuiWidgetAnimation::addWidget(QGraphicsWidget *widget)
{
    Q_D(DuiWidgetAnimation);

    d->animations.insert(widget, Animations());
}

void DuiWidgetAnimation::removeWidget(QGraphicsWidget *widget)
{
    Q_D(DuiWidgetAnimation);

    if (d->animations.contains(widget)) {
        Animations &animationList = d->animations[widget];

        foreach(QPropertyAnimation * animation, animationList) {
            group()->removeAnimation(animation);
            delete animation;
        }

        d->animations.remove(widget);
    }
}

void DuiWidgetAnimation::setDuration(int msecs, const QString &property)
{
    Q_D(DuiWidgetAnimation);

    if (property.isEmpty())
        d->duration = msecs;

    foreach(const Animations & list, d->animations) {

        if (property.isEmpty()) {
            foreach(QPropertyAnimation * a, list) {
                a->setDuration(msecs);
            }
        } else {
            QPropertyAnimation *a = list.value(property, NULL);
            if (a) {
                a->setDuration(msecs);
            }
        }
    }
}

void DuiWidgetAnimation::setEasingCurve(const QEasingCurve &curve, const QString &property)
{
    Q_D(DuiWidgetAnimation);

    if (property.isEmpty())
        d->easingCurve = curve;

    foreach(const Animations & list, d->animations) {

        if (property.isEmpty()) {
            foreach(QPropertyAnimation * a, list) {
                a->setEasingCurve(curve);
            }
        } else {
            QPropertyAnimation *a = list.value(property, NULL);
            if (a) {
                a->setEasingCurve(curve);
            }
        }
    }
}

//void setSourceValue(const QString& property, const QVariant& value);
//void enableAnimation(const QString& property, bool enable);

void DuiWidgetAnimation::setTargetValue(QGraphicsWidget *widget, const QString &property, const QVariant &value)
{
    Q_D(DuiWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets(property, value);
    else {
        d->getAnimation(widget, property)->setStartValue(widget->property(property.toAscii()));
        d->getAnimation(widget, property)->setEndValue(value);
    }
}

void DuiWidgetAnimation::setKeyFrameAt(QGraphicsWidget *widget, const QString &property, qreal step, const QVariant &value)
{
    Q_D(DuiWidgetAnimation);
    d->getAnimation(widget, property)->setKeyValueAt(step, value);
}

void DuiWidgetAnimation::setKeyFrames(QGraphicsWidget *widget, const QString &property, const QVector<QVariant>& values)
{
    const int valuesSize = values.size();
    Q_D(DuiWidgetAnimation);
    if (valuesSize == 0)
        return;
    else if (valuesSize == 1) {
        d->getAnimation(widget, property)->setKeyValueAt(1.0, values.last());
    }

    QPropertyAnimation *a = d->getAnimation(widget, property);
    const qreal step = 1.0 / (valuesSize - 1);
    for (int i = 0; i < valuesSize - 1; ++i) {
        a->setKeyValueAt(i * step, values[i]);
    }
    a->setKeyValueAt(1.0, values.last());
}

void DuiWidgetAnimation::setKeyFrames(QGraphicsWidget *widget, const QString &property, const QVector< QPair<qreal, QVariant> >& values)
{
    Q_D(DuiWidgetAnimation);
    d->getAnimation(widget, property)->setKeyValues(values);
}

void DuiWidgetAnimation::setTargetGeometry(QGraphicsWidget *widget, const QRectF &target)
{
    Q_D(DuiWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("geometry", target);
    else {
        d->getAnimation(widget, "geometry")->setStartValue(widget->geometry());
        d->getAnimation(widget, "geometry")->setEndValue(target);
    }
}

void DuiWidgetAnimation::setTargetOpacity(QGraphicsWidget *widget, qreal target)
{
    Q_D(DuiWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("opacity", target);
    else {
        d->getAnimation(widget, "opacity")->setStartValue(widget->opacity());
        d->getAnimation(widget, "opacity")->setEndValue(target);
    }
}

void DuiWidgetAnimation::setTargetSize(QGraphicsWidget *widget, const QSizeF &target)
{
    Q_D(DuiWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("size", target);
    else {
        d->getAnimation(widget, "size")->setStartValue(widget->size());
        d->getAnimation(widget, "size")->setEndValue(target);
    }
}

void DuiWidgetAnimation::setTargetPosition(QGraphicsWidget *widget, const QPointF &target)
{
    Q_D(DuiWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("pos", target);
    else {
        d->getAnimation(widget, "pos")->setStartValue(widget->pos());
        d->getAnimation(widget, "pos")->setEndValue(target);
    }
}
#endif

