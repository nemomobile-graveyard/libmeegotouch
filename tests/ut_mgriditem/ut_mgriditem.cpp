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
#include "ut_mgriditem.h"
#include "mgriditem.h"
#include "mgriditem_p.h"

#define TITLE "testtitle"

void Ut_MGridItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mgriditem" };
    app = new MApplication(argc, app_name);
}

void Ut_MGridItem::cleanupTestCase()
{
    delete app;
}

void Ut_MGridItem::init()
{
    gridItem = new MGridItem ();
}

void Ut_MGridItem::cleanup()
{
    delete gridItem;
}

void Ut_MGridItem::testSetImage ()
{
    QString id (TITLE);

    gridItem->setImage(id);

    QCOMPARE (gridItem->image(), id);
}

void Ut_MGridItem::testSetPixmap ()
{
    QPixmap pixmap (100, 100);
    gridItem->setPixmap (pixmap);
    QCOMPARE ( ((MGridItemPrivate *) (gridItem->d_ptr))->pixmap, pixmap);
}

void Ut_MGridItem::testSetTitle ()
{
    QString title (TITLE);

    gridItem->setTitle (title);

    QCOMPARE (gridItem->title(), title);
}

void Ut_MGridItem::testSetSubtitle ()
{
    QString title (TITLE);

    gridItem->setSubtitle(title);

    QCOMPARE (gridItem->subtitle(), title);
}

void Ut_MGridItem::testSetImageVisible ()
{
    gridItem->setImageVisible(true);

    QCOMPARE (gridItem->isImageVisible(), true);

    gridItem->setImageVisible(false);

    QCOMPARE (gridItem->isImageVisible(), false);
}

void Ut_MGridItem::testSetTitleVisible ()
{
    gridItem->setTitleVisible(true);

    QCOMPARE (gridItem->isTitleVisible(), true);

    gridItem->setTitleVisible(false);

    QCOMPARE (gridItem->isTitleVisible(), false);
}

void Ut_MGridItem::testSetSubtitleVisible ()
{
    gridItem->setSubtitleVisible(true);

    QCOMPARE (gridItem->isSubtitleVisible(), true);

    gridItem->setSubtitleVisible(false);

    QCOMPARE (gridItem->isSubtitleVisible(), false);
}

void Ut_MGridItem::testIsImageVisible ()
{
    gridItem->setImageVisible(true);

    QCOMPARE (gridItem->isImageVisible(), true);

    gridItem->setImageVisible(false);

    QCOMPARE (gridItem->isImageVisible(), false);
}

void Ut_MGridItem::testIsTitleVisible ()
{
    gridItem->setTitleVisible(true);

    QCOMPARE (gridItem->isTitleVisible(), true);

    gridItem->setTitleVisible(false);

    QCOMPARE (gridItem->isTitleVisible(), false);
}

void Ut_MGridItem::testIsSubtitleVisible ()
{
    gridItem->setSubtitleVisible(true);

    QCOMPARE (gridItem->isSubtitleVisible(), true);

    gridItem->setSubtitleVisible(false);

    QCOMPARE (gridItem->isSubtitleVisible(), false);
}

void Ut_MGridItem::testSetSelected ()
{
    gridItem->setSelected(true);

    QCOMPARE (gridItem->isSelected(), true);

    gridItem->setSelected(false);

    QCOMPARE (gridItem->isSelected(), false);
}


QTEST_APPLESS_MAIN(Ut_MGridItem)
