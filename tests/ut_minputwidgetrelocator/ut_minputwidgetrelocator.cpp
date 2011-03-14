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

#include "mabstractscroller.h"
#include "mdeviceprofile.h"
#include "mrelocatorstyle.h"
#include "mscrollchain.h"
#include "mscenemanager.h"
#include "mscene.h"
#include "minputmethodstate.h"
#include "minputwidgetrelocator.h"
#include "mkeyboardstatetracker.h"
#include "ut_minputwidgetrelocator.h"
#include "scrollablewidget.h"
#include "scroller.h"

#include <QGraphicsView>

namespace {
    // Stubbed style values. These are what subject sees as values read from MRelocatorStyle.
    qreal gStyleVerticalAnchorPosition = 0.0;
    qreal gStyleTopNoGoMargin = 0.0;
    qreal gStyleBottomNoGoMargin = 0.0;
}

Q_DECLARE_METATYPE(Ut_MInputWidgetRelocator::Operations);
Q_DECLARE_METATYPE(Ut_MInputWidgetRelocator::Zone);

void Ut_MInputWidgetRelocator::initTestCase()
{
    qRegisterMetaType<Operations>("Operations");
    qRegisterMetaType<Zone>("Zone");

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_minputwidgetrelocator" };
    MApplication::setLoadMInputContext(false);

    app = new MApplication(argc, app_name);

    sceneManager = new MSceneManager(new MScene(QRectF(QPointF(),
                                                       MDeviceProfile::instance()->resolution())));
    scene = sceneManager->scene();
    view = new QGraphicsView(scene);

    QApplication::setActiveWindow(view);
    view->show();
    view->setFocus();
    QTest::qWaitForWindowShown(view);
    QTest::qWait(1000);
    QVERIFY(view->isVisible());

    rootElement = new QGraphicsWidget;
    rootElement->setFlag(QGraphicsItem::ItemHasNoContents, true);
    scene->addItem(rootElement);

    Scroller::maximumOffset = QPoint(1000, 1000);
    MScrollChain::registerScroller<ScrollableWidget>(QSharedPointer<Scroller>(new Scroller));
}

void Ut_MInputWidgetRelocator::cleanupTestCase()
{
    delete sceneManager;
    sceneManager = 0;
    delete view;
    view = 0;
    delete app;
    app = 0;
}

void Ut_MInputWidgetRelocator::init()
{
    // Let's have something funny here so we get "changed" signals.
    MInputMethodState::instance()->setInputMethodArea(QRect(1, 2, 3, 4));

    subject = new MInputWidgetRelocator(*sceneManager, rootElement, M::Landscape);

    parentScrollableWidget = QPointer<ScrollableWidget>(new ScrollableWidget(rootElement));
    parentScrollableWidget->resize(1000, 1000);
    childScrollableWidget = QPointer<ScrollableWidget>(new ScrollableWidget);
    childScrollableWidget->resize(1000, 1000);
    inputWidget = QPointer<InputWidget>(new InputWidget);

    parentScrollableWidget->setContentItem(childScrollableWidget);
    childScrollableWidget->setContentItem(inputWidget);
}

void Ut_MInputWidgetRelocator::cleanup()
{
    delete inputWidget;
    delete childScrollableWidget;
    delete parentScrollableWidget;

    delete subject;
    subject = 0;
}

void Ut_MInputWidgetRelocator::testPostponeAndUpdate_data()
{
    QTest::addColumn<Operations>("operations");
    QTest::addColumn<bool>("updated");

    // Operations RotationEnd, HwKbChange, and SipChange always sets an update pending.

    QTest::newRow("begin rotation") << (Operations() << RotationBegin) << false;
    QTest::newRow("end rotation") << (Operations() << RotationEnd) << true;
    QTest::newRow("begin scene window animation") << (Operations() << SceneWindowAppearing) << false;
    QTest::newRow("begin and end scene window animation")
            << (Operations() << SceneWindowAppearing << SceneWindowAppeared) << false;

    QTest::newRow("rotation during animation, animation not finished")
            << (Operations() << SceneWindowAppearing << RotationBegin << RotationEnd) << false;
    QTest::newRow("rotation during animation, animation finished")
            << (Operations() << SceneWindowAppearing
                             << RotationBegin << RotationEnd
                             << SceneWindowAppeared) << true;

    QTest::newRow("hwkb changed state") << (Operations() << HwKbChange) << true;
    QTest::newRow("hwkb changed state but animating") << (Operations() << SceneWindowAppearing << HwKbChange) << false;
    QTest::newRow("hwkb changed state but rotating") << (Operations() << RotationBegin << HwKbChange) << false;

    QTest::newRow("sip changed") << (Operations() << SipChange) << true;
    QTest::newRow("sip changed but animating") << (Operations() << SceneWindowAppearing << SipChange) << false;
    QTest::newRow("sip changed but rotating") << (Operations() << RotationBegin << SipChange) << false;

    QTest::newRow("sip changed, rotation finished but animation didn't")
            << (Operations() << RotationBegin << SceneWindowAppearing
                             << SipChange << RotationEnd) << false;
    QTest::newRow("sip changed, rotation and animation finished")
            << (Operations() << RotationBegin << SceneWindowAppearing
                             << SipChange << RotationEnd << SceneWindowAppeared) << true;
}

