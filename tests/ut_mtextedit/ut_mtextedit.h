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

#ifndef UT_MTEXTEDIT_H
#define UT_MTEXTEDIT_H

#include <QtTest>
#include <QObject>

class MApplication;
class MApplicationWindow;
class MTextEdit;

Q_DECLARE_METATYPE(MTextEdit *);

class Ut_MTextEdit : public QObject
{
    Q_OBJECT

private:
    void confirmKeyEventIgnored(MTextEdit *subject, int key, int expectedReturnPressed);
    void constraintTest(MTextEdit *subject, const QString &input, const QString &expectedOutput);

    MTextEdit *m_subject;
    MApplication *m_app;
    MApplicationWindow *m_appWindow;

    QStringList validStrings;
    static const QString testString;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testSetText();
    void testInsert();
    void testKeyPressEvent();
    void testSingleLineKeyPressEvent();
    void testFocusInEvent();
    void testFocusOutEvent();
    void testInputMethodEvent();
    void testSetCursorPosition();
    void testSetTextCursor();
    void testTextInteractionFlags();
    void testConstraints();
    void testReadOnly();
    void testMaxLength_data();
    void testMaxLength();
    //void testFeedback();
    void testBadData();
    void testSelection();
    void testAutoSelection();
    void testPrompt();
    void testValidator();
    void testClear();
    void testCursorPositionChanged();
    void testCopyPaste();
    void testPasteOnPreedit();
    void testCut_data();
    void testCut();
    void testInputMethodQuery();
    void testConstrainedPaste();

    void testSignChange_data();
    void testSignChange();

    void testSetContentType_data();
    void testSetContentType();

    void testInputMethodHints();

    void testAttachToolbar();

    void testPasswordEchoOnEditClearing();

    void testReturnKeyPressed_data();
    void testReturnKeyPressed();

    void testLineBreakSent_data();
    void testLineBreakSent();

    void testCommitLineBreakAfterPreedit_data();
    void testCommitLineBreakAfterPreedit();
};

#endif

