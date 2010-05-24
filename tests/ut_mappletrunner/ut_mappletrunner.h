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

#ifndef _UT_MAPPLETRUNNER_
#define _UT_MAPPLETRUNNER_

#include <QObject>
#include <QHash>

class MAppletRunner;
class MAppletMetaData;
class QGraphicsWidget;

// Test case must inherit QObject
class Ut_MAppletRunner: public QObject
{
    Q_OBJECT

private:
    /** MAppletRunner instance under testing. */
    MAppletRunner *m_instance;
    MAppletMetaData *meta;
    QGraphicsWidget *widget;

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

#endif //_UT_MAPPLETRUNNER_
