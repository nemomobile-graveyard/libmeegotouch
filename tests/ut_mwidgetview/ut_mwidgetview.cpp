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
#include <QGraphicsSceneMouseEvent>

#include <mwidgetview.h>

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

QTEST_APPLESS_MAIN(Ut_MWidgetView)
