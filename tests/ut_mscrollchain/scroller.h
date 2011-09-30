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
#ifndef SCROLLER_H
#define SCROLLER_H

#include <mabstractscroller.h>
#include <QEasingCurve>

class ScrollableWidget;

class Scroller : public MAbstractScroller
{
public:
    Scroller();
    virtual ~Scroller();

    virtual QPoint queryScrollingAmount(const QGraphicsWidget *widget,
                                        const QRect &targetRect,
                                        const QPoint &originPoint,
                                        const QPoint &currentOffset);
    virtual void applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset,
                                int duration, const QEasingCurve &easingCurve);
    virtual void restoreScrolling(QGraphicsWidget *widget);

    static QPoint maximumOffset; // absolute offset values

    int lastScrollDuration;
    QEasingCurve lastEasingCurve;
};

#endif // SCROLLER_H
