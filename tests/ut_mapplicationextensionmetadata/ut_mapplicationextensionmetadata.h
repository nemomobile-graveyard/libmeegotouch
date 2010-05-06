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

#ifndef UT_MAPPLICATIONEXTENSIONMETADATA_H
#define UT_MAPPLICATIONEXTENSIONMETADATA_H

#include <QObject>

class MApplicationExtensionMetaData;

class Ut_MApplicationExtensionMetaData : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testValidMetaData();
    void testBinaryMissing();
    void testBinaryAndRunnerPaths();
    void testInvalidRunner();
    void testInvalidExtension();
    void testExtensionMissing();
    void testInterfaceMissing();
    void testNameMissing();
    void testExtraIdentifier();
    void testResourceIdentifier();

private:
    MApplicationExtensionMetaData *m_subject;
};

#endif
