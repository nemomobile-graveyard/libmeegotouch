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

#include "ut_mvideowidget.h"
#include "mvideowidget.h"
#include "mvideowidgetview.h"

#include <mapplication.h>

ut_mvideowidget::ut_mvideowidget()
{
}

ut_mvideowidget::~ut_mvideowidget()
{
}

bool ut_mvideowidget::waitVideoReady(int msec)
{
    ready = false;
    QTime t;
    t.start();
    while( !ready && t.elapsed() < msec ) {
        usleep(100000);
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 500);
    }
    return ready;
}

bool ut_mvideowidget::waitPositionChange(int msec)
{
    QTime t;
    t.start();
    quint64 pos = m_subject->position();
    while( m_subject->position() == pos && t.elapsed() < msec ) {
        qDebug() << pos << m_subject->position();   
        usleep(100000);
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 500);
    }
    qDebug() << pos << m_subject->position();   
    return m_subject->position() != pos;
}

bool ut_mvideowidget::waitPositionFreeze(int msec)
{
    QTime t;
    t.start();
    quint64 pos = m_subject->position();
    int count = 0;
    do {
        pos = m_subject->position();
        usleep(100000);
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 500);
        if( m_subject->position() == pos )
            count++;
        else
            count = 0;
        qDebug() << pos << m_subject->position();
    } while( count < 2 && t.elapsed() < msec );
    
    return m_subject->position() == pos;
}

void ut_mvideowidget::videoReady()
{   
    ready = true;
}

void ut_mvideowidget::init()
{
    m_subject = new MVideoWidget();
    m_subject->setView(new MVideoWidgetView(m_subject));
    connect(m_subject, SIGNAL(videoReady()),
            this, SLOT(videoReady()));    
}

void ut_mvideowidget::cleanup()
{
    delete m_subject;
}

void ut_mvideowidget::initTestCase()
{
#define MAX_PARAMS 8
    char *argv[MAX_PARAMS];
    int argc = 2;

    argv[0] = strdup("./ut_mbutton");
    argv[1] = strdup("-software");
    
    app = new MApplication(argc, argv);

    free(argv[0]);
    free(argv[1]);
}

void ut_mvideowidget::cleanupTestCase()
{
    delete app;
}

void ut_mvideowidget::testInitialState()
{
    QCOMPARE(m_subject->state(), MVideo::NotReady);
}

void ut_mvideowidget::testBogusOpen()
{
    QSignalSpy videoReadySpy(m_subject, SIGNAL(videoReady()));
    m_subject->open(QString("bogusdata"));
    waitVideoReady();

    QCOMPARE(videoReadySpy.count(), 0);
    QCOMPARE(m_subject->state(), MVideo::NotReady);
}

void ut_mvideowidget::testOpen()
{
    QSignalSpy videoReadySpy(m_subject, SIGNAL(videoReady()));
    m_subject->open(QString(qApp->applicationDirPath() + "/test.mov"));
    waitVideoReady();

    QCOMPARE(videoReadySpy.count(), 1);
    QCOMPARE(m_subject->state(), MVideo::Stopped);
    QCOMPARE(m_subject->position(), (quint64)0);
    QVERIFY(m_subject->length() > 0);
}

void ut_mvideowidget::testPlayback()
{
    m_subject->open(QString(qApp->applicationDirPath() + "/test.mov"));
    //m_subject->setMuted(true);
    waitVideoReady();

    //test initial -> play
    bool ok;
    m_subject->play();
    ok = waitPositionChange();
    QCOMPARE(m_subject->state(), MVideo::Playing);
    QVERIFY(m_subject->position() > 0);
    QVERIFY(ok);
    
    //test play -> pause
    m_subject->pause();
    ok = waitPositionFreeze();
    QCOMPARE(m_subject->state(), MVideo::Paused);
    QVERIFY(ok);
    
    //test pause -> play
    m_subject->play();
    ok = waitPositionChange();
    QCOMPARE(m_subject->state(), MVideo::Playing);
    QVERIFY(ok);
    
    //test play -> stop
    m_subject->stop();
    ok = waitPositionFreeze();
    QVERIFY(ok);
    QCOMPARE(m_subject->state(), MVideo::Stopped);
    QCOMPARE(m_subject->position(), (quint64)0);

    //test pause -> stop
    m_subject->play();
    ok = waitPositionChange();
    QVERIFY(ok);
    m_subject->pause();
    ok = waitPositionFreeze();    
    QVERIFY(ok);
    m_subject->stop();
    ok = waitPositionFreeze();
    QVERIFY(ok);
    QCOMPARE(m_subject->state(), MVideo::Stopped);
    QCOMPARE(m_subject->position(), (quint64)0);
    
    //stop -> pause
    m_subject->pause();
    ok = waitPositionFreeze();
    QVERIFY(ok);
    QCOMPARE(m_subject->state(), MVideo::Paused);
    QCOMPARE(m_subject->position(), (quint64)0);     
}

void ut_mvideowidget::testVolumeControl()
{
    m_subject->open(QString(qApp->applicationDirPath() + "/test.mov"));
    waitVideoReady();

    //mute
    m_subject->setMuted(true);
    QVERIFY(m_subject->isMuted());
    m_subject->setMuted(false);
    QVERIFY(!m_subject->isMuted());

    //volume
    m_subject->setVolume(-0.1);
    QCOMPARE(m_subject->volume(), 0.0);
    m_subject->setVolume(1000);
    QCOMPARE(m_subject->volume(), 10.0);
}

void ut_mvideowidget::testPositionControl()
{
    m_subject->open(QString(qApp->applicationDirPath() + "/test.mov"));
    waitVideoReady();

    //seeking when playing
    m_subject->play();
    m_subject->seek(500);
    waitPositionChange();
    QVERIFY(m_subject->position() >= 500);

    //seeking when paused
    m_subject->pause();
    waitPositionFreeze();
    m_subject->seek(0);
    m_subject->play();
    waitPositionChange();
    QVERIFY(m_subject->position() < 500);
    
    //seeking when stopped
    m_subject->stop();
    waitPositionFreeze();
    QVERIFY(m_subject->position() == 0);
    m_subject->seek(500);
    m_subject->play();
    waitPositionChange();
    QVERIFY(m_subject->position() >= 500);
        
    //seek past EOS
    m_subject->seek(m_subject->length() + 1000);
    waitPositionChange();
    QVERIFY(m_subject->position() < m_subject->length());
    
    //non-looping EOS
    m_subject->setLooping(false);
    m_subject->seek(m_subject->length() - 500);
    bool ok = waitPositionFreeze();
    QVERIFY(ok);
    QCOMPARE(m_subject->state(), MVideo::Stopped);
    QCOMPARE(m_subject->position(), (quint64)0);
    
    //looping EOS
    m_subject->setLooping(true);
    m_subject->seek(m_subject->length() - 500);
    m_subject->play();
    waitPositionChange();
    waitPositionFreeze();
    //QVERIFY(!ok);
    QCOMPARE(m_subject->state(), MVideo::Playing);
    QVERIFY(m_subject->position() > 0);
}

QTEST_APPLESS_MAIN(ut_mvideowidget)
