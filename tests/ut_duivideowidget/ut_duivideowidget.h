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

#ifndef _ut_duivideowidget_
#define _ut_duivideowidget_

#include <QtTest/QtTest>
#include <QObject>

#include <duiapplication.h>
#include <duivideowidget.h>

Q_DECLARE_METATYPE(DuiVideoWidget *);

class ut_duivideowidget: public QObject
{
    Q_OBJECT;

public:
    ut_duivideowidget();
    virtual ~ut_duivideowidget();

private:

    DuiVideoWidget *m_subject;
    DuiApplication *app;


    bool waitVideoReady(int msec=1000);
    bool waitPositionChange(int msec=1000);
    bool waitPositionFreeze(int msec=1000);
    bool ready;
public slots:

    void videoReady();

private slots:

    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testInitialState();
    void testBogusOpen();
    void testOpen();
    void testPlayback();
    void testVolumeControl();
    void testPositionControl();
};
#endif
