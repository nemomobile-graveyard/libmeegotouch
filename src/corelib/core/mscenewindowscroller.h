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

#ifndef MSCENEWINDOWSCROLLER_H
#define MSCENEWINDOWSCROLLER_H

#include "mabstractscroller.h"

class MSceneWindow;
class QPointF;

//! \internal

class MSceneWindowScroller : public MAbstractScroller
{
    Q_OBJECT
signals:
    //! This signal requests immediate displacement of the given scene window. The signal should be
    //! connected to using Qt::DirectConnection, or similar, since after emitting it is assumed the
    //! window has been moved.
    void sceneWindowDislocationRequest(MSceneWindow *sceneWindow, const QPointF &displacement);

    //! This signal requests undoing previous displacement of a given scene window.
    void sceneWindowUndoDislocationRequest(MSceneWindow *sceneWindow);

public:
    virtual QPoint queryScrollingAmount(const QGraphicsWidget *widget,
                                        const QRect &targetRect,
                                        const QPoint &originPoint,
                                        const QPoint &currentOffset);
    virtual void applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset);
    virtual void restoreScrolling(QGraphicsWidget *widget);
};

//! \internal_end

#endif // MSCENEWINDOWSCROLLER_H
