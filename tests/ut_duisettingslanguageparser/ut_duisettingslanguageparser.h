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

#ifndef UT_DUISETTINGSLANGUAGEPARSER_H
#define UT_DUISETTINGSLANGUAGEPARSER_H

#include <QtTest/QtTest>
#include <QObject>

class DuiSettingsLanguageParser;
class DuiSettingsLanguageBinary;

class Ut_DuiSettingsLanguageParser : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test methods starting with E test error cases
    void EtestReadFromInaccessibleSource();
    void EtestCallCreateSettingsBinaryBeforeReadFrom();
    void EtestParseEmptyXML();
    void EtestParseInvalidRootElement();
    void testParseSettingsNode();
    void testParseSelectionInSettings();
    void EtestParseSelectionWithoutKey();
    void testParseSelectionOneValue();
    void EtestParseSelectionWithInvalidValue();
    void testParseSelectionManyValues();

    void testParseTextInSettings();
    void EtestParseTextWithoutKey();
    void EtestParseTextWithoutTitle();
    void testParseBooleanInSettings();
    void EtestParseBooleanWithoutTitle();
    void EtestParseBooleanWithoutKey();
    void EtestParseBooleanEmpty();

    void testParseIntegerInSettings();
    void testParseIntegerWithoutMax() ;
    void testParseIntegerWithoutMin();
    void testParseIntegerWithoutMinOrMax();
    void EtestParseIntegerWithoutKey();
    void EtestParseIntegerWithoutTitle();
    void EtestParseIntegerWithInsaneMinMax();

    void testParseGroupInSettings();
    void testParseMultipleGroupsInSettings();

private:
    void executeParsingTest(const char *xml);

    DuiSettingsLanguageParser *m_subject;
    // A settings binary helper used for verifying the test results
    DuiSettingsLanguageBinary *m_testSettingsBinary;
};

#endif
