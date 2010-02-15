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

#include <QObject>
#include <QtDebug>
#include <QString>
#include "ut_duiservicemapper.h"

#include "duiservicemapper_p.h"

const int Ut_DuiServiceMapper::noAllFiles;
QTextStream Ut_DuiServiceMapper::globalDummy;
int Ut_DuiServiceMapper::noFiles;
int Ut_DuiServiceMapper::lineNo;
int Ut_DuiServiceMapper::fileNo;
QString Ut_DuiServiceMapper::lines[4][2];
QString Ut_DuiServiceMapper::services[3];
QString Ut_DuiServiceMapper::interfaces[3];

QStringList DuiServiceMapperPrivate::fillServiceFileList() const
{
    QStringList retVal;
    for (int thisFileNo = 0; thisFileNo < Ut_DuiServiceMapper::noFiles; ++thisFileNo) {
        QString filename = QString("services/%1.service").arg(thisFileNo);
        retVal << filename;
    }

    return retVal;
}

bool DuiServiceMapperPrivate::fileExistsAndReadable(const QString &fileName) const
{
    Q_UNUSED(fileName);
    bool retVal;

    QString baseName = QFileInfo(fileName).baseName();
    int thisFileNo = baseName.toInt();
    retVal = thisFileNo < Ut_DuiServiceMapper::noFiles;

    return retVal;
}

QIODevice *DuiServiceMapperPrivate::accessFile(const QString &fileName) const
{
    Q_UNUSED(fileName);
    QBuffer *thisFile = new QBuffer();
    thisFile->open(QIODevice::ReadWrite);

    int lastFileNo = Ut_DuiServiceMapper::noFiles - 1;

    QString baseName = QFileInfo(fileName).baseName();
    int thisFileNo = baseName.toInt();

    bool thisFileShouldExist = thisFileNo <= lastFileNo;
    if (thisFileShouldExist) {
        for (int thisLineNo = 0; thisLineNo < 2; ++thisLineNo) {
            QTextStream output(thisFile);
            output << Ut_DuiServiceMapper::lines[ thisFileNo ][ thisLineNo ] << endl;
        }
    }
    thisFile->seek(0);

    return thisFile;
}

void Ut_DuiServiceMapper::init()
{
    // always start with two files
    //mkFiles( 2 );
    noFiles = 2;

    // reset to beginning of first file for each test
    // difficult to keep track otherwise
    lineNo = 0;
    fileNo = 0;

    m_subject = new DuiServiceMapper("/");
}

void Ut_DuiServiceMapper::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiServiceMapper::initTestCase()
{
    services[0] = "com.nokia.EmailService";
    interfaces[0] = "com.nokia.EmailServiceIf";
    services[1] = "org.maemo.EmailService";
    interfaces[1] = "org.maemo.EmailServiceIf";
    services[2] = "com.google.EmailService";
    interfaces[2] = "com.google.EmailServiceIf";

    lines[0][0] = "Interface=" + interfaces[0];
    lines[0][1] = "Name=" + services[0];
    lines[1][0] = "Interface=" + interfaces[1];
    lines[1][1] = "Name=" + services[1];
    lines[2][0] = "Interface=" + interfaces[2];
    lines[2][1] = "Name=" + services[2];
    lines[3][0] = "Interface=not provided";
    lines[3][1] = "Name=not provided";

    //QDir(".").mkdir( "services" );
}

void Ut_DuiServiceMapper::cleanupTestCase()
{
    //mkFiles(0);
    noFiles = 0;
    QDir(".").rmdir("services");
}

void Ut_DuiServiceMapper::mkFiles(int noFiles)
{
    int lastFileNo = noFiles - 1;

    for (int thisFileNo = 0; thisFileNo < noAllFiles; ++thisFileNo) {
        QString filename = QString("services/%1.service").arg(thisFileNo);

        QFile thisFile(filename);
        bool thisFileShouldExist = thisFileNo <= lastFileNo;
        if (thisFile.exists()) {
            if (!thisFileShouldExist) {
                thisFile.remove();
            }
        } else {
            if (thisFileShouldExist) {
                bool openFailed = !thisFile.open(QIODevice::WriteOnly | QIODevice::Text);
                if (openFailed) {
                    qCritical() << "Could not open file for writing";
                }

                for (int thisLineNo = 0; thisLineNo < 2; ++thisLineNo) {
                    QTextStream output(&thisFile);
                    output << lines[ thisFileNo ][ thisLineNo ] << endl;
                }
            }
        }

        thisFile.close();
    }
}

void Ut_DuiServiceMapper::checkServiceNamesNoIf()
{
    // check without interface - should list all
    QStringList myServices = m_subject->serviceNames();

    QCOMPARE(myServices.size(), 2);
    QCOMPARE(myServices[0], services[0]);
    QCOMPARE(myServices[1], services[1]);
}

void Ut_DuiServiceMapper::checkServiceNamesIf()
{
    // check with interface
    QStringList myServices = m_subject->serviceNames("com.nokia.EmailServiceIf");

    QCOMPARE(myServices.size(), 1);
    QCOMPARE(myServices[0], services[0]);
}

void Ut_DuiServiceMapper::checkServiceName()
{
    QString myService = m_subject->serviceName(interfaces[0]);
    QCOMPARE(myService, services[0]);
}

void Ut_DuiServiceMapper::checkServicePath()
{
    QString myPath = m_subject->servicePath(interfaces[0]);
    QCOMPARE(myPath, QString("/"));
}

void Ut_DuiServiceMapper::checkHandleServiceChangedAddLastFile()
{
    QSignalSpy serviceAvailableSpy(m_subject,
                                   SIGNAL(serviceAvailable(QString, QString)));

    // add a file - constructed with 2 files
    //mkFiles( 3 );
    noFiles = 3;
    m_subject->handleServiceChanged("/");

    QCOMPARE(serviceAvailableSpy.count(), 1);
}

void Ut_DuiServiceMapper::checkHandleServiceChangedRmLastFile()
{
    QSignalSpy serviceUnavailableSpy(m_subject,
                                     SIGNAL(serviceUnavailable(QString)));

    // remove a file - constructed with 2 files
    //mkFiles( 1 );
    noFiles = 1;
    m_subject->handleServiceChanged("/");

    QCOMPARE(serviceUnavailableSpy.count(), 1);
}

void Ut_DuiServiceMapper::checkServiceNameBadIf()
{
    //mkFiles( 4 );
    noFiles = 4;
    lineNo = 3;
    QString myService = m_subject->serviceName("not provided");
    QVERIFY(myService.isEmpty());
}

QTEST_MAIN(Ut_DuiServiceMapper)
