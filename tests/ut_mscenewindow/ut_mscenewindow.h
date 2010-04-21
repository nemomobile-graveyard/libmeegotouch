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

#ifndef UT_MSCENEWINDOW_H
#define UT_MSCENEWINDOW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MSceneWindow>

class MWindow;

class Ut_MSceneWindow : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testAccessors();

    void testAppear();
    void testAppearWithoutSceneManager();

    void testAppearedDisappearedSignals();

    void testDismiss();

    void testDestroyWhenDoneCallingDisappear();
    void testDestroyWhenDoneCallingDismiss();
    void testDestroyWhenDismissedCallingDisappear();
    void testDestroyWhenDismissedCallingDismiss();
    void testDismissedStateReset();

private:
    void processPendingEvents();

    MSceneWindow *m_subject;
    MWindow *window;
};

Q_DECLARE_METATYPE(MSceneWindow::WindowType);
Q_DECLARE_METATYPE(MSceneWindow::DeletionPolicy);

#endif
