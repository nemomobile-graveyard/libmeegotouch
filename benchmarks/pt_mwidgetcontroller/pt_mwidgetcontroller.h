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

#ifndef PT_MWIDGETCONTROLLER_H
#define PT_MWIDGETCONTROLLER_H

#include <QtTest/QtTest>
#include <QObject>

#include <MApplicationPage>

class MWidgetController;

class Pt_MWidgetControllerPage : public MApplicationPage {

    Q_OBJECT
public:
    Pt_MWidgetControllerPage();
    virtual ~Pt_MWidgetControllerPage();

public Q_SLOTS:
    void pageAppeared();
};

class Pt_MWidgetController : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void test_qGraphicsWidgets();

    void test_mWidgetController();
    void test_mLabel();
    void test_mButton();

private:  
};

#endif
