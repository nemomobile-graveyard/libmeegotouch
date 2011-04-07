/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSAREASLIDEANIMATIONSTYLE_H
#define MSTATUSAREASLIDEANIMATIONSTYLE_H

#include <mabstractwidgetanimationstyle.h>
#include <QEasingCurve>

//! \internal
class MStatusAreaSlideAnimationStyle : public MAbstractWidgetAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MStatusAreaSlideAnimationStyle)

    M_STYLE_ATTRIBUTE(int, statusBarSlideDelay, StatusBarSlideDelay)
    M_STYLE_ATTRIBUTE(QEasingCurve, statusBarSlideEasingCurve, StatusBarSlideEasingCurve)
    M_STYLE_ATTRIBUTE(int, statusBarSlideDuration, StatusBarSlideDuration)

    M_STYLE_ATTRIBUTE(int, widgetSlideDelay,WidgetSlideDelay)
    M_STYLE_ATTRIBUTE(QEasingCurve, widgetSlideEasingCurve, WidgetSlideEasingCurve)
    M_STYLE_ATTRIBUTE(int, widgetSlideDuration, WidgetSlideDuration)
};

class MStatusAreaSlideAnimationStyleContainer : public MAbstractWidgetAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MStatusAreaSlideAnimationStyle)
};
//! \internal_end

#endif

