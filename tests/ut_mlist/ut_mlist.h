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

#ifndef UT_MLIST_H
#define UT_MLIST_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/mlist class declaration
#include <mlist.h>
#include <mapplication.h>

Q_DECLARE_METATYPE(MList *);
Q_DECLARE_METATYPE(QModelIndex);

//MyIndexedModel class
class MyIndexedModel : public QAbstractItemModel
{
public:
    MyIndexedModel(QObject * root = NULL);
    virtual ~MyIndexedModel();

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QObject * root;
};

//ListTesterModel class
enum ListElementType {
    Button = 0,
    Label = 1,
    Image = 2
};

class ListTesterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ListTesterModel(QObject* parent=0);
    virtual ~ListTesterModel();


    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setListElementType(ListElementType elementType);

public slots:

private:
    ListElementType elementType;
    QList<MWidget*> widgets;
    QStringList theThumbnails;
    QStringList theHeaderData;
    QVector<QStringList> theData;

};

//ut_mlist class
class Ut_mlist : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testCreateListWithIndexedModel();
    void testCreateListWithListModel();
    void testHeaderCreator();
    void testScrollTo();
    void testIndexVisible();
    void testColums();
    void testSelectItem();
    void testAbstractModel();
    void testVisibleItem();
    void testLongTap();
    void testModelScrollTo();


private:
    MList *m_subject;
    MyIndexedModel* indexedModel;
    ListTesterModel* listTesterModel;
};

#endif
