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

#ifndef MABSTRACTSCROLLER_H
#define MABSTRACTSCROLLER_H

#include <QObject>

class QGraphicsWidget;
class QPoint;
class QRect;

//! \internal

/*! \brief Interface for extending widget scrolling support in MScrollChain
 *
 *  Through this interface, once registered, the MScrollChain queries how
 *  much a widget can scroll, actually scroll it, and restore any scrolling.
 */
class MAbstractScroller : public QObject
{
    Q_OBJECT
public:
    MAbstractScroller();
    virtual ~MAbstractScroller();

    /*! \brief Queries how much widget can scroll given the origin and target areas.
     *
     *  The scroller should not make assumptions about its own position since some
     *  parent widget might have already planned scrolling.
     *
     *  Current limitation: If scrolling amount of the particular widget is restricted,
     *  and this method is called more than once, subsequent queries might promise too
     *  much and exceed the scrolling range of the widget.
     *  This method could be replaced by getters for viewport area and scrollable range,
     *  and then MScrollChain would do the math, keeping track of scrolling position.
     *
     *  \param widget The scrollable widget in question.
     *  \param targetRect Target rectangle of the area of interest, in local widget coordinates.
     *  \param originPoint Current position (top left) of the area to be moved to targetRect, in local widget coordinates.
     *  \param currentOffset The amount that \a widget is already planned to be scrolled but not yet applied.
     */
    virtual QPoint queryScrollingAmount(const QGraphicsWidget *widget,
                                        const QRect &targetRect,
                                        const QPoint &originPoint,
                                        const QPoint &currentOffset) = 0;

    //! \brief Scrolls the contents of given \a widget by the amount of \a contentsOffset.
    virtual void applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset) = 0;

    //! \brief Stops any ongoing scrolling animation in the given \a widget.
    virtual void stopScrolling(QGraphicsWidget *widget);

    //! \brief Restores the widget to its original state (Optional)
    virtual void restoreScrolling(QGraphicsWidget *widget);

    //! \brief Helper method for moving a small rectangle inside a larger area.
    static void moveRectInsideArea(const QRect &area, QRect &rect);
};

//! \internal_end

#endif // MABSTRACTSCROLLER_H
