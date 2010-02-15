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
#include <QGraphicsSceneMouseEvent>
#include <duibutton.h>
#include "duibutton_p.h"
#include <duibuttonview.h>
#include "views/duibuttonview_p.h"
#include "ut_duibutton.h"
#include "duiapplication.h"
#include "duicancelevent.h"
#include <duibuttongroup.h>

void Ut_DuiButton::init()
{
    m_subject = new DuiButton();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    m_tmp = 0;
}

void Ut_DuiButton::cleanup()
{
    delete m_subject;
    m_subject = 0;
}
DuiApplication *app;

void Ut_DuiButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duibutton" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiButton::cleanupTestCase()
{
    delete app;
}

void Ut_DuiButton::testConstructionAndDestruction()
{
    QString  ButtonText("NewButton");
    m_tmp = new DuiButton(ButtonText);
    QCOMPARE(m_tmp->text(), ButtonText);
    delete m_tmp;

    QString  IconId("Icon-home");
    m_tmp = new DuiButton(IconId, ButtonText);
    QCOMPARE(m_tmp->text(), ButtonText);
    QCOMPARE(m_tmp->iconID(), IconId);
    delete m_tmp;
}

void Ut_DuiButton::testTextVisible()
{
    m_subject->setTextVisible(false);
    QVERIFY(m_subject->isTextVisible() == false);
    m_subject->setTextVisible(true);
    QVERIFY(m_subject->isTextVisible() == true);
}

void Ut_DuiButton::testIconVisible()
{
    m_subject->setIconVisible(false);
    QVERIFY(m_subject->isIconVisible() == false);
    m_subject->setIconVisible(true);
    QVERIFY(m_subject->isIconVisible() == true);
}

void Ut_DuiButton::testCheckable()
{
    m_subject->setCheckable(false);
    QVERIFY(m_subject->isCheckable() == false);
    m_subject->setCheckable(true);
    QVERIFY(m_subject->isCheckable() == true);


    QSignalSpy toggledSpy(m_subject, SIGNAL(toggled(bool)));

    m_subject->setCheckable(true);
    m_subject->setChecked(true);
    QVERIFY(m_subject->isChecked() == true);
    QCOMPARE(toggledSpy.count(), 1);
    QVERIFY(toggledSpy.takeFirst().at(0).toBool() == true);

    m_subject->setChecked(false);
    QCOMPARE(toggledSpy.count(), 1);
    QVERIFY(toggledSpy.takeFirst().at(0).toBool() == false);


    m_subject->setChecked(true);
    m_subject->setCheckable(false);
    QVERIFY(m_subject->isCheckable() == false);
    QVERIFY(m_subject->isChecked() == false);
}

void Ut_DuiButton::testPressedReleased()
{
    QSignalSpy pressedSpy(m_subject, SIGNAL(pressed()));
    QSignalSpy releasedSpy(m_subject, SIGNAL(released()));

    m_subject->setDown(true);
    QVERIFY(m_subject->isDown());
    QCOMPARE(pressedSpy.count(), 1);

    m_subject->setDown(false);
    QVERIFY(!m_subject->isDown());
    QCOMPARE(releasedSpy.count(), 1);
}

void Ut_DuiButton::testIconID()
{
    QString myIconID("myIconID");
    m_subject->setIconID(myIconID);
    QCOMPARE(m_subject->iconID(), myIconID);
}

void Ut_DuiButton::testToggledIconID()
{
    QString myToggledIconID("myToggledIconID");
    m_subject->setToggledIconID(myToggledIconID);
    QCOMPARE(m_subject->toggledIconID(), myToggledIconID);
}

void Ut_DuiButton::testText()
{
    QString myQString("testing setText()");
    m_subject->setText(myQString);
    QCOMPARE(m_subject->text(), myQString);
}

void Ut_DuiButton::setView()
{
    DuiButtonView *myButtonView = new DuiButtonView(m_subject);
    //myButtonView->updateStyle();

    m_subject->setView(myButtonView);

    // compare addresses
    QCOMPARE(m_subject->view(), myButtonView);
}

void Ut_DuiButton::click()
{
    QSignalSpy pressedSpy(m_subject, SIGNAL(pressed()));
    QSignalSpy releasedSpy(m_subject, SIGNAL(released()));
    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));

    m_subject->click();
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QCOMPARE(clickedSpy.count(), 1);
}

void Ut_DuiButton::toggle()
{
    m_subject->setCheckable(true);
    m_subject->setChecked(true);

    QSignalSpy toggledSpy(m_subject, SIGNAL(toggled(bool)));

    // -> to false
    m_subject->toggle();

    QVERIFY(m_subject->isChecked() == false);

    // verify signal & the argument
    QCOMPARE(toggledSpy.count(), 1);
    QVERIFY(toggledSpy.takeFirst().at(0).toBool() == false);

    // -> to true
    m_subject->toggle();
    QVERIFY(m_subject->isChecked() == true);

    // verify signal & the argument
    QCOMPARE(toggledSpy.count(), 1);
    QVERIFY(toggledSpy.takeFirst().at(0).toBool() == true);
}

void Ut_DuiButton::testPressedCanceled()
{
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(&mouseEvent);
    QVERIFY(m_subject->isDown() == true);

    QSignalSpy spy(m_subject, SIGNAL(clicked()));
    DuiCancelEvent event;
    m_subject->cancelEvent(&event);

    QCOMPARE(spy.count(), 0);
    QVERIFY(m_subject->isDown() == false);
}

void Ut_DuiButton::testGroup()
{
    DuiButtonGroup group;

    m_subject->setCheckable(true);
    group.addButton(m_subject);
    m_subject->setChecked(true);
    m_subject->setChecked(false);
    QVERIFY(m_subject->isChecked() == true);
    delete m_subject;
    m_subject = NULL;
    QCOMPARE(group.buttons().count(), 0);
}

/*
 * Check that DuiButton doesn't change the model when
 * it's passed on the constructor.
 */
void Ut_DuiButton::testConstructorWithModel()
{
    DuiButtonModel *model = new DuiButtonModel;
    model->setText("foo");

    DuiButton *button = new DuiButton(0, model);

    QCOMPARE(model->text(), QString("foo"));
    QCOMPARE(button->text(), QString("foo"));

    delete button;
    // model should have gone to heaven on DuiButton's destructor.
}

QTEST_APPLESS_MAIN(Ut_DuiButton)
