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

#ifndef UT_MAPPLETLOADER_H
#define UT_MAPPLETLOADER_H

#include <mappletinterface.h>
#include <QObject>

class MAppletMetaData;
class MockDataStore;
class MockAppletSettingsInterface;

class TestApplet : public QObject, public MAppletInterface
{
    Q_OBJECT
    Q_INTERFACES(MAppletInterface)

public:
    // methods derived from MAppletInterface
    virtual QGraphicsWidget *constructWidget(const MAppletMetaData &, MDataStore &, MDataAccess &);

    // A flag to tell if we should use the deprecated or the new applet construction interface
    static bool useDeprecatedInterface;
};

class SomeQObject : public QObject
{
    Q_OBJECT

public:
    virtual ~SomeQObject();
};


class Ut_MAppletLoader : public QObject
{
    Q_OBJECT

    MAppletMetaData *metadata;
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
    void testAppletLoadingUsesCorrectLoaderHints();
};

#endif
