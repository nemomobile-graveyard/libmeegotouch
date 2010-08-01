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
 

#ifndef MWIDGETRECTANGULARCLIPANIMATIONSTYLE_H
#define MWIDGETRECTANGULARCLIPANIMATIONSTYLE_H

#include <mabstractwidgetanimationstyle.h>
#include <QEasingCurve>

class MWidgetRectangularClipAnimationStyle : public MAbstractWidgetAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MWidgetRectangularClipAnimationStyle)

    M_STYLE_ATTRIBUTE(QEasingCurve, easingCurve, EasingCurve)
    M_STYLE_ATTRIBUTE(int, clipMargin, ClipMargin)
};

class MWidgetRectangularClipAnimationStyleContainer : public MAbstractWidgetAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MWidgetRectangularClipAnimationStyle)
};

#endif // MWIDGETRECTANGULARCLIPANIMATIONSTYLE_H
