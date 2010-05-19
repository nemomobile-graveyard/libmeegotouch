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
 

#ifndef UT_MABSTRACTITEMMODEL_H
#define UT_MABSTRACTITEMMODEL_H

#include <QObject>

class Ut_TestModel;

class Ut_MAbstractItemModel : public QObject
{
    Q_OBJECT
public:
    Ut_MAbstractItemModel();
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testFlatRowCount();
    void testGroupCount();
    void testGroup1Count();
    void testGroup3Count();
    void testGroup4Count();
    
    void testItemAtFlatRow3();
    void testItemAtFlatRow6();
    void testItemAtFlatRow9();
    void testItemAtFlatRow15();
    
    void testItemAtRow3InGroup0();
    void testItemAtRow2InGroup1();
    void testItemAtRow5InGroup3();
    void testItemAtRow0InGroup4();
    
    void testGroup0Title();
    void testGroup3Title();
    void testGroup4Title();
    
private:
    Ut_TestModel *model;
};

#endif // UT_MABSTRACTITEMMODEL_H
