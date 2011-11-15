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

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <mwidgetview.h>
#include <mwidgetview_p.h>

#include "ut_mwidgetview.h"

class MyMWidgetView : public MWidgetView
{
public:
    MyMWidgetView(MWidgetController *controller);
    virtual ~MyMWidgetView() {}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    virtual void drawBackground(QPainter *painter,
                                const QStyleOptionGraphicsItem *option) const
    {
        Q_UNUSED(painter);
        const_cast<MyMWidgetView *>(this)->lastBgExposedRect
            = option ? option->exposedRect : QRectF();
    }
    virtual void drawContents(QPainter *painter,
                              const QStyleOptionGraphicsItem *option) const
    {
        Q_UNUSED(painter);
        const_cast<MyMWidgetView *>(this)->lastContentsExposedRect
            = option ? option->exposedRect : QRectF();
    }
    virtual void drawForeground(QPainter *painter,
                                const QStyleOptionGraphicsItem *option) const
    {
        Q_UNUSED(painter);
        const_cast<MyMWidgetView *>(this)->lastFgExposedRect
            = option ? option->exposedRect : QRectF();
    }

    QRectF lastBgExposedRect;
    QRectF lastContentsExposedRect;
    QRectF lastFgExposedRect;
};

MyMWidgetView::MyMWidgetView(MWidgetController *controller)
    : MWidgetView(controller)
{
}


void MyMWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MyMWidgetView::setGeometry(const QRectF &rect)
{
    Q_UNUSED(rect);
}

QRectF MyMWidgetView::boundingRect() const
{
    return QRectF();
}

QSizeF MyMWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);
    return QSizeF();
}

void Ut_MWidgetView::init()
{
}

void Ut_MWidgetView::cleanup()
{
}

void Ut_MWidgetView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbuttonview" };
    app = new MApplication(argc, app_name);

    // MWidgetView is pure abstract
    // to test, we need something derived from it

    m_controller = new MWidgetController();
    m_subject = new MyMWidgetView(m_controller);
    m_controller->setView(m_subject);
}

void Ut_MWidgetView::cleanupTestCase()
{
    //delete m_subject;
    //m_subject = 0;
    delete m_controller;
    m_controller = 0;

    delete app;
    app = 0;
}

void Ut_MWidgetView::setObjectName()
{
    m_subject->setObjectName("myTestString");
}

void Ut_MWidgetView::shape()
{
    m_subject->shape();
}

void Ut_MWidgetView::resizeEvent()
{
    QGraphicsSceneResizeEvent myQGSE;

    m_subject->resizeEvent(&myQGSE);
}

void Ut_MWidgetView::mousePressEvent()
{
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(mouseEvent);
}

void Ut_MWidgetView::mouseReleaseEvent()
{
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
    m_subject->mouseReleaseEvent(mouseEvent);
}

void Ut_MWidgetView::mouseMoveEvent()
{
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    m_subject->mouseMoveEvent(mouseEvent);
}

void Ut_MWidgetView::setBottomMarginParameter(MWidgetStyleContainer &container, int param)
{
    const MWidgetStyle *const_s = container.operator ->();
    MWidgetStyle *s = const_cast<MWidgetStyle *>(const_s);
    s->setMarginBottom(param);
}

void Ut_MWidgetView::testThatActiveStyleIsUsedInActiveState()
{
    // Define style parameter in default-mode style
    MWidgetStyleContainer &container = m_subject->style();
    setBottomMarginParameter(container, 3);
    QCOMPARE(container->marginBottom(), 3);

    // Define style parameter in active-mode style
    container.setModeActive();
    setBottomMarginParameter(container, 1);
    QCOMPARE(container->marginBottom(), 1);

    // Reset the style container to default mode
    container.setModeDefault();

    // Verify that default style is used
    QCOMPARE(m_subject->style()->marginBottom(), 3);

    // Set view to active state
    m_subject->setActive(true);

    // Verify that active style is used
    QCOMPARE(m_subject->style()->marginBottom(), 1);

    // Set view back to default state
    m_subject->setActive(false);

    // Verify that default style is used
    QCOMPARE(m_subject->style()->marginBottom(), 3);
}

void Ut_MWidgetView::testExposedRectTranslation()
{
    MyMWidgetView *subject = static_cast<MyMWidgetView *>(m_subject);

    // Get pointer to writable style.
    MWidgetStyle *style = const_cast<MWidgetStyle *>(m_subject->style().operator ->());

    const int margin = 5;
    style->setMarginLeft(margin);
    style->setMarginTop(margin);
    subject->applyStyle();

    // Set exposedRect to 10x10 rect sitting at item origin.
    QStyleOptionGraphicsItem option;
    option.exposedRect = QRectF(0, 0, 10, 10);

    QPainter painter;
    subject->MWidgetView::paint(&painter, &option);

    // exposedRect should be translated in drawBackground,
    // drawContents, but not in drawForeground.
    QCOMPARE(subject->lastBgExposedRect,
             QRectF(-5, -5, 10, 10));
    QCOMPARE(subject->lastContentsExposedRect,
             QRectF(-5, -5, 10, 10));
    QCOMPARE(subject->lastFgExposedRect,
             QRectF(0, 0, 10, 10));
}

