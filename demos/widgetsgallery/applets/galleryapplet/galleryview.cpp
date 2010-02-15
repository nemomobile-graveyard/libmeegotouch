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

#include "galleryview.h"
#include "gallery.h"
#include <QDebug>
#include <QPainter>
#include <DuiTheme>
#include <DuiButton>
#include <QByteArray>
#include <QBuffer>


GalleryView::GalleryView(Gallery *i) :
    DuiWidgetView(i),
    controller(i)
{
}

GalleryView::~GalleryView()
{
}

QSizeF GalleryView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    return QSizeF(256, 256);
}
