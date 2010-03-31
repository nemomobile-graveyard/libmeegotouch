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

#ifndef DUIWIDGETFADEOUTANIMATION_P_H
#define DUIWIDGETFADEOUTANIMATION_P_H

#include "duiabstractwidgetanimation_p.h"

class QPropertyAnimation;

class DuiWidgetFadeOutAnimationPrivate : public DuiAbstractWidgetAnimationPrivate
{
    Q_DECLARE_PUBLIC(DuiWidgetFadeOutAnimation)
public:
    virtual ~DuiWidgetFadeOutAnimationPrivate() {}
private:
    qreal originalOpacity;
    bool played;
    QPropertyAnimation *opacityAnimation;
};

#endif
