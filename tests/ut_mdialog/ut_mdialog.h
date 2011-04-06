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

#ifndef UT_MDIALOG_H
#define UT_MDIALOG_H

#include <QtTest/QtTest>
#include <MDialog>
#include <QObject>

class MApplication;

class Ut_MDialog: public QObject
{
    Q_OBJECT

private:
    MDialog *dialog;
    MApplication *app;

private slots:

    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void constructorWithParameters();
    void testStandardButtonsInConstructor();

    void initVals();
    void settersAndGetters();
    void checkStandardButtons();
    void addStandardButtons();
    void addNonStandardButtonModel();
    void addNonStandardButtonsWithRole();
    void addInvalidStandardButton();
    void addExistingStandardButton();
    void checkLayouts();
    void acceptDialog();
    void acceptDialogDirectly();
    void removeButton();
    void rejectDialog();
    void rejectDialogDirectly();
    void dismissDialog();
    void testRotation();
    void testSystemDialog();
    void testSystemModalDialog();
    void testCentralWidgetSurviveDoubleDelete();
};

#endif
