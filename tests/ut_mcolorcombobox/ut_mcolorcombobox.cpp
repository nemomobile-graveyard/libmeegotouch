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

#include "ut_mcolorcombobox.h"

#include <QColor>
#include "mapplication.h"
#include <mcolorcombobox.h>

MApplication *app;

void Ut_MColorComboBox::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcolorcombobox" };
    app = new MApplication(argc, app_name);
}

void Ut_MColorComboBox::cleanupTestCase()
{
    delete app;
}

void Ut_MColorComboBox::init()
{
    m_subject = new MColorComboBox();
}

void Ut_MColorComboBox::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MColorComboBox::testTitle()
{
    QString title("Test");
    m_subject->setTitle(title);
    QCOMPARE(m_subject->title(), title);
}

void Ut_MColorComboBox::testPickedColor()
{
    QColor color("red");
    m_subject->setPickedColor(color);
    QCOMPARE(m_subject->pickedColor(), color);
}

QTEST_APPLESS_MAIN(Ut_MColorComboBox)
