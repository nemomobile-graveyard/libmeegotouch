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

#include "ut_mcolorcomboboxbutton.h"

#include <QColor>
#include <MLabel>
#include <mcolorcomboboxbutton.h>
#include <mcolorwidget.h>
#include "mapplication.h"

MApplication *app;

void Ut_MColorComboBoxButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcolorcomboboxbutton" };
    app = new MApplication(argc, app_name);
}

void Ut_MColorComboBoxButton::cleanupTestCase()
{
    delete app;
}

void Ut_MColorComboBoxButton::init()
{
    m_subject = new MColorComboBoxButton();
}

void Ut_MColorComboBoxButton::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MColorComboBoxButton::testTitle()
{
    QString title("title");
    m_subject->setTitle(title);

    QCOMPARE(m_subject->titleLabel->text(), title);
}

void Ut_MColorComboBoxButton::testTitleStyleName()
{
    QString titleStyleName("titleStyleName");
    m_subject->setTitleStyleName(titleStyleName);

    QCOMPARE(m_subject->titleLabel->styleName(), titleStyleName);
}

void Ut_MColorComboBoxButton::testColor()
{
    QColor color(Qt::blue);
    m_subject->setColor(color);

    QCOMPARE(m_subject->colorWidget->color(), color);
}

void Ut_MColorComboBoxButton::testColorStyleName()
{
    QString colorStyleName("colorStyleName");
    m_subject->setColorStyleName(colorStyleName);

    QCOMPARE(m_subject->colorWidget->styleName(), colorStyleName);
}

QTEST_APPLESS_MAIN(Ut_MColorComboBoxButton)
