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
 
#ifndef MGRAPHICSWIDGETDIMEFFECT_H
#define MGRAPHICSWIDGETDIMEFFECT_H

#include <QGraphicsEffect>
#include <MGraphicsEffect>

#include "mgraphicshighlighteffectstyle.h"

class MGraphicsHighlightEffectPrivate;

//! \internal

class MGraphicsHighlightEffect : public MGraphicsEffect
{
    Q_OBJECT
    Q_PROPERTY(qreal strength READ strength WRITE setStrength)
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor)

public:
    MGraphicsHighlightEffect(QObject *parent = NULL);
    virtual ~MGraphicsHighlightEffect();

    void setStrength(qreal strength);
    qreal strength() const;

    void setHighlightColor(const QColor &color);
    const QColor &highlightColor() const;

protected:
    virtual void draw(QPainter *painter);

private:
    M_GRAPHICS_EFFECT(MGraphicsHighlightEffectStyle)
    Q_DECLARE_PRIVATE(MGraphicsHighlightEffect)
    MGraphicsHighlightEffectPrivate *d_ptr;
};

//! \internal_end

#endif // MGRAPHICSWIDGETDIMEFFECT_H
