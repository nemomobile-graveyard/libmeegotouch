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

#ifndef DUIPANNABLEVIEWPORT_P_H
#define DUIPANNABLEVIEWPORT_P_H

#include "duipannablewidget_p.h"

class QGraphicsWidget;
class DuiStyle;
class DuiPositionIndicator;
class QGraphicsLinearLayout;
class DuiPannableViewportLayout;

class DuiPannableViewportPrivate : public DuiPannableWidgetPrivate
{
    Q_DECLARE_PUBLIC(DuiPannableViewport)
public:
    DuiPannableViewportPrivate();
    virtual ~DuiPannableViewportPrivate();

    QRectF currentRange;
    QGraphicsWidget *pannedWidget;
    DuiPannableViewportLayout *viewportLayout;
    DuiPositionIndicator *positionIndicator;

    bool haveEmittedSizePosChanged;
    QSizeF lastEmittedViewportSize;
    QRectF lastEmittedPannedRange;
    QPointF lastEmittedPannedPos;

    void emitSizePosChanged(const QSizeF &viewportSize,
                            const QRectF &pannedRange,
                            const QPointF &pannedPos);

    /*!
     * \brief Recalculates the range of the base class (i.e. range of
     * the physics).
     */
    void recalculatePhysRange();

    void sendOnDisplayChangeEventToDuiWidgets(QGraphicsItem *item,
            DuiOnDisplayChangeEvent *event);
    void _q_resolvePannedWidgetIsOnDisplay();

    friend class Ut_DuiPannableViewport;
};

#endif
