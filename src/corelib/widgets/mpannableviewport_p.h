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

#ifndef MPANNABLEVIEWPORT_P_H
#define MPANNABLEVIEWPORT_P_H

#include "mpannablewidget_p.h"
#include "../core/mresizelistener.h"

class QGraphicsWidget;
class MStyle;
class MPositionIndicator;
class QGraphicsLinearLayout;
class MPannableViewportLayout;

class MPannableViewportPrivate : public MPannableWidgetPrivate
{
    Q_DECLARE_PUBLIC(MPannableViewport)
public:
    MPannableViewportPrivate();
    virtual ~MPannableViewportPrivate();

    QRectF currentRange;
    QGraphicsWidget *pannedWidget;
    MPannableViewportLayout *viewportLayout;
    MPositionIndicator *positionIndicator;
    int inputMethodAreaHeight;
    MResizeListener resizeListener;

    /*!
     * \brief Sets new value of the range attribute with emitting
     * rangeChanged() signal if needed.
     */
    void setNewRange(const QRectF &newRange);

    /*!
     * \brief Recalculates the range of the base class (i.e. range of
     * the physics).
     */
    void recalculatePhysRange();

    void setInputMethodArea(const QRect &imArea);

    void sendOnDisplayChangeEventToMWidgets(QGraphicsItem *item,
            MOnDisplayChangeEvent *event);
    void _q_resolvePannedWidgetIsOnDisplay();
    void _q_positionIndicatorEnabledChanged();
    void _q_pannedWidgetResized(QGraphicsWidget *widget);
};

#endif
