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

#ifndef UT_MMASHUPCANVAS_
#define UT_MMASHUPCANVAS_

#include <QObject>
#include <MWidgetView>
#include "mmashupcanvasmodel.h"
#include "mmashupcanvasstyle.h"

class TestMashupCanvas;
class MApplication;
class MApplicationWindow;


class TestMashupCanvasView : public MWidgetView
{
    M_VIEW(MMashupCanvasModel, MMashupCanvasStyle)

public:
    TestMashupCanvasView(MWidgetController *canvas) : MWidgetView(canvas) {}

};

class Ut_MMashupCanvas : public QObject
{
    Q_OBJECT

private:
    // TestMashupCanvas is derived from MMashupCanvas
    TestMashupCanvas *canvas;
    // TestMashupCanvasView is derived from MMashupCanvasView
    TestMashupCanvasView *view;
    // MApplication instance required by MWidget.
    MApplication *app;

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    // Executed once before first test case
    void initTestCase();

    // Executed once after last test case
    void cleanupTestCase();

    void testAddition();
    void testRemoval();
    void testCategories();
    void testUniqueCanvasIdentifiers();
    void testEmptyCanvasIdentifier();
    void testServiceAddress();
};
#endif // UT_MMASHUPCANVAS_
