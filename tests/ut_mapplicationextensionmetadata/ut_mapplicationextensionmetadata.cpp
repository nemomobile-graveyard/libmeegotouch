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

#include <QtTest/QtTest>
#include <mapplicationextensionmetadata.h>
#include "ut_mapplicationextensionmetadata.h"

const QString EXTENSION_BINARY_KEY("X-MeeGoApplicationExtension/Extension");
const QString IDENTIFIER_KEY("X-MeeGoApplicationExtension/Identifier");
const QString INTERFACE_KEY("X-MeeGoApplicationExtension/Interface");

QString gType = "MApplicationExtension";
QString gRunner = "";
QString gExtension = "";
QString gInterface = "";
QString gExtraIdentifier = "";
QString gName = "";
QString gIcon = "";

// MDesktop entry stubs
QString MDesktopEntry::value(const QString &key) const
{
    if (key == "Desktop Entry/Type") {
        return QString(gType);
    } else if (key == "Desktop Entry/Name") {
        return QString(gName);
    } else if (key == "Desktop Entry/Icon") {
        return QString(gIcon);
    } else if (key == "Desktop Entry/Exec") {
        return QString(gRunner);
    } else if (key == EXTENSION_BINARY_KEY) {
        return QString(gExtension);
    } else if (key == INTERFACE_KEY) {
        return QString(gInterface);
    } else if (key == IDENTIFIER_KEY) {
        return QString(gExtraIdentifier);
    }
    return QString();
}

bool MDesktopEntry::contains(const QString &key) const
{
    if (key == "Desktop Entry/Type") {
        return gType != "";
    } else if (key == "Desktop Entry/Name") {
        return gName != "";
    } else if (key == "Desktop Entry/Icon") {
        return gIcon != "";
    } else if (key == "Desktop Entry/Exec") {
        return gRunner != "";
    } else if (key == EXTENSION_BINARY_KEY) {
        return gExtension != "";
    } else if (key == INTERFACE_KEY) {
        return gInterface != "";
    } else if (key == IDENTIFIER_KEY) {
        return gExtraIdentifier != "";
    }
    return false;
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    if (filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + "inexisting")
        return false;
    else
        return (filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gExtension ||
                filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gRunner);
}

bool QFileInfo::isFile() const
{
    if (filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + "inexisting")
        return false;
    else
        return (filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gExtension ||
                filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gRunner);
}

bool QFileInfo::isExecutable() const
{
    if (filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + "inexisting")
        return false;
    else
        return (filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gExtension ||
                filePath() == QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gRunner);
}

void Ut_MApplicationExtensionMetaData::init()
{
    m_subject = new MApplicationExtensionMetaData("");
    gName = "Unit Test";
    gIcon = "Icon-UnitTest";
    gRunner = "mappletrunner";
    gExtension = "test.so";
    gInterface = "test/1.0";
}

void Ut_MApplicationExtensionMetaData::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MApplicationExtensionMetaData::initTestCase()
{
}

void Ut_MApplicationExtensionMetaData::cleanupTestCase()
{
}

void Ut_MApplicationExtensionMetaData::testValidMetaData()
{
    // Runner is defined. This marks out-of-process applet
    QCOMPARE(m_subject->isValid(), true);

    // Runner is not defined. This marks in-process applet
    gRunner = "";
    QCOMPARE(m_subject->isValid(), true);
}

void Ut_MApplicationExtensionMetaData::testBinaryMissing()
{
    gExtension = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_MApplicationExtensionMetaData::testBinaryAndRunnerPaths()
{
    // APPLICATION_EXTENSION_LIBS path should be prepended to binary and runner paths.
    QString absoluteBinaryPath = QFileInfo(QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gExtension).absoluteFilePath();
    QString absoluteRunnerPath = QFileInfo(QString(APPLICATION_EXTENSION_LIBS) + QDir::separator() + gRunner).absoluteFilePath();
    QCOMPARE(m_subject->extensionBinary(), absoluteBinaryPath);
    QCOMPARE(m_subject->runnerBinary(), absoluteRunnerPath);
}

void Ut_MApplicationExtensionMetaData::testInvalidRunner()
{
    gRunner = "inexisting";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_MApplicationExtensionMetaData::testInvalidExtension()
{
    gExtension = "inexisting";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_MApplicationExtensionMetaData::testExtensionMissing()
{
    gExtension = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_MApplicationExtensionMetaData::testInterfaceMissing()
{
    gInterface = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_MApplicationExtensionMetaData::testNameMissing()
{
    gName   = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_MApplicationExtensionMetaData::testExtraIdentifier()
{
    gExtraIdentifier = "";
    QVERIFY(!m_subject->contains(IDENTIFIER_KEY));
    gExtraIdentifier = "foo";
    QVERIFY(m_subject->contains(IDENTIFIER_KEY));
}

void Ut_MApplicationExtensionMetaData::testResourceIdentifier()
{
    // Valid cases
    gExtraIdentifier = "foo";
    gExtension = "libfoo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString("foo"));
    gExtension = M_INSTALL_LIBS "/libfoo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString("foo"));
    gExtraIdentifier = "xyz";
    gExtension = M_INSTALL_LIBS "/libfoo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString("xyz"));

    // Invalid cases
    gExtraIdentifier = "";
    gExtension = "";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));
    gExtension = "foo";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));
    gExtension = "foo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));
    gExtension = "libfoo";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));

}

QTEST_MAIN(Ut_MApplicationExtensionMetaData)
