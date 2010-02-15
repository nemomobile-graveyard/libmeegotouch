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

#ifndef UT_SERVICEMAPPER_H
#define UT_SERVICEMAPPER_H

#include <QtTest/QtTest>
#include <QObject>

#include <duiservicemapper.h>

Q_DECLARE_METATYPE(DuiServiceMapper *);

class Ut_DuiServiceMapper : public QObject
{
    Q_OBJECT

public:
    static const int noAllFiles = 3;
    static int noFiles;
    static QTextStream globalDummy;
    static int lineNo;
    static int fileNo;
    static QString lines[4][2];
    static QString services[3];
    static QString interfaces[3];

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void checkHandleServiceChangedRmLastFile();
    void checkServiceNamesNoIf();
    void checkServiceNamesIf();
    void checkServiceName();
    void checkServicePath();
    void checkHandleServiceChangedAddLastFile();
    void checkServiceNameBadIf();

private:
    DuiServiceMapper *m_subject;
    void mkFiles(int noFiles);

};
#endif
