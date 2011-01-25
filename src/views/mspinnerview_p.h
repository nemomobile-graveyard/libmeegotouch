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

#include <QElapsedTimer>

class MStyle;
class MProgressIndicator;
class QPixmap;
class QPropertyAnimation;

class MSpinnerViewPrivate
{
    Q_DECLARE_PUBLIC(MSpinnerView)

protected:
    MSpinnerView *q_ptr;

public:
    MSpinnerViewPrivate();
    virtual ~MSpinnerViewPrivate();

    void refreshStyle();
    void refreshModel();

    const MWindow* getMWindow();

    QPropertyAnimation *createAnimation();
    void resumeAnimation();
    void pauseAnimation();

    void reloadFrames();
    void releaseUsedPixmaps();

    void _q_pauseAnimation();
    void _q_resumeAnimation();
    void _q_pauseOrResumeAnimation();

    void _q_switcherEntered();
    void _q_switcherExited();

    MProgressIndicator *controller;

    QPropertyAnimation* positionAnimation;
    int currentFrame;
    QList<const QPixmap *> animationPixmaps;

#ifdef M_UNIT_TEST
    M_UNIT_TEST;
#endif
};

#endif // MSPINNERVIEW_P_H
