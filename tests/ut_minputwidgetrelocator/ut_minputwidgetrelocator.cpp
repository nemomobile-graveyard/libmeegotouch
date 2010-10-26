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
#include "minputmethodstate.h"
#include "minputwidgetrelocator.h"
#include "mkeyboardstatetracker.h"
#include "ut_minputwidgetrelocator.h"

#include <QGraphicsView>

namespace {
    QRect gLastTargetRect;
    int gScrollCallCount = 0;
    QPointer<QGraphicsWidget> gDummyWidget;

    // Stubbed style values. These are what subject sees as values read from MRelocatorStyle.
    qreal gStyleVerticalAnchorPosition = 0.0;
    qreal gStyleTopNoGoMargin = 0.0;
    qreal gStyleBottomNoGoMargin = 0.0;
}

Q_DECLARE_METATYPE(Ut_MInputWidgetRelocator::Operations);
Q_DECLARE_METATYPE(Ut_MInputWidgetRelocator::Zone);


class InputWidget : public QGraphicsWidget
{
public:
    InputWidget(QGraphicsItem *parent = 0)
        :QGraphicsWidget(parent)
    {
        setFlag(QGraphicsItem::ItemIsFocusable, true);
        setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);
        resize(100, 20);
    }

    virtual ~InputWidget()
    {
    }

    QRect microFocusRect() const
    {
        return rect().toRect(); // only height is important at the moment
    }

protected:
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const
    {
        if (query == Qt::ImMicroFocus) {
            return microFocusRect();
        }
        return QGraphicsWidget::inputMethodQuery(query);
    }
};

void Ut_MInputWidgetRelocator::initTestCase()
{
    qRegisterMetaType<Operations>("Operations");
    qRegisterMetaType<Zone>("Zone");

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_minputwidgetrelocator" };
    MApplication::setLoadMInputContext(false);

    app = new MApplication(argc, app_name);

    scene = new QGraphicsScene(QRectF(QPointF(), MDeviceProfile::instance()->resolution()));
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

    gDummyWidget = QPointer<QGraphicsWidget>(new QGraphicsWidget);
    scene->addItem(gDummyWidget);
}

void Ut_MInputWidgetRelocator::cleanupTestCase()
{
    delete scene;
    scene = 0;
    delete view;
    view = 0;
    delete app;
    app = 0;
}

void Ut_MInputWidgetRelocator::init()
{
    subject = new MInputWidgetRelocator(scene, rootElement, M::Landscape);
}

void Ut_MInputWidgetRelocator::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MInputWidgetRelocator::testPostponeAndUpdate_data()
{
    QTest::addColumn<Operations>("operations");
    QTest::addColumn<int>("numberOfUpdates");

    // Operations RotationEnd, HwKbChange, and SipChange always sets an update pending.

    QTest::newRow("begin rotation") << (Operations() << RotationBegin) << 0;
    QTest::newRow("end rotation") << (Operations() << RotationEnd) << 1;
    QTest::newRow("begin scene window animation") << (Operations() << SceneWindowAppearing) << 0;
    QTest::newRow("begin and end scene window animation")
            << (Operations() << SceneWindowAppearing << SceneWindowAppeared) << 0;

    QTest::newRow("rotation during animation, animation not finished")
            << (Operations() << SceneWindowAppearing << RotationBegin << RotationEnd) << 0;
    QTest::newRow("rotation during animation, animation finished")
            << (Operations() << SceneWindowAppearing
                             << RotationBegin << RotationEnd
                             << SceneWindowAppeared) << 1;

    QTest::newRow("hwkb changed state") << (Operations() << HwKbChange) << 1;
    QTest::newRow("hwkb changed state but animating") << (Operations() << SceneWindowAppearing << HwKbChange) << 0;
    QTest::newRow("hwkb changed state but rotating") << (Operations() << RotationBegin << HwKbChange) << 0;

    QTest::newRow("sip changed") << (Operations() << SipChange) << 1;
    QTest::newRow("sip changed but animating") << (Operations() << SceneWindowAppearing << SipChange) << 0;
    QTest::newRow("sip changed but rotating") << (Operations() << RotationBegin << SipChange) << 0;

    QTest::newRow("sip changed, rotation finished but animation didn't")
            << (Operations() << RotationBegin << SceneWindowAppearing
                             << SipChange << RotationEnd) << 0;
    QTest::newRow("sip changed, rotation and animation finished")
            << (Operations() << RotationBegin << SceneWindowAppearing
                             << SipChange << RotationEnd << SceneWindowAppeared) << 1;
}

