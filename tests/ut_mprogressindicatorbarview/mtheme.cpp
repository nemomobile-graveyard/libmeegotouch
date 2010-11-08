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

#include <QString>
#include <QPixmap>
#include <QDebug>
#include <stdio.h>

#include <mtheme.h>
#include "ut_mbuttonview.h" // for themePixmap

MTheme::MTheme()
{
}

MTheme::~MTheme()
{
}

MTheme *MTheme::instance()
{
    static MTheme singleton;
    return &singleton;
}

QPixmap *MTheme::pixmap(const QString &name, int width, int height)
{
    Q_UNUSED(name);
    QPixmap *retVal = 0;

    if (!MButtonViewTest::themePixmap.contains(name))
        MButtonViewTest::themePixmap[ name ] = new QPixmap(width, height);

    retVal = MButtonViewTest::themePixmap[ name ];

    return retVal;
}

QPixmap *MTheme::pixmap(const QString &name, const QSizeF &size)
{
    return pixmap(name, (int)size.width(), (int)size.height());
}

QPixmap *MTheme::pixmap(const QString &name, const QSize &size)
{
    return pixmap(name, size.width(), size.height());
}
