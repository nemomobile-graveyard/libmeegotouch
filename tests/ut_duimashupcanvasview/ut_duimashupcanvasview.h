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

#ifndef UT_DUIMASHUPCANVASVIEW_H
#define UT_DUIMASHUPCANVASVIEW_H

#include <QObject>
#include "duimashupcanvasview.h"

class DuiApplicationWindow;
class DuiApplication;
class DuiMashupCanvasView;
class DuiMashupCanvas;
class DuiWidget;
class DuiDataStore;

class TestDuiMashupCanvasView : public DuiMashupCanvasView
{
    DUI_VIEW(DuiMashupCanvasModel, DuiMashupCanvasStyle)

public:
    TestDuiMashupCanvasView(DuiMashupCanvas *canvas) : DuiMashupCanvasView(canvas) {}

    DuiMashupCanvasStyle *modifiableStyle() {
        DuiMashupCanvasStyleContainer &sc = style();
        const DuiMashupCanvasStyle *const_s = sc.operator ->();
        DuiMashupCanvasStyle *s = const_cast<DuiMashupCanvasStyle *>(const_s);
        return s;
    }

    void applyStyle() {
        DuiMashupCanvasView::applyStyle();
    }
};

class Ut_DuiMashupCanvasView : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test that the layout is set up as expected
    void testLayoutPolicy();
    // Test widget addition with flow layout policy
    void testAdditionWithFlowLayoutPolicy();
    // Test widget removal with flow layout policy
    void testRemovalWithFlowLayoutPolicy();
    // Test container mode
    void testSettingContainerModeOff();
    void testSettingContainerModeOn();
    void testSettingContainerModeOnWhenContainerModeIsOn();
    void testSettingContainerModeOffWhenContainerModeIsOff();
    void testSettingContainerModeOffWidgetsHaveCorrectLayoutOrder();

private:
    bool widgetInLayout(DuiWidget *widget);
    void addWidgetToMashupCanvas(DuiWidget *widget, DuiDataStore *dataStore);
    void removeWidgetFromMashupCanvas(DuiWidget *widget);
    DuiContainer *container(QGraphicsWidget *widget);
    bool isWidgetInContainer(QGraphicsWidget *widget);
    void verifyWidgetContainerVisibility(QList<DuiWidget *> *widgetList, bool visible);

    // Widget creation helpers
    QList<DuiWidget *> *createWidgets(int numberOfWidgets, bool containerMode = true);
    // DuiApplication instance required by DuiWidget.
    DuiApplication *app;
    // DuiApplicationWindow instance required to get the scene and scene manager.
    DuiApplicationWindow *appWindow;
    // The object being tested
    TestDuiMashupCanvasView *m_subject;
    // Controller for the view
    DuiMashupCanvas *mashupCanvas;
};

#endif
