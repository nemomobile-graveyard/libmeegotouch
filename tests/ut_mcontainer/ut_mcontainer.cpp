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
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>
#include <QWeakPointer>

#include <mcontainer.h>
#include "mcontainer_p.h"
#include <mcontainerview.h>
#include "views/mcontainerview_p.h"

#include "ut_mcontainer.h"
#include "mapplication.h"

#include "mcancelevent.h"

void Ut_MContainer::init()
{
    m_subject = new MContainer();
}

void Ut_MContainer::cleanup()
{
    delete m_subject;
    m_subject = 0;
}
MApplication *app;

void Ut_MContainer::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcontainer" };
    app = new MApplication(argc, app_name);
}

void Ut_MContainer::cleanupTestCase()
{
    delete app;
}

void Ut_MContainer::centralWidget()
{
    // check that there exists centralWidget
    QVERIFY(m_subject->centralWidget() != NULL);
}

void Ut_MContainer::setCentralWidget()
{
    MWidget *tmp = new MWidget();

    m_subject->setCentralWidget(tmp);

    // check that there exists centralWidget
    QVERIFY(m_subject->centralWidget() == tmp);
}

void Ut_MContainer::replaceCentralWidget()
{
    QGraphicsWidget *widget = new QGraphicsWidget;
    QWeakPointer<QGraphicsWidget> widgetPointer(widget);

    m_subject->setCentralWidget(widget);

    QVERIFY(m_subject->centralWidget() == widget);

    m_subject->setCentralWidget(0);

    QVERIFY(m_subject->centralWidget() == 0);
    QCOMPARE(widgetPointer.isNull(), true);
}

void Ut_MContainer::replaceCentralWidgetWithoutDestroying()
{
    QGraphicsWidget *widget = new QGraphicsWidget;
    QWeakPointer<QGraphicsWidget> widgetPointer(widget);

    m_subject->setCentralWidget(widget);

    QVERIFY(m_subject->centralWidget() == widget);

    m_subject->setCentralWidget(0, false);

    QVERIFY(m_subject->centralWidget() == 0);
    QCOMPARE(widgetPointer.isNull(), false);
    QVERIFY(widget->parentItem() == 0);
    QVERIFY(widget->parent() == 0);
    QVERIFY(widget->scene() == 0);

    // clean up
    delete widget;
}

void Ut_MContainer::title()
{
    QString myQString("testing title()");

    m_subject->setTitle(myQString);
    QCOMPARE(m_subject->title(), myQString);
}

void Ut_MContainer::text()
{
    QString myQString("testing text()");

    m_subject->setText(myQString);
    QCOMPARE(m_subject->text(), myQString);
}

void Ut_MContainer::iconID()
{
    QString myIconID("testing iconID()");

    m_subject->setIconID(myIconID);
    QCOMPARE(m_subject->iconID(), myIconID);
}

void Ut_MContainer::headerVisible()
{
    // default state
    QVERIFY(m_subject->headerVisible() == true);

    m_subject->setHeaderVisible(false);

    QVERIFY(m_subject->headerVisible() == false);
}

void Ut_MContainer::toggleHeaderVisible()
{
    // default state
    QVERIFY(m_subject->headerVisible() == true);

    m_subject->toggleHeaderVisible();

    QVERIFY(m_subject->headerVisible() == false);

    m_subject->toggleHeaderVisible();

    QVERIFY(m_subject->headerVisible() == true);
}


void Ut_MContainer::toggleProgressIndicatorVisible()
{
    // default state
    QVERIFY(m_subject->isProgressIndicatorVisible() == false);

    m_subject->toggleProgressIndicatorVisible();

    QVERIFY(m_subject->isProgressIndicatorVisible() == true);

    m_subject->toggleProgressIndicatorVisible();

    QVERIFY(m_subject->isProgressIndicatorVisible() == false);
}

void Ut_MContainer::testCreatingViewImmediatelyInLayout()
{
    //Add the container to a layout then force the view to be created by calling minimumSize()
    QGraphicsLinearLayout layout(Qt::Horizontal);
    layout.addItem(m_subject);
    m_subject->minimumSize();

}
void Ut_MContainer::testCreatingViewImmediately()
{
    //Force the view to be created by calling minimumSize()
    //This checks BUG:126088 which causes a segmentation fault
    m_subject->minimumSize();
}

void Ut_MContainer::testCancelEvent()
{
    QSignalSpy spy(m_subject, SIGNAL(headerClicked()));

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(&mouseEvent);

    MCancelEvent event;
    m_subject->cancelEvent(&event);

    QVERIFY(spy.count() == 0);
}

void Ut_MContainer::testSurviveCentralWidgetDoubleDelete()
{
    QGraphicsWidget *newCentralWidget = new QGraphicsWidget(m_subject);
    QGraphicsWidget *anotherCentralWidget = new QGraphicsWidget(m_subject);
    m_subject->setCentralWidget(newCentralWidget);
    delete newCentralWidget;
    m_subject->setCentralWidget(anotherCentralWidget);
    QVERIFY(true);
}

void Ut_MContainer::testSurviveCentralWidgetReplace()
{
    QGraphicsWidget *newCentralWidget = new QGraphicsWidget(m_subject);
    QGraphicsWidget *anotherCentralWidget = new QGraphicsWidget(m_subject);
    m_subject->setCentralWidget(newCentralWidget);
    m_subject->setCentralWidget(anotherCentralWidget, false);
    delete newCentralWidget;
    QVERIFY(m_subject->centralWidget() == anotherCentralWidget);
}

// test all of the set/get with both visible and invisble header, because it should work for instance
// that one sets the e.g. iconID now and makes the header visible later

void Ut_MContainer::setTitleWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setTitle("title");

    QCOMPARE(m_subject->title(), QString("title"));
}

void Ut_MContainer::setTitleWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setTitle("title");

    QCOMPARE(m_subject->title(), QString("title"));
}

void Ut_MContainer::setIconIDWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setIconID("icon-m-framework-close");

    QCOMPARE(m_subject->iconID(), QString("icon-m-framework-close"));
}

void Ut_MContainer::setIconIDWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setIconID("icon-m-framework-close");

    QCOMPARE(m_subject->iconID(), QString("icon-m-framework-close"));
}

void Ut_MContainer::setTextWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setText("text");

    QCOMPARE(m_subject->text(), QString("text"));
}

void Ut_MContainer::setTextWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setText("text");

    QCOMPARE(m_subject->text(), QString("text"));
}

void Ut_MContainer::setProgressIndicatorVisible()
{
    m_subject->setProgressIndicatorVisible(true);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), true);
}

void Ut_MContainer::setProgressIndicatorInVisible()
{
    m_subject->setProgressIndicatorVisible(false);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), false);
}

void Ut_MContainer::setProgressIndicatorWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setProgressIndicatorVisible(true);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), true);
}

void Ut_MContainer::setProgressIndicatorWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setProgressIndicatorVisible(true);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), true);
}


QTEST_APPLESS_MAIN(Ut_MContainer)
