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

#ifndef MBASICLAYOUTANIMATIONSTYLE_H
#define MBASICLAYOUTANIMATIONSTYLE_H

#include <mlayoutanimationstyle.h>

#include <QtGlobal>
#include <QEasingCurve>

class MBasicLayoutAnimationStyle : public MLayoutAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MBasicLayoutAnimationStyle)

    M_STYLE_ATTRIBUTE(int, duration, Duration)
    M_STYLE_ATTRIBUTE(QEasingCurve, xTranslationEasingCurve, XTranslationEasingCurve)
    M_STYLE_ATTRIBUTE(QEasingCurve, yTranslationEasingCurve, YTranslationEasingCurve)
    M_STYLE_ATTRIBUTE(QEasingCurve, opacityEasingCurve, OpacityEasingCurve)
    M_STYLE_ATTRIBUTE(qreal, initialShowingOpacity, InitialShowingOpacity)
    M_STYLE_ATTRIBUTE(qreal, finalHidingOpacity, FinalHidingOpacity)
    M_STYLE_ATTRIBUTE(bool, animateOpacity, AnimateOpacity)
};

// TODO: get rid of this container
class M_CORE_EXPORT MBasicLayoutAnimationStyleContainer : public MLayoutAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MBasicLayoutAnimationStyle)
};

#endif
