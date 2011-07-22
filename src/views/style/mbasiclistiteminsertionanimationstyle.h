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

#ifndef MLISTITEMINSERTIONANIMATIONSTYLE_H
#define MLISTITEMINSERTIONANIMATIONSTYLE_H

#include <manimationstyle.h>
#include <QEasingCurve>

//! \internal
class MBasicListItemInsertionAnimationStyle : public MAnimationStyle
{
    Q_OBJECT
    M_STYLE(MBasicListItemInsertionAnimationStyle)

    M_STYLE_ATTRIBUTE(int, insertDelay, InsertDelay)
    M_STYLE_ATTRIBUTE(int, insertDuration, InsertDuration)
    M_STYLE_ATTRIBUTE(qreal, insertOpacity, InsertOpacity)
    M_STYLE_ATTRIBUTE(qreal, insertScale, InsertScale)
    M_STYLE_ATTRIBUTE(QEasingCurve, insertCurve, InsertCurve)

    M_STYLE_ATTRIBUTE(int, moveDelay, MoveDelay)
    M_STYLE_ATTRIBUTE(int, moveDuration, MoveDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, moveCurve, MoveCurve)
};

class MBasicListItemInsertionAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER(MBasicListItemInsertionAnimationStyle)
};
//! \internal_end

#endif // MLISTITEMINSERTIONANIMATIONSTYLE_H
