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

#ifndef UT_MMASHUPCANVASVIEW_H
#define UT_MMASHUPCANVASVIEW_H

#include <QObject>
#include "mmashupcanvasview.h"

class MApplicationWindow;
class MApplication;
class MMashupCanvasView;
class MMashupCanvas;
class MWidget;
class MDataStore;

class TestMMashupCanvasView : public MMashupCanvasView
{
    M_VIEW(MMashupCanvasModel, MMashupCanvasStyle)

public:
    TestMMashupCanvasView(MMashupCanvas *canvas) : MMashupCanvasView(canvas) {}

    MMashupCanvasStyle *modifiableStyle() {
        MMashupCanvasStyleContainer &sc = style();
        const MMashupCanvasStyle *const_s = sc.operator ->();
        MMashupCanvasStyle *s = const_cast<MMashupCanvasStyle *>(const_s);
        return s;
    }

    void applyStyle() {
        MMashupCanvasView::applyStyle();
    }
};

class Ut_MMashupCanvasView : public QObject
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
    bool widgetInLayout(MWidget *widget);
    void addWidgetToMashupCanvas(QGraphicsWidget *widget, MDataStore *dataStore);
    void removeWidgetFromMashupCanvas(MWidget *widget);
    MContainer *container(QGraphicsWidget *widget);
    bool isWidgetInContainer(QGraphicsWidget *widget);
    void verifyWidgetContainerVisibility(QList<MWidget *> *widgetList, bool visible);

    // Widget creation helpers
    void createWidgets(int numberOfWidgets, bool containerMode = true);
    // A container that createWidgets() fills
    QList<MWidget*> createdWidgets;
    // MApplication instance required by MWidget.
    MApplication *app;
    // MApplicationWindow instance required to get the scene and scene manager.
    MApplicationWindow *appWindow;
    // The object being tested
    TestMMashupCanvasView *m_subject;
    // Controller for the view
    MMashupCanvas *mashupCanvas;
};

#endif
