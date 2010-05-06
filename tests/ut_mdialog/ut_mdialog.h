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

#define MAX_PARAMS 10
class Ut_MDialog: public QObject
{
    Q_OBJECT

private:
    MDialog *dialog;
    MApplication *app;
    MApplication *buildApp(int count, const QString &params);

private slots:

    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void constructorWithParameters();

    void initVals();
    void settersAndGetters();
    void addStandardButtons();
    void addInvalidStandardButton();
    void addExistingStandardButton();
    void acceptDialog();
    void removeButton();
    void rejectDialog();
    void dismissDialog();
    void testRotation();
};

#endif
