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

#ifndef UT_DUIBUTTON_H
#define UT_DUIBUTTON_H

#include <QtTest/QtTest>
#include <QObject>

#include <duibutton.h>

Q_DECLARE_METATYPE(DuiButton *);

class Ut_DuiButton : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstructionAndDestruction();
    void testIconID();
    void testToggledIconID();
    void testText();
    void testTextVisible();
    void testCheckable();
    void testIconVisible();
    void testPressedReleased();
    void testPressedCanceled();
    void testGroup();
    void testConstructorWithModel();

    void setView();

    void click();
    void toggle();
private:
    DuiButton *m_subject;
    DuiButton *m_tmp;
};

#endif
