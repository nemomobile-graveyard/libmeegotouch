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
#ifndef MCONTENTFADEANDSLIDEANIMATIONSTYLE_H
#define MCONTENTFADEANDSLIDEANIMATIONSTYLE_H

#include "mabstractwidgetanimationstyle.h"
#include <QEasingCurve>
#include <QPointF>

//! \internal
class MContentFadeAndSlideAnimationStyle : public MAbstractWidgetAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MContentFadeAndSlideAnimationStyle)

    M_STYLE_ATTRIBUTE(QString, contentObjectName, ContentObjectName)

    M_STYLE_ATTRIBUTE(qreal, fadeOutOpacity, FadeOutOpacity)
    M_STYLE_ATTRIBUTE(int, fadeOutDuration, FadeOutDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, fadeOutEasingCurve, FadeOutEasingCurve)

    M_STYLE_ATTRIBUTE(QPointF, slideOutDisplacement, SlideOutDisplacement)
    M_STYLE_ATTRIBUTE(int, slideOutDuration, SlideOutDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, slideOutEasingCurve, SlideOutEasingCurve)

    M_STYLE_ATTRIBUTE(int, delay, Delay)

    M_STYLE_ATTRIBUTE(qreal, fadeInOpacity, FadeInOpacity)
    M_STYLE_ATTRIBUTE(int, fadeInDuration, FadeInDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, fadeInEasingCurve, FadeInEasingCurve)

    M_STYLE_ATTRIBUTE(QPointF, slideInDisplacement, SlideInDisplacement)
    M_STYLE_ATTRIBUTE(int, slideInDuration, SlideInDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, slideInEasingCurve, SlideInEasingCurve)
};

class MContentFadeAndSlideAnimationStyleContainer : public MAbstractWidgetAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MContentFadeAndSlideAnimationStyle)
};
//! \internal_end

#endif // MContentFadeAndSlideAnimationmationSTYLE_H
