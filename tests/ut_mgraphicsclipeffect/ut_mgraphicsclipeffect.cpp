/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <MApplication>
#include <QPixmap>
#include "ut_mgraphicsclipeffect.h"
#include <QGraphicsView>

void Ut_MGraphicsClipEffect::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MGraphicsClipEffect" };
    app = new MApplication(argc, app_name);
}

void Ut_MGraphicsClipEffect::cleanupTestCase()
{
    delete app;
}

void Ut_MGraphicsClipEffect::init()
{
    clipEffect = new MGraphicsClipEffect (NULL);
}

void Ut_MGraphicsClipEffect::cleanup()
{
    delete clipEffect;
}

void Ut_MGraphicsClipEffect::testSetDrawRect()
{
    QRect rect (QPoint (0, 0), QPoint(100, 100));
    clipEffect->setDrawRect(rect);
    QCOMPARE (((MGraphicsClipEffectPrivate *) (clipEffect->d_ptr))->drawRectangle, rect);
    QCOMPARE (clipEffect->drawRect(), rect);
}

void Ut_MGraphicsClipEffect::testSetClipMargin()
{
    clipEffect->setClipMargin (MARGIN);
    QCOMPARE (((MGraphicsClipEffectPrivate *) (clipEffect->d_ptr))->margin, MARGIN);
    QCOMPARE (clipEffect->clipMargin(), MARGIN);
}


QTEST_APPLESS_MAIN(Ut_MGraphicsClipEffect)