void Ut_MInputWidgetRelocator::testPostponeAndUpdate()
{
    QFETCH(Operations, operations);
    QFETCH(bool, updated);

    const QRect sipRect(QPoint(0, 300), scene->sceneRect().size().toSize());

    const QPointF invalidPos(-1000, -1000);
    inputWidget->setPos(invalidPos); // Position somewhere beyond nogo zones so relocation takes place.
    inputWidget->setFocus();

    foreach (Operation op, operations) {
        switch (op) {
        case RotationBegin:
            subject->handleRotationBegin(M::Landscape);
            break;
        case RotationEnd:
            subject->handleRotationFinished();
            break;
        case SceneWindowAppearing:
            subject->sceneWindowStateHasChanged(0, MSceneWindow::Appearing, MSceneWindow::Disappeared);
            break;
        case SceneWindowAppeared:
            subject->sceneWindowStateHasChanged(0, MSceneWindow::Appeared, MSceneWindow::Appearing);
            break;
        case SipChange:
            // Emit protected signal
            QVERIFY(QMetaObject::invokeMethod(MInputMethodState::instance(),
                                              "inputMethodAreaChanged",
                                              Qt::DirectConnection,
                                              Q_ARG(QRect, sipRect)));
            // Signal is connected via queued connection. Visit event loop.
            qApp->processEvents();
            break;
        case HwKbChange:
            // Emit protected signal
            QVERIFY(QMetaObject::invokeMethod(MKeyboardStateTracker::instance(),
                                              "stateChanged",
                                              Qt::DirectConnection));
            break;
        }
    }

    const QPointF actualPos = rootElement->mapFromItem(inputWidget, 0, 0);
    qDebug() << "actual: " << actualPos << ", invalid Pos: " << invalidPos;
    bool actualUpdated = actualPos != invalidPos;
    QCOMPARE(actualUpdated, updated);
}

void Ut_MInputWidgetRelocator::testTargetPosition_data()
{
    QTest::addColumn<M::Orientation>("orientation");
    QTest::addColumn<QRect>("sipRect");
    QTest::addColumn<Zone>("initialZone");

    // Sip rectangle is given in scene coordinates.
    const QSize screenSize(MDeviceProfile::instance()->resolution());
    const QRect sipRect(0, screenSize.height() / 2,
                        screenSize.width(),
                        screenSize.height() - (screenSize.height() / 2));

    QTest::newRow("no sip, widget at upper nogo zone")   << M::Landscape << QRect() << UpperNoGoZone;
    QTest::newRow("no sip, widget already visible")    << M::Landscape << QRect() << AllowedZone;
    QTest::newRow("no sip, widget at lower nogo zone")   << M::Landscape << QRect() << LowerNoGoZone;
    QTest::newRow("with sip, widget at upper nogo zone") << M::Landscape << sipRect << UpperNoGoZone;
    QTest::newRow("with sip, widget already visible")  << M::Landscape << sipRect << AllowedZone;
    QTest::newRow("with sip, widget at lower nogo zone") << M::Landscape << sipRect << LowerNoGoZone;

    QTest::newRow("PT, no sip, widget at upper nogo zone")   << M::Portrait << QRect() << UpperNoGoZone;
    QTest::newRow("PT, no sip, widget already visible")    << M::Portrait << QRect() << AllowedZone;
    QTest::newRow("PT, no sip, widget at lower nogo zone")   << M::Portrait << QRect() << LowerNoGoZone;
    QTest::newRow("PT, with sip, widget at upper nogo zone") << M::Portrait << sipRect << UpperNoGoZone;
    QTest::newRow("PT, with sip, widget already visible")  << M::Portrait << sipRect << AllowedZone;
    QTest::newRow("PT, with sip, widget at lower nogo zone") << M::Portrait << sipRect << LowerNoGoZone;
}

