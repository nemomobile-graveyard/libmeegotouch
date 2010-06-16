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

#ifndef UT_SERVICEMAPPER_H
#define UT_SERVICEMAPPER_H

#include <QtTest/QtTest>
#include <QObject>

#include <mservicemapper.h>

Q_DECLARE_METATYPE(MServiceMapper *);

class Ut_MServiceMapper : public QObject
{
    Q_OBJECT

public:
    static const int initFiles = 2;
    static int m_noFiles;
    static QTextStream m_globalDummy;
    static int m_lineNo;
    static int m_fileNo;
    static QString m_lines[4][2];
    static QString m_services[3];
    static QString m_interfaces[3];
    static QStringList m_currentServiceFileList;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void checkServiceNamesNoIf();
    void checkServiceNamesIf();
    void checkServiceName();
    void checkServicePath();
    void checkServiceNameBadIf();
    void checkInterfaceName_data();
    void checkInterfaceName();
    void checkHandleServiceChangedAddLastFile();
    void checkHandleServiceChangedRmLastFile();
    void checkHandleServiceChangedAddAndRemoveFiles();

private:
    MServiceMapper *m_subject;
    void fillServiceFileList( int noFiles );

};
#endif
