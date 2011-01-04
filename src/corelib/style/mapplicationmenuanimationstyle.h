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

#ifndef MAPPLICATIONMENUANIMATIONSTYLE_H
#define MAPPLICATIONMENUANIMATIONSTYLE_H

#include <mdialoganimationstyle.h>

//! \internal
class MApplicationMenuAnimationStyle: public MDialogAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MApplicationMenuAnimationStyle)

    M_STYLE_ATTRIBUTE(bool, titleBarHasCustomEasingCurve, TitleBarHasCustomEasingCurve)
    M_STYLE_ATTRIBUTE(QEasingCurve, titleBarAnimationEasingCurve, TitleBarAnimationEasingCurve)

    M_STYLE_ATTRIBUTE(int, contentsViewportPositionAnimationDuration, ContentsViewportPositionAnimationDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, contentsViewportPositionAnimationEasingCurve, ContentsViewportPositionAnimationEasingCurve)
    M_STYLE_ATTRIBUTE(qreal, contentsViewportPositionAnimationDistance, ContentsViewportPositionAnimationDistance)
};

class MApplicationMenuAnimationStyleContainer : public MDialogAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MApplicationMenuAnimationStyle)
};
//! \internal_end

#endif

