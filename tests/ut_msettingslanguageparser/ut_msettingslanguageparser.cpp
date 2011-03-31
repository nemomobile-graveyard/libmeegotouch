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

#include "ut_msettingslanguageparser.h"

#include "msettingslanguageparser.h"
#include "msettingslanguagebinary.h"
#include <msettingslanguagenode.h>
#include <msettingslanguagesettings_stub.h>
#include <msettingslanguageselection_stub.h>
#include <msettingslanguageoption_stub.h>
#include <msettingslanguageboolean_stub.h>
#include <msettingslanguageinteger_stub.h>
#include <msettingslanguagetext_stub.h>
#include <msettingslanguagegroup_stub.h>

// A QIODevice that we can control nicely for testing purposes
class InaccessibleDevice : public QIODevice
{
public:
    InaccessibleDevice() :
        dataSize(-1),
        openable(true) {
    }

    // The return value that readData() and writeData() returns
    qint64 dataSize;
    // A flag to control if a call to open() succeeds or not
    bool openable;

    bool open(OpenMode mode) {
        QIODevice::open(mode);
        return openable;
    }

protected:
    qint64 readData(char *data, qint64 maxSize) {
        Q_UNUSED(data);
        Q_UNUSED(maxSize);
        return dataSize;
    }

    qint64 writeData(const char *data, qint64 maxSize) {
        Q_UNUSED(data);
        Q_UNUSED(maxSize);
        return dataSize;
    }
};


// Called before the first testfunction is executed
void Ut_MSettingsLanguageParser::initTestCase()
{
    m_subject = NULL;
    m_testSettingsBinary = NULL;
}

// Called after the last testfunction was executed
void Ut_MSettingsLanguageParser::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_MSettingsLanguageParser::init()
{
    m_subject = new MSettingsLanguageParser();
    gMSettingsLanguageSelectionStub->stubReset();
    gMSettingsLanguageOptionStub->stubReset();
    gMSettingsLanguageBooleanStub->stubReset();
    gMSettingsLanguageIntegerStub->stubReset();
}

// Called after every testfunction
void Ut_MSettingsLanguageParser::cleanup()
{
    delete m_subject;
    m_subject = NULL;
    delete m_testSettingsBinary;
    m_testSettingsBinary = NULL;
}

void Ut_MSettingsLanguageParser::executeParsingTest(const char *xml)
{
    QByteArray ba(xml);
    QBuffer b(&ba);
    m_subject->readFrom(b);
    delete m_testSettingsBinary;
    m_testSettingsBinary = m_subject->createSettingsBinary();
}

