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

#ifndef _UT_MAPPLETBUTTON_
#define _UT_MAPPLETBUTTON_

#include <QtTest/QtTest>
#include <QObject>
#include "mapplication.h"

class MAppletButton;

class Ut_MAppletButton : public QObject
{
    Q_OBJECT

private:
    MApplication *app;

    // MAppletButton instance under testing
    MAppletButton *subject;
    QString path;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testInitialization();
};

#endif //_UT_MAPPLETBUTTON_
