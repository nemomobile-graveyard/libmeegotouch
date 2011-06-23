/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QAbstractAnimation>
#include <msheetview.h>
#include <msheet.h>

#include "ut_msheetview.h"
#include "msheetview_p.h"

#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mbasicsheetheader.h"
#include "qanimationgroup.h"

MApplication *app;

Ut_MSheetView::Ut_MSheetView() :
        subject(0)
{
}

void Ut_MSheetView::initTestCase()
{
    static int argc = 2;
    static char *app_name[2] = { (char *) "./ut_msheetview", (char *) "-software" };
    app = new MApplication(argc, app_name);

    sheet = new MSheet;
    subject = new MSheetView(sheet);
    sheet->setView(subject);

    window = new MApplicationWindow;
    window->show();

    QTest::qWaitForWindowShown(window);

    sheet->appear(window);
    sheet->setHeaderWidget(new MBasicSheetHeader);
}

void Ut_MSheetView::cleanupTestCase()
{
    delete sheet;
    sheet = 0;
    delete app;
    app = 0;
}

void Ut_MSheetView::testHeaderHiding()
{
    QTest::qWaitForWindowShown(window);

    QRectF initHeaderGeometry = subject->d_func()->headerSlot->geometry();
    QRectF initCentralGeometry = subject->d_func()->centralSlot->geometry();

    // check initial conditions
    QVERIFY(sheet->isVisible() == true);
    QVERIFY(sheet->headerWidget()->isVisible() == true);


    // switch off header visibility
    sheet->setHeaderVisible(false);
    QVERIFY(subject->d_func()->headerHidingAnimation->state() == QAbstractAnimation::Running);
    QVERIFY(subject->d_func()->headerSlot->isVisible() == true);

    // fast-forward animations
    subject->d_func()->headerHidingAnimation->setCurrentTime(subject->d_func()->headerHidingAnimation->duration());

    // and next the header should become hidden and animation should be stopped
    QVERIFY(subject->d_func()->headerHidingAnimation->state() == QAbstractAnimation::Stopped);
    QVERIFY(subject->d_func()->headerSlot->isVisible() == false);

    QApplication::processEvents();

    // check header and central slot geometries after visibility change
    // header should disappear and the central slot should
    // fill all the available space
    QVERIFY(subject->d_func()->headerSlot->x() == initHeaderGeometry.x());
    QVERIFY(subject->d_func()->headerSlot->y() == -initHeaderGeometry.height());
    QVERIFY(subject->d_func()->headerSlot->geometry().width() == initHeaderGeometry.width());
    QVERIFY(subject->d_func()->headerSlot->geometry().height() == initHeaderGeometry.height());
    QVERIFY(subject->d_func()->centralSlot->x() == initHeaderGeometry.x());
    QVERIFY(subject->d_func()->centralSlot->y() == initHeaderGeometry.y());
    QVERIFY(subject->d_func()->centralSlot->geometry().width() == initCentralGeometry.width());
    QVERIFY(subject->d_func()->centralSlot->geometry().height() == initCentralGeometry.height() + initHeaderGeometry.height());

    // check conditons when visiblity status hasn't been changed
    sheet->setHeaderVisible(false);
    QVERIFY(subject->d_func()->headerHidingAnimation->state() == QAbstractAnimation::Stopped);
    QVERIFY(subject->d_func()->headerSlot->isVisible() == false);


    // switch on header visibility
    sheet->setHeaderVisible(true);
    QVERIFY(subject->d_func()->headerHidingAnimation->state() == QAbstractAnimation::Running);
    QVERIFY(subject->d_func()->headerSlot->isVisible() == true);

    // fast-forward animations. OBS: it's running backwards now
    subject->d_func()->headerHidingAnimation->setCurrentTime(0);

    // and next the header should become invisible and animation should be stopped
    QVERIFY(subject->d_func()->headerHidingAnimation->state() == QAbstractAnimation::Stopped);
    QVERIFY(subject->d_func()->headerSlot->isVisible() == true);

    QApplication::processEvents();

    // check header and central slot geometries after visibility change
    // header should be appeared and the central slot should be
    // shrunk by height of the header
    QVERIFY(subject->d_func()->headerSlot->geometry() == initHeaderGeometry);
    QVERIFY(subject->d_func()->centralSlot->geometry() == initCentralGeometry);

    // check conditons when visiblity status hasn't been changed
    sheet->setHeaderVisible(true);
    QVERIFY(subject->d_func()->headerHidingAnimation->state() == QAbstractAnimation::Stopped);
    QVERIFY(subject->d_func()->headerSlot->isVisible() == true);
}

QTEST_APPLESS_MAIN(Ut_MSheetView)
