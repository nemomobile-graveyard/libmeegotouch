/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "ut_duipositionindicator.h"
#include <duipositionindicator.h>
#include "../../src/corelib/widgets/duipositionindicator_p.h"
#include <duipositionindicatorview.h>
#include <duiapplication.h>

#include <QTest>

DuiApplication *app;

void Ut_DuiPositionIndicator::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duipositionindicator" };
    app = new DuiApplication(argc, app_name);
}


void Ut_DuiPositionIndicator::cleanupTestCase()
{
    delete app;
}

void Ut_DuiPositionIndicator::init()
{
    m_subject = new DuiPositionIndicator(0);
}

void Ut_DuiPositionIndicator::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiPositionIndicator::updateSizePosData_data()
{
    QTest::addColumn<QSizeF>("viewportSize");
    QTest::addColumn<QRectF>("pannedRange");
    QTest::addColumn<QPointF>("pannedPos");

    QTest::newRow("position 1") << QSizeF(10.003, 466.011) << QRectF(0, 0, 122.090, 500.98798) << QPointF(300.003, -40102112);
    QTest::newRow("position 2") << QSizeF(500.292, 0.00033) << QRectF(0, 0, 690.784, 9.1910) << QPointF(-300.003, 0.0003000003);
    QTest::newRow("position 3") << QSizeF(0.0000000001, 200.003273218) << QRectF(0, 0, 122.93847, 943.1991918) << QPointF(0.0000000001, -0.003273218);
}

//Update new position of position indicator
void Ut_DuiPositionIndicator::updateSizePosData()
{
    QFETCH(QSizeF, viewportSize);
    QFETCH(QRectF, pannedRange);
    QFETCH(QPointF, pannedPos);

    m_subject->setPosition(pannedPos);
    m_subject->setRange(pannedRange);
    m_subject->setViewportSize(viewportSize);

    QCOMPARE(m_subject->viewportSize(), viewportSize);
    QCOMPARE(m_subject->range(), pannedRange);
    QCOMPARE(m_subject->position(), pannedPos);

}

QTEST_APPLESS_MAIN(Ut_DuiPositionIndicator)
