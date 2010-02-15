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
#include <QSignalSpy>

#include <duibuttongroup.h>
#include <duibutton.h>
#include <duitheme.h>
#include <duiapplication.h>

#include "ut_duibuttongroup.h"

void Ut_DuiButtonGroup::init()
{
    m_subject = new DuiButtonGroup();
}

void Ut_DuiButtonGroup::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

DuiApplication *app;

void Ut_DuiButtonGroup::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duibuttongroup" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiButtonGroup::cleanupTestCase()
{
    delete app;
}

void Ut_DuiButtonGroup::addButton()
{
    DuiButton b1;

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

void Ut_DuiButtonGroup::button()
{
    DuiButton b1;
    b1.setCheckable(true);
    m_subject->addButton(&b1, 10);
    QVERIFY(m_subject->button(11) == 0);
    QVERIFY(m_subject->button(10) == &b1);

    DuiButton b2;
    DuiButton b3;
    DuiButton b4;
    DuiButton b5;
    DuiButton b6;

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

void Ut_DuiButtonGroup::buttons()
{
    const QList<DuiButton *> list = m_subject->buttons();
    QVERIFY(list.count() == 0);

    DuiButton b1;
    DuiButton b2;
    DuiButton b3;
    DuiButton b4;
    DuiButton b5;
    DuiButton b6;

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

    const QList<DuiButton *> list1 = m_subject->buttons();
    QCOMPARE(list1.count(), 6);
}

void Ut_DuiButtonGroup::checkedButton()
{
    DuiButton b1;
    DuiButton b2;
    DuiButton b3;
    DuiButton b4;
    DuiButton b5;
    DuiButton b6;

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

void Ut_DuiButtonGroup::checkedId()
{
    DuiButton b1;
    DuiButton b2;
    DuiButton b3;
    DuiButton b4;
    DuiButton b5;
    DuiButton b6;

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

void Ut_DuiButtonGroup::exclusive()
{
    DuiButton b1;
    DuiButton b2;
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

void Ut_DuiButtonGroup::switchModes()
{
    DuiButton b1;
    DuiButton b2;
    DuiButton b3;
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    b1.setCheckable(true);
    b2.setCheckable(true);
    b3.setCheckable(true);

    //Case: Add unchecked buttons to a group with non-exclusive
    //mode, and then change the mode to exclusive.
    //This should result in first button as checked.
    m_subject->setExclusive(false);
    b1.setChecked(false);
    b2.setChecked(false);
    m_subject->addButton(&b1, 1);
    m_subject->addButton(&b2, 2);
    m_subject->setExclusive(true);

    QVERIFY(b1.isChecked() == true);
    QVERIFY(b2.isChecked() == false);

    //Case: make more than one button in non-exclusive
    //mode as checked, and then change the mode to exclusive
    //This should result in unchecking all the buttons
    //except the first checked button.
    m_subject->removeButton(&b1);
    m_subject->removeButton(&b2);
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

    //Case: remove checked button in exclusive
    //mode.This should result in first (checkable) button as checked.
    m_subject->removeButton(&b1);
    m_subject->removeButton(&b2);
    m_subject->removeButton(&b3);

    m_subject->setExclusive(true);
    b1.setChecked(false);
    b2.setChecked(true);
    b3.setChecked(false);
    m_subject->addButton(&b1, 1);
    m_subject->addButton(&b2, 2);
    m_subject->addButton(&b3, 3);

    m_subject->removeButton(&b2);

    QVERIFY(b1.isChecked() == true);
    QVERIFY(b3.isChecked() == false);

    //Case: non-checkable button made checked
    m_subject->removeButton(&b1);
    m_subject->removeButton(&b2);
    m_subject->removeButton(&b3);

    b1.setCheckable(false);
    m_subject->addButton(&b1, 1);
    b1.model()->setChecked(true);
}

void Ut_DuiButtonGroup::id()
{
    DuiButton b1;
    DuiButton b2;
    DuiButton b3;
    DuiButton b4;
    DuiButton b5;
    DuiButton b6;

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

void Ut_DuiButtonGroup::removeButton()
{
    DuiButton b1;
    DuiButton b2;
    DuiButton b3;
    DuiButton b4;
    DuiButton b5;
    DuiButton b6;

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

void Ut_DuiButtonGroup::setId()
{
    DuiButton b1;
    DuiButton b2;
    DuiButton b3;

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

void Ut_DuiButtonGroup::buttonPressReleaseClick()
{
    DuiButton b1;

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QSignalSpy buttonPressedSpy(m_subject, SIGNAL(buttonPressed(DuiButton *)));
    QSignalSpy buttonPressedIdSpy(m_subject, SIGNAL(buttonPressed(int)));
    QSignalSpy buttonReleasedSpy(m_subject, SIGNAL(buttonReleased(DuiButton *)));
    QSignalSpy buttonReleasedIdSpy(m_subject, SIGNAL(buttonReleased(int)));
    QSignalSpy buttonClickedSpy(m_subject, SIGNAL(buttonClicked(DuiButton *)));
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


QTEST_APPLESS_MAIN(Ut_DuiButtonGroup)
