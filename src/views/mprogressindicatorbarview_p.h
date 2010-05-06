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

#ifndef MPROGRESSINDICATORBARVIEW_P_H
#define MPROGRESSINDICATORBARVIEW_P_H

#include <QObject>

class MStyle;
class MProgressIndicator;
class QPixmap;
class QTimer;

class MProgressIndicatorBarViewPrivate
{
    Q_DECLARE_PUBLIC(MProgressIndicatorBarView)

protected:
    MProgressIndicatorBarView *q_ptr;

public:
    MProgressIndicatorBarViewPrivate();
    virtual ~MProgressIndicatorBarViewPrivate();

    MProgressIndicator *controller;

    float elementSize;
    int activeElementCount;
    int speed;
    qreal position;
    QTimer *timer;

#ifdef M_UNIT_TEST
    M_UNIT_TEST;
#endif
};

#endif // MPROGRESSINDICATORBARVIEW_P_H
