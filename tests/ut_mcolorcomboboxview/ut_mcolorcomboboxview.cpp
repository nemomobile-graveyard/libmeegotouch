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

#include "ut_mcolorcomboboxview.h"

#include <QColor>
#include <QSignalSpy>
#include <QGraphicsSceneMouseEvent>
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mcolorcomboboxview.h"
#include "mcolorcomboboxbutton.h"
#include "mcolorcombobox.h"
#include "mcolorlist.h"

MApplication *app;

void Ut_MColorComboBoxView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcolorcomboboxview" };
    app = new MApplication(argc, app_name);
}

void Ut_MColorComboBoxView::cleanupTestCase()
{
    delete app;
}

void Ut_MColorComboBoxView::init()
{
    m_combo = new MColorComboBox();
    m_subject = new MColorComboBoxView(m_combo);
    m_combo->setView(m_subject);
}

void Ut_MColorComboBoxView::cleanup()
{
    delete m_combo;
}

void Ut_MColorComboBoxView::testSetTitle()
{
    QString title("test");
    m_combo->setTitle(title);

    QVERIFY(m_subject->d_func()->popup);
    QCOMPARE(m_subject->d_func()->popup->title(), title);
}

void Ut_MColorComboBoxView::testSelectColor()
{
    MApplicationWindow win;
    win.show();

    MColorComboBoxButton *button = m_subject->d_func()->button;
    QVERIFY(button);

    // click the button and check that popup is launched
    QSignalSpy clickedSpy(button, SIGNAL(clicked()));
    button->click();
    QCOMPARE(clickedSpy.count(), 1);
    QTest::qWait(1000);
    QVERIFY(m_subject->d_func()->popup->isOnDisplay());

    //select a color and check that the popup gets closed
    MColorList *colorList = m_subject->d_func()->widget;
    QVERIFY(colorList);

    QSignalSpy colorPickedSpy(colorList, SIGNAL(colorPicked()));
    QColor color(Qt::blue);
    colorList->setSelectedColor(color);
    QCOMPARE(colorPickedSpy.count(), 1);

    QTest::qWait(1000);
    QVERIFY(!m_subject->d_func()->popup->isOnDisplay());
}


QTEST_APPLESS_MAIN(Ut_MColorComboBoxView)
