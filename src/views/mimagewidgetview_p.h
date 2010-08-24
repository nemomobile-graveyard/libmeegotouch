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

#ifndef MIMAGEWIDGETVIEW_P_H
#define MIMAGEWIDGETVIEW_P_H

#include "private/mwidgetview_p.h"

class MImageWidget;

class MImageWidgetViewPrivate : public MWidgetViewPrivate
{
public:
    MImageWidgetViewPrivate();
    ~MImageWidgetViewPrivate();

    void calculateDrawRect(const QSizeF &imageSize);
    QSizeF calculateSourceSize(const QSizeF &imageSize);
    void calculateSourceRect(const QSizeF &imageSize);

    void checkPixmapSize();

    void drawBorders(QPainter *painter, const QRectF &drawRect) const;

    void applyStyle();

    const QPixmap *createPixmapFromTheme();
    QSize preferredImageSize();

    void updateImageGeometry();

    QRectF drawRect;
    QRectF sourceRect;

private:
    Q_DECLARE_PUBLIC(MImageWidgetView)
    MImageWidget *controller;

    QSize cachedPixmapSize;

    //border related
    qreal borderOpacity;
    QBrush brush;

    qreal leftBorder;
    qreal topBorder;
    qreal rightBorder;
    qreal bottomBorder;
};


#endif // MIMAGEWIDGETVIEW_P_H
