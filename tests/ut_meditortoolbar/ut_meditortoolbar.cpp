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

#include "ut_meditortoolbar.h"
#include "views/meditortoolbar.h"
#include "views/meditortoolbar_p.h"
#include "views/mtopleveloverlay.h"

#include <QAction>
#include <QGraphicsLayout>

#include <MApplication>
#include <MApplicationWindow>
#include <MButton>
#include <MScene>
#include <MSceneManager>
#include <MSceneWindow>

Q_DECLARE_METATYPE(Ut_MEditorToolbar::Operations)

#ifndef __arm__
// on scratchbox we must assume we're in landscape mode
M::Orientation MSceneManager::orientation() const
{
    return M::Landscape;
}
#endif // !__arm__

void Ut_MEditorToolbar::initTestCase()
{
    qRegisterMetaType<M::OrientationAngle>("M::OrientationAngle");

    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_meditortoolbar" };
    MApplication::setLoadMInputContext(false);
    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;

    fakeParent = new MSceneWindow;
    m_appWindow->sceneManager()->appearSceneWindowNow(fakeParent, MSceneWindow::DestroyWhenDone);
    qApp->processEvents();
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
    subject = new MEditorToolbar(fakeParent);
}

void Ut_MEditorToolbar::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MEditorToolbar::testAppear()
{
    MEditorToolbarStyle *style = const_cast<MEditorToolbarStyle *>(subject->style().operator ->());
    style->setHideAnimationDuration(1);
    style->setShowAnimationDuration(1);

    // After creation, it should be disappeared.
    QCOMPARE(subject->isAppeared(), false);

    // Show the widget.
    subject->appear();
    // Immediately the state is appeared.
    QCOMPARE(subject->isAppeared(), true);

    // After appearance animation the state is appeared
    QTest::qWait(100);
    QCOMPARE(subject->isAppeared(), true);

    subject->disappear();
    // Immediately the state is disappeared
    QCOMPARE(subject->isAppeared(), false);

    QTest::qWait(100);
    // After disappearance animation the state is disappeared
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
    QTest::qWait(200);
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
    subject->appear();
    subject->setPosition(position, MEditorToolbar::BelowPointOfInterest);
    qApp->processEvents(); // Allow setPosition() to take effect

    QPointF mappedActualPos = subject->parentItem()->mapToItem(fakeParent,
                                                               subject->pos());
    QCOMPARE(mappedActualPos, position);
}

void Ut_MEditorToolbar::testForbiddenRegion_data()
{
    QTest::addColumn<QPointF>("fakeParentPos");
    QTest::addColumn<QPointF>("position");
    QTest::addColumn<QRegion>("forbiddenRegion");
    QTest::addColumn<bool>("actionIsVisible");
    QTest::addColumn<qreal>("m32"); // QTransform::m32 is vertical translation

    QTest::newRow("no forbidden area, action is invisible")
            << QPointF(2.0, 5.0) << QPointF() << QRegion() << false << static_cast<qreal>(0.0f); // use static_cast to qreal to work on x86
    QTest::newRow("use forbidden area, action is invisible")
            << QPointF(2.0, 5.0) << QPointF() << QRegion(QRect(-10, -10, 20, 20)) << false << static_cast<qreal>(10.0f);

    QTest::newRow("no forbidden area, action is visible")
            << QPointF(2.0, 5.0) << QPointF() << QRegion() << true << static_cast<qreal>(0.0f);
    QTest::newRow("use forbidden area, action is visible")
            << QPointF(2.0, 5.0) << QPointF() << QRegion(QRect(-10, -10, 20, 20)) << true << static_cast<qreal>(10.0f);
}

void Ut_MEditorToolbar::testForbiddenRegion()
{
    QFETCH(QPointF, fakeParentPos);
    QFETCH(QPointF, position);
    QFETCH(QRegion, forbiddenRegion);
    QFETCH(bool,    actionIsVisible);
    QFETCH(qreal, m32);

    QAction action("zorro", 0);
    action.setVisible(actionIsVisible);
    subject->addAction(&action);
    qApp->processEvents(); // Allow subject to react.

    fakeParent->setPos(fakeParentPos);
    subject->appear();
    subject->setForbiddenRegion(forbiddenRegion);
    subject->setPosition(position, MEditorToolbar::BelowPointOfInterest);
    qApp->processEvents(); // Allow setPosition() to take effect

    // check vertical translation
    QCOMPARE(subject->transform().m32(), m32);
}

