/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_MEDITORTOOLBAR_H
#define UT_MEDITORTOOLBAR_H

#include <QObject>
#include <QtTest/QtTest>
#include <QList>

class MApplication;
class MApplicationWindow;
class MButton;
class MEditorToolbar;
class MSceneWindow;

class Ut_MEditorToolbar : public QObject
{
    Q_OBJECT

public:
    enum Operation {
        Appear,
        Disappear,
        AutoHideEnable,
        AutoHideDisable
    };

    typedef QList<Operation> Operations;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testAppear();
    void testItemVisibility();
    void testPositionMapping_data();
    void testPositionMapping();
    void testForbiddenRegion_data();
    void testForbiddenRegion();
    void testVerticalPositioning();
    void testActionTextChange();
    void testAutoHide_data();
    void testAutoHide();
    void testAnimationDuration_data();
    void testAnimationDuration();
    void testZOrder();

private:
    QList<MButton *> subjectButtons() const;

private:
    MApplication *m_app;
    MApplicationWindow *m_appWindow;
    MEditorToolbar *subject;
    MSceneWindow *fakeParent;
};

#endif

