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

#include <duiwidgetview.h>

#include "ut_duiwidgetview.h"

class MyDuiWidgetView : public DuiWidgetView
{
public:
    MyDuiWidgetView(DuiWidgetController *controller);
    virtual ~MyDuiWidgetView() {}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
};

MyDuiWidgetView::MyDuiWidgetView(DuiWidgetController *controller)
    : DuiWidgetView(controller)
{
}


void MyDuiWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MyDuiWidgetView::setGeometry(const QRectF &rect)
{
    Q_UNUSED(rect);
}

QRectF MyDuiWidgetView::boundingRect() const
{
    return QRectF();
}

QSizeF MyDuiWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);
    return QSizeF();
}

void Ut_DuiWidgetView::init()
{
}

void Ut_DuiWidgetView::cleanup()
{
}

void Ut_DuiWidgetView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duibuttonview" };
    app = new DuiApplication(argc, app_name);

    // DuiWidgetView is pure abstract
    // to test, we need something derived from it

    m_controller = new DuiWidgetController();
    m_subject = new MyDuiWidgetView(m_controller);
    m_controller->setView(m_subject);
}

void Ut_DuiWidgetView::cleanupTestCase()
{
    //delete m_subject;
    //m_subject = 0;
    delete m_controller;
    m_controller = 0;

    delete app;
    app = 0;
}

void Ut_DuiWidgetView::setObjectName()
{
    m_subject->setObjectName("myTestString");
}

void Ut_DuiWidgetView::shape()
{
    m_subject->shape();
}

void Ut_DuiWidgetView::resizeEvent()
{
    QGraphicsSceneResizeEvent myQGSE;

    m_subject->resizeEvent(&myQGSE);
}

void Ut_DuiWidgetView::mousePressEvent()
{
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(mouseEvent);
}

void Ut_DuiWidgetView::mouseReleaseEvent()
{
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
    m_subject->mouseReleaseEvent(mouseEvent);
}

void Ut_DuiWidgetView::mouseMoveEvent()
{
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    m_subject->mouseMoveEvent(mouseEvent);
}

void Ut_DuiWidgetView::setBottomMarginParameter(DuiWidgetStyleContainer &container, int param)
{
    const DuiWidgetStyle *const_s = container.operator ->();
    DuiWidgetStyle *s = const_cast<DuiWidgetStyle *>(const_s);
    s->setMarginBottom(param);
}

void Ut_DuiWidgetView::testThatActiveStyleIsUsedInActiveState()
{
    // Define style parameter in default-mode style
    DuiWidgetStyleContainer &container = m_subject->style();
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

QTEST_APPLESS_MAIN(Ut_DuiWidgetView)
