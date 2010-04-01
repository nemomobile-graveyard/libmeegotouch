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

#ifndef DUIIMAGEWIDGET_P_H
#define DUIIMAGEWIDGET_P_H

#include "private/duiwidgetcontroller_p.h"

class DuiImageWidget;

class DuiImageWidgetPrivate : public DuiWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(DuiImageWidget)

public:
    DuiImageWidgetPrivate();
    virtual ~DuiImageWidgetPrivate();

    void cleanUp();
    DuiImageWidgetPrivate &operator=(const DuiImageWidgetPrivate &other);
    void setImageName(const QString &imageName, const QSize &s = QSize());

    QSizeF imageDataSize() const;

    void deepCopy(const DuiImageWidget &);

    const QPixmap        *pixmap;
    QString               imageName;

    bool                  deletePixmap;
};

#endif
