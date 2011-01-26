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

#ifndef MSCENEWINDOWSTYLE_H
#define MSCENEWINDOWSTYLE_H

#include <QPointF>
#include <mwidgetstyle.h>

class M_VIEWS_EXPORT MSceneWindowStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MSceneWindowStyle)

    M_STYLE_ATTRIBUTE(Qt::Alignment,  horizontalAlign,      HorizontalAlign)
    M_STYLE_ATTRIBUTE(Qt::Alignment,  verticalAlign,        VerticalAlign)
    M_STYLE_ATTRIBUTE(QPointF,        offset,               Offset)
    M_STYLE_ATTRIBUTE(int,            disappearTimeout,     DisappearTimeout)

    /*!
        \property MSceneWindowStyle::appearFeedback
        \brief Feedback given when scene window reaches the Appearing state.
      */
    M_STYLE_ATTRIBUTE(MFeedback, appearFeedback, AppearFeedback)

    /*!
       \property MSceneWindowStyle::appearanceAnimation
       \brief Appearance animation to be used.
        Format: class name
        E.g. "MWidgetSlideAnimation"

        If not set (left empty), the animation will be taken from the appropriate
        property in MSceneManagerStyle, according to the scene window type.
     */
    M_STYLE_ATTRIBUTE(QString, appearanceAnimation, AppearanceAnimation)

    /*!
       \property MSceneWindowStyle::disappearanceAnimation
       \brief Disappearance animation to be used.
        Format: class name
        E.g. "MWidgetSlideAnimation"

        If not set (left empty), the animation will be taken from the appropriate
        property in MSceneManagerStyle, according to the scene window type.
     */
    M_STYLE_ATTRIBUTE(QString, disappearanceAnimation, DisappearanceAnimation)
};

class M_VIEWS_EXPORT MSceneWindowStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MSceneWindowStyle)
};

#endif

