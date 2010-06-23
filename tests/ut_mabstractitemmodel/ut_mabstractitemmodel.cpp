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
 
#include "ut_mabstractitemmodel.h"

#include <MAbstractItemModel>

#include <QtTest/QtTest>

class Ut_TestModel : public MAbstractItemModel
{
public:
    Ut_TestModel() 
        : MAbstractItemModel() 
    {
        groupTitles << "Group 1";
        groupTitles << "Group 2";
        groupTitles << "Group 3";
        groupTitles << "Group 4";
        groupTitles << "Group 5";
        
        items[0] << "Item 1.1";
        items[0] << "Item 1.2";
        items[0] << "Item 1.3";
        items[0] << "Item 1.4";

        items[1] << "Item 2.1";
        items[1] << "Item 2.2";
        items[1] << "Item 2.3";
        items[1] << "Item 2.4";
        items[1] << "Item 2.5";
        
        items[2] << "Item 3.1";
        items[2] << "Item 3.2";
        items[2] << "Item 3.3";
        
        items[3] << "Item 4.1";
        items[3] << "Item 4.2";
        items[3] << "Item 4.3";
        items[3] << "Item 4.4";
        items[3] << "Item 4.5";
        items[3] << "Item 4.6";
        
        items[4] << "Item 5.1";
        items[4] << "Item 5.2";
        items[4] << "Item 5.3";
        items[4] << "Item 5.4";       
    }
    
    virtual ~Ut_TestModel() {}
    
    int groupCount() const 
    {
        return groupTitles.count();
    }
    
    int rowCountInGroup(int group = -1) const
    {
        if (!isGrouped()) {
            int totalCount = 0;
            for (int i = 0; i < items.count(); i++) {
                totalCount += items[i].count();
            }
            return totalCount;
        }
        return items[group].count();
    }
    
    QString groupTitle(int group) const
    {
        return groupTitles[group];
    }
    
    QVariant itemData(int row, int group, int role = Qt::DisplayRole) const
    {
        if (role == Qt::DisplayRole) {
            if (isGrouped()) {
                return items[group][row];
            } else {
                group = 0;
                int groupSize = items[0].count();
                while (row >= groupSize) {
                    group++;
                    row -= groupSize;
                    groupSize = items[group].count();
                }
                return items[group].at(row);
            }
        }
        
        return QVariant();
    }
    
private:
    QStringList groupTitles;
    QMap<int, QStringList> items;
};

Ut_MAbstractItemModel::Ut_MAbstractItemModel()
    : model( 0 )
{
}

void Ut_MAbstractItemModel::initTestCase()
{    
}

void Ut_MAbstractItemModel::cleanupTestCase()
{
}

void Ut_MAbstractItemModel::init()
{
    model = new Ut_TestModel;
}

void Ut_MAbstractItemModel::cleanup()
{
    delete model;    
}

void Ut_MAbstractItemModel::testFlatRowCount()
{
    QCOMPARE(model->rowCountInGroup(), 22);
}

void Ut_MAbstractItemModel::testGroupCount()
{
    model->setGrouped(true);
    QCOMPARE(model->groupCount(), 5);
}

void Ut_MAbstractItemModel::testGroup1Count()
{
    model->setGrouped(true);
    QCOMPARE(model->rowCountInGroup(1), 5);
}

void Ut_MAbstractItemModel::testGroup3Count()
{
    model->setGrouped(true);
    QCOMPARE(model->rowCountInGroup(3), 6);    
}

void Ut_MAbstractItemModel::testGroup4Count()
{
    model->setGrouped(true);
    QCOMPARE(model->rowCountInGroup(4), 4);
}

void Ut_MAbstractItemModel::testItemAtFlatRow3()
{
    QCOMPARE(model->itemData(3, -1).toString(), QString("Item 1.4"));
}

void Ut_MAbstractItemModel::testItemAtFlatRow6()
{
    QCOMPARE(model->itemData(6, -1).toString(), QString("Item 2.3"));
}

void Ut_MAbstractItemModel::testItemAtFlatRow9()
{
    QCOMPARE(model->itemData(9, -1).toString(), QString("Item 3.1"));
}

void Ut_MAbstractItemModel::testItemAtFlatRow15()
{
    QCOMPARE(model->itemData(15, -1).toString(), QString("Item 4.4"));
}

void Ut_MAbstractItemModel::testItemAtRow3InGroup0()
{
    model->setGrouped(true);
    QCOMPARE(model->itemData(3, 0).toString(), QString("Item 1.4"));
}

void Ut_MAbstractItemModel::testItemAtRow2InGroup1()
{
    model->setGrouped(true);
    QCOMPARE(model->itemData(2, 1).toString(), QString("Item 2.3"));
}

void Ut_MAbstractItemModel::testItemAtRow5InGroup3()
{
    model->setGrouped(true);
    QCOMPARE(model->itemData(5, 3).toString(), QString("Item 4.6"));
}

void Ut_MAbstractItemModel::testItemAtRow0InGroup4()
{
    model->setGrouped(true);
    QCOMPARE(model->itemData(0, 4).toString(), QString("Item 5.1"));
}

void Ut_MAbstractItemModel::testGroup0Title()
{
    model->setGrouped(true);
    QCOMPARE(model->groupTitle(0), QString("Group 1"));
}

void Ut_MAbstractItemModel::testGroup3Title()
{
    model->setGrouped(true);
    QCOMPARE(model->groupTitle(3), QString("Group 4"));
}

void Ut_MAbstractItemModel::testGroup4Title()
{
    model->setGrouped(true);
    QCOMPARE(model->groupTitle(4), QString("Group 5"));
}


QTEST_APPLESS_MAIN(Ut_MAbstractItemModel);
