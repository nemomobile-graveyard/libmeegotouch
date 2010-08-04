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

#include "mresizelistener.h"

#include <QGraphicsSceneResizeEvent>
#include <QGraphicsWidget>
#include <QEvent>
#include <QDebug>

MResizeListener::MResizeListener(QObject *parent)
    : QObject(parent)
{
}

MResizeListener::~MResizeListener()
{
}

bool MResizeListener::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneResize) {
        // Has to be QGraphicsWidget when got GraphicsSceneResize.
        emit widgetResized(static_cast<QGraphicsWidget *>(watched));
    }
    return false;
}
