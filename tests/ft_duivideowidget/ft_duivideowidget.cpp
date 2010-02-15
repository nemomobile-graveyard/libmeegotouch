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

#include "ft_duivideowidget.h"
#include "duivideowidget.h"

#include <duiapplication.h>

ft_duivideowidget::ft_duivideowidget()
{
    gst_init(NULL, NULL);
}

ft_duivideowidget::~ft_duivideowidget()
{
    gst_deinit();
}

void ft_duivideowidget::init()
{
    m_subject = new DuiVideoWidget();
}

void ft_duivideowidget::cleanup()
{
    delete m_subject;
}

void ft_duivideowidget::initTestCase()
{
#define MAX_PARAMS 8
    char *argv[MAX_PARAMS];
    int argc = 2;

    argv[0] = strdup("./ut_duibutton");
    argv[1] = strdup("-software");

    app = new DuiApplication(argc, argv);

    free(argv[0]);
    free(argv[1]);
}

void ft_duivideowidget::cleanupTestCase()
{
    delete app;
}

void ft_duivideowidget::testInitialState()
{
    QCOMPARE(m_subject->isPlaying(), false);
}

void ft_duivideowidget::testBogusOpen()
{
    QCOMPARE(m_subject->open(QString("bogusdata")), false);
}

void ft_duivideowidget::testOpen()
{
    QCOMPARE(m_subject->open(QString("test.avi")), true);
}

void ft_duivideowidget::testPause()
{
    // disable sounds, we don't want to hear those during the test
    m_subject->mute(true);

    QCOMPARE(m_subject->open(QString("test.avi"), true), true);

QTest: qSleep(1000);
    QCOMPARE(m_subject->isPlaying(), true);

    m_subject->pause();
QTest: qSleep(1000);
    QCOMPARE(m_subject->isPlaying(), false);

    m_subject->play();
QTest: qSleep(1000);
    QCOMPARE(m_subject->isPlaying(), true);
}

void ft_duivideowidget::testVolume()
{
    m_subject->open(QString("test.avi"));

    // <min
    m_subject->setVolume(-0.1);
    QCOMPARE(m_subject->volume(), 0.0);

    // >max
    m_subject->setVolume(1000);
    QCOMPARE(m_subject->volume(), 1.0);
}

void ft_duivideowidget::testSeek()
{
    m_subject->open(QString("test.avi"));

    // wait for gstreamer machinery
QTest: qSleep(1000);

    // working seek
    QCOMPARE(m_subject->seek(10), true);

    // not working seek
    QCOMPARE(m_subject->seek(-1), false);
}

void ft_duivideowidget::testCurrentPos()
{
}

void ft_duivideowidget::testMute()
{
    m_subject->open(QString("test.avi"));

    QVERIFY(m_subject->muted() == false);

    // wait for gstreamer machinery
QTest: qSleep(1000);

    m_subject->mute(true);
    QVERIFY(m_subject->muted() == true);
}

void ft_duivideowidget::testCurrentFrame()
{
    m_subject->open(QString("test.avi"));

    // wait for gstreamer machinery
QTest: qSleep(1000);

    QImage test = m_subject->currentFrame();
    QVERIFY(test.isNull() != true);
}

void ft_duivideowidget::testScaling()
{
    QVERIFY(m_subject->scaling() == true);
    m_subject->setScaling(false);
    QVERIFY(m_subject->scaling() == false);
}

void ft_duivideowidget::testKeepAspectRatio()
{
    QVERIFY(m_subject->keepAspectRatio() == true);
    m_subject->setKeepAspectRatio(false);
    QVERIFY(m_subject->keepAspectRatio() == false);
}

void ft_duivideowidget::testBackgroundFill()
{
    QVERIFY(m_subject->backgroundFill() == false);
    m_subject->setBackgroundFill(true);
    QVERIFY(m_subject->backgroundFill() == true);
}

void ft_duivideowidget::testBackgroundColor()
{
    QColor red("#ff0000");
    QVERIFY(m_subject->backgroundColor() == QColor(Qt::black));
    m_subject->setBackgroundColor(red);
    QVERIFY(m_subject->backgroundColor() == red);
}

void ft_duivideowidget::testRepeat()
{
    QVERIFY(m_subject->repeat() == false);
    m_subject->setRepeat(true);
    QVERIFY(m_subject->repeat() == true);
}

void ft_duivideowidget::testVideoFilename()
{
    QFileInfo info("test.avi");
    m_subject->open(QString("test.avi"));
    QCOMPARE(m_subject->videoFilename(), info.absoluteFilePath());
}


QTEST_APPLESS_MAIN(ft_duivideowidget)
