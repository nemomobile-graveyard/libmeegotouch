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
#include <MApplication>
#include <MApplicationWindow>
#include <MWidgetController>
#include <MScalableImage>
#include <MSceneManager>
#include <mextendingbackgroundview.h>
#include "ut_mextendingbackgroundview.h"

TestExtendingBackgroundView::TestExtendingBackgroundView(MWidgetController *controller) : MExtendingBackgroundView(controller)
{
}

QRectF TestExtendingBackgroundView::boundingRect() const
{
    return MExtendingBackgroundView::boundingRect();
}

QRectF TestExtendingBackgroundView::geometry() const
{
    return MExtendingBackgroundView::geometry();
}

MExtendingBackgroundStyle *TestExtendingBackgroundView::modifiableStyle()
{
    MExtendingBackgroundStyleContainer &sc = style();
    const MExtendingBackgroundStyle *const_s = sc.operator ->();
    MExtendingBackgroundStyle *s = const_cast<MExtendingBackgroundStyle *>(const_s);
    return s;
}


void Ut_MExtendingBackgroundView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mextendingbackgroundview" };
    app = new MApplication(argc, app_name);
    appWin = new MApplicationWindow;
}

void Ut_MExtendingBackgroundView::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_MExtendingBackgroundView::init()
{
    controller = new MWidgetController;
    view = new TestExtendingBackgroundView(controller);
    controller->setView(view);
}

void Ut_MExtendingBackgroundView::cleanup()
{
    delete controller;
}

void Ut_MExtendingBackgroundView::testBoundingRect()
{
    // Use a scalable image in the view's style
    int left = 10;
    int right = 20;
    int top = 30;
    int bottom = 40;
    MScalableImage image(new QPixmap(100, 100), left, right, top, bottom);
    view->modifiableStyle()->setBackgroundImage(&image);
    controller->setGeometry(QRectF(10, 10, 50, 50));

    // Get the view's geometry
    QRectF geometry = view->geometry();

    // No background extending
    view->modifiableStyle()->setExtendDirection("");
    QRectF boundingRect = view->boundingRect();
    QCOMPARE(boundingRect, QRectF(0, 0, geometry.width(), geometry.height()));

    // Background extends to the left
    view->modifiableStyle()->setExtendDirection("left");
    boundingRect = view->boundingRect();
    QCOMPARE(boundingRect, QRectF(-geometry.x() - left, 0, geometry.x() + geometry.width() + left, geometry.height()));

    // Background extends to the right
    view->modifiableStyle()->setExtendDirection("right");
    boundingRect = view->boundingRect();
    QCOMPARE(boundingRect, QRectF(0, 0, MApplication::activeApplicationWindow()->sceneManager()->visibleSceneSize().width() - geometry.x() + right, geometry.height()));

    // Background extends to top
    view->modifiableStyle()->setExtendDirection("top");
    boundingRect = view->boundingRect();
    QCOMPARE(boundingRect, QRectF(0, -geometry.y() - top, geometry.width(), geometry.y() + geometry.height() + top));

    // Background extends to bottom
    view->modifiableStyle()->setExtendDirection("bottom");
    boundingRect = view->boundingRect();
    QCOMPARE(boundingRect, QRectF(0, 0, geometry.width(), MApplication::activeApplicationWindow()->sceneManager()->visibleSceneSize().height() - geometry.y() + bottom));
}

QTEST_APPLESS_MAIN(Ut_MExtendingBackgroundView)
