/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
 

#ifndef MGRAPHICSCLIPEFFECT_H
#define MGRAPHICSCLIPEFFECT_H

#include <QGraphicsEffect>

class MGraphicsClipEffectPrivate;

//! \internal
class MGraphicsClipEffect : public QGraphicsEffect
{
    Q_OBJECT
    Q_PROPERTY(QRect drawRect READ drawRect WRITE setDrawRect)

#ifdef UNIT_TEST
    friend class Ut_MGraphicsClipEffect;
#endif

public:
    MGraphicsClipEffect(QObject *parent);
    virtual ~MGraphicsClipEffect();

    void setDrawRect(const QRect &rect);
    QRect drawRect() const;

    void setClipMargin(int margin);
    int clipMargin() const;

protected:
    virtual void draw(QPainter *painter);

private:
    Q_DECLARE_PRIVATE(MGraphicsClipEffect)
    MGraphicsClipEffectPrivate *d_ptr;
};
//! \internal_end

#endif // MGRAPHICSWIDGETCLIPEFFECT_H
