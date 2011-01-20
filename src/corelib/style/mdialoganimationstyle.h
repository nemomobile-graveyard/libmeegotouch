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

#ifndef MDIALOGANIMATIONSTYLE_H
#define MDIALOGANIMATIONSTYLE_H

#include <manimationstyle.h>
#include <QEasingCurve>

//! \internal
class MDialogAnimationStyle: public MAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MDialogAnimationStyle)

    M_STYLE_ATTRIBUTE(QString, titleBarObjectName, TitleBarObjectName)
    M_STYLE_ATTRIBUTE(QString, contentsViewportObjectName, ContentsViewportObjectName)
    M_STYLE_ATTRIBUTE(QString, buttonBoxObjectName, ButtonBoxObjectName)

    M_STYLE_ATTRIBUTE(int, titleBarAnimationDelay, TitleBarAnimationDelay)
    M_STYLE_ATTRIBUTE(int, contentsViewportAnimationDelay, ContentsViewportAnimationDelay)
    M_STYLE_ATTRIBUTE(int, buttonBoxAnimationDelay, ButtonBoxAnimationDelay)

    M_STYLE_ATTRIBUTE(int, titleBarAnimationDuration, TitleBarAnimationDuration)
    M_STYLE_ATTRIBUTE(qreal, titleBarAnimationDistance, TitleBarAnimationDistance)
    M_STYLE_ATTRIBUTE(QEasingCurve, titleBarAnimationEasingCurve, TitleBarAnimationEasingCurve)

    M_STYLE_ATTRIBUTE(int, contentsViewportOpacityAnimationDuration, ContentsViewportOpacityAnimationDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, contentsViewportOpacityAnimationEasingCurve, ContentsViewportOpacityAnimationEasingCurve)

    M_STYLE_ATTRIBUTE(int, contentsViewportScaleAnimationDuration, ContentsViewportScaleAnimationDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, contentsViewportScaleAnimationEasingCurve, ContentsViewportScaleAnimationEasingCurve)

    M_STYLE_ATTRIBUTE(int, buttonBoxAnimationDuration, ButtonBoxAnimationDuration)
    M_STYLE_ATTRIBUTE(qreal, buttonBoxAnimationDistance, ButtonBoxAnimationDistance)
    M_STYLE_ATTRIBUTE(QEasingCurve, buttonBoxAnimationEasingCurve, ButtonBoxAnimationEasingCurve)

    M_STYLE_ATTRIBUTE(qreal, scale, Scale)
    M_STYLE_ATTRIBUTE(qreal, opacity, Opacity)
};

class MDialogAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MDialogAnimationStyle)
};
//! \internal_end

#endif

