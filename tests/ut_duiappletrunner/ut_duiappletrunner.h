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

#ifndef _UT_DUIAPPLETRUNNER_
#define _UT_DUIAPPLETRUNNER_

#include <QObject>
#include <QHash>

class DuiAppletRunner;
class DuiAppletMetaData;
class DuiWidget;

// Test case must inherit QObject
class Ut_DuiAppletRunner: public QObject
{
    Q_OBJECT

private:
    /** DuiAppletRunner instance under testing. */
    DuiAppletRunner *m_instance;
    DuiAppletMetaData *meta;
    DuiWidget *widget;

public:
    static bool timerImmediateTimeout;
    static bool quitCalled;
    static QHash<QString, QString> gConfKeyMap;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testInitializationSuccess();
    void testInitializationFailInvalidAppletId();
    void testInitializationFailInvalidInstanceDataLocation();
    void testInitializationFailInvalidMetadata();
    void testInitializationFailConstructWidgetFails();
};

#endif //_UT_DUIAPPLETRUNNER_
