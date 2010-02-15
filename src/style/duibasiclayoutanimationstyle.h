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

#ifndef DUIBASICLAYOUTANIMATIONSTYLE_H
#define DUIBASICLAYOUTANIMATIONSTYLE_H

#include <duilayoutanimationstyle.h>

#include <QtGlobal>
#include <QEasingCurve>

class DUI_EXPORT DuiBasicLayoutAnimationStyle : public DuiLayoutAnimationStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiBasicLayoutAnimationStyle)

    DUI_STYLE_ATTRIBUTE(int, duration, Duration)
    DUI_STYLE_ATTRIBUTE(QEasingCurve, geometryEasingCurve, GeometryEasingCurve)
    DUI_STYLE_ATTRIBUTE(QEasingCurve, opacityEasingCurve, OpacityEasingCurve)
    DUI_STYLE_ATTRIBUTE(qreal, initialShowingOpacity, InitialShowingOpacity)
    DUI_STYLE_ATTRIBUTE(qreal, initialShowingScaleFactor, InitialShowingScaleFactor)
    DUI_STYLE_ATTRIBUTE(qreal, finalHidingOpacity, FinalHidingOpacity)
    DUI_STYLE_ATTRIBUTE(qreal, finalHidingScaleFactor, FinalHidingScaleFactor)
};

// TODO: get rid of this container
class DUI_EXPORT DuiBasicLayoutAnimationStyleContainer : public DuiLayoutAnimationStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiBasicLayoutAnimationStyle)
};

#endif
