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

#ifndef MLISTITEMANIMATIONSTYLE_H
#define MLISTITEMANIMATIONSTYLE_H

#include <manimationstyle.h>
#include <QEasingCurve>

//! \internal
class MBasicListItemDeletionAnimationStyle : public MAnimationStyle
{
    Q_OBJECT
    M_STYLE(MBasicListItemDeletionAnimationStyle)

    M_STYLE_ATTRIBUTE(int, deleteDelay, DeleteDelay)
    M_STYLE_ATTRIBUTE(int, deleteDuration, DeleteDuration)
    M_STYLE_ATTRIBUTE(qreal, deleteOpacity, DeleteOpacity)
    M_STYLE_ATTRIBUTE(qreal, deleteScale, DeleteScale)
    M_STYLE_ATTRIBUTE(QEasingCurve, deleteCurve, DeleteCurve)

    M_STYLE_ATTRIBUTE(int, moveDelay, MoveDelay)
    M_STYLE_ATTRIBUTE(int, moveDuration, MoveDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, moveCurve, MoveCurve)
};

class MBasicListItemDeletionAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER(MBasicListItemDeletionAnimationStyle)
};
//! \internal_end

#endif
