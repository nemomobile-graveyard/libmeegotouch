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

#include <QtTest/QtTest>

#include "ut_duiappletmetadata.h"
#include "duiappletmetadata.h"

QString gType = "DUIApplet";
QString gRunner = "";
QString gApplet = "";
QString gName = "";
QString gIcon = "";
QString gExtraIdentifier = "";

// DuiDesktop entry stubs
QString DuiDesktopEntry::value(const QString &key) const
{
    if (key == "Desktop Entry/Type") {
        return QString(gType);
    } else if (key == "Desktop Entry/Name") {
        return QString(gName);
    } else if (key == "Desktop Entry/Icon") {
        return QString(gIcon);
    } else if (key == "Desktop Entry/Exec") {
        return QString(gRunner);
    } else if (key == "DUI/X-DUIApplet-Applet") {
        return QString(gApplet);
    } else if (key == "DUI/X-DUIApplet-Identifier") {
        return QString(gExtraIdentifier);
    }
    return QString();
}

bool DuiDesktopEntry::contains(const QString &key) const
{
    if (key == "Desktop Entry/Type") {
        return gType != "";
    } else if (key == "Desktop Entry/Name") {
        return gName != "";
    } else if (key == "Desktop Entry/Icon") {
        return gIcon != "";
    } else if (key == "Desktop Entry/Exec") {
        return gRunner != "";
    } else if (key == "DUI/X-DUIApplet-Applet") {
        return gApplet != "";
    } else if (key == "DUI/X-DUIApplet-Identifier") {
        return gExtraIdentifier != "";
    }
    return false;
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    if (filePath() == QString(APPLET_LIBS) + QDir::separator() + "inexisting")
        return false;
    else
        return (filePath() == QString(APPLET_LIBS) + QDir::separator() + gApplet ||
                filePath() == QString(APPLET_LIBS) + QDir::separator() + gRunner);
}

bool QFileInfo::isFile() const
{
    if (filePath() == QString(APPLET_LIBS) + QDir::separator() + "inexisting")
        return false;
    else
        return (filePath() == QString(APPLET_LIBS) + QDir::separator() + gApplet ||
                filePath() == QString(APPLET_LIBS) + QDir::separator() + gRunner);
}

bool QFileInfo::isExecutable() const
{
    if (filePath() == QString(APPLET_LIBS) + QDir::separator() + "inexisting")
        return false;
    else
        return (filePath() == QString(APPLET_LIBS) + QDir::separator() + gApplet ||
                filePath() == QString(APPLET_LIBS) + QDir::separator() + gRunner);
}

void Ut_DuiAppletMetaData::init()
{
    m_subject = new DuiAppletMetaData("");
    gName = "Unit Test";
    gIcon = "Icon-UnitTest";
    gRunner = "duiappletrunner";
    gApplet = "test.so";
}

void Ut_DuiAppletMetaData::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiAppletMetaData::testValidMetaData()
{
    // Runner is defined. This marks out-of-process applet
    QCOMPARE(m_subject->isValid(), true);

    // Runner is not defined. This marks in-process applet
    gRunner = "";
    QCOMPARE(m_subject->isValid(), true);
}

void Ut_DuiAppletMetaData::testBinaryMissing()
{
    gApplet = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiAppletMetaData::testBinaryAndRunnerPaths()
{
    // APPLET_LIBS path should be prepended to binary and runner paths.
    QString absoluteBinaryPath = QFileInfo(QString(APPLET_LIBS) + QDir::separator() + gApplet).absoluteFilePath();
    QString absoluteRunnerPath = QFileInfo(QString(APPLET_LIBS) + QDir::separator() + gRunner).absoluteFilePath();
    QCOMPARE(m_subject->appletBinary(), absoluteBinaryPath);
    QCOMPARE(m_subject->runnerBinary(), absoluteRunnerPath);
}

void Ut_DuiAppletMetaData::testInvalidRunner()
{
    gRunner = "inexisting";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiAppletMetaData::testInvalidApplet()
{
    gApplet = "inexisting";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiAppletMetaData::testAppletMissing()
{
    gApplet = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiAppletMetaData::testNameMissing()
{
    gName   = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiAppletMetaData::testIconMissing()
{
    gIcon   = "";
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiAppletMetaData::testExtraIdentifier()
{
    gExtraIdentifier = "";
    QVERIFY(!m_subject->contains("DUI/X-DUIApplet-Identifier"));
    gExtraIdentifier = "foo";
    QVERIFY(m_subject->contains("DUI/X-DUIApplet-Identifier"));
}

void Ut_DuiAppletMetaData::testResourceIdentifier()
{
    // Valid cases
    gExtraIdentifier = "foo";
    gApplet = "libfoo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString("foo"));
    gApplet = DUI_INSTALL_LIBS "/libfoo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString("foo"));
    gExtraIdentifier = "xyz";
    gApplet = DUI_INSTALL_LIBS "/libfoo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString("xyz"));

    // Invalid cases
    gExtraIdentifier = "";
    gApplet = "";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));
    gApplet = "foo";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));
    gApplet = "foo.so";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));
    gApplet = "libfoo";
    QCOMPARE(m_subject->resourceIdentifier(), QString(""));

}

QTEST_APPLESS_MAIN(Ut_DuiAppletMetaData)
