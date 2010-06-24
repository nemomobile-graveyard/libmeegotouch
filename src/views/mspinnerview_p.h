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

#ifndef MSPINNERVIEW_P_H
#define MSPINNERVIEW_P_H

#include <QObject>
#include <QPointF>
#include <QRect>

class MStyle;
class MProgressIndicator;
class QPixmap;
class QTimer;

class MSpinnerViewPrivate
{
    Q_DECLARE_PUBLIC(MSpinnerView)

protected:
    MSpinnerView *q_ptr;

public:

    MSpinnerViewPrivate();
    virtual ~MSpinnerViewPrivate();

    void animationTimeout();
    void visibilityChanged();

    MProgressIndicator *controller;
    mutable QPixmap *pieImage;

    int position;
    qreal elapsed;

    QTimer *timer;
    
    QRect bgRect;
    QRect fgRect;


#ifdef M_UNIT_TEST
    M_UNIT_TEST;
#endif
};

#endif // MSPINNERVIEW_P_H
