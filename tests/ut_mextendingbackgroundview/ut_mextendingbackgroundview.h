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

#ifndef UT_MEXTENDINGBACKGROUNDVIEW_H
#define UT_MEXTENDINGBACKGROUNDVIEW_H

#include <QtTest/QtTest>
#include <QObject>

#include <mextendingbackgroundview.h>
#include <mextendingbackgroundstyle.h>

class MApplication;
class MApplicationWindow;
class MWidgetController;
class MExtendingBackgroundView;

class TestExtendingBackgroundView : public MExtendingBackgroundView
{
    M_VIEW(MWidgetModel, MExtendingBackgroundStyle)

public:
    TestExtendingBackgroundView(MWidgetController *controller);
    QRectF boundingRect() const;
    QRectF geometry() const;
    MExtendingBackgroundStyle *modifiableStyle();
};

class Ut_MExtendingBackgroundView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testBoundingRect();

private:
    MApplication *app;
    MApplicationWindow *appWin;
    MWidgetController *controller;
    TestExtendingBackgroundView *view;
};

#endif
