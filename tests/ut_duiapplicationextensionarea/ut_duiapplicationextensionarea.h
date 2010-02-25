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

#ifndef UT_DUIAPPLICATIONEXTENSIONAREA_
#define UT_DUIAPPLICATIONEXTENSIONAREA_

#include <QObject>
#include <DuiWidgetView>
#include "duiapplicationextensionareamodel.h"
#include "duiapplicationextensionareastyle.h"

class DuiApplicationExtensionArea;
class DuiApplication;
class DuiApplicationWindow;

class Ut_DuiApplicationExtensionArea : public QObject
{
    Q_OBJECT

private:
    // TestApplicationExtensionArea is derived from DuiApplicationExtensionArea
    DuiApplicationExtensionArea *area;
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
};

#endif // UT_DUIAPPLICATIONEXTENSIONAREA_
