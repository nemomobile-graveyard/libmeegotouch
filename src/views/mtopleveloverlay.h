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
#ifndef MTOPLEVELOVERLAY_H
#define MTOPLEVELOVERLAY_H

#include <mnamespace.h>
#include <QGraphicsWidget>

class MSceneManager;
class QGestureEvent;
class QPanGesture;

//! \internal

//! Overlay widget to allow magnifier to stay on top of other widgets.
class MTopLevelOverlay : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit MTopLevelOverlay(const MSceneManager *sceneManager);
    bool isAppeared() const;

protected:
    //! \reimp
    virtual void panGestureEvent(QGestureEvent *event, QPanGesture *panGesture);
    //! \reimp_end

private slots:
    void rotateAndResizeToFullscreen(M::OrientationAngle angle);

private:
    const MSceneManager * const sceneManager;

    friend class Ut_MTextMagnifier;
};

//! \internal_end

#endif
