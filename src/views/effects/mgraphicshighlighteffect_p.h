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
 

#ifndef MGRAPHICSWIDGETOVERLAYEFFECT_P_H
#define MGRAPHICSWIDGETOVERLAYEFFECT_P_H

#include <QObject>
#include <QColor>
#include <QPoint>

#include <mparallelanimationgroup.h>

class MGraphicsHighlightEffect;

class QImage;
class QPainter;
class QPauseAnimation;
class QPropertyAnimation;
class QPixmap;

class MGraphicsHighlightEffectPrivate
{
public:
    MGraphicsHighlightEffectPrivate();
    virtual ~MGraphicsHighlightEffectPrivate();

    void applyStyle();

    void drawComposedImage(QPoint offset, QPainter *painter, const QPixmap *pixmap);

private:
    Q_DECLARE_PUBLIC(MGraphicsHighlightEffect)
    MGraphicsHighlightEffect *q_ptr;

    QColor highlightColor;
    qreal strength;

    QImage *composed;
    QPainter *composer;

    QSequentialAnimationGroup *animationGroup;
    QPauseAnimation *delayAnimation;
    QPropertyAnimation *strengthAnimation;
};

#endif // MGRAPHICSWIDGETOVERLAYEFFECT_P_H
