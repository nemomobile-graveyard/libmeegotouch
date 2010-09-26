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

#ifndef UT_MEXTENSIONAREAVIEW_H
#define UT_MEXTENSIONAREAVIEW_H

#include <QObject>
#include "mextensionareaview.h"
#include "mextensionareaview_p.h"

class MApplicationWindow;
class MApplication;

class TestMExtensionAreaViewPrivate : public MExtensionAreaViewPrivate
{
public:
    TestMExtensionAreaViewPrivate();
    ~TestMExtensionAreaViewPrivate();
};

class TestMExtensionAreaView : public MExtensionAreaView
{
    M_VIEW(MExtensionAreaModel, MExtensionAreaStyle)

public:
    TestMExtensionAreaView(MExtensionAreaViewPrivate* priv, MExtensionArea *canvas) : MExtensionAreaView(priv, canvas) {}

    MExtensionAreaStyle *modifiableStyle() {
        MExtensionAreaStyleContainer &sc = style();
        const MExtensionAreaStyle *const_s = sc.operator ->();
        MExtensionAreaStyle *s = const_cast<MExtensionAreaStyle *>(const_s);
        return s;
    }

    void applyStyle() {
        MExtensionAreaView::applyStyle();
    }
};

class Ut_MExtensionAreaView : public QObject
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

    void testDestruction();
private:
    MApplication *app;
    // MApplicationWindow instance required to get the scene and scene manager.
    MApplicationWindow *appWindow;
    // The object being tested
    TestMExtensionAreaView *m_subject;
    TestMExtensionAreaViewPrivate *m_subject_private;
    // Controller for the view
    MExtensionArea *extensionArea;
};

#endif
