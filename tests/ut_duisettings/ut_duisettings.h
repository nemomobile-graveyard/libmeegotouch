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

#ifndef UT_DUISETTINGS_H
#define UT_DUISETTINGS_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/DuiSettings class declaration
#include <duisettings.h>

Q_DECLARE_METATYPE(DuiSettings *);

class Ut_DuiSettings : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void globalSettings();

private:
    DuiSettings *m_subject;
};

#endif
