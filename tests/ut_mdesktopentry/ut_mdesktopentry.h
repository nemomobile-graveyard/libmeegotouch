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

#ifndef UT_MDESKTOPENTRY_H
#define UT_MDESKTOPENTRY_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/mdesktopentry class declaration
#include <mdesktopentry.h>

class Ut_MDesktopEntry : public QObject
{
    Q_OBJECT

private:
    MDesktopEntry *m_subject;

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void testFileNameMethod();
    void testMissingType();
    void testMissingName();
    void testMissingExec();
    void testIsHidden();
    void testApplication();
    void testLink();
    void testVersion();
    void testGenericName();
    void testMisc();

    void testNoDisplay();
    void testTerminal();
    void testStartupNotify();
    void testLocalizedNameLogicalId();
    void testLocalizedNamePostfix();
    void testMultipleValues();
    void testMultipleValuesWithoutTrailingSemicolon();
    void testMultipleValuesWithEscapedTrailingSemicolon();
    void testCommentedLine();
    void testEqualSignValues();
    void testMultipleKeys();
    void testKeyStartingWithWhitespaces();
    void testInvalidKeys();
    void testMultipleGroupNames();
    void testGroupNameLocation();
    void testInvalidGroupNames();
};
#endif
