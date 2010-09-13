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

#include <QObject>
#include <QDBusAbstractInterface>
#include <QDBusConnection>
#include "ut_mservicefwbaseif.h"

QString Ut_MServiceFwBaseIf::serviceFwService;

class EmailServiceIfProxy : public QDBusAbstractInterface
{
public:
    static inline const char *staticInterfaceName() {
        return "com.nokia.EmailServiceIf";
    }

public:
    EmailServiceIfProxy() : QDBusAbstractInterface(QString(), QString(), EmailServiceIfProxy::staticInterfaceName(), QDBusConnection::sessionBus(), 0) {
    };

    virtual ~EmailServiceIfProxy() {};

Q_SIGNALS: // SIGNALS
    void messageSent(const QString &message);
};

class MyServiceFwIf : public MServiceFwBaseIf
{
public:
    MyServiceFwIf(QDBusAbstractInterface *ifProxy) :
        MServiceFwBaseIf("com.nokia.TextProcessorIf", 0) {
	setInterfaceProxy(ifProxy);
    };

    virtual ~MyServiceFwIf() {
    };

    virtual void setService(const QString &service) {
        setServiceName(service);
    };
};

void Ut_MServiceFwBaseIf::init()
{
    Ut_MServiceFwBaseIf::serviceFwService = "";

    m_subject = new MyServiceFwIf(new EmailServiceIfProxy());

    // no point in testing these
    if ( !  m_subject->isValid() ) {
        qWarning() << "m_subject is not valid";
    }
    m_subject->serviceNames("com.nokia.TextProcessorIf");
    m_subject->serviceName();
    m_subject->serviceFwProxy();
    // purely for coverage stats - useless otherwise
    m_subject->serviceFwProxy()->servicePath("com.nokia.TextProcessorIf");
}

void Ut_MServiceFwBaseIf::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MServiceFwBaseIf::initTestCase()
{
}

void Ut_MServiceFwBaseIf::cleanupTestCase()
{
}

// no current service; new service for other if
// should not get signal:serviceAvailable
// should not get signal:serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceAvailable0()
{
    QSignalSpy serviceAvailableSpy(m_subject,
                                   SIGNAL(serviceAvailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("");

    m_subject->handleServiceAvailable("com.google.TextProcessor", "com.google.TexrProcessorIf");
    QCOMPARE(serviceAvailableSpy.count(), 0);
    QCOMPARE(serviceChangedSpy.count(), 0);
}

// no current service; new service for this if
// should get signal:serviceAvailable
// should get signal:serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceAvailable1()
{
    QSignalSpy serviceAvailableSpy(m_subject,
                                   SIGNAL(serviceAvailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("");

    QString thisService = "org.maemo.TextProcessor";
    serviceFwService = thisService;

    m_subject->handleServiceAvailable(thisService, m_subject->interfaceName());
    QCOMPARE(serviceAvailableSpy.count(), 1);
    QCOMPARE(serviceChangedSpy.count(), 1);
}

// current service; new preferred service for other if
// should not get signal:serviceAvilable
// should not get signal:serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceAvailable2()
{
    QSignalSpy serviceAvailableSpy(m_subject,
                                   SIGNAL(serviceAvailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("org.maemo.TextProcessor");

    QString thisService = "com.nokia.TextProcessor";
    serviceFwService = thisService;

    m_subject->handleServiceAvailable(thisService, "org.maemo.MailService");
    QCOMPARE(serviceAvailableSpy.count(), 0);
    QCOMPARE(serviceChangedSpy.count(), 0);
}

// current service; new preferred service for this if
// should not get signal:serviceAvilable
// should get signal:serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceAvailable3()
{
    QSignalSpy serviceAvailableSpy(m_subject,
                                   SIGNAL(serviceAvailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("org.maemo.TextProcessor");

    QString thisService = "com.nokia.TextProcessor";
    serviceFwService = thisService;

    m_subject->handleServiceAvailable(thisService, m_subject->interfaceName());
    QCOMPARE(serviceAvailableSpy.count(), 0);
    QCOMPARE(serviceChangedSpy.count(), 1);
}

// no current service; dead service is for other if
// should not get serviceUnavailable
// should not get serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceUnavailable0()
{
    QSignalSpy serviceUnavailableSpy(m_subject,
                                     SIGNAL(serviceUnavailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("");

    QString thisService = "com.nokia.TextProcessor";
    serviceFwService = "";

    m_subject->handleServiceUnavailable(thisService);
    QCOMPARE(serviceUnavailableSpy.count(), 0);
    QCOMPARE(serviceChangedSpy.count(), 0);
}

// current service; dead service is for other if
// should not get serviceUnavailable
// should not get serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceUnavailable1()
{
    QSignalSpy serviceUnavailableSpy(m_subject,
                                     SIGNAL(serviceUnavailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("com.nokia.TextProcessor");

    serviceFwService = m_subject->serviceName();

    m_subject->handleServiceUnavailable("com.nokia.EmailService");
    QCOMPARE(serviceUnavailableSpy.count(), 0);
    QCOMPARE(serviceChangedSpy.count(), 0);
}

// current service; dead service is not last for this if
// should not get serviceUnavailable
// should get serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceUnavailable2()
{
    QSignalSpy serviceUnavailableSpy(m_subject,
                                     SIGNAL(serviceUnavailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("com.nokia.TextProcessor");

    serviceFwService = "org.maemo.TextProcessor";

    m_subject->handleServiceUnavailable(m_subject->serviceName());
    QCOMPARE(serviceUnavailableSpy.count(), 0);
    QCOMPARE(serviceChangedSpy.count(), 1);
}

// current service; dead service is last for this if
// should get serviceUnavailable
// should not get serviceChanged
void Ut_MServiceFwBaseIf::testHandleServiceUnavailable3()
{
    QSignalSpy serviceUnavailableSpy(m_subject,
                                     SIGNAL(serviceUnavailable(QString)));
    QSignalSpy serviceChangedSpy(m_subject,
                                 SIGNAL(serviceChanged(QString)));

    m_subject->setService("com.nokia.TextProcessor");

    serviceFwService = "";

    m_subject->handleServiceUnavailable(m_subject->serviceName());
    QCOMPARE(serviceUnavailableSpy.count(), 1);
    QCOMPARE(serviceChangedSpy.count(), 0);
}

void Ut_MServiceFwBaseIf::testResolveServiceName()
{
    serviceFwService = "com.nokia.TextProcessor";

    QString thisService = m_subject->resolveServiceName("com.nokia.TextProcessorIf", "");
    QCOMPARE(thisService, serviceFwService);

    thisService = m_subject->resolveServiceName("com.nokia.TextProcessorIf", "org.maemo.TextProcessor");
    QCOMPARE(thisService, QString("org.maemo.TextProcessor"));
}

QTEST_APPLESS_MAIN(Ut_MServiceFwBaseIf)
