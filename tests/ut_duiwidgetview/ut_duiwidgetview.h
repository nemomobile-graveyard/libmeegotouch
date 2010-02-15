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

#ifndef UT_DUIWIDGETVIEW_H
#define UT_DUIWIDGETVIEW_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/DuiWidgetView class declaration
#include <duiwidgetview.h>
#include <DuiWidgetController>
#include <DuiApplication>

Q_DECLARE_METATYPE(DuiWidgetView *);

class Ut_DuiWidgetView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void setObjectName();
    void shape();
    void resizeEvent();
    void mousePressEvent();
    void mouseReleaseEvent();
    void mouseMoveEvent();
    void testThatActiveStyleIsUsedInActiveState();

private:
    void setBottomMarginParameter(DuiWidgetStyleContainer &container, int param);

    DuiWidgetView *m_subject;
    DuiWidgetController *m_controller;
    DuiApplication *app;
};

#endif
