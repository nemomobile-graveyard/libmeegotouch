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

#ifndef DUISCENEWINDOWANIMATORSTYLE_H
#define DUISCENEWINDOWANIMATORSTYLE_H

#include <duistyle.h>
#include <QPoint>

class DUI_EXPORT DuiSceneWindowAnimatorStyle : public DuiStyle
{
    Q_OBJECT
    DUI_STYLE(DuiSceneWindowAnimatorStyle)

    DUI_STYLE_ATTRIBUTE(QString, transitionInType,      TransitionInType)
    DUI_STYLE_ATTRIBUTE(int,     transitionInDuration,  TransitionInDuration)
    DUI_STYLE_ATTRIBUTE(bool, transitionInFade,      TransitionInFade)
    DUI_STYLE_ATTRIBUTE(QString, transitionOutType,     TransitionOutType)
    DUI_STYLE_ATTRIBUTE(int,     transitionOutDuration, TransitionOutDuration)
    DUI_STYLE_ATTRIBUTE(bool, transitionOutFade,     TransitionOutFade)
    DUI_STYLE_ATTRIBUTE(QPoint, transitionInDestination, TransitionInDestination)
    DUI_STYLE_ATTRIBUTE(QPoint, transitionOutDestination, TransitionOutDestination)
};

class DUI_EXPORT DuiSceneWindowAnimatorStyleContainer : public DuiStyleContainer
{
    DUI_STYLE_CONTAINER(DuiSceneWindowAnimatorStyle)
};

#endif

