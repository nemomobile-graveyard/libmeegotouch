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

#ifndef UT_DUIAPPLETLOADER_H
#define UT_DUIAPPLETLOADER_H

#include <duiappletinterface.h>
#include <QObject>

class DuiAppletMetaData;
class MockDataStore;
class MockAppletSettingsInterface;

class TestApplet : public QObject, public DuiAppletInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiAppletInterface)

public:
    // methods derived from DuiAppletInterface
    virtual DuiWidget *constructWidget(const DuiAppletMetaData &, DuiDataStore &, DuiDataAccess &);

    // A flag to tell if we should use the deprecated or the new applet construction interface
    static bool useDeprecatedInterface;
};

class SomeQObject : public QObject
{
    Q_OBJECT

public:
    virtual ~SomeQObject();
};


class Ut_DuiAppletLoader : public QObject
{
    Q_OBJECT

    DuiAppletMetaData *metadata;
    MockDataStore *dataStore;
    MockAppletSettingsInterface *appletSettingsInterface;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testAppletLoading();
    void testAppletLoadingFailNullAppletObject();
    void testAppletLoadingFailWrongTypeAppletObject();
};

#endif
