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

#ifndef MSCENELAYEREFFECTDIMSTYLE_H
#define MSCENELAYEREFFECTDIMSTYLE_H

#include <QEasingCurve>
#include <mscenelayereffectstyle.h>

class M_VIEWS_EXPORT MSceneLayerEffectDimStyle : public MSceneLayerEffectStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MSceneLayerEffectDimStyle)

    M_STYLE_ATTRIBUTE(qreal,        opacity, Opacity)
    M_STYLE_ATTRIBUTE(QEasingCurve, easingCurve, EasingCurve)
    M_STYLE_ATTRIBUTE(int,          fadeDuration, FadeDuration)
};

class M_VIEWS_EXPORT MSceneLayerEffectDimStyleContainer : public MSceneLayerEffectStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MSceneLayerEffectDimStyle)
};

#endif
