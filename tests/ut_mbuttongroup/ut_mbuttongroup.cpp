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
#include <QSignalSpy>

#include <mbuttongroup.h>
#include <mbutton.h>
#include <mtheme.h>
#include <mapplication.h>

#include "ut_mbuttongroup.h"

void Ut_MButtonGroup::init()
{
    m_subject = new MButtonGroup();
}

void Ut_MButtonGroup::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

MApplication *app;

void Ut_MButtonGroup::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbuttongroup" };
    app = new MApplication(argc, app_name);
}

void Ut_MButtonGroup::cleanupTestCase()
{
    delete app;
}

void Ut_MButtonGroup::addButton()
{
    MButton b1;

    b1.setCheckable(true);

    QCOMPARE(m_subject->buttons().count(), 0);

    m_subject->addButton(&b1);
    m_subject->addButton(&b1);
    QCOMPARE(m_subject->buttons().count(), 1);

    m_subject->removeButton(&b1);
    QCOMPARE(m_subject->buttons().count(), 0);

    m_subject->addButton(&b1, 10);
    QCOMPARE(m_subject->buttons().count(), 1);

    m_subject->removeButton(&b1);
    QCOMPARE(m_subject->buttons().count(), 0);

    m_subject->addButton(&b1, -1);
    QCOMPARE(m_subject->buttons().count(), 1);
}

void Ut_MButtonGroup::button()
{
    MButton b1;
    b1.setCheckable(true);
    m_subject->addButton(&b1, 10);
    QVERIFY(m_subject->button(11) == 0);
    QVERIFY(m_subject->button(10) == &b1);

    MButton b2;
    MButton b3;
    MButton b4;
    MButton b5;
    MButton b6;

    b2.setCheckable(true);
    b3.setCheckable(true);
    b4.setCheckable(true);
    b5.setCheckable(true);
    b6.setCheckable(true);

    m_subject->addButton(&b2, 10);
    m_subject->addButton(&b3, 11);
    m_subject->addButton(&b4, 12);
    m_subject->addButton(&b5, 13);
    m_subject->addButton(&b6, 14);

    QVERIFY(m_subject->button(10) != &b1);
    QVERIFY(m_subject->button(10) == &b2);
    QVERIFY(m_subject->button(14) == &b6);

    m_subject->removeButton(&b2);
    QVERIFY(m_subject->button(10) == &b1);

    m_subject->removeButton(&b6);
    QVERIFY(m_subject->button(14) == 0);

    m_subject->removeButton(&b1);
    m_subject->removeButton(&b3);
    m_subject->removeButton(&b4);
    m_subject->removeButton(&b5);
}

void Ut_MButtonGroup::buttons()
{
    const QList<MButton *> list = m_subject->buttons();
    QVERIFY(list.count() == 0);

    MButton b1;
    MButton b2;
    MButton b3;
    MButton b4;
    MButton b5;
    MButton b6;

    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);
    b4.setCheckable(true);
    b5.setCheckable(true);
    b6.setCheckable(true);

    m_subject->addButton(&b1);
    m_subject->addButton(&b2);
    m_subject->addButton(&b3);
    m_subject->addButton(&b4);
    m_subject->addButton(&b5);
    m_subject->addButton(&b6);

    const QList<MButton *> list1 = m_subject->buttons();
    QCOMPARE(list1.count(), 6);
}

void Ut_MButtonGroup::checkedButton()
{
    MButton b1;
    MButton b2;
    MButton b3;
    MButton b4;
    MButton b5;
    MButton b6;

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);
    b4.setCheckable(true);
    b5.setCheckable(true);

    m_subject->addButton(&b1, 10);
    m_subject->addButton(&b2, 10);
    m_subject->addButton(&b3, 11);
    m_subject->addButton(&b4, 12);

    QVERIFY(m_subject->checkedButton() == 0);

    b6.setCheckable(false);
    b6.setChecked(true);
    m_subject->addButton(&b6, 14);
    QVERIFY(m_subject->checkedButton() == 0);
    b6.setChecked(true);
    QVERIFY(m_subject->checkedButton() == 0);

    b6.setCheckable(true);
    b6.setChecked(true);
    QVERIFY(m_subject->checkedButton() == &b6);

    m_subject->removeButton(&b2);
    QVERIFY(m_subject->checkedButton() == &b6);

    b5.setChecked(true);
    m_subject->addButton(&b5, 13);
    QVERIFY(m_subject->checkedButton() == &b5);

    m_subject->removeButton(&b1);
    m_subject->removeButton(&b2);
    m_subject->removeButton(&b3);
    m_subject->removeButton(&b4);
    m_subject->removeButton(&b5);
}

