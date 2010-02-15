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

#ifndef GALLERYVIEW_H
#define GALLERYVIEW_H

#include <duiwidgetview.h>
#include <QRectF>
#include <QPainterPath>
#include <QPen>

class Gallery;

class GalleryView : public DuiWidgetView
{
    Q_OBJECT

public:
    GalleryView(Gallery *controller);
    virtual ~GalleryView();

protected:
    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    //! \reimp_end

private:
    //! Controller
    Gallery *controller;
};

#endif
