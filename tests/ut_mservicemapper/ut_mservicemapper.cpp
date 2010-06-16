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
#include <QtDebug>
#include <QString>
#include "ut_mservicemapper.h"

#include "mservicemapper_p.h"

QTextStream Ut_MServiceMapper::m_globalDummy;
int Ut_MServiceMapper::m_noFiles;
int Ut_MServiceMapper::m_lineNo;
int Ut_MServiceMapper::m_fileNo;
QString Ut_MServiceMapper::m_lines[4][2];
QString Ut_MServiceMapper::m_services[3];
QString Ut_MServiceMapper::m_interfaces[3];
QStringList Ut_MServiceMapper::m_currentServiceFileList;

class MyMServiceMapperPrivate: public MServiceMapperPrivate
{
public:
    MyMServiceMapperPrivate( const QString &serviceFileDir );
    ~MyMServiceMapperPrivate();

    QStringList fillServiceFileList() const;
    bool fileExistsAndReadable( const QString &filename ) const;
    QIODevice *accessFile( const QString &fileName ) const;
};

MyMServiceMapperPrivate::MyMServiceMapperPrivate( const QString &serviceFileDir ) :
    MServiceMapperPrivate( serviceFileDir )
{
}

QStringList MyMServiceMapperPrivate::fillServiceFileList() const
{
    QStringList retVal( Ut_MServiceMapper::m_currentServiceFileList );

    return retVal;
}

bool MyMServiceMapperPrivate::fileExistsAndReadable(const QString &fileName) const
{
    Q_UNUSED(fileName);
    bool retVal;

    QString baseName = QFileInfo(fileName).baseName();
    int thisFileNo = baseName.toInt();
    retVal = thisFileNo < Ut_MServiceMapper::m_noFiles;

    return retVal;
}

QIODevice *MyMServiceMapperPrivate::accessFile(const QString &fileName) const
{
    Q_UNUSED(fileName);
    QBuffer *thisFile = new QBuffer();
    thisFile->open(QIODevice::ReadWrite);

    int lastFileNo = Ut_MServiceMapper::m_noFiles - 1;

    QString baseName = QFileInfo(fileName).baseName();
    int thisFileNo = baseName.toInt();

    bool thisFileShouldExist = thisFileNo <= lastFileNo;
    if (thisFileShouldExist) {
        for (int thisLineNo = 0; thisLineNo < 2; ++thisLineNo) {
            QTextStream output(thisFile);
            output << Ut_MServiceMapper::m_lines[ thisFileNo ][ thisLineNo ] << endl;
        }
    }
    thisFile->seek(0);

    return thisFile;
}

void Ut_MServiceMapper::init()
{
    // always start with two files
    fillServiceFileList( initFiles );

    // reset to beginning of first file for each test
    // difficult to keep track otherwise
    m_lineNo = 0;
    m_fileNo = 0;

    QString serviceFileDir;

    m_subject = new MServiceMapper( new MyMServiceMapperPrivate( serviceFileDir ) );
}

void Ut_MServiceMapper::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MServiceMapper::initTestCase()
{
    m_services[0] = "com.nokia.EmailService";
    m_interfaces[0] = "com.nokia.EmailServiceIf";
    m_services[1] = "org.maemo.EmailService";
    m_interfaces[1] = "org.maemo.EmailServiceIf";
    m_services[2] = "com.google.EmailService";
    m_interfaces[2] = "com.google.EmailServiceIf";

    m_lines[0][0] = "Interface=" + m_interfaces[0];
    m_lines[0][1] = "Name=" + m_services[0];
    m_lines[1][0] = "Interface=" + m_interfaces[1];
    m_lines[1][1] = "Name=" + m_services[1];
    m_lines[2][0] = "Interface=" + m_interfaces[2];
    m_lines[2][1] = "Name=" + m_services[2];
    m_lines[3][0] = "Interface=not provided";
    m_lines[3][1] = "Name=not provided";
}

void Ut_MServiceMapper::cleanupTestCase()
{
    m_currentServiceFileList.clear();
}

void Ut_MServiceMapper::checkServiceNamesNoIf()
{
    // check without interface - should list all
    QStringList myServices = m_subject->serviceNames();

    QCOMPARE(myServices.size(), 2);
    QCOMPARE(myServices[0], m_services[0]);
    QCOMPARE(myServices[1], m_services[1]);
}

