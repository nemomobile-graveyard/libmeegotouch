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

#include <qdrawutil.h>
#include <QVarLengthArray>
#include <QPainter>

class MScalableImagePrivate
{
    Q_DECLARE_PUBLIC(MScalableImage)
public:

    typedef QVarLengthArray<QPainter::PixmapFragment, 16> QPixmapFragmentsArray;

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

    void drawBorderPixmap(QPainter *painter,
                          const QRectF &targetRect,
                          const QMargins &targetMargins,
                          const QPixmap &pixmap,
                          const QRect &sourceRect,
                          const QMargins &sourceMargins,
                          const QTileRules &rules = QTileRules(),
                          QDrawBorderPixmap::DrawingHints hints = 0
                          );

    inline void drawBorderPixmap(QPainter *painter,
                                 const QRectF &target,
                                 const QMargins &margins,
                                 const QPixmap &pixmap) const
    {
        const_cast<MScalableImagePrivate*>(this)->drawBorderPixmap(painter, target, margins, pixmap, pixmap.rect(), margins);
    }


    MScalableImageType m_imageType;

    const QPixmap *m_image;

    QMargins m_preferredMargins;

    QString pixmapId;

protected:
    MScalableImage *q_ptr;
};

#endif // MWIDGET_P_H
