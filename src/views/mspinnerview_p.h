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

class MStyle;
class MProgressIndicator;
class QTimer;
class QPixmap;
class QBrush;
class QPen;
class QRect;
class QPropertyAnimation;

class MSpinnerViewPrivate
{
    Q_DECLARE_PUBLIC(MSpinnerView)

protected:
    MSpinnerView *q_ptr;

public:

    MSpinnerViewPrivate();
    virtual ~MSpinnerViewPrivate();

    void resumeAnimation();
    void pauseAnimation();

    MProgressIndicator *controller;

    mutable QBrush pieBrush;
    mutable QPen piePen;

    QPropertyAnimation* positionAnimation;
    int angle;

#ifdef M_UNIT_TEST
    M_UNIT_TEST;
#endif
};

#endif // MSPINNERVIEW_P_H