void Ut_MInputWidgetRelocator::testTargetPosition()
{
    QFETCH(M::Orientation, orientation);
    QFETCH(QRect, sipRect);
    QFETCH(Zone, initialZone);

    // Apply orientation
    subject->handleRotationBegin(orientation);
    subject->handleRotationFinished();

    updateStubbedStyleValues(orientation);

    MInputMethodState::instance()->setInputMethodArea(sipRect);
    qApp->processEvents(); //  Process above area change.

    QRect exposedContentRect(rootElement->mapRectFromScene(scene->sceneRect()).toRect());
    if (!sipRect.isNull()) {
        sipRect = rootElement->mapRectFromScene(sipRect).toRect();
        // Assume sip comes from bottom of screen.
        exposedContentRect.setBottom(sipRect.top());
    }

    moveWidgetToZone(inputWidget, initialZone, exposedContentRect, orientation);
    inputWidget->setFocus();

    QRectF actualRectBefore = rootElement->mapRectFromItem(inputWidget, inputWidget->rect());
    subject->update();
    QRectF actualRectAfter = rootElement->mapRectFromItem(inputWidget, inputWidget->rect());

    // Test two cases here:
    // 1) If widget was initially beyond nogo zones the new position should be at anchor point.
    // 2) If widget was already in allowed zone it should remain where it was.

    switch (initialZone) {
    case UpperNoGoZone:
    case LowerNoGoZone:
        QCOMPARE(actualRectAfter.y(), verticalAnchorPosition(orientation));
        QVERIFY(allowedZone(exposedContentRect, orientation).contains(actualRectAfter.toRect()));
        break;
    case AllowedZone:
        // Already in allowed zone. Shouldn't have moved.
        QCOMPARE(actualRectAfter, actualRectBefore);
        break;
    }
}

void Ut_MInputWidgetRelocator::testDockBottom_data()
{
    QTest::addColumn<M::Orientation>("orientation");
    QTest::addColumn<QRect>("sipRect");
    QTest::addColumn<Zone>("initialZone");

    // Sip rectangle is given in scene coordinates.
    const QSize screenSize(MDeviceProfile::instance()->resolution());
    const QRect sipRect(0, screenSize.height() / 2,
                        screenSize.height() - (screenSize.height() / 2),
                        screenSize.width());

    QTest::newRow("no sip, widget at upper nogo zone")   << M::Landscape << QRect() << UpperNoGoZone;
    QTest::newRow("no sip, widget already visible")    << M::Landscape << QRect() << AllowedZone;
    QTest::newRow("no sip, widget at lower nogo zone")   << M::Landscape << QRect() << LowerNoGoZone;
    QTest::newRow("with sip, widget at upper nogo zone") << M::Landscape << sipRect << UpperNoGoZone;
    QTest::newRow("with sip, widget already visible")  << M::Landscape << sipRect << AllowedZone;
    QTest::newRow("with sip, widget at lower nogo zone") << M::Landscape << sipRect << LowerNoGoZone;

    QTest::newRow("PT, no sip, widget at upper nogo zone")   << M::Portrait << QRect() << UpperNoGoZone;
    QTest::newRow("PT, no sip, widget already visible")    << M::Portrait << QRect() << AllowedZone;
    QTest::newRow("PT, no sip, widget at lower nogo zone")   << M::Portrait << QRect() << LowerNoGoZone;
    QTest::newRow("PT, with sip, widget at upper nogo zone") << M::Portrait << sipRect << UpperNoGoZone;
    QTest::newRow("PT, with sip, widget already visible")  << M::Portrait << sipRect << AllowedZone;
    QTest::newRow("PT, with sip, widget at lower nogo zone") << M::Portrait << sipRect << LowerNoGoZone;
}

