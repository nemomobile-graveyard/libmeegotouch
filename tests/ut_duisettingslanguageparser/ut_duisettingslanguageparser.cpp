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

#include "ut_duisettingslanguageparser.h"

#include "duisettingslanguageparser.h"
#include "duisettingslanguagebinary.h"
#include <duisettingslanguagenode.h>
#include <duisettingslanguagesettings_stub.h>
#include <duisettingslanguageselection_stub.h>
#include <duisettingslanguageoption_stub.h>
#include <duisettingslanguageboolean_stub.h>
#include <duisettingslanguageinteger_stub.h>
#include <duisettingslanguagetext_stub.h>
#include <duisettingslanguagegroup.h>

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
void Ut_DuiSettingsLanguageParser::initTestCase()
{
    m_subject = NULL;
    m_testSettingsBinary = NULL;
}

// Called after the last testfunction was executed
void Ut_DuiSettingsLanguageParser::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_DuiSettingsLanguageParser::init()
{
    m_subject = new DuiSettingsLanguageParser();
    gDuiSettingsLanguageSelectionStub->stubReset();
    gDuiSettingsLanguageOptionStub->stubReset();
    gDuiSettingsLanguageBooleanStub->stubReset();
    gDuiSettingsLanguageIntegerStub->stubReset();
}

// Called after every testfunction
void Ut_DuiSettingsLanguageParser::cleanup()
{
    delete m_subject;
    m_subject = NULL;
    delete m_testSettingsBinary;
    m_testSettingsBinary = NULL;
}

void Ut_DuiSettingsLanguageParser::executeParsingTest(const char *xml)
{
    QByteArray ba(xml);
    QBuffer b(&ba);
    m_subject->readFrom(b);
    delete m_testSettingsBinary;
    m_testSettingsBinary = m_subject->createSettingsBinary();
}

void Ut_DuiSettingsLanguageParser::EtestReadFromInaccessibleSource()
{
    InaccessibleDevice inaccessibleDevice;

    // A source where reading fails
    m_subject->readFrom(inaccessibleDevice);
    QCOMPARE(m_subject->createSettingsBinary(), (DuiSettingsLanguageBinary *)NULL);

    // A source that fails to open and reading fails
    inaccessibleDevice.openable = false;
    m_subject->readFrom(inaccessibleDevice);
    QCOMPARE(m_subject->createSettingsBinary(), (DuiSettingsLanguageBinary *)NULL);

    // A source that fails to open but reading succeeds (which actually should never happen, but let's still test it)
    inaccessibleDevice.dataSize = 10;
    m_subject->readFrom(inaccessibleDevice);
    QCOMPARE(m_subject->createSettingsBinary(), (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestCallCreateSettingsBinaryBeforeReadFrom()
{
    QCOMPARE(m_subject->createSettingsBinary(), (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestParseEmptyXML()
{
    executeParsingTest("");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestParseInvalidRootElement()
{
    executeParsingTest("<foobar></foobar>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);

    executeParsingTest("<selection key=\"key\"></selection>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::testParseSettingsNode()
{
    executeParsingTest("<settings></settings>");
    QVERIFY(dynamic_cast<const DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0)));
}

void Ut_DuiSettingsLanguageParser::testParseSelectionInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\"></selection>\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QCOMPARE(si->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const DuiSettingsLanguageSelection *>(si->children().at(0))));
    QCOMPARE(gDuiSettingsLanguageSelectionStub->stubLastCallTo("constructor").parameter<QString>(0), QString("key"));
}

void Ut_DuiSettingsLanguageParser::EtestParseSelectionWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <selection></selection>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::testParseSelectionOneValue()
{
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\">\n"
                       "    <option title=\"Title\">5</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(gDuiSettingsLanguageSelectionStub->stubCallCount("addOption"), 1);
    QCOMPARE(gDuiSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<QString>(0), QString("Title"));
    QCOMPARE(gDuiSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<int>(1), 5);
}

void Ut_DuiSettingsLanguageParser::EtestParseSelectionWithInvalidValue()
{
    // value element doesn't have a required title attribute
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\">\n"
                       "    <option>5</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);

    // value element has invalid content (should be an integer)
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\">\n"
                       "    <option title=\"Title\">foo</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::testParseSelectionManyValues()
{
    executeParsingTest("<settings>\n"
                       "  <selection key=\"key\">\n"
                       "    <option title=\"Title1\">5</option>\n"
                       "    <option title=\"Title2\">6</option>\n"
                       "  </selection>\n"
                       "</settings>");
    QCOMPARE(gDuiSettingsLanguageSelectionStub->stubCallCount("addOption"), 2);
    QCOMPARE(gDuiSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<QString>(0), QString("Title2"));
    QCOMPARE(gDuiSettingsLanguageSelectionStub->stubLastCallTo("addOption").parameter<int>(1), 6);
}

void Ut_DuiSettingsLanguageParser::testParseTextInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <text key=\"aKey\" title=\"aTitle\"/>\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QCOMPARE(si->numChildren(), uint(1));
    QCOMPARE(gDuiSettingsLanguageTextStub->stubLastCallTo("constructor").parameter<QString>(0), QString("aKey"));
    QCOMPARE(gDuiSettingsLanguageTextStub->stubLastCallTo("constructor").parameter<QString>(1), QString("aTitle"));
}

void Ut_DuiSettingsLanguageParser::EtestParseTextWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <text/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestParseTextWithoutTitle()
{
    executeParsingTest("<settings>\n"
                       "  <text key=\"aKey\"/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::testParseBooleanInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <boolean key=\"key\" title=\"titleBoolean\"/>\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QCOMPARE(si->numChildren(), uint(1));
    QCOMPARE(gDuiSettingsLanguageBooleanStub->stubLastCallTo("constructor").parameter<QString>(0), QString("key"));
    QCOMPARE(gDuiSettingsLanguageBooleanStub->stubLastCallTo("constructor").parameter<QString>(1), QString("titleBoolean"));
}

void Ut_DuiSettingsLanguageParser::EtestParseBooleanWithoutTitle()
{
    executeParsingTest("<settings>\n"
                       "  <boolean key=\"keyBoolean\"/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestParseBooleanWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <boolean title=\"titleBoolean\"/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestParseBooleanEmpty()
{
    executeParsingTest("<settings>\n"
                       "  <boolean/>\n"
                       "</settings>");
    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::testParseIntegerInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" min=\"-90\" max=\"90\"/>\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QCOMPARE(si->numChildren(), uint(1));
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubLastCallTo("constructor").parameter<QString>(0), QString("aKey"));
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubLastCallTo("constructor").parameter<QString>(1), QString("aTitle"));
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubLastCallTo("setMinValue").parameter<int>(0), -90);
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubLastCallTo("setMaxValue").parameter<int>(0), 90);
}

void Ut_DuiSettingsLanguageParser::testParseIntegerWithoutMax()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" min=\"80\" />\n"
                       "</settings>");
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubLastCallTo("setMinValue").parameter<int>(0), 80);
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubCallCount("setMaxValue"), 0);
}

void Ut_DuiSettingsLanguageParser::testParseIntegerWithoutMin()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" max=\"-180\" />\n"
                       "</settings>");
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubCallCount("setMinValue"), 0);
    QCOMPARE(gDuiSettingsLanguageIntegerStub->stubLastCallTo("setMaxValue").parameter<int>(0), -180);
}

