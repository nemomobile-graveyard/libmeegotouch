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

#ifndef UT_DUITEXTEDIT_H
#define UT_DUITEXTEDIT_H

#include <QtTest>
#include <QObject>

class DuiApplication;
class DuiApplicationWindow;
class DuiTextEdit;
class DuiCompleter;
class CompleterTestModel;

Q_DECLARE_METATYPE(DuiCompleter *);

class Ut_DuiCompleter : public QObject
{
    Q_OBJECT

private:
    DuiTextEdit *m_editWidget;
    DuiCompleter *m_subject;
    DuiApplication *m_app;
    DuiApplicationWindow *m_window;
    CompleterTestModel *testModel;
    QStringList modelColumn1, modelColumn2;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void checkConstruct();

    void checkWidget();

    void checkPrefix();

    void checkCandidates();

    void checkMultipleEntries();

    void checkComplete();

    void checkTrim();
};

#endif

