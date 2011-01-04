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

#include "mwidgetanimation.h"

#if QT_VERSION >= 0x040600

#include "mwidgetanimation_p.h"

#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QGraphicsWidget>

MWidgetAnimationPrivate::MWidgetAnimationPrivate()
    : duration(500),
      easingCurve()
{
}

MWidgetAnimationPrivate::~MWidgetAnimationPrivate()
{
}

QPropertyAnimation *MWidgetAnimationPrivate::getAnimation(QGraphicsWidget *widget, const QString &property)
{
    Q_Q(MWidgetAnimation);

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

void MWidgetAnimationPrivate::setTargetForAllWidgets(const QString &property, const QVariant &value)
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
MWidgetAnimation::MWidgetAnimation(MWidgetAnimationPrivate *dd, QObject *parent) :
    MGroupAnimation(dd, MGroupAnimation::Parallel, parent)
{
}

// public constructor
MWidgetAnimation::MWidgetAnimation(QObject *parent) :
    MGroupAnimation(new MWidgetAnimationPrivate, MGroupAnimation::Parallel, parent)
{
}

// destructor
MWidgetAnimation::~MWidgetAnimation()
{
}

void MWidgetAnimation::addWidget(QGraphicsWidget *widget)
{
    Q_D(MWidgetAnimation);

    d->animations.insert(widget, Animations());
}

void MWidgetAnimation::removeWidget(QGraphicsWidget *widget)
{
    Q_D(MWidgetAnimation);

    if (d->animations.contains(widget)) {
        Animations &animationList = d->animations[widget];

        foreach(QPropertyAnimation * animation, animationList) {
            group()->removeAnimation(animation);
            delete animation;
        }

        d->animations.remove(widget);
    }
}

void MWidgetAnimation::setDuration(int msecs, const QString &property)
{
    Q_D(MWidgetAnimation);

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

void MWidgetAnimation::setEasingCurve(const QEasingCurve &curve, const QString &property)
{
    Q_D(MWidgetAnimation);

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

void MWidgetAnimation::setTargetValue(QGraphicsWidget *widget, const QString &property, const QVariant &value)
{
    Q_D(MWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets(property, value);
    else {
        d->getAnimation(widget, property)->setStartValue(widget->property(property.toAscii()));
        d->getAnimation(widget, property)->setEndValue(value);
    }
}

void MWidgetAnimation::setKeyFrameAt(QGraphicsWidget *widget, const QString &property, qreal step, const QVariant &value)
{
    Q_D(MWidgetAnimation);
    d->getAnimation(widget, property)->setKeyValueAt(step, value);
}

void MWidgetAnimation::setKeyFrames(QGraphicsWidget *widget, const QString &property, const QVector<QVariant>& values)
{
    const int valuesSize = values.size();
    Q_D(MWidgetAnimation);
    if (valuesSize == 0)
        return;
    else if (valuesSize == 1) {
        d->getAnimation(widget, property)->setKeyValueAt(1.f, values.last());
    }

    QPropertyAnimation *a = d->getAnimation(widget, property);
    const qreal step = 1.f / (valuesSize - 1);
    for (int i = 0; i < valuesSize - 1; ++i) {
        a->setKeyValueAt(i * step, values[i]);
    }
    a->setKeyValueAt(1.0, values.last());
}

void MWidgetAnimation::setKeyFrames(QGraphicsWidget *widget, const QString &property, const QVector< QPair<qreal, QVariant> >& values)
{
    Q_D(MWidgetAnimation);
    d->getAnimation(widget, property)->setKeyValues(values);
}

void MWidgetAnimation::setTargetGeometry(QGraphicsWidget *widget, const QRectF &target)
{
    Q_D(MWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("geometry", target);
    else {
        d->getAnimation(widget, "geometry")->setStartValue(widget->geometry());
        d->getAnimation(widget, "geometry")->setEndValue(target);
    }
}

void MWidgetAnimation::setTargetOpacity(QGraphicsWidget *widget, qreal target)
{
    Q_D(MWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("opacity", target);
    else {
        d->getAnimation(widget, "opacity")->setStartValue(widget->opacity());
        d->getAnimation(widget, "opacity")->setEndValue(target);
    }
}

void MWidgetAnimation::setTargetSize(QGraphicsWidget *widget, const QSizeF &target)
{
    Q_D(MWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("size", target);
    else {
        d->getAnimation(widget, "size")->setStartValue(widget->size());
        d->getAnimation(widget, "size")->setEndValue(target);
    }
}

void MWidgetAnimation::setTargetPosition(QGraphicsWidget *widget, const QPointF &target)
{
    Q_D(MWidgetAnimation);
    if (!widget)
        d->setTargetForAllWidgets("pos", target);
    else {
        d->getAnimation(widget, "pos")->setStartValue(widget->pos());
        d->getAnimation(widget, "pos")->setEndValue(target);
    }
}
#endif

