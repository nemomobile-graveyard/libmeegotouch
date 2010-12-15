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

#ifndef MSCALABLEIMAGE_P_H
#define MSCALABLEIMAGE_P_H

#include <QSize>
#include <QList>
#include <QPixmap>
#include <QMargins>

#include "mscalableimage.h"

class QPainter;

class MScalableImagePrivate
{
    Q_DECLARE_PUBLIC(MScalableImage)
public:
    enum MScalableImageType {
        MScalable9,
        MScalable1
        //MScalable3H,
        //MScalable3V,
    };

    MScalableImagePrivate();
    virtual ~MScalableImagePrivate();

    void validateSize() const;

    void drawScalable9(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const;
    void drawScalable1(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const;
    //void drawScalable3H(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const;
    //void drawScalable3V(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const;

    void outputDownscaleWarning(const QString& origin, qreal w, qreal h) const;

    MScalableImageType m_imageType;

    const QPixmap *m_image;

    QMargins m_preferredMargins;

    QString pixmapId;
    QString cachedImageKey;

protected:
    MScalableImage *q_ptr;
    bool downscaleWarningPrinted;
};

#endif // MSCALABLEIMAGE_P_H