void Ut_MWidgetView::testDrawBackground_data()
{
    QTest::addColumn<qreal>("parentOpacity");
    QTest::addColumn<qreal>("itemOpacity");
    QTest::addColumn<qreal>("itemBackgroundOpacity");
    QTest::addColumn<bool>("itemIgnoresParentOpacity");
    QTest::addColumn<QRgb>("backgroundColor");
    QTest::addColumn<QRgb>("expectedColor");

    // Composed color is 0x00 + 0xFF * 0 = 0 ~0x00
    QTest::newRow("parent, item and background opaque")
            << (qreal)1.0 << (qreal)1.0 << (qreal)1.0 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x00,0x00,0x00,0xFF);
    // Composed color is 0x00 + 0xFF * 0.5 = 127.5 ~0x80
    QTest::newRow("parent opaque, item semi-transparent, background opaque")
            << (qreal)1.0 << (qreal)0.5 << (qreal)1.0 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x80,0x80,0x80,0xFF);
    // Same as previous computation
    QTest::newRow("parent semi-transparent, item opaque, background opaque")
            << (qreal)0.5 << (qreal)1.0 << (qreal)1.0 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x80,0x80,0x80,0xFF);
    // Same as previous computation
    QTest::newRow("parent and item opaque, background semi-transparent")
            << (qreal)1.0 << (qreal)1.0 << (qreal)0.5 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x80,0x80,0x80,0xFF);
    // Composed color is 0x00 + 0xFF * 0.75 = 191.25 ~0xC0
    QTest::newRow("parent and item semi-transparent, background opaque")
            << (qreal)0.5 << (qreal)0.5 << (qreal)1.0 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0xC0,0xC0,0xC0,0xFF);
    // Same as previous
    QTest::newRow("parent opaque, item and background semi-transparent")
            << (qreal)1.0 << (qreal)0.5 << (qreal)0.5 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0xC0,0xC0,0xC0,0xFF);
    // Same as previous
    QTest::newRow("parent semi-transparent, item opaque, background semi-transparent")
            << (qreal)0.5 << (qreal)1.0 << (qreal)0.5 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0xC0,0xC0,0xC0,0xFF);
    // Composed color is 0x00 + 0xFF * 0.875 = 223.125 ~0xE0
    QTest::newRow("parent, item and background semi-transparent")
            << (qreal)0.5 << (qreal)0.5 << (qreal)0.5 << false << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0xE0,0xE0,0xE0,0xFF);


    // Same combinations, but item ignores parent opacity
    // Composed color is 0x00 + 0xFF * 0 = 0 ~0x00
    QTest::newRow("parent, item and background opaque, item ignores parent opacity")
            << (qreal)1.0 << (qreal)1.0 << (qreal)1.0 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x00,0x00,0x00,0xFF);
    // Composed color is 0x00 + 0xFF * 0.5 = 127.5 ~0x80
    QTest::newRow("parent opaque, item semi-transparent, background opaque, item ignores parent opacity")
            << (qreal)1.0 << (qreal)0.5 << (qreal)1.0 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x80,0x80,0x80,0xFF);
    // Composed color is 0x00 + 0xFF * 0 = 0 ~0x00
    QTest::newRow("parent semi-transparent, item opaque, background opaque, item ignores parent opacity")
            << (qreal)0.5 << (qreal)1.0 << (qreal)1.0 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x00,0x00,0x00,0xFF);
    // Composed color is 0x00 + 0xFF * 0.5 = 127.5 ~0x80
    QTest::newRow("parent and item opaque, background semi-transparent, item ignores parent opacity")
            << (qreal)1.0 << (qreal)1.0 << (qreal)0.5 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x80,0x80,0x80,0xFF);
    // Same as previous
    QTest::newRow("parent and item semi-transparent, background opaque, item ignores parent opacity")
            << (qreal)0.5 << (qreal)0.5 << (qreal)1.0 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x80,0x80,0x80,0xFF);
    // Composed color is 0x00 + 0xFF * 0.75 = 191.25 ~0xC0
    QTest::newRow("parent opaque, item and background semi-transparent, item ignores parent opacity")
            << (qreal)1.0 << (qreal)0.5 << (qreal)0.5 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0xC0,0xC0,0xC0,0xFF);
    // Composed color is 0x00 + 0xFF * 0.5 = 127.5 ~0x80
    QTest::newRow("parent semi-transparent, item opaque, background semi-transparent, item ignores parent opacity")
            << (qreal)0.5 << (qreal)1.0 << (qreal)0.5 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0x80,0x80,0x80,0xFF);
    // Composed color is 0x00 + 0xFF * 0.75 = 191.25 ~0xC0
    QTest::newRow("parent, item and background semi-transparent, item ignores parent opacity")
            << (qreal)0.5 << (qreal)0.5 << (qreal)0.5 << true << qRgba(0x00,0x00,0x00,0xFF) << qRgba(0xC0,0xC0,0xC0,0xFF);
}

