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

#ifndef MSCENEWINDOWANIMATIONSTYLE_H
#define MSCENEWINDOWANIMATIONSTYLE_H

#include <manimationstyle.h>
//#include <QEasingCurve>

class M_CORE_EXPORT MSceneWindowAnimationStyle : public MAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MSceneWindowAnimationStyle)

    M_STYLE_ATTRIBUTE(int, showDuration, ShowDuration)
    M_STYLE_ATTRIBUTE(int, hideDuration, HideDuration)
    M_STYLE_ATTRIBUTE(int, moveDuration, MoveDuration)

    //M_STYLE_ATTRIBUTE(QEasingCurve, showCurve, ShowCurve)
    //M_STYLE_ATTRIBUTE(QEasingCurve, hideCurve, HideCurve)
    //M_STYLE_ATTRIBUTE(QEasingCurve, moveCurve, MoveCurve)

};

// TODO: get rid of this container
class MSceneWindowAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MSceneWindowAnimationStyle)
};

#endif
