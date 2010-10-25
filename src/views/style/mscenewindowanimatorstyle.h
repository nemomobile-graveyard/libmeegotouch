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

#ifndef MSCENEWINDOWANIMATORSTYLE_H
#define MSCENEWINDOWANIMATORSTYLE_H

#include <mstyle.h>
#include <QPoint>

class M_VIEWS_EXPORT MSceneWindowAnimatorStyle : public MStyle
{
    Q_OBJECT
    M_STYLE(MSceneWindowAnimatorStyle)

    M_STYLE_ATTRIBUTE(QString, transitionInType,      TransitionInType)
    M_STYLE_ATTRIBUTE(int,     transitionInDuration,  TransitionInDuration)
    M_STYLE_ATTRIBUTE(bool, transitionInFade,      TransitionInFade)
    M_STYLE_ATTRIBUTE(QString, transitionOutType,     TransitionOutType)
    M_STYLE_ATTRIBUTE(int,     transitionOutDuration, TransitionOutDuration)
    M_STYLE_ATTRIBUTE(bool, transitionOutFade,     TransitionOutFade)
    M_STYLE_ATTRIBUTE(QPoint, transitionInDestination, TransitionInDestination)
    M_STYLE_ATTRIBUTE(QPoint, transitionOutDestination, TransitionOutDestination)
};

class M_VIEWS_EXPORT MSceneWindowAnimatorStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER(MSceneWindowAnimatorStyle)
};

#endif