void Ut_MButtonGroup::checkedId()
{
    MButton b1;
    MButton b2;
    MButton b3;
    MButton b4;
    MButton b5;
    MButton b6;

    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);
    b4.setCheckable(true);
    b5.setCheckable(true);
    b6.setCheckable(true);

    m_subject->addButton(&b1, 10);
    m_subject->addButton(&b2, 10);
    m_subject->addButton(&b3, 11);
    m_subject->addButton(&b4, 12);
    m_subject->addButton(&b5, 13);

    QCOMPARE(m_subject->checkedId(), -1);

    b6.setChecked(true);
    m_subject->addButton(&b6, 14);

    QCOMPARE(m_subject->checkedId(), 14);
}

void Ut_MButtonGroup::exclusive()
{
    MButton b1;
    MButton b2;
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    b1.setCheckable(true);
    b2.setCheckable(true);

    QVERIFY(m_subject->exclusive() == true);
    m_subject->setExclusive(false);
    m_subject->addButton(&b1, 1);
    m_subject->addButton(&b2, 2);
    b1.setChecked(true);
    b2.setChecked(true);

    QVERIFY(b1.isChecked() == true);
    QVERIFY(b2.isChecked() == true);
    QVERIFY(m_subject->exclusive() == false);

    m_subject->removeButton(&b1);
    m_subject->removeButton(&b2);
    m_subject->setExclusive(true);
    m_subject->addButton(&b1, 1);
    m_subject->addButton(&b2, 2);
    b1.setChecked(true);
    b2.setChecked(true);
    QVERIFY(b1.isChecked() == false);
    QVERIFY(b2.isChecked() == true);
    QVERIFY(m_subject->exclusive() == true);
}

void Ut_MButtonGroup::testMakePopulatedGroupExclusive()
{
    //Case: Add unchecked buttons to a group with non-exclusive
    //mode, and then change the mode to exclusive.
    //This should result in first button as checked.

    MButton b1;
    MButton b2;
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    b1.setCheckable(true);
    b2.setCheckable(true);

    m_subject->setExclusive(false);
    b1.setChecked(false);
    b2.setChecked(false);
    m_subject->addButton(&b1, 1);
    m_subject->addButton(&b2, 2);
    m_subject->setExclusive(true);

    QVERIFY(b1.isChecked() == true);
    QVERIFY(b2.isChecked() == false);
}

void Ut_MButtonGroup::testMakePopulatedGroupWithTwoCkeckedButtonsExclusive()
{
    //Case: make more than one button in non-exclusive
    //mode as checked, and then change the mode to exclusive
    //This should result in unchecking all the buttons
    //except the first checked button.

    MButton b1;
    MButton b2;
    MButton b3;
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);

    m_subject->setExclusive(false);
    m_subject->addButton(&b1, 1);
    m_subject->addButton(&b2, 2);
    m_subject->addButton(&b3, 3);
    b1.setChecked(false);
    b2.setChecked(true);
    b3.setChecked(true);
    m_subject->setExclusive(true);

    QVERIFY(b1.isChecked() == false);
    QVERIFY(b2.isChecked() == true);
    QVERIFY(b3.isChecked() == false);
}

void Ut_MButtonGroup::testRemoveCheckedButton()
{
    //Case: remove checked button in exclusive mode.
    //This should result in first (checkable) button as checked.

    MButton b1;
    MButton b2;
    MButton b3;
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);

    m_subject->setExclusive(true);
    b1.setChecked(false);
    b2.setChecked(true);
    b3.setChecked(false);
    m_subject->addButton(&b1, 1);
    m_subject->addButton(&b2, 2);
    m_subject->addButton(&b3, 3);

    m_subject->removeButton(&b2);
    // validation of checked button is deferred to a separate event
    QCoreApplication::processEvents();

    QVERIFY(b1.isChecked() == true);
    QVERIFY(b3.isChecked() == false);
}

