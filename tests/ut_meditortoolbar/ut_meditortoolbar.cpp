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

#include "ut_meditortoolbar.h"
#include "views/meditortoolbar.h"

#include <QAction>
#include <QGraphicsLayout>

#include <MApplication>
#include <MApplicationWindow>
#include <MButton>
#include <MScene>

void Ut_MEditorToolbar::initTestCase()
{
    qRegisterMetaType<M::OrientationAngle>("M::OrientationAngle");

    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_meditortoolbar" };
    MApplication::setLoadMInputContext(false);
    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;

    fakeParent = new MWidget;
    m_appWindow->scene()->addItem(fakeParent);
}

void Ut_MEditorToolbar::cleanupTestCase()
{
    delete m_appWindow;
    m_appWindow = 0;
    delete m_app;
    m_app = 0;
}

void Ut_MEditorToolbar::init()
{
    fakeParent->setPos(QPointF());
    subject = new MEditorToolbar(*fakeParent);
}

void Ut_MEditorToolbar::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MEditorToolbar::testAppear()
{
    // After creation, it should be disappeared.
    QCOMPARE(subject->isAppeared(), false);

    // Show the widget.
    subject->appear();
    QCOMPARE(subject->isAppeared(), true);

    subject->disappear();
    QCOMPARE(subject->isAppeared(), false);
}

void Ut_MEditorToolbar::testItemVisibility()
{
    // Item visibility is controlled with action visibility but is
    // udpated only when event loop is visited. Therefore let's call
    // processEvents() after action visibility is changed.

    QAction action("format c:", 0);
    action.setVisible(false);
    subject->addAction(&action);
    qApp->processEvents();

    QCOMPARE(subject->isVisible(), false);

    subject->appear();

    // There are no visible actions in it, so the item is hidden.
    QCOMPARE(subject->isVisible(), false);

    // Set action visible -> creates visible button.
    action.setVisible(true);
    qApp->processEvents();
    QCOMPARE(subject->isVisible(), true);

    subject->disappear();
    QCOMPARE(subject->isVisible(), false);
}

void Ut_MEditorToolbar::testPositionMapping_data()
{
    // The subject is not really child of fakeParent but
    // it rather follows it artificially allowing relative
    // positions to be used.

    QTest::addColumn<QPointF>("fakeParentPos");
    QTest::addColumn<QPointF>("position"); // to be passed to subject->setPosition()

    QTest::newRow("fake parent at origin, subject at origin")
        << QPointF() << QPointF();
    QTest::newRow("fake parent translated, subject at origin")
        << QPointF(2.0, 5.0) << QPointF();
    QTest::newRow("fake parent at origin, subject translated")
        << QPointF() << QPointF(2.0, 5.0);
    QTest::newRow("fake parent translated, subject translated")
        << QPointF(2.0, 5.0) << QPointF(10.0, 15.0);
}

void Ut_MEditorToolbar::testPositionMapping()
{
    QFETCH(QPointF, fakeParentPos);
    QFETCH(QPointF, position);

    fakeParent->setPos(fakeParentPos);
    subject->setPosition(position, MEditorToolbarArrow::ArrowUp);

    QPointF mappedActualPos = subject->parentItem()->mapToItem(fakeParent,
                                                               subject->pos());
    QCOMPARE(mappedActualPos, position);
}

void Ut_MEditorToolbar::testVerticalPositioning()
{
    QAction action("hero", 0);
    action.setVisible(true);
    subject->addAction(&action);
    qApp->processEvents(); // Allow subject to react.

    QVERIFY(subject->rect().isValid());

    subject->setPosition(QPointF(), MEditorToolbarArrow::ArrowUp);

    // In parent coordinates
    QPointF subjectCenter = fakeParent->mapFromItem(subject,
                                                    subject->rect().center());

    // Arrow points up, so widget should be below parent's origin.
    QVERIFY(subjectCenter.y() > 0.0f);


    // Test other way around.

    subject->setPosition(QPointF(), MEditorToolbarArrow::ArrowDown);

    // In parent coordinates
    subjectCenter = fakeParent->mapFromItem(subject,
                                            subject->rect().center());

    // Arrow points down, so widget should be above parent's origin.
    QVERIFY(subjectCenter.y() < 0.0f);
}

void Ut_MEditorToolbar::testActionTextChange()
{
    const QString oldText("granpa");
    const QString newText("junior");

    QAction action(oldText, 0);
    action.setVisible(true);
    subject->addAction(&action);
    qApp->processEvents(); // Allow subject to react.

    QCOMPARE(subjectButtons().first()->text(), oldText);
    action.setText(newText);
    QCOMPARE(subjectButtons().first()->text(), newText);
}

QList<MButton *> Ut_MEditorToolbar::subjectButtons() const
{
    QList<MButton *> buttons;
    for (int i = 0; i < subject->layout()->count(); ++i) {
        QGraphicsLayoutItem *layoutItem = subject->layout()->itemAt(i);
        MButton *button = dynamic_cast<MButton *>(layoutItem);
        Q_ASSERT(button);
        buttons << button;
    }
    return buttons;
}

QTEST_APPLESS_MAIN(Ut_MEditorToolbar)
