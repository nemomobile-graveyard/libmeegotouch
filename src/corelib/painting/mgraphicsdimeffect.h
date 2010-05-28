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

#ifndef MGRAPHICSDIMEFFECT_H
#define MGRAPHICSDIMEFFECT_H

#include "mgraphicseffect.h"
#include "mgraphicsdimeffectstyle.h"

class MGraphicsDimEffectPrivate;

//! \internal
/*!
\class MGraphicsDimEffect
\brief The MGraphicsDimEffect class provides a dimming effect

\section Detailed Description
    The MGraphicsDimEffect class provides a dimming effect.
    A dim effect renders the source with a dimming value. This effect is useful
    for making items have a darker rendering effect.
*/
class MGraphicsDimEffect : public MGraphicsEffect
{
    Q_OBJECT
    M_GRAPHICS_EFFECT(MGraphicsDimEffectStyle)

public:
    MGraphicsDimEffect(QObject * p = 0);
    virtual ~MGraphicsDimEffect();

protected:
    //! \reimp
    virtual void draw(QPainter* painter);
    virtual void sourceChanged(ChangeFlags flags);
    //! \reimp_end

    virtual qreal baseRatio() const;

private:
    Q_DECLARE_PRIVATE(MGraphicsDimEffect)
    MGraphicsDimEffectPrivate *d_ptr;
};
//! \internal_end

#endif
