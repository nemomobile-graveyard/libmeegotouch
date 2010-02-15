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

#include <QString>
#include <QPixmap>
#include <QDebug>
#include <stdio.h>

#include <duitheme.h>
#include "ut_duibuttonview.h" // for themePixmap

DuiTheme::DuiTheme()
{
}

DuiTheme::~DuiTheme()
{
}

DuiTheme *DuiTheme::instance()
{
    static DuiTheme singleton;
    return &singleton;
}

QPixmap *DuiTheme::pixmap(const QString &name, int width, int height)
{
    Q_UNUSED(name);
    QPixmap *retVal = 0;

    if (!DuiButtonViewTest::themePixmap.contains(name))
        DuiButtonViewTest::themePixmap[ name ] = new QPixmap(width, height);

    retVal = DuiButtonViewTest::themePixmap[ name ];

    return retVal;
}

QPixmap *DuiTheme::pixmap(const QString &name, const QSizeF &size)
{
    return pixmap(name, (int)size.width(), (int)size.height());
}

QPixmap *DuiTheme::pixmap(const QString &name, const QSize &size)
{
    return pixmap(name, size.width(), size.height());
}