void Ut_MSettingsLanguageParser::EtestReadFromInaccessibleSource()
{
    InaccessibleDevice inaccessibleDevice;

    // A source where reading fails
    m_subject->readFrom(inaccessibleDevice);
    QCOMPARE(m_subject->createSettingsBinary(), (MSettingsLanguageBinary *)NULL);

    // A source that fails to open and reading fails
    inaccessibleDevice.openable = false;
    m_subject->readFrom(inaccessibleDevice);
    QCOMPARE(m_subject->createSettingsBinary(), (MSettingsLanguageBinary *)NULL);

    // A source that fails to open but reading succeeds (which actually should never happen, but let's still test it)
    inaccessibleDevice.dataSize = 10;
    m_subject->readFrom(inaccessibleDevice);
    QCOMPARE(m_subject->createSettingsBinary(), (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestCallCreateSettingsBinaryBeforeReadFrom()
{
    QCOMPARE(m_subject->createSettingsBinary(), (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseEmptyXML()
{
    executeParsingTest("");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseInvalidRootElement()
{
    executeParsingTest("<foobar></foobar>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);

    executeParsingTest("<selection key=\"key\" title=\"title\"></selection>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::testParseSettingsNode()
{
    executeParsingTest("<settings></settings>");
    QVERIFY(dynamic_cast<const MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0)));
}

void Ut_MSettingsLanguageParser::testParseSelectionInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\" title=\"title\"></selection>\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    QCOMPARE(si->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const MSettingsLanguageSelection *>(si->children().at(0))));
    QCOMPARE(gMSettingsLanguageSelectionStub->stubLastCallTo("constructor").parameter<QString>(0), QString("key"));
}

void Ut_MSettingsLanguageParser::EtestParseSelectionWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <selection title=\"title\"></selection>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseSelectionWithoutTitle()
{
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\"></selection>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::testParseSelectionOneValue()
{
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\" title=\"title\">\n"
                       "    <option title=\"Title\">5</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(gMSettingsLanguageSelectionStub->stubCallCount("addOption"), 1);
    QCOMPARE(gMSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<QString>(0), QString("Title"));
    QCOMPARE(gMSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<int>(1), 5);
}

void Ut_MSettingsLanguageParser::EtestParseSelectionWithInvalidValue()
{
    // value element doesn't have a required title attribute
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\" title=\"title\">\n"
                       "    <option>5</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);

    // value element has invalid content (should be an integer)
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\" title=\"title\">\n"
                       "    <option title=\"Title\">foo</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::testParseSelectionManyValues()
{
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\" title=\"title\">\n"
                       "    <option title=\"Title1\">5</option>\n"
                       "    <option title=\"Title2\">6</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(gMSettingsLanguageSelectionStub->stubCallCount("addOption"), 2);
    QCOMPARE(gMSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<QString>(0), QString("Title2"));
    QCOMPARE(gMSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<int>(1), 6);
}

void Ut_MSettingsLanguageParser::testParseTextInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <text key=\"aKey\" title=\"aTitle\"/>\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    QCOMPARE(si->numChildren(), uint(1));
    QCOMPARE(gMSettingsLanguageTextStub->stubLastCallTo("constructor").parameter<QString>(0), QString("aKey"));
    QCOMPARE(gMSettingsLanguageTextStub->stubLastCallTo("constructor").parameter<QString>(1), QString("aTitle"));
}

void Ut_MSettingsLanguageParser::EtestParseTextWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <text/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseTextWithoutTitle()
{
    executeParsingTest("<settings>\n"
                       "  <text key=\"aKey\"/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::testParseBooleanInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <boolean key=\"key\" title=\"titleBoolean\"/>\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    QCOMPARE(si->numChildren(), uint(1));
    QCOMPARE(gMSettingsLanguageBooleanStub->stubLastCallTo("constructor").parameter<QString>(0), QString("key"));
    QCOMPARE(gMSettingsLanguageBooleanStub->stubLastCallTo("constructor").parameter<QString>(1), QString("titleBoolean"));
}

void Ut_MSettingsLanguageParser::EtestParseBooleanWithoutTitle()
{
    executeParsingTest("<settings>\n"
                       "  <boolean key=\"keyBoolean\"/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseBooleanWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <boolean title=\"titleBoolean\"/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseBooleanEmpty()
{
    executeParsingTest("<settings>\n"
                       "  <boolean/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::testParseIntegerInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" min=\"-90\" max=\"90\"/>\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    QCOMPARE(si->numChildren(), uint(1));
    QCOMPARE(gMSettingsLanguageIntegerStub->stubLastCallTo("constructor").parameter<QString>(0), QString("aKey"));
    QCOMPARE(gMSettingsLanguageIntegerStub->stubLastCallTo("constructor").parameter<QString>(1), QString("aTitle"));
    QCOMPARE(gMSettingsLanguageIntegerStub->stubLastCallTo("setMinValue").parameter<int>(0), -90);
    QCOMPARE(gMSettingsLanguageIntegerStub->stubLastCallTo("setMaxValue").parameter<int>(0), 90);
}

void Ut_MSettingsLanguageParser::testParseIntegerWithoutMax()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" min=\"80\" />\n"
                       "</settings>");
    QCOMPARE(gMSettingsLanguageIntegerStub->stubLastCallTo("setMinValue").parameter<int>(0), 80);
    QCOMPARE(gMSettingsLanguageIntegerStub->stubCallCount("setMaxValue"), 0);
}

void Ut_MSettingsLanguageParser::testParseIntegerWithoutMin()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" max=\"-180\" />\n"
                       "</settings>");
    QCOMPARE(gMSettingsLanguageIntegerStub->stubCallCount("setMinValue"), 0);
    QCOMPARE(gMSettingsLanguageIntegerStub->stubLastCallTo("setMaxValue").parameter<int>(0), -180);
}

void Ut_MSettingsLanguageParser::testParseIntegerWithoutMinOrMax()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" />\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    int val;
    // no min value
    QVERIFY(! dynamic_cast<MSettingsLanguageInteger *>(si->children().at(0))->minValue(val));
    // no max value
    QVERIFY(! dynamic_cast<MSettingsLanguageInteger *>(si->children().at(0))->maxValue(val));
}

void Ut_MSettingsLanguageParser::EtestParseIntegerWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <integer/>\n"
                       "</settings>");

    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseIntegerWithoutTitle()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\"/>\n"
                       "</settings>");

    QCOMPARE(m_testSettingsBinary, (MSettingsLanguageBinary *)NULL);
}

void Ut_MSettingsLanguageParser::EtestParseIntegerWithInsaneMinMax()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" min=\"80\" max=\"-70\"/>\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    int val;
    // no min value
    QVERIFY(! dynamic_cast<MSettingsLanguageInteger *>(si->children().at(0))->minValue(val));
    // no max value
    QVERIFY(! dynamic_cast<MSettingsLanguageInteger *>(si->children().at(0))->maxValue(val));
}

void Ut_MSettingsLanguageParser::testParseGroupInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <group title=\"title\">\n"
                       "    <boolean key=\"aKey\" title=\"titleBoolean\"/>\n"
                       "  </group>\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    QCOMPARE(si->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const MSettingsLanguageGroup *>(si->children().at(0))));
    QVERIFY((dynamic_cast<const MSettingsLanguageBoolean *>(si->children().at(0)->children().at(0))));
}

void Ut_MSettingsLanguageParser::testParseMultipleGroupsInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <boolean key=\"aKey\" title=\"titleBoolean\"/>\n"
                       "  <group title=\"title\">\n"
                       "    <integer key=\"aKey\" title=\"aTitle\"/>\n"
                       "  </group>\n"
                       "  <group title=\"title\">\n"
                       "    <text key=\"aKey\" title=\"aTitle\"/>\n"
                       "  </group>\n"
                       "</settings>");
    MSettingsLanguageSettings *si = dynamic_cast<MSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QVERIFY(si != NULL);
    QCOMPARE(si->numChildren(), uint(3));
    QVERIFY((dynamic_cast<const MSettingsLanguageBoolean *>(si->children().at(0))));

    QVERIFY((dynamic_cast<const MSettingsLanguageGroup *>(si->children().at(1))));
    QCOMPARE(si->children().at(1)->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const MSettingsLanguageInteger *>(si->children().at(1)->children().at(0))));

    QVERIFY((dynamic_cast<const MSettingsLanguageGroup *>(si->children().at(2))));
    QCOMPARE(si->children().at(2)->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const MSettingsLanguageText *>(si->children().at(2)->children().at(0))));
}

QTEST_MAIN(Ut_MSettingsLanguageParser)
