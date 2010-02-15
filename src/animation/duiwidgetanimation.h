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

#ifndef DUIWIDGETANIMATION_H
#define DUIWIDGETANIMATION_H

#include <QtGlobal>

#include <DuiGroupAnimation>
#include <QPair>
#include <QVector>
#include <duianimationstyle.h>

class DuiWidgetAnimationPrivate;
class QGraphicsWidget;
class QEasingCurve;
class QRectF;
class QSizeF;
class QPointF;

/*!
  \class DuiWidgetAnimation
  \brief DuiWidgetAnimation class provides easy to use interface for animating
         widgets.

  The class is based on the QVariantAnimation and QAnimationGroup classes. A
  single QVariantAnimation class offers only interface for animating single
  property of a single object. DuiWidgetAnimation combines these two animation
  classes and provides an easy to use interface for animating several objects and
  properties at the same time.

  \section examples Examples

  Animate single widget from it's current position to another.
  \code
   DuiWidgetAnimation* a = new DuiWidgetAnimation(NULL);
   a->setTargetPosition(widget, QPointF(50,50));
   a->start(QAbstractAnimation::DeleteWhenStopped);
  \endcode

  Animate several widgets at the same time.
  \code
   DuiWidgetAnimation* a = new DuiWidgetAnimation(NULL);
   a->setTargetOpacity(widget0, 0.0);
   a->setTargetOpacity(widget1, 0.0);
   a->setTargetOpacity(widget2, 0.0);
   a->setTargetOpacity(widget3, 0.0);
   a->start(QAbstractAnimation::DeleteWhenStopped);
  \endcode

  Animate multiple properties at different speed.
  \code
   DuiWidgetAnimation* a = new DuiWidgetAnimation(NULL);
   a->setTargetOpacity(widget, 0.0);
   a->setDuration(1000, "opacity");
   a->setTargetGeometry(widget, QRecF(0,0,50,50));
   a->setDuration(500, "geometry");
   a->start(QAbstractAnimation::DeleteWhenStopped);
  \endcode

  Keyframed animation.
  \code
   DuiWidgetAnimation* a = new DuiWidgetAnimation(NULL);
   QVector<QVariant> keyframes;
   keyframes.add(QPointF(50,50));
   keyframes.add(QPointF(75,50));
   keyframes.add(QPointF(125,10));
   a->setKeyframes(widget, "position", keyframes);
   a->start(QAbstractAnimation::DeleteWhenStopped);
  \endcode
 */
class DuiWidgetAnimation : public DuiGroupAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiWidgetAnimation)
    DUI_ANIMATION(DuiAnimationStyle)

    /*!
      \brief Constructs the widget animation.

      This constructor is meant to be used inside the libdui to share the
      private data class pointer.
     */
    DuiWidgetAnimation(DuiWidgetAnimationPrivate *dd, QObject *parent);

public:

    /*!
      \brief Constructs the widget animation.
     */
    DuiWidgetAnimation(QObject *parent = NULL);

    /*!
      \brief Destructs the widget animation.
     */
    virtual ~DuiWidgetAnimation();

    /*!
      \brief Add new widget to animation
    */
    void addWidget(QGraphicsWidget *widget);

    /*!
      \brief Remove widget from animation
    */
    void removeWidget(QGraphicsWidget *widget);

    /*!
      \brief Set duration for an animation.

      If \a property equals to empty string the \a duration is used for all the
      different property animations. If \a property defines a valid property
      name the duration of the specific property animation is set.
    */
    void setDuration(int msecs, const QString &property = QString());

    /*!
      \brief Set easing curve for an animation.

      If \a property equals to empty string the \a curve is used for all the
      different property animations. If \a property defines a valid property
      name the curve of the specific property animation is set.
    */
    void setEasingCurve(const QEasingCurve &curve, const QString &property = QString());

    /*!
      \brief Sets target value for a property animation.
    */
    void setTargetValue(QGraphicsWidget *widget, const QString &property, const QVariant &value);
    //void enableAnimation(const QString& property, bool enable);
    //void setSourceValue(const QString& property, const QVariant& value);

    /*!
      \brief Creates a key frame for \a property at the given \a step with the given \a value.

      The given \a step must be in the range 0.0 to 1.0.
    */
    void setKeyFrameAt(QGraphicsWidget *widget, const QString &property, qreal step, const QVariant &value);

    /*!
      \brief Initializes keyframe animation for \a property with given keyframe \a values.

      The steps between the keyframes are equally sized.
    */
    void setKeyFrames(QGraphicsWidget *widget, const QString &property, const QVector<QVariant>& values);

    /*!
      \brief Initializes keyframe animation for \a property with given keyframe step and values pairs.
    */
    void setKeyFrames(QGraphicsWidget *widget, const QString &property, const QVector< QPair<qreal, QVariant> >& values);

    //void setKeyframeAt(qreal step, const QRect& value);
    //void setKeyframeAt(qreal step, qreal value);
    //void setKeyframeAt(qreal step, const QSize& value);
    //void setKeyframeAt(qreal step, const PointF& value);

    /*!
      \brief Sets target for geometry animation. If \a widget == NULL, the same target
             is set for all the added widgets, otherwise the target is set only for the
             specific widget.
    */
    void setTargetGeometry(QGraphicsWidget *widget, const QRectF &target);
    //void enableGeometryAnimation(bool enable);
    //void setSourceGeometry(const QRectF& geometry);

    /*!
      \brief Sets target for opacity animation. If \a widget == NULL, the same target
             is set for all the added widgets, otherwise the target is set only for the
             specific widget.
    */
    void setTargetOpacity(QGraphicsWidget *widget, qreal target);
    //void enableOpacityAnimation(bool enable);
    //void setSourceOpacity(qreal opacity);

    /*!
      \brief Sets target for size animation. If \a widget == NULL, the same target
             is set for all the added widgets, otherwise the target is set only for the
             specific widget.
    */
    void setTargetSize(QGraphicsWidget *widget, const QSizeF &target);
    //void enableSizeAnimation(bool enable);
    //void setSourceSize(const QSizeF& size);

    /*!
      \brief Sets target for position animation. If \a widget == NULL, the same target
             is set for all the added widgets, otherwise the target is set only for the
             specific widget.
    */
    void setTargetPosition(QGraphicsWidget *widget, const QPointF &target);
    //void enablePositionAnimation(bool enable);
    //void setSourcePosition(const QPointF& position);
};

#endif
