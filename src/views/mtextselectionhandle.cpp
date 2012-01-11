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

#include "mtextselectionhandle.h"
#include "mdebug.h"

#include "meatgesturefilter.h"

namespace {
    const int DefaultMovementSensitivity = 3;
    const qreal NormalOpacity = 1.0f;

    qreal distance(const QPointF &pos, const QRectF &rect)
    {
        if (rect.contains(pos))
            return 0;
        return (pos - rect.center()).manhattanLength();
    }
}

MTextSelectionHandle::MTextSelectionHandle()
    : opacityAnimation(this, "opacity"),
      m_pressed(false),
      hiddenTemporarily(false),
      mMovementSensitivity(DefaultMovementSensitivity),
      positionIsVisible(false),
      disabled(false)
{
    setFocusProxy(0);
    setFocusPolicy(Qt::NoFocus);

    grabGestureWithCancelPolicy(Qt::TapGesture);
    grabGestureWithCancelPolicy(Qt::TapAndHoldGesture);
    installEventFilter(new MEatGestureFilter(this));

    setOpacity(0.0f);
    hide();

    connect(&opacityAnimation, SIGNAL(finished()),
            this, SLOT(onShowHideAnimationFinished()));
}

MTextSelectionHandle::~MTextSelectionHandle()
{
    // Before destroying parent, detach so it doesn't try to destroy us.
    setParentItem(0);
}

void MTextSelectionHandle::appear()
{
    if (hiddenTemporarily) {
        return;
    }

    const qreal targetOpacity = disabled ? style()->disableOpacity() : NormalOpacity;

    const bool isCompletelyVisible = (qFuzzyCompare(opacity(), targetOpacity)
                                      && opacityAnimation.state() != QAbstractAnimation::Running);
    const bool isShowInProgress = (qFuzzyCompare(opacityAnimation.endValue().toReal(), targetOpacity)
                                   && opacityAnimation.state() == QAbstractAnimation::Running
                                   && opacityAnimation.direction() == QAbstractAnimation::Forward);

    if (isVisible() && (isCompletelyVisible || isShowInProgress)) {
        return;
    }

    show();
    opacityAnimation.setEndValue(targetOpacity);
    opacityAnimation.setDirection(QAbstractAnimation::Forward);

    if (opacityAnimation.state() != QAbstractAnimation::Running) {
        opacityAnimation.setStartValue(opacity());
        opacityAnimation.start();
    }
}

void MTextSelectionHandle::disappear()
{
    if (!isVisible()
        || (qFuzzyIsNull(opacityAnimation.endValue().toReal())
            && opacityAnimation.state() == QAbstractAnimation::Running)) {
        return;
    }

    opacityAnimation.setStartValue(0.0f);
    opacityAnimation.setDirection(QAbstractAnimation::Backward);

    if (opacityAnimation.state() != QAbstractAnimation::Running) {
        opacityAnimation.setEndValue(opacity());
        opacityAnimation.start();
    }
}

void MTextSelectionHandle::hideTemporarily()
{
    hiddenTemporarily = true;
    disappear();
}

void MTextSelectionHandle::restore()
{
    if (!hiddenTemporarily) {
        return;
    }

    hiddenTemporarily = false;

    if (positionIsVisible) {
        appear();
    }
}

void MTextSelectionHandle::setPositionVisibility(bool visible)
{
    if (positionIsVisible == visible) {
        return;
    }

    positionIsVisible = visible;

    if (visible) {
        appear();
    } else {
        disappear();
    }
}

void MTextSelectionHandle::updatePosition(const QPointF &pos)
{
    const QPointF correction(preferredWidth() / 2,
                             style()->visualOffset());
    setPos(pos - correction);
}

void MTextSelectionHandle::setPressed(bool press)
{
    if (m_pressed == press)
        return;

    m_pressed = press;

    if (m_pressed) {
        emit pressed(this);
    } else {
        emit released(this);
    }
}

bool MTextSelectionHandle::isPressed() const
{
    return m_pressed;
}

QPointF MTextSelectionHandle::hotSpot() const
{
    const QPointF correction(preferredWidth() / 2, 0);
    return pos() + correction;
}

void MTextSelectionHandle::applyStyle()
{
    mMovementSensitivity = style()->movementSensitivity();

    opacityAnimation.setEasingCurve(style()->showHideEasingCurve());
    opacityAnimation.setDuration(style()->showHideDuration());

    prepareGeometryChange(); // in case if bounding rect will be changed due to size of graphic asset
    update();
}

QRectF MTextSelectionHandle::boundingRect() const
{
    return QRectF(QPointF(0.0f, 0.0f),
                  style()->preferredSize());
}

QSizeF MTextSelectionHandle::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which) {
    case Qt::MinimumSize:
        return MStylableWidget::sizeHint(which, constraint);

    case Qt::MaximumSize:
        return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    case Qt::PreferredSize:
        return style()->preferredSize();

    default:
        mWarning("MTextSelectionHandle") << __PRETTY_FUNCTION__
                                         << "don't know how to handle the value of 'which':"
                                         << which;
    }

    return QSizeF(0, 0);
}

void MTextSelectionHandle::onShowHideAnimationFinished()
{
    if (!opacity()) {
        hide();
        if (!hiddenTemporarily) {
            m_pressed = false;
            emit disappeared();
        }
    }
}

int MTextSelectionHandle::movementSensitivity() const
{
    return mMovementSensitivity;
}

void MTextSelectionHandle::skipTransitions()
{
    if (opacityAnimation.state() == QAbstractAnimation::Running) {
        opacityAnimation.stop();
        if (opacityAnimation.direction() == QAbstractAnimation::Forward) {
            setOpacity(opacityAnimation.endValue().toReal());
        } else {
            setOpacity(opacityAnimation.startValue().toReal());
        }
        onShowHideAnimationFinished();
    }
}

void MTextSelectionHandle::disable()
{
    if (disabled) {
        return;
    }

    disabled = true;

    if (!isVisible() || isHideInProgress()) {
        return;
    }

    if (hiddenTemporarily || !positionIsVisible) {
        return;
    }

    opacityAnimation.setDirection(QAbstractAnimation::Backward);
    opacityAnimation.setStartValue(style()->disableOpacity());

    if (opacityAnimation.state() != QAbstractAnimation::Running) {
        opacityAnimation.setEndValue(opacity());
        opacityAnimation.start();
    }
}

void MTextSelectionHandle::enable()
{
    if (!disabled) {
        return;
    }

    disabled = false;

    if (!isVisible() || isHideInProgress()) {
        return;
    }

    if (hiddenTemporarily || !positionIsVisible) {
        return;
    }

    opacityAnimation.setDirection(QAbstractAnimation::Forward);
    opacityAnimation.setEndValue(NormalOpacity);

    if (opacityAnimation.state() != QAbstractAnimation::Running) {
        opacityAnimation.setStartValue(opacity());
        opacityAnimation.start();
    }
}

QRectF MTextSelectionHandle::region() const
{
    return geometry().adjusted(0, 0, 0, -style()->regionPadding());
}

bool MTextSelectionHandle::isReady() const
{
    return isVisible() && !disabled && !isHideInProgress() && !m_pressed;
}

bool MTextSelectionHandle::isHideInProgress() const
{
    return (qFuzzyIsNull(opacityAnimation.startValue().toReal())
            && opacityAnimation.state() == QAbstractAnimation::Running
            && opacityAnimation.direction() == QAbstractAnimation::Backward);
}
