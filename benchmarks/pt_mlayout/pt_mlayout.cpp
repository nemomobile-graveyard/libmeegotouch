/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QObject>
#include <MTheme>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <QGraphicsScene>
#include <MApplication>

#include "mclassfactory.h"
#include "pt_mlayout.h"

MApplication *app(NULL);

void Pt_MLayout::initTestCase()
{
    static int argc = 1;
    char *argv[1] = { (char *) "./pt_mlayout" };
    app = new MApplication(argc, argv);

}

void Pt_MLayout::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Pt_MLayout::init()
{
    m_scene = new QGraphicsScene;
    m_form = new QGraphicsWidget;
    m_scene->addItem(m_form);

}

void Pt_MLayout::cleanup()
{
}
void Pt_MLayout::layoutPerformance_data()
{
    QTest::addColumn<bool>("qtOnly");
    QTest::addColumn<bool>("testSizeHint");

    QTest::newRow("MLayout") << false << false;
    QTest::newRow("MLayout with sizeHint call") << false << true;
    QTest::newRow("QGraphicsLayout") << true << false;
    QTest::newRow("QGraphicsLayout with sizeHint call") << true << true;
}

void Pt_MLayout::layoutPerformance()
{
    QFETCH(bool, qtOnly);
    QFETCH(bool, testSizeHint);

    QGraphicsLinearLayout *qlayout;
    MLayout *layout;
    MLinearLayoutPolicy *policy;
    if(qtOnly) {
        qlayout = new QGraphicsLinearLayout(Qt::Vertical, m_form);
    } else {
        layout = new MLayout(m_form);
        policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    }

    MButton *a = new MButton;
    MButton *b = new MButton;

    if (qtOnly) {
        qlayout->addItem(a);
        qlayout->addItem(b);
    } else {
        policy->addItem(a);
        policy->addItem(b);
    }

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    QBENCHMARK {
        if (qtOnly) {
            qlayout->invalidate();
            qlayout->activate();
            if(testSizeHint)
                (void)qlayout->preferredSize();
        } else {
            layout->invalidate();
            layout->activate();
            if(testSizeHint)
                (void)layout->preferredSize();
        }
    }
}

QTEST_APPLESS_MAIN(Pt_MLayout)