void Ut_MInputWidgetRelocator::testDockBottom()
{
    QFETCH(M::Orientation, orientation);
    QFETCH(QRect, sipRect);
    QFETCH(Zone, initialZone);

    // Apply orientation
    subject->handleRotationBegin(orientation);
    subject->handleRotationFinished();

    updateStubbedStyleValues(orientation);

    MInputMethodState::instance()->setInputMethodArea(sipRect);
    qApp->processEvents(); //  Process above area change.

    QRect exposedContentRect(rootElement->mapRectFromScene(scene->sceneRect()).toRect());
    if (!sipRect.isNull()) {
        sipRect = rootElement->mapRectFromScene(sipRect).toRect();
        // Assume sip comes from bottom of screen.
        exposedContentRect.setBottom(sipRect.top());
    }

    // We will dock childScrollableWidget.
    childScrollableWidget->setProperty("dockBottom", QVariant(true));
    childScrollableWidget->resize(10, 10);

    moveWidgetToZone(inputWidget, initialZone, exposedContentRect, orientation);
    inputWidget->setFocus();

    subject->update();
    const QRectF dockedRectAfter = rootElement->mapRectFromItem(childScrollableWidget,
                                                                childScrollableWidget->rect());

    qDebug() << "docked rect bottom: " << dockedRectAfter.toRect().bottom()
            << " vs. exposed content rectangle bottom: " << exposedContentRect.bottom();

    // No matter where the input widget was it should always dock to bottom.
    // FIXME: Since the scrolling system does not play with qreals we might get rounding errors here.
    //        Allow 1 pixel error.
    qreal error = dockedRectAfter.bottom()
                  - static_cast<qreal>(exposedContentRect.bottom());
    QVERIFY(qAbs<qreal>(error) <= 1.0);
}

// Helper methods

QRect Ut_MInputWidgetRelocator::allowedZone(const QRect &exposedContentRect, M::Orientation orientation) const
{
    return exposedContentRect.adjusted(0, topNoGoMargin(orientation),
                                       0, bottomNoGoMargin(orientation));
}

// Following values have been copied from real base theme style
// but they don't need to be precisely the same.
qreal Ut_MInputWidgetRelocator::verticalAnchorPosition(M::Orientation orientation) const
{
    if (orientation == M::Landscape) {
        // Could return different value when hardware keyboard open (150.0).
        return 40.0;
    } else {
        return 200;
    }
}

qreal Ut_MInputWidgetRelocator::topNoGoMargin(M::Orientation orientation) const
{
    if (orientation == M::Landscape) {
        return 40.0;
    } else {
        return 48.0;
    }
}

qreal Ut_MInputWidgetRelocator::bottomNoGoMargin(M::Orientation orientation) const
{
    if (orientation == M::Landscape) {
        return 40.0;
    } else {
        return 48.0;
    }
}

void Ut_MInputWidgetRelocator::moveWidgetToZone(InputWidget *widget,
                                                Ut_MInputWidgetRelocator::Zone zone,
                                                const QRect &exposedContentRect,
                                                M::Orientation orientation) const
{
    const QRect microRect(widget->microFocusRect());
    const QRect allowedRect(allowedZone(exposedContentRect, orientation));

    switch (zone) {
    case UpperNoGoZone:
        widget->setPos(widget->pos().x(), allowedRect.top() - microRect.bottom());
        break;
    case LowerNoGoZone:
        widget->setPos(widget->pos().x(), allowedRect.bottom() - microRect.top());
        break;
    case AllowedZone:
        widget->setPos(widget->pos().x(), allowedRect.center().y());
        break;
    }
}

void Ut_MInputWidgetRelocator::updateStubbedStyleValues(M::Orientation orientation) const
{
    gStyleVerticalAnchorPosition = verticalAnchorPosition(orientation);
    gStyleTopNoGoMargin = topNoGoMargin(orientation);
    gStyleBottomNoGoMargin = bottomNoGoMargin(orientation);
}

QTEST_APPLESS_MAIN(Ut_MInputWidgetRelocator)

// Stubbing MRelocatorStyle which is defined in stub mrelocatorstyle.h in current test directory.
qreal MRelocatorStyle::verticalAnchorPosition() const {return gStyleVerticalAnchorPosition;}
qreal MRelocatorStyle::topNoGoMargin() const {return gStyleTopNoGoMargin;}
qreal MRelocatorStyle::bottomNoGoMargin() const {return gStyleBottomNoGoMargin;}
