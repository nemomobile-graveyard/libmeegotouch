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
 
#ifndef MGRAPHICSHIGHLIGHTEFFECT_H
#define MGRAPHICSHIGHLIGHTEFFECT_H

#include <MGraphicsEffectStyle>

#include <QColor>

//! \internal

class MGraphicsHighlightEffectStyle : public MGraphicsEffectStyle
{
    Q_OBJECT
    M_STYLE(MGraphicsHighlightEffectStyle)

    M_STYLE_ATTRIBUTE(QColor, color, Color)
    M_STYLE_ATTRIBUTE(qreal, delay, Delay)
    M_STYLE_ATTRIBUTE(qreal, duration, Duration)
    M_STYLE_ATTRIBUTE(qreal, startStrength, StartStrength)
    M_STYLE_ATTRIBUTE(qreal, endStrength, EndStrength)

};

class MGraphicsHighlightEffectStyleContainer : public MGraphicsEffectStyleContainer
{
    M_STYLE_CONTAINER(MGraphicsHighlightEffectStyle)
};

//! \internal_end

#endif // MGRAPHICSHIGHLIGHTEFFECT_H