void Ut_MServiceMapper::checkServiceNamesIf()
{
    // check with interface
    QStringList myServices = m_subject->serviceNames("com.nokia.EmailServiceIf");

    QCOMPARE(myServices.size(), 1);
    QCOMPARE(myServices[0], m_services[0]);
}

void Ut_MServiceMapper::checkServiceName()
{
    QString myService = m_subject->serviceName(m_interfaces[0]);
    QCOMPARE(myService, m_services[0]);
}

void Ut_MServiceMapper::checkServicePath()
{
    QString myPath = m_subject->servicePath(m_interfaces[0]);
    QCOMPARE(myPath, QString("/"));
}

void Ut_MServiceMapper::checkHandleServiceChangedAddLastFile()
{
    QSignalSpy serviceAvailableSpy(m_subject,
                                   SIGNAL(serviceAvailable(QString, QString)));

    fillServiceFileList( 3 );
    m_subject->handleServiceChanged("/");

    QCOMPARE(serviceAvailableSpy.count(), 1);
}

void Ut_MServiceMapper::checkHandleServiceChangedRmLastFile()
{
    QSignalSpy serviceUnavailableSpy(m_subject,
                                     SIGNAL(serviceUnavailable(QString)));

    fillServiceFileList( 1 );
    m_subject->handleServiceChanged("/");

    QCOMPARE(serviceUnavailableSpy.count(), 1);
}

void Ut_MServiceMapper::checkServiceNameBadIf()
{
    fillServiceFileList( 4 );
    m_lineNo = 3;
    QString myService = m_subject->serviceName("not provided");
    QVERIFY(myService.isEmpty());
}

void Ut_MServiceMapper::checkHandleServiceChangedAddAndRemoveFiles()
{
    //
    // set up
    //

    m_currentServiceFileList.clear();
    m_noFiles = 5; // bit of a hack really - has to be >= the last file

    // set the 'last' list of files
    {
        m_currentServiceFileList
            << "0.service"
            // 1 will be added
            << "2.service" // will be removed
            << "3.service"
            << "4.service";

        m_subject->handleServiceChanged("/");
    }

    //
    // now for the test....
    //

    // listen for signals
    QSignalSpy serviceAvailableSpy(m_subject,
                                   SIGNAL(serviceAvailable(QString, QString)));
    QSignalSpy serviceUnavailableSpy(m_subject,
                                     SIGNAL(serviceUnavailable(QString)));

    m_currentServiceFileList.clear();
    m_currentServiceFileList
        << "0.service"
        << "1.service" // added
        // 2 has been removed
        << "3.service"
        << "4.service";

    m_subject->handleServiceChanged("/");

    {
        QCOMPARE(serviceAvailableSpy.count(), 1);
        QList<QVariant> arguments = serviceAvailableSpy.takeFirst();
        QCOMPARE( arguments.at(0).toString(), m_services[1] );
        QCOMPARE( arguments.at(1).toString(), m_interfaces[1] );
    }

    {
        QCOMPARE(serviceUnavailableSpy.count(), 1);
        QList<QVariant> arguments = serviceUnavailableSpy.takeFirst();
        QCOMPARE( arguments.at(0).toString(), m_services[2] );
    }
}

void Ut_MServiceMapper::fillServiceFileList( int noFiles )
{
    m_currentServiceFileList.clear();

    m_noFiles = noFiles;
    for (int thisFileNo = 0; thisFileNo < noFiles; ++thisFileNo) {
        QString filename = QString("services/%1.service").arg(thisFileNo);
        m_currentServiceFileList << filename;
    }
}

void Ut_MServiceMapper::checkInterfaceName_data()
{
    QTest::addColumn<QString>( "serviceName" );
    QTest::addColumn<QString>( "interfaceName" );
    for ( int i=0; i<initFiles; ++i ) {
        QTest::newRow(m_services[i].toAscii().constData()) << m_services[i] << m_interfaces[i];
    }

    QTest::newRow("None existent service") << "BogusService" << "";
}

void Ut_MServiceMapper::checkInterfaceName()
{
    QFETCH(QString, serviceName);
    QFETCH(QString, interfaceName);
    QString actual = m_subject->interfaceName( serviceName );
    QCOMPARE( actual, interfaceName );
}

QTEST_MAIN(Ut_MServiceMapper)
