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

#ifndef UT_MRICHTEXTEDIT_H
#define UT_MRICHTEXTEDIT_H

#include <QtTest>
#include <QObject>

class MApplication;
class MApplicationWindow;
class MRichTextEdit;

Q_DECLARE_METATYPE(MRichTextEdit *);

class Ut_MRichTextEdit : public QObject
{
    Q_OBJECT

private:
    MRichTextEdit *m_subject;
    MApplication *m_app;
    MApplicationWindow *m_appWindow;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSetFontBold();
    void testSetFontItalic();
    void testSetFontUnderline();
    void testCopyPaste();
    void testCut();
    void testStyleOnPreedit();
    void testSetHtml();
};

#endif

