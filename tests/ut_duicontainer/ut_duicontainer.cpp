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

#include <QObject>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

#include <duicontainer.h>
#include "duicontainer_p.h"
#include <duicontainerview.h>
#include "views/duicontainerview_p.h"

#include "ut_duicontainer.h"
#include "duiapplication.h"

#include "duicancelevent.h"

void Ut_DuiContainer::init()
{
    m_subject = new DuiContainer();
}

void Ut_DuiContainer::cleanup()
{
    delete m_subject;
    m_subject = 0;
}
DuiApplication *app;

void Ut_DuiContainer::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duicontainer" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiContainer::cleanupTestCase()
{
    delete app;
}

void Ut_DuiContainer::centralWidget()
{
    // check that there exists centralWidget
    QVERIFY(m_subject->centralWidget() != NULL);
}

void Ut_DuiContainer::setCentralWidget()
{
    DuiWidget *tmp = new DuiWidget();

    m_subject->setCentralWidget(tmp);

    // check that there exists centralWidget
    QVERIFY(m_subject->centralWidget() == tmp);

    delete tmp;
}

void Ut_DuiContainer::title()
{
    QString myQString("testing title()");

    m_subject->setTitle(myQString);
    QCOMPARE(m_subject->title(), myQString);
}

void Ut_DuiContainer::text()
{
    QString myQString("testing text()");

    m_subject->setText(myQString);
    QCOMPARE(m_subject->text(), myQString);
}

void Ut_DuiContainer::iconID()
{
    QString myIconID("testing iconID()");

    m_subject->setIconID(myIconID);
    QCOMPARE(m_subject->iconID(), myIconID);
}

void Ut_DuiContainer::headerVisible()
{
    // default state
    QVERIFY(m_subject->headerVisible() == true);

    m_subject->setHeaderVisible(false);

    QVERIFY(m_subject->headerVisible() == false);
}

void Ut_DuiContainer::toggleHeaderVisible()
{
    // default state
    QVERIFY(m_subject->headerVisible() == true);

    m_subject->toggleHeaderVisible();

    QVERIFY(m_subject->headerVisible() == false);

    m_subject->toggleHeaderVisible();

    QVERIFY(m_subject->headerVisible() == true);
}


void Ut_DuiContainer::toggleProgressIndicatorVisible()
{
    // default state
    QVERIFY(m_subject->isProgressIndicatorVisible() == false);

    m_subject->toggleProgressIndicatorVisible();

    QVERIFY(m_subject->isProgressIndicatorVisible() == true);

    m_subject->toggleProgressIndicatorVisible();

    QVERIFY(m_subject->isProgressIndicatorVisible() == false);
}

void Ut_DuiContainer::testCreatingViewImmediatelyInLayout()
{
    //Add the container to a layout then force the view to be created by calling minimumSize()
    QGraphicsLinearLayout layout(Qt::Horizontal);
    layout.addItem(m_subject);
    m_subject->minimumSize();

}
void Ut_DuiContainer::testCreatingViewImmediately()
{
    //Force the view to be created by calling minimumSize()
    //This checks BUG:126088 which causes a segmentation fault
    m_subject->minimumSize();
}

void Ut_DuiContainer::testCancelEvent()
{
    QSignalSpy spy(m_subject, SIGNAL(headerClicked()));

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(&mouseEvent);

    DuiCancelEvent event;
    m_subject->cancelEvent(&event);

    QVERIFY(spy.count() == 0);
}

// test all of the set/get with both visible and invisble header, because it should work for instance
// that one sets the e.g. iconID now and makes the header visible later

void Ut_DuiContainer::setTitleWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setTitle("title");

    QCOMPARE(m_subject->title(), QString("title"));
}

void Ut_DuiContainer::setTitleWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setTitle("title");

    QCOMPARE(m_subject->title(), QString("title"));
}

void Ut_DuiContainer::setIconIDWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setIconID("Icon-close");

    QCOMPARE(m_subject->iconID(), QString("Icon-close"));
}

void Ut_DuiContainer::setIconIDWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setIconID("Icon-close");

    QCOMPARE(m_subject->iconID(), QString("Icon-close"));
}

void Ut_DuiContainer::setTextWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setText("text");

    QCOMPARE(m_subject->text(), QString("text"));
}

void Ut_DuiContainer::setTextWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setText("text");

    QCOMPARE(m_subject->text(), QString("text"));
}

void Ut_DuiContainer::setProgressIndicatorVisible()
{
    m_subject->setProgressIndicatorVisible(true);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), true);
}

void Ut_DuiContainer::setProgressIndicatorInVisible()
{
    m_subject->setProgressIndicatorVisible(false);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), false);
}

void Ut_DuiContainer::setProgressIndicatorWithHeaderVisible()
{
    m_subject->setHeaderVisible(true);
    m_subject->setProgressIndicatorVisible(true);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), true);
}

void Ut_DuiContainer::setProgressIndicatorWithHeaderInvisible()
{
    m_subject->setHeaderVisible(false);
    m_subject->setProgressIndicatorVisible(true);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), true);
}


QTEST_APPLESS_MAIN(Ut_DuiContainer)
