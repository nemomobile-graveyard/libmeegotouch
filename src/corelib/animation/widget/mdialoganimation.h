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

#ifndef MDIALOGANIMATION_H
#define MDIALOGANIMATION_H

#include "mabstractwidgetanimation.h"
#include <mdialoganimationstyle.h>

#include <QPointF>

//! \internal

class MDialogAnimationPrivate;

class MDialogAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin)
    M_ANIMATION_GROUP(MDialogAnimationStyle)

public:
    MDialogAnimation(QObject *parent = NULL);

    virtual void restoreTargetWidgetState();

    void setTransitionDirection(TransitionDirection direction);

    QPointF origin() const;
    void setOrigin(const QPointF &pos);

protected:
    MDialogAnimation(MDialogAnimationPrivate *dd, QObject *parent = NULL);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_DECLARE_PRIVATE(MDialogAnimation)
};

//! \internal_end

#endif // MDIALOGANIMATION_H
