/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUISCENEWINDOWANIMATIONSTYLE_H
#define DUISCENEWINDOWANIMATIONSTYLE_H

#include <duianimationstyle.h>
//#include <QEasingCurve>

class DUI_EXPORT DuiSceneWindowAnimationStyle : public DuiAnimationStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiSceneWindowAnimationStyle)

    DUI_STYLE_ATTRIBUTE(int, showDuration, ShowDuration)
    DUI_STYLE_ATTRIBUTE(int, hideDuration, HideDuration)
    DUI_STYLE_ATTRIBUTE(int, moveDuration, MoveDuration)

    //DUI_STYLE_ATTRIBUTE(QEasingCurve, showCurve, ShowCurve)
    //DUI_STYLE_ATTRIBUTE(QEasingCurve, hideCurve, HideCurve)
    //DUI_STYLE_ATTRIBUTE(QEasingCurve, moveCurve, MoveCurve)

};

// TODO: get rid of this container
class DuiSceneWindowAnimationStyleContainer : public DuiAnimationStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiSceneWindowAnimationStyle)
};

#endif
