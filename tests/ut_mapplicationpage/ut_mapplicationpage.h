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
#include <MApplicationPage>

#define MAX_PARAMS 10

class MApplication;
class MApplicationWindow;

Q_DECLARE_METATYPE(MApplicationPage *);
Q_DECLARE_METATYPE(MEscapeButtonPanelModel::EscapeMode);

class Ut_MApplicationPage : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testInitialValues();
    void testProperties();
    void testCentralWidget();
    void testCreateContent();
    void testPageTitleChanged();
    void testRememberPosition();
    void testForgetPosition();
    void testActionUpdated();
    void testDefaultComponentDisplayModes();
    void testSettingComponentsDisplayModes();
    void testSettingMultipleComponentsDisplayModes();
    void testSettingAllComponentsDisplayMode();
    void testUpdatingWindowTitleWithChangingPageTitle();

private:
    MApplicationPage *m_subject;
    MApplicationWindow *appWin;
    MApplication *app;

};

//Q_DECLARE_METATYPE(MApplicationPage*);

#endif