void Ut_DuiSettingsLanguageParser::testParseIntegerWithoutMinOrMax()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" />\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));

    int val;
    // no min value
    QVERIFY(! dynamic_cast<DuiSettingsLanguageInteger *>(si->children().at(0))->minValue(val));
    // no max value
    QVERIFY(! dynamic_cast<DuiSettingsLanguageInteger *>(si->children().at(0))->maxValue(val));
}

void Ut_DuiSettingsLanguageParser::EtestParseIntegerWithoutKey()
{
    executeParsingTest("<settings>\n"
                       "  <integer/>\n"
                       "</settings>");

    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestParseIntegerWithoutTitle()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\"/>\n"
                       "</settings>");

    QCOMPARE(m_testSettingsBinary, (DuiSettingsLanguageBinary *)NULL);
}

void Ut_DuiSettingsLanguageParser::EtestParseIntegerWithInsaneMinMax()
{
    executeParsingTest("<settings>\n"
                       "  <integer key=\"aKey\" title=\"aTitle\" min=\"80\" max=\"-70\"/>\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));

    int val;
    // no min value
    QVERIFY(! dynamic_cast<DuiSettingsLanguageInteger *>(si->children().at(0))->minValue(val));
    // no max value
    QVERIFY(! dynamic_cast<DuiSettingsLanguageInteger *>(si->children().at(0))->maxValue(val));
}

void Ut_DuiSettingsLanguageParser::testParseGroupInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <group>\n"
                       "    <boolean key=\"aKey\" title=\"titleBoolean\"/>\n"
                       "  </group>\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QCOMPARE(si->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const DuiSettingsLanguageGroup *>(si->children().at(0))));
    QVERIFY((dynamic_cast<const DuiSettingsLanguageBoolean *>(si->children().at(0)->children().at(0))));
}

void Ut_DuiSettingsLanguageParser::testParseMultipleGroupsInSettings()
{
    executeParsingTest("<settings>\n"
                       "  <boolean key=\"aKey\" title=\"titleBoolean\"/>\n"
                       "  <group>\n"
                       "    <integer key=\"aKey\" title=\"aTitle\"/>\n"
                       "  </group>\n"
                       "  <group>\n"
                       "    <text key=\"aKey\" title=\"aTitle\"/>\n"
                       "  </group>\n"
                       "</settings>");
    DuiSettingsLanguageSettings *si = dynamic_cast<DuiSettingsLanguageSettings *>(m_testSettingsBinary->children().at(0));
    QCOMPARE(si->numChildren(), uint(3));
    QVERIFY((dynamic_cast<const DuiSettingsLanguageBoolean *>(si->children().at(0))));

    QVERIFY((dynamic_cast<const DuiSettingsLanguageGroup *>(si->children().at(1))));
    QCOMPARE(si->children().at(1)->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const DuiSettingsLanguageInteger *>(si->children().at(1)->children().at(0))));

    QVERIFY((dynamic_cast<const DuiSettingsLanguageGroup *>(si->children().at(2))));
    QCOMPARE(si->children().at(2)->numChildren(), uint(1));
    QVERIFY((dynamic_cast<const DuiSettingsLanguageText *>(si->children().at(2)->children().at(0))));
}

QTEST_MAIN(Ut_DuiSettingsLanguageParser)
