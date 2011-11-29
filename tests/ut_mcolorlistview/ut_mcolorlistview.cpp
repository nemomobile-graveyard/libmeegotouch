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
#include "ut_mcolorlistview.h"
#include <QColor>
#include <QSignalSpy>
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mcolorlist.h"
#include "mcolorlistview.h"
#include "mcolorlistview_p.h"
#include "mbutton.h"
#include "mbuttongroup.h"

MApplication *app;

void Ut_MColorListView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcolorlistview" };
    app = new MApplication(argc, app_name);
}

void Ut_MColorListView::cleanupTestCase()
{
    delete app;
}

void Ut_MColorListView::init()
{
    m_colorList = new MColorList();
    m_subject = new MColorListView(m_colorList);
    m_colorList->setView(m_subject);
}

void Ut_MColorListView::cleanup()
{
    delete m_colorList;
}

void Ut_MColorListView::testSelectColor()
{
    qRegisterMetaType<MButton*>("MButton*");

    QList<MButton*> buttons = m_subject->d_func()->buttonGroup->buttons();

    // click on every button and verify that they get checked and 
    // the selected color is applied to the controller as well
    for (int i = 0; i < buttons.size(); i++) {
        MButton* button = buttons[i];
        QSignalSpy clickedSpy(m_subject->d_func()->buttonGroup, SIGNAL(buttonClicked(MButton*)));
        button->click();
        QCOMPARE(clickedSpy.count(), 1);

        QVERIFY(button->isChecked());
        QCOMPARE(m_colorList->selectedColor(), m_subject->d_func()->colors[i]);
    }
}

QTEST_APPLESS_MAIN(Ut_MColorListView)
