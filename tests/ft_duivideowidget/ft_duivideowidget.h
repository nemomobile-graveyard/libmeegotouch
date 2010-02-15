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

#ifndef _FT_DUIVIDEOWIDGET_
#define _FT_DUIVIDEOWIDGET_

#include <QtTest/QtTest>
#include <QObject>

#include <duiapplication.h>
#include <duivideowidget.h>

Q_DECLARE_METATYPE(DuiVideoWidget *);

class ft_duivideowidget: public QObject
{
    Q_OBJECT;

public:
    ft_duivideowidget();
    virtual ~ft_duivideowidget();

private:

    DuiVideoWidget *m_subject;
    DuiApplication *app;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testInitialState();
    void testBogusOpen();
    void testOpen();
    void testPause();
    void testVolume();
    void testSeek();
    void testCurrentPos();
    void testMute();
    void testCurrentFrame();
    void testScaling();
    void testKeepAspectRatio();
    void testBackgroundFill();
    void testBackgroundColor();
    void testRepeat();
    void testVideoFilename();

};
#endif
