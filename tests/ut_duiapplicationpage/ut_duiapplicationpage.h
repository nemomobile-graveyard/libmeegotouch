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

#ifndef UT_DUISCENEWINDOW_H
#define UT_DUISCENEWINDOW_H

#include <QtTest/QtTest>
#include <QObject>
#include <DuiApplicationPage>

#define MAX_PARAMS 10

class DuiApplication;
class DuiApplicationWindow;

Q_DECLARE_METATYPE(DuiApplicationPage *);
Q_DECLARE_METATYPE(DuiEscapeButtonPanelModel::EscapeMode);

class Ut_DuiApplicationPage : public QObject
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

private:
    DuiApplicationPage *m_subject;
    DuiApplicationWindow *appWin;
    DuiApplication *app;

};

//Q_DECLARE_METATYPE(DuiApplicationPage*);

#endif
