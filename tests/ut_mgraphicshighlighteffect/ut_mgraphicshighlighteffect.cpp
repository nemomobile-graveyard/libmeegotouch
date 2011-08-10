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

#include "ut_mgraphicshighlighteffect.h"

#include <MApplication>
#include <MTheme>
#include <mgraphicshighlighteffectstyle.h>

#include <QPixmap>
#include <QGraphicsView>

void Ut_MGraphicsHighlightEffect::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MGraphicsHighlightEffect" };
    app = new MApplication(argc, app_name);
}

void Ut_MGraphicsHighlightEffect::cleanupTestCase()
{
    delete app;
}

void Ut_MGraphicsHighlightEffect::init()
{
    effect = qobject_cast<MGraphicsHighlightEffect*>(MTheme::effect("MGraphicsHighlightEffect"));
    QVERIFY(effect);
}

void Ut_MGraphicsHighlightEffect::cleanup()
{
    delete effect;
    effect = NULL;
}

void Ut_MGraphicsHighlightEffect::testSetStrength()
{
    effect->setStrength((qreal)0.5f);
    QCOMPARE(effect->strength(), (qreal)0.5f);
}

void Ut_MGraphicsHighlightEffect::testSetHighlightColor()
{
    effect->setHighlightColor(Qt::lightGray);
    QCOMPARE(effect->highlightColor(), QColor(Qt::lightGray));
}

QTEST_APPLESS_MAIN(Ut_MGraphicsHighlightEffect)
