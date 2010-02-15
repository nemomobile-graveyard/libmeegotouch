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

#ifndef UT_DUIMASHUPCANVAS_
#define UT_DUIMASHUPCANVAS_

#include <QObject>
#include <DuiWidgetView>
#include "duimashupcanvasmodel.h"
#include "duimashupcanvasstyle.h"

class TestMashupCanvas;
class DuiApplication;
class DuiApplicationWindow;


class TestMashupCanvasView : public DuiWidgetView
{
    DUI_VIEW(DuiMashupCanvasModel, DuiMashupCanvasStyle)

public:
    TestMashupCanvasView(DuiWidgetController *canvas) : DuiWidgetView(canvas) {}

};

class Ut_DuiMashupCanvas : public QObject
{
    Q_OBJECT

private:
    // TestMashupCanvas is derived from DuiMashupCanvas
    TestMashupCanvas *canvas;
    // TestMashupCanvasView is derived from DuiMashupCanvasView
    TestMashupCanvasView *view;
    // DuiApplication instance required by DuiWidget.
    DuiApplication *app;

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
#endif // UT_DUIMASHUPCANVAS_
