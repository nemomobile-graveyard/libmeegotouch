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

#ifndef MIMAGEWIDGET_P_H
#define MIMAGEWIDGET_P_H

#include "private/mwidgetcontroller_p.h"

class MImageWidget;

class M_CORE_EXPORT MImageWidgetPrivate : public MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MImageWidget)

public:
    MImageWidgetPrivate();
    virtual ~MImageWidgetPrivate();

    void cleanUp();

    QSizeF imageDataSize(const QRectF& cropRect) const;

    void setPixmap(const QPixmap* pixmap, bool takeOwnership);
    void setImage(const QImage& image);

    const QPixmap* getPixmap();
    const QImage& getImage();
    void init();

    mutable QPixmap imagePlaceHolder;

private:
    const QPixmap *pixmap;
    QImage image;

    bool ownPixmap;
};

#endif