void Ut_MInputWidgetRelocator::testPostponeAndUpdate()
{
    QFETCH(Operations, operations);
    QFETCH(int, numberOfUpdates);

    const QRect sipRect(QPoint(0, 300), scene->sceneRect().size().toSize());
    MInputMethodState::instance()->setInputMethodArea(sipRect);

    // Only create input widget to be able to follow MScrollChain::run().
    InputWidget widget(rootElement);
    moveWidgetToZone(&widget, UpperNoGoZone, scene->sceneRect().toRect(), M::Landscape);
    widget.setPos(-100, -100); // Position somewhere beyond nogo zones so relocation takes place.
    widget.setFocus();

    gScrollCallCount = 0;

    foreach (Operation op, operations) {
        switch (op) {
        case RotationBegin:
            subject->handleRotationBegin();
            break;
        case RotationEnd:
            subject->handleRotationFinished(M::Landscape);
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
            break;
        case HwKbChange:
            // Emit protected signal
            QVERIFY(QMetaObject::invokeMethod(MKeyboardStateTracker::instance(),
                                              "stateChanged",
                                              Qt::DirectConnection));
            break;
        }
    }

    QCOMPARE(gScrollCallCount, numberOfUpdates);
}

void Ut_MInputWidgetRelocator::testTargetPosition_data()
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

void Ut_MInputWidgetRelocator::testTargetPosition()
{
    QFETCH(M::Orientation, orientation);
    QFETCH(QRect, sipRect);
    QFETCH(Zone, initialZone);

    // Apply orientation
    subject->handleRotationBegin();
    subject->handleRotationFinished(orientation);

    updateStubbedStyleValues(orientation);

    MInputMethodState::instance()->setInputMethodArea(sipRect);
    QRect exposedContentRect(rootElement->mapRectFromScene(scene->sceneRect()).toRect());
    if (!sipRect.isNull()) {
        sipRect = rootElement->mapRectFromScene(sipRect).toRect();
        // Assume sip comes from bottom of screen.
        exposedContentRect.setBottom(sipRect.top());
    }

    InputWidget widget(rootElement);
    moveWidgetToZone(&widget, initialZone, exposedContentRect, orientation);
    widget.setFocus();

    // Set target rect of stub first as invalid.
    const QRect invalidRect(-10, -10, -10, -10);
    gLastTargetRect = invalidRect;

    subject->update();

    // Test two cases here:
    // 1) If widget was initially beyond nogo zones the new position should be at anchor point.
    // 2) If widget was already in allowed zone it should remain where it was.

    switch (initialZone) {
    case UpperNoGoZone:
    case LowerNoGoZone:
        QCOMPARE(static_cast<qreal>(gLastTargetRect.y()), verticalAnchorPosition(orientation));
        QVERIFY(allowedZone(exposedContentRect, orientation).contains(gLastTargetRect));
        break;
    case AllowedZone:
        // Already in allowed zone. Shouldn't have moved.
        QCOMPARE(gLastTargetRect, invalidRect);
        break;
    }
}

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

// Stubbing MScrollChain

MScrollChain::MScrollChain(const QGraphicsItem *, const QGraphicsItem *)
    : scrollTarget(0)
{
}

MScrollChain::~MScrollChain()
{
}

int MScrollChain::count() const
{
    return 1;
}

const QGraphicsWidget *MScrollChain::widgetAt(int index) const
{
    Q_UNUSED(index);
    return gDummyWidget;
}

void MScrollChain::restore(const MScrollChain *excludeChain)
{
    Q_UNUSED(excludeChain);
}

void MScrollChain::addBottomUpScroll(const QRect &targetRect, const QPoint &originPoint, int index)
{
    gLastTargetRect = targetRect;
    Q_UNUSED(originPoint);
    Q_UNUSED(index);
}

void MScrollChain::applyScrolling()
{
    ++gScrollCallCount;
}

QTEST_APPLESS_MAIN(Ut_MInputWidgetRelocator)

// Stubbing MRelocatorStyle which is defined in stub mrelocatorstyle.h in current test directory.
qreal MRelocatorStyle::verticalAnchorPosition() const {return gStyleVerticalAnchorPosition;}
qreal MRelocatorStyle::topNoGoMargin() const {return gStyleTopNoGoMargin;}
qreal MRelocatorStyle::bottomNoGoMargin() const {return gStyleBottomNoGoMargin;}
