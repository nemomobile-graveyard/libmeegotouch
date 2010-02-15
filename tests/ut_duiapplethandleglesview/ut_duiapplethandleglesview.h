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

#ifndef UT_DUIAPPLETHANDLEGLESVIEW
#define UT_DUIAPPLETHANDLEGLESVIEW

#include <QObject>
#include "duiapplethandleglesview.h"

class DuiTestAppletHandleGLESView : public DuiAppletHandleGLESView
{
    Q_OBJECT

    DUI_VIEW(DuiAppletHandleModel, DuiAppletHandleStyle)
public:
    DuiTestAppletHandleGLESView(DuiAppletHandle *handle);
};


// Test case must inherit QObject
class Ut_DuiAppletHandleGLESView : public QObject
{
    Q_OBJECT

public:

private:
    //! DuiAppletHandleGLESView instance under testing.
    DuiTestAppletHandleGLESView *handleView;
    DuiAppletHandle *handle;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testBrokenAppletDrawing();
    void testPixmapAndTextureCleanup();
    void testUniforms();
};
#endif // UT_DUIAPPLETHANDLEGLESVIEW
