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

#include "ft_mslider.h"
#include <QtTest>
#include <QImage>
#include "mapplication.h"
#include "mslider.h"
#include "mtheme.h"

MApplication *app;

void Ft_MSlider::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ft_mslider" };
    app = new MApplication(argc, argv);

    MTheme::loadCSS("msliderstyle.css");
}

void Ft_MSlider::cleanupTestCase()
{
    delete app;
}

void Ft_MSlider::sliderResize()
{
    MSlider *s = new MSlider();

    s->setMinimumSize(310, 70);
    s->resize(310, 70);
    QCOMPARE(s->size(), QSizeF(310, 70));

    s->setMinimumSize(100, 400);
    s->resize(100, 400);
    QCOMPARE(s->size(), QSizeF(100, 400));

    s->setMinimumSize(310, 70);
    s->resize(310, 70);
    QCOMPARE(s->size(), QSizeF(310, 70));

    delete s;
}

QTEST_APPLESS_MAIN(Ft_MSlider)