void Ut_MEditorToolbar::testVerticalPositioning()
{
    QAction action("hero", 0);
    action.setVisible(true);
    subject->addAction(&action);
    qApp->processEvents(); // Allow subject to react.

    QVERIFY(subject->rect().isValid());

    subject->appear();
    subject->setPosition(QPointF(), MEditorToolbar::BelowPointOfInterest);
    qApp->processEvents(); // Allow setPosition() to take effect

    // In parent coordinates
    QPointF subjectCenter = fakeParent->mapFromItem(subject,
                                                    subject->rect().center());

    // Widget is below 0, arrow pointing up.
    QVERIFY(subjectCenter.y() > 0.0f);


    // Test other way around.

    subject->setPosition(QPointF(), MEditorToolbar::AbovePointOfInterest);
    qApp->processEvents(); // Allow setPosition() to take effect

    // In parent coordinates
    subjectCenter = fakeParent->mapFromItem(subject,
                                            subject->rect().center());

    // Widget is above 0, arrow pointing down.
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

void Ut_MEditorToolbar::testAutoHide_data()
{
    QTest::addColumn<Operations>("ops");
    QTest::addColumn<bool>("isAppearedImmediately");
    QTest::addColumn<bool>("isAppearedAfterTimeout");

    QTest::newRow("1") << (Operations() << Appear) << true << true;
    QTest::newRow("2") << (Operations() << Appear << AutoHideEnable) << true << false;
    QTest::newRow("3") << (Operations() << Appear << AutoHideEnable << AutoHideDisable) << true << true;
    QTest::newRow("4") << (Operations() << Appear << AutoHideEnable << AutoHideDisable << AutoHideEnable) << true << false;
    QTest::newRow("5") << (Operations() << AutoHideEnable << Appear) << true << false;
    QTest::newRow("6") << (Operations() << AutoHideEnable << Appear << Disappear) << false << false;
    QTest::newRow("7") << (Operations() << AutoHideEnable << Appear << Disappear << Appear) << true << false;
}

void Ut_MEditorToolbar::testAutoHide()
{
    QFETCH(Operations, ops);
    QFETCH(bool, isAppearedImmediately);
    QFETCH(bool, isAppearedAfterTimeout);

    MEditorToolbarStyle *style = const_cast<MEditorToolbarStyle *>(subject->style().operator ->());
    style->setHideTimeout(1);
    style->setAutoHideAnimationDuration(1);
    style->setHideAnimationDuration(1);
    style->setShowAnimationDuration(1);

    foreach (Operation op, ops) {
        switch (op) {
        case Appear:
            subject->appear();
            break;
        case Disappear:
            subject->disappear();
            break;
        case AutoHideEnable:
            subject->setAutoHideEnabled(true);
            break;
        case AutoHideDisable:
            subject->setAutoHideEnabled(false);
            break;
        default:
            break;
        }
    }

    QCOMPARE(subject->isAppeared(), isAppearedImmediately);
    QTest::qWait(100);
    QCOMPARE(subject->isAppeared(), isAppearedAfterTimeout);
}

void Ut_MEditorToolbar::testAnimationDuration_data()
{
    // Set any duration values. Only thing that matters is that they are different.
    const int autoHideDuration = 5;
    const int hideDuration = 6;
    const int showDuration = 7;

    // These are the same for all rows.
    QTest::addColumn<int>("autoHideDuration");
    QTest::addColumn<int>("hideDuration");
    QTest::addColumn<int>("showDuration");

    QTest::addColumn<Operations>("ops");
    QTest::addColumn<int>("expectedDuration");

    QTest::newRow("disappear")
        << autoHideDuration << hideDuration << showDuration
        << (Operations() << Disappear) << hideDuration;

    QTest::newRow("appear")
        << autoHideDuration << hideDuration << showDuration
        << (Operations() << Appear) << showDuration;

    QTest::newRow("auto-hide")
        << autoHideDuration << hideDuration << showDuration
        << (Operations() << Appear << AutoHideEnable) << autoHideDuration;
}

void Ut_MEditorToolbar::testAnimationDuration()
{
    QFETCH(Operations, ops);
    QFETCH(int, expectedDuration);

    QFETCH(int, autoHideDuration);
    QFETCH(int, hideDuration);
    QFETCH(int, showDuration);

    MEditorToolbarStyle *style = const_cast<MEditorToolbarStyle *>(subject->style().operator ->());
    style->setHideTimeout(1);
    style->setAutoHideAnimationDuration(autoHideDuration);
    style->setHideAnimationDuration(hideDuration);
    style->setShowAnimationDuration(showDuration);

    foreach (Operation op, ops) {
        switch (op) {
        case Appear:
            subject->appear();
            break;
        case Disappear:
            subject->disappear();
            break;
        case AutoHideEnable:
            subject->setAutoHideEnabled(true);
            break;
        case AutoHideDisable:
            subject->setAutoHideEnabled(false);
            break;
        default:
            break;
        }
    }

    if (subject->isAutoHideEnabled()) {
        // Wait for the 1msec auto hide timeout.
        QTest::qWait(50);
    }

    QCOMPARE(subject->d_func()->hideAnimation.duration(), expectedDuration);
}

void Ut_MEditorToolbar::testZOrder()
{
    QScopedPointer<MTopLevelOverlay> overlay(new MTopLevelOverlay(fakeParent->sceneManager()));
    QVERIFY(subject->d_ptr->overlay->zValue() > overlay->zValue());
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
