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

#ifndef UT_MAPPLICATIONEXTENSIONAREAVIEW_H
#define UT_MAPPLICATIONEXTENSIONAREAVIEW_H

#include <QObject>
#include "mapplicationextensionareaview.h"

class MApplicationWindow;
class MApplication;
class MApplicationExtensionAreaView;
class MApplicationExtensionArea;
class MWidget;
class MDataStore;

class TestMApplicationExtensionAreaView : public MApplicationExtensionAreaView
{
    M_VIEW(MApplicationExtensionAreaModel, MApplicationExtensionAreaStyle)

public:
    TestMApplicationExtensionAreaView(MApplicationExtensionArea *canvas) : MApplicationExtensionAreaView(canvas) {}

    MApplicationExtensionAreaStyle *modifiableStyle() {
        MApplicationExtensionAreaStyleContainer &sc = style();
        const MApplicationExtensionAreaStyle *const_s = sc.operator ->();
        MApplicationExtensionAreaStyle *s = const_cast<MApplicationExtensionAreaStyle *>(const_s);
        return s;
    }

    void applyStyle() {
        MApplicationExtensionAreaView::applyStyle();
    }
};

class Ut_MApplicationExtensionAreaView : public QObject
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
    // Test widget addition
    void testAddition();
    // Test widget removal
    void testRemoval();
    // Test theme changes
    void testApplyStyle();
    // Test that widgets appear in the order set in the extension area
    void testOrdering();

private:
    bool widgetInLayout(MWidget *widget);
    int widgetPosInLayout(MWidget *widget);
    void addWidgetToApplicationExtensionArea(QGraphicsWidget *widget, MDataStore *dataStore);
    void removeWidgetFromApplicationExtensionArea(QGraphicsWidget *widget);

    // Widget creation helpers
    void createWidgets(int numberOfWidgets, bool containerMode = true);
    // A container that createWidgets method fills
    QList<MWidget *> createdWidgets;
    // MApplication instance required by MWidget.
    MApplication *app;
    // MApplicationWindow instance required to get the scene and scene manager.
    MApplicationWindow *appWindow;
    // The object being tested
    TestMApplicationExtensionAreaView *m_subject;
    // Controller for the view
    MApplicationExtensionArea *extensionArea;
};

#endif
