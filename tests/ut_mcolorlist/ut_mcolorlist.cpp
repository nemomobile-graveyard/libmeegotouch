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

#include "ut_mcolorlist.h"

#include <QColor>
#include "mapplication.h"
#include "mcolorlist.h"

MApplication *app;

void Ut_MColorList::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcolorlist" };
    app = new MApplication(argc, app_name);
}

void Ut_MColorList::cleanupTestCase()
{
    delete app;
}

void Ut_MColorList::init()
{
    m_subject = new MColorList();
}

void Ut_MColorList::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MColorList::testSelectedColor()
{
    QColor color(Qt::blue);
    QSignalSpy colorPickedSpy(m_subject, SIGNAL(colorPicked()));

    m_subject->setSelectedColor(color);

    QCOMPARE(colorPickedSpy.count(), 1);
    QCOMPARE(m_subject->selectedColor(), color);
}

QTEST_APPLESS_MAIN(Ut_MColorList)
