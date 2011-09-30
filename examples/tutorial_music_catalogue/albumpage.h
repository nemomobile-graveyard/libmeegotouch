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
#ifndef ALBUMPAGE_H
#define ALBUMPAGE_H

#include <MApplicationPage>

class MImageWidget;
class QGestureEvent;
class QPinchGesture;

#include "data.h"

class AlbumPage : public MApplicationPage {
    Q_OBJECT
public:
    AlbumPage(const Album *album, QGraphicsItem *parent = 0);

protected:
    virtual void createContent();

    // From MWidget
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture);

private slots:
    void fadeInAlbumCover();

private:
    const Album *album;
    MImageWidget *albumCover;
};

#endif
