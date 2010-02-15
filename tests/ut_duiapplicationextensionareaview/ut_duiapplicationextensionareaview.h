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

#ifndef UT_DUIAPPLICATIONEXTENSIONAREAVIEW_H
#define UT_DUIAPPLICATIONEXTENSIONAREAVIEW_H

#include <QObject>
#include "duiapplicationextensionareaview.h"

class DuiApplicationWindow;
class DuiApplication;
class DuiApplicationExtensionAreaView;
class DuiApplicationExtensionArea;
class DuiWidget;
class DuiDataStore;

class TestDuiApplicationExtensionAreaView : public DuiApplicationExtensionAreaView
{
    DUI_VIEW(DuiApplicationExtensionAreaModel, DuiApplicationExtensionAreaStyle)

public:
    TestDuiApplicationExtensionAreaView(DuiApplicationExtensionArea *canvas) : DuiApplicationExtensionAreaView(canvas) {}

    DuiApplicationExtensionAreaStyle *modifiableStyle() {
        DuiApplicationExtensionAreaStyleContainer &sc = style();
        const DuiApplicationExtensionAreaStyle *const_s = sc.operator ->();
        DuiApplicationExtensionAreaStyle *s = const_cast<DuiApplicationExtensionAreaStyle *>(const_s);
        return s;
    }

    void applyStyle() {
        DuiApplicationExtensionAreaView::applyStyle();
    }
};

class Ut_DuiApplicationExtensionAreaView : public QObject
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

private:
    bool widgetInLayout(DuiWidget *widget);
    void addWidgetToApplicationExtensionArea(DuiWidget *widget, DuiDataStore *dataStore);
    void removeWidgetFromApplicationExtensionArea(DuiWidget *widget);

    // Widget creation helpers
    QList<DuiWidget *> *createWidgets(int numberOfWidgets, bool containerMode = true);
    // DuiApplication instance required by DuiWidget.
    DuiApplication *app;
    // DuiApplicationWindow instance required to get the scene and scene manager.
    DuiApplicationWindow *appWindow;
    // The object being tested
    TestDuiApplicationExtensionAreaView *m_subject;
    // Controller for the view
    DuiApplicationExtensionArea *extensionArea;
};

#endif