void Ut_MButtonGroup::testNonCheckableButtonMadeChecked()
{
    //Case: non-checkable button made checked

    MButton b1;
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    b1.setCheckable(true);

    m_subject->setExclusive(true);

    b1.setCheckable(false);
    m_subject->addButton(&b1, 1);
    b1.model()->setChecked(true);

    // I guess we are only checking whether it's going to crash?
}

void Ut_MButtonGroup::id()
{
    MButton b1;
    MButton b2;
    MButton b3;
    MButton b4;
    MButton b5;
    MButton b6;

    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);
    b4.setCheckable(true);
    b5.setCheckable(true);
    b6.setCheckable(true);

    m_subject->addButton(&b1, 10);
    m_subject->addButton(&b2, 10);
    m_subject->addButton(&b3, 11);
    m_subject->addButton(&b4, 12);
    m_subject->addButton(&b5, 13);

    QCOMPARE(m_subject->id(&b1), 10);
    QCOMPARE(m_subject->id(&b5), 13);
    QCOMPARE(m_subject->id(&b4), 12);
    QCOMPARE(m_subject->id(&b3), 11);
    QCOMPARE(m_subject->id(&b2), 10);
    QCOMPARE(m_subject->id(&b6), -1);

    m_subject->removeButton(&b5);
    QCOMPARE(m_subject->id(&b5), -1);

}

void Ut_MButtonGroup::removeButton()
{
    MButton b1;
    MButton b2;
    MButton b3;
    MButton b4;
    MButton b5;
    MButton b6;

    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);
    b4.setCheckable(true);
    b5.setCheckable(true);
    b6.setCheckable(true);

    QCOMPARE(m_subject->buttons().count(), 0);

    m_subject->addButton(&b1, 10);
    m_subject->addButton(&b2, 10);
    m_subject->addButton(&b3, 11);
    m_subject->addButton(&b4, 12);
    m_subject->addButton(&b5, 13);
    m_subject->addButton(&b6, 14);

    QCOMPARE(m_subject->buttons().count(), 6);
    m_subject->removeButton(&b1);
    QCOMPARE(m_subject->buttons().count(), 5);

    m_subject->removeButton(&b2);
    m_subject->removeButton(&b3);
    m_subject->removeButton(&b4);
    m_subject->removeButton(&b5);
    m_subject->removeButton(&b6);

    QCOMPARE(m_subject->buttons().count(), 0);
}

void Ut_MButtonGroup::setId()
{
    MButton b1;
    MButton b2;
    MButton b3;

    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);

    m_subject->setId(&b1, 10);

    m_subject->addButton(&b1);
    m_subject->addButton(&b2);
    m_subject->addButton(&b3);

    m_subject->setId(&b1, 10);
    m_subject->setId(&b2, 10);
    m_subject->setId(&b3, 11);
    m_subject->setId(&b1, -1);

    QCOMPARE(m_subject->id(&b2), 10);
    QCOMPARE(m_subject->id(&b1), 10);
    QCOMPARE(m_subject->id(&b3), 11);

}

void Ut_MButtonGroup::buttonPressReleaseClick()
{
    MButton b1;

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QSignalSpy buttonPressedSpy(m_subject, SIGNAL(buttonPressed(MButton *)));
    QSignalSpy buttonPressedIdSpy(m_subject, SIGNAL(buttonPressed(int)));
    QSignalSpy buttonReleasedSpy(m_subject, SIGNAL(buttonReleased(MButton *)));
    QSignalSpy buttonReleasedIdSpy(m_subject, SIGNAL(buttonReleased(int)));
    QSignalSpy buttonClickedSpy(m_subject, SIGNAL(buttonClicked(MButton *)));
    QSignalSpy buttonClickedIdSpy(m_subject, SIGNAL(buttonClicked(int)));

    b1.setCheckable(true);
    m_subject->addButton(&b1, 1);
    b1.setDown(true);

    QCOMPARE(buttonPressedSpy.count(), 1);
    QCOMPARE(buttonPressedIdSpy.count(), 1);

    b1.setDown(false);
    QCOMPARE(buttonReleasedSpy.count(), 1);
    QCOMPARE(buttonReleasedIdSpy.count(), 1);

    b1.click();
    QCOMPARE(buttonClickedSpy.count(), 1);
    QCOMPARE(buttonClickedIdSpy.count(), 1);
}


QTEST_APPLESS_MAIN(Ut_MButtonGroup)