void Ut_MWidgetView::testDrawBackground()
{
    if (QPixmap::defaultDepth() < 24)
        QSKIP("Executed only on higher pixmap color depths", SkipAll); // Skip the test as the expected value is not for lower bit depths

    QFETCH(qreal, parentOpacity);
    QFETCH(qreal, itemOpacity);
    QFETCH(qreal, itemBackgroundOpacity);
    QFETCH(bool, itemIgnoresParentOpacity);
    QFETCH(QRgb, backgroundColor);
    QFETCH(QRgb, expectedColor);

    // Prepare scene
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *window = new QGraphicsView;
    window->setBackgroundBrush(Qt::white);
    window->resize(100,100);
    window->setScene(scene);

    // Prepare parent widget
    QGraphicsWidget *parent = new QGraphicsWidget;
    m_controller->setParentItem(parent);
    parent->setOpacity(parentOpacity);
    scene->addItem(parent);

    // Prepare test widget view
    MWidgetView *view = new MWidgetView;
    m_controller->setView(view);
    m_subject = view;
    m_controller->setGeometry(QRectF(0, 0, 100, 100));
    m_controller->setOpacity(itemOpacity);
    m_controller->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, itemIgnoresParentOpacity);

    // Get pointer to writable style.
    MWidgetStyle *style = const_cast<MWidgetStyle *>(m_subject->style().operator ->());
    style->setBackgroundColor(QColor::fromRgba(backgroundColor));
    style->setBackgroundOpacity(itemBackgroundOpacity);

    QImage image = QPixmap::grabWidget(window).toImage();
    QCOMPARE(image.pixel(50, 50), expectedColor);

    scene->removeItem(parent);
    m_controller->setParentItem(NULL);

    delete parent;
    delete window;
    delete scene;
}

void Ut_MWidgetView::testDisabledChildKeepsStyleModeThroughoutTap()
{
    MWidgetController *child = new MWidgetController(m_controller);
    MWidgetView *childView = new MyMWidgetView(child);
    child->setView(childView);

    // Define style parameter in default-mode style
    MWidgetStyleContainer &container = childView->style();
    setBottomMarginParameter(container, 2);
    QCOMPARE(container->marginBottom(), 2);

    // Define style parameter in disabled-mode style
    container.setModeDisabled();
    setBottomMarginParameter(container, 5);
    QCOMPARE(container->marginBottom(), 5);

    // Reset the style container to default mode
    container.setModeDefault();

    child->setEnabled(false);

    // Verify that disabled style is used
    QCOMPARE(childView->style()->marginBottom(), 5);

    m_subject->d_ptr->setPressed(true);

    // Verify that disabled style is kept
    QCOMPARE(childView->style()->marginBottom(), 5);

    m_subject->d_ptr->setPressed(false);

    // Verify that disabled style is kept
    QCOMPARE(childView->style()->marginBottom(), 5);
}

void Ut_MWidgetView::testPressedModePropagatesToChildren()
{
    MWidgetController *child = new MWidgetController(m_controller);
    MWidgetView *childView = new MyMWidgetView(child);
    child->setView(childView);

    // Define style parameter in default-mode style
    MWidgetStyleContainer &container = childView->style();
    setBottomMarginParameter(container, 2);
    QCOMPARE(container->marginBottom(), 2);

    // Define style parameter in pressed-mode style
    container.setModePressed();
    setBottomMarginParameter(container, 5);
    QCOMPARE(container->marginBottom(), 5);

    // Reset the style container to default mode
    container.setModeDefault();

    // Verify that default style is used
    QCOMPARE(childView->style()->marginBottom(), 2);

    m_subject->d_ptr->setPressed(true);

    // Verify that pressed style is used
    QCOMPARE(childView->style()->marginBottom(), 5);

    m_subject->d_ptr->setPressed(false);

    // Verify that we go back to default style
    QCOMPARE(childView->style()->marginBottom(), 2);
}

QTEST_APPLESS_MAIN(Ut_MWidgetView)
