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

#ifndef DUISCALABLEIMAGE_P_H
#define DUISCALABLEIMAGE_P_H

#include <QSize>
#include <QList>
#include <QPixmap>
#include <QMargins>

#include "duiscalableimage.h"

class DuiScalableImagePrivate
{
    Q_DECLARE_PUBLIC(DuiScalableImage)
public:

    enum DuiScalableImageType {
        DuiScalable9,
        DuiScalable1
        //DuiScalable3H,
        //DuiScalable3V,
    };

    DuiScalableImagePrivate();
    virtual ~DuiScalableImagePrivate();

    void validateSize() const;

    void drawScalable9(int x, int y, int w, int h, QPainter *painter) const;
    void drawScalable1(int x, int y, int w, int h, QPainter *painter) const;
    void drawScalable3H(int x, int y, int w, int h, QPainter *painter) const;
    void drawScalable3V(int x, int y, int w, int h, QPainter *painter) const;

    DuiScalableImageType m_imageType;

    const QPixmap *m_image;

    bool m_useGLRenderer;

    QMargins m_preferredMargins;

    QString pixmapId;

protected:
    DuiScalableImage *q_ptr;
};

#endif // DUIWIDGET_P_H
