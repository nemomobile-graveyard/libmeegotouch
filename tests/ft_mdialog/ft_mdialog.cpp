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

#include "ft_mdialog.h"
#include "mdialog.h"
#include <QtTest>
#include <QSignalSpy>
#include <MApplication>
#include <MApplicationWindow>

MApplication *app;
MApplicationWindow *appWindow;

void Ft_MDialog::init()
{
    m_subject = new MDialog();
}

void Ft_MDialog::cleanup()
{
    delete m_subject;
}

void Ft_MDialog::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ft_mdialog" };
    app = new MApplication(argc, argv);
    appWindow = new MApplicationWindow;
}

void Ft_MDialog::cleanupTestCase()
{
    delete appWindow;
    // Disabled for now since there is bug in qemu and/or QFileSystemWatcher code ..
    // delete app;
}

void Ft_MDialog::test_signals()
{
    QSignalSpy spy_accept(m_subject, SIGNAL(accepted()));
    QSignalSpy spy_reject(m_subject, SIGNAL(rejected()));
    QSignalSpy spy_finish(m_subject, SIGNAL(finished(int)));

    // test accept signal, use spy to check that it was signalled
    m_subject->accept();
    QCOMPARE(spy_accept.count(), 1);

    // test reject signal, use spy to check that it was signalled
    m_subject->reject();
    QCOMPARE(spy_reject.count(), 1);

    // test done signal with accepted param, use spy to check that accept was signalled
    m_subject->done(MDialog::Accepted);
    QCOMPARE(spy_accept.count(), 2);

    // test done signal with rejected param, use spy to check that reject was signalled
    m_subject->done(MDialog::Rejected);
    QCOMPARE(spy_reject.count(), 2);

    // we should get finish signal in all of the cases above
    QCOMPARE(spy_finish.count(), 4);
}

void Ft_MDialog::test_eventloop()
{
    //QVERIFY(false); //This test fails, and hangs - John Tapsell
    // It was hanging for me as well when I had my scratchbox env
    // pointing its DBUS_SESSION_BUS_ADDRESS to my Ubuntu host D-Bus.
    // When I started using the D-Bus from scratchbox itself the hanging issues
    // disappeared.
    // $ meego-sb-session start
    // $ source /tmp/session_bus_address.user
    // - Daniel d'Andrada

    // use timer to cancel the event loop when we get back into the message pump
    // another way to do this would involve creating another thread or something similar...
    QTimer::singleShot(0, this, SLOT(timer()));

    // launch the dialog, it will start event loop and block here
    int result = m_subject->exec();

    // The exec will return standard button id; if the dialog was dismissed
    // with either accept() or reject() (or the equivalent done() calls),
    // a DialogCode result is returned instead.
    QCOMPARE(result, 99);

    // result() will return the result set in timer() method.
    QCOMPARE(99, m_subject->result());
}

void Ft_MDialog::test_other()
{
    m_subject->setResult(99);
    QCOMPARE(99, m_subject->result());
    m_subject->setResult(99);
    QCOMPARE(99, m_subject->result());
}

void Ft_MDialog::timer()
{
    // timer signalled timeout, cancel eventloop here with some result value
    m_subject->done(99);
}

void Ft_MDialog::testDismissDialogInEventLoop()
{
    //QVERIFY(false);  //This test hangs - John Tapsell
    // It was hanging for me as well when I had my scratchbox env
    // pointing its DBUS_SESSION_BUS_ADDRESS to my Ubuntu host D-Bus.
    // When I started using the D-Bus from scratchbox itself the hanging issues
    // disappeared.
    // $ meego-sb-session start
    // $ source /tmp/session_bus_address.user
    // - Daniel d'Andrada

    QSignalSpy spyAccepted(m_subject, SIGNAL(accepted()));
    QSignalSpy spyRejected(m_subject, SIGNAL(rejected()));
    QSignalSpy spyFinished(m_subject, SIGNAL(finished(int)));

    // ABI FREEZE: Release this:
    //QTimer::singleShot(500, m_subject, SLOT(dismiss()));
    QTimer::singleShot(500, m_subject, SLOT(close()));

    // bail out after 5 seconds if the dismiss() is not successful
    QTimer::singleShot(5000, QCoreApplication::instance(), SLOT(quit()));

    m_subject->exec(appWindow);

    QCOMPARE(spyAccepted.count(), 0);
    QCOMPARE(spyRejected.count(), 1);
    QCOMPARE(spyFinished.count(), 1);
}

QTEST_APPLESS_MAIN(Ft_MDialog)
