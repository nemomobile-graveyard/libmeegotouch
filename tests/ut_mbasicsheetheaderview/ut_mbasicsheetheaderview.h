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

#ifndef UT_STATUSBARVIEW_H
#define UT_STATUSBARVIEW_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class MButton;
class MBasicSheetHeaderView;
class MBasicSheetHeader;

class Ut_MBasicSheetHeaderView : public QObject
{
    Q_OBJECT
public:
    Ut_MBasicSheetHeaderView();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testClickingPositiveButtonTriggersPositiveAction();
    void testClickingNegativeButtonTriggersNegativeAction();

    void testDisablingPositiveActionDisablesPositiveButton();
    void testDisablingNegativeActionDisablesNegativeButton();

    void testPositiveButtonTextUsesActionText();
    void testNegativeButtonTextUsesActionText();

private:
    // helpers
    void testClickingButtonTriggersAction(MButton *button, QAction *action);
    void testDisablingActionDisablesButton(QAction *action, MButton *button);

    MBasicSheetHeaderView *subject;
    MBasicSheetHeader *controller;
    MApplication *app;
};

#endif
