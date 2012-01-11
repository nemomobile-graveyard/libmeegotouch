/***************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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
#include "meatgesturefilter.h"

#include <QEvent>

MEatGestureFilter::MEatGestureFilter(QObject *parent)
    :QObject(parent)
{
}

bool MEatGestureFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Gesture || event->type() == QEvent::GestureOverride) {
        event->accept();
        return true;
    } else {
        return QObject::eventFilter(watched, event);
    }
}
