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

#ifndef DUIWIDGETFADEINANIMATIONSTYLE_H
#define DUIWIDGETFADEINANIMATIONSTYLE_H

#include <duiabstractwidgetanimationstyle.h>
#include <QEasingCurve>

//! \internal
class DUI_EXPORT DuiWidgetFadeInAnimationStyle : public DuiAbstractWidgetAnimationStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiWidgetFadeInAnimationStyle)

    DUI_STYLE_ATTRIBUTE(qreal, opacity, Opacity)
    DUI_STYLE_ATTRIBUTE(QEasingCurve, easingCurve, EasingCurve)
};

class DUI_EXPORT DuiWidgetFadeInAnimationStyleContainer : public DuiAbstractWidgetAnimationStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiWidgetFadeInAnimationStyle)
};
//! \internal_end

#endif

