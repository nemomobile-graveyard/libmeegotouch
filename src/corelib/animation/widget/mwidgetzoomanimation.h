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

#ifndef MWIDGETZOOMANIMATION_H
#define MWIDGETZOOMANIMATION_H

#include "mabstractwidgetanimation.h"
#include <mwidgetzoomanimationstyle.h>

#include <QPointF>

//! \internal

class MWidgetZoomAnimationPrivate;

class MWidgetZoomAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin)
    M_ANIMATION_GROUP(MWidgetZoomAnimationStyle)

public:
    MWidgetZoomAnimation(QObject *parent = NULL);

    virtual void setTargetWidget(MWidgetController *widget);
    virtual void restoreTargetWidgetState();

    void setTransitionDirection(TransitionDirection direction);
    void setOrigin(const QPointF &pos);
    QPointF origin() const;

protected:
    MWidgetZoomAnimation(MWidgetZoomAnimationPrivate *dd, QObject *parent = NULL);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_DECLARE_PRIVATE(MWidgetZoomAnimation)

#ifdef UNIT_TEST
    friend class Ut_MWidgetZoomAnimation;
#endif
};

//! \internal_end

#endif // MWIDGETZOOMANIMATION_H
