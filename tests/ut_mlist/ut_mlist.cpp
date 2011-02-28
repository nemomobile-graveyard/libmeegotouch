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

#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include <mlist.h>
#include <mcontentitem.h>
#include <mabstractcellcreator.h>
#include <mapplication.h>
#include <QAbstractListModel>

#include "ut_mlist.h"


MApplication *app;

class MContentItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
    MContentItemCreator()
    {
       destroyed = false;
    }

    ~MContentItemCreator()
    {
        destroyed = true;
    }

    void updateCell(const QModelIndex& index, MWidget * cell) const
    {
       MContentItem * contentItem = qobject_cast<MContentItem *>(cell);
        QVariant data = index.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        contentItem->boundingRect();
        contentItem->setItemMode(MContentItem::TopLeft);

        if (contentItem->title().isNull() || contentItem->title().isEmpty()) {
            contentItem->setTitle(rowData[0]);
        }

        if (contentItem->subtitle().isNull() || contentItem->subtitle().isEmpty()) {
            contentItem->setSubtitle(rowData[0]);
        }

        if ( contentItem->pixmap().isNull() ) {
            contentItem->setPixmap( QPixmap("icon-home"));
        }
    }
    static bool destroyed;
};

bool MContentItemCreator::destroyed = false;

//ut_mlist class
void Ut_mlist::init()
{
    m_subject = new MList();
    qRegisterMetaType<QModelIndex>("QModelIndex");
}

void Ut_mlist::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_mlist::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mlist" };
    app = new MApplication(argc, app_name);
}

void Ut_mlist::cleanupTestCase()
{
        delete app;
}

void Ut_mlist::testCreateListWithIndexedModel()
{
    indexedModel = new MyIndexedModel;
    m_subject->setItemModel(indexedModel);

    QCOMPARE( m_subject->itemModel(), indexedModel );

    delete indexedModel;
}

void Ut_mlist::testCreateListWithListModel()
{
    MContentItemCreator * cellCreator = new MContentItemCreator();
    m_subject->setCellCreator(cellCreator);
    listTesterModel = new ListTesterModel( this );
    m_subject->setShowGroups(false);
    m_subject->setItemModel(listTesterModel);

    listTesterModel->insertRow(0);

    QCOMPARE( m_subject->itemModel(), listTesterModel );
    QVERIFY( m_subject->showGroups() == false );
    QCOMPARE (m_subject->cellCreator(), cellCreator);

    delete listTesterModel;
}

void Ut_mlist::testHeaderCreator()
{
    MContentItemCreator * cellCreator = new MContentItemCreator();
    m_subject->setHeaderCreator(cellCreator);
    listTesterModel = new ListTesterModel();
    m_subject->setItemModel(listTesterModel);

    QCOMPARE( m_subject->itemModel(), listTesterModel );
    QCOMPARE (m_subject->headerCreator(), cellCreator);

    delete m_subject;
    m_subject = 0;

    QVERIFY(MContentItemCreator::destroyed);
}

void Ut_mlist::testScrollTo()
{
    indexedModel = new MyIndexedModel;
    m_subject->setShowGroups(true);
    m_subject->setItemModel(indexedModel);
    m_subject->setSelectionMode(MList::SingleSelection);

    QCOMPARE( m_subject->itemModel(), indexedModel );

    QModelIndex index = indexedModel->index(2, 0);

    QSignalSpy spy(m_subject, SIGNAL(scrollToIndex(QModelIndex)));

    m_subject->scrollTo(index); //triggering the signal

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QModelIndex result = qvariant_cast<QModelIndex>(spy.at(0).at(0));

    QCOMPARE( result, index );

    delete indexedModel;
}

void Ut_mlist::testIndexVisible()
{
    indexedModel = new MyIndexedModel;
    m_subject->setItemModel(indexedModel);
    m_subject->setIndexVisible( true );

    QCOMPARE( m_subject->itemModel(), indexedModel );
    QVERIFY( m_subject->indexVisible() == true );

    m_subject->setIndexVisible( false );
    QVERIFY( m_subject->indexVisible() == false );

    delete indexedModel;
}

void Ut_mlist::testColums()
{
    indexedModel = new MyIndexedModel;
    m_subject->setItemModel(indexedModel);
    QCOMPARE( m_subject->itemModel(), indexedModel );

    QVERIFY( m_subject->columns() == 1 );
    m_subject->setColumns(5);
    QVERIFY( m_subject->columns() == 5 );

    delete indexedModel;
}

void Ut_mlist::testSelectItem()
{

    indexedModel = new MyIndexedModel;
    m_subject->setItemModel(indexedModel);

    QCOMPARE( m_subject->itemModel(), indexedModel );

    m_subject->setSelectionMode(MList::SingleSelection);
    QVERIFY( m_subject->selectionMode() == MList::SingleSelection );

    QAbstractItemModel *model = (QAbstractItemModel*) m_subject->itemModel();
    QModelIndex index = model->index(1, 0);
    m_subject->selectItem(index);

    QVERIFY( m_subject->selectionModel()->hasSelection() == true );
    QVERIFY(m_subject->selectionModel()->isSelected(index) == true);

    delete indexedModel;
}

void Ut_mlist::testAbstractModel()
{

    indexedModel = new MyIndexedModel;
    m_subject->setItemModel(indexedModel);

    QAbstractItemModel *model = (QAbstractItemModel*) m_subject->itemModel();
    QModelIndex index = model->index(1, 0);
    m_subject->selectItem(index);

    QVERIFY( model->columnCount() == 1);
    QCOMPARE( model, indexedModel );

    delete indexedModel;
}

void Ut_mlist::testVisibleItem()
{
    indexedModel = new MyIndexedModel;
    m_subject->setItemModel(indexedModel);
    m_subject->setSelectionMode(MList::SingleSelection);

    QVERIFY( m_subject->indexVisible() == false );
    m_subject->setIndexVisible( true );
    QVERIFY( m_subject->indexVisible() == true );

    QModelIndex indexFirst = indexedModel->index(2, 0);
    QModelIndex indexLast = indexedModel->index(3, 1);
    m_subject->model()->setFirstVisibleItem( indexFirst );
    m_subject->model()->setLastVisibleItem( indexLast );

    QVERIFY( m_subject->firstVisibleItem() == indexFirst );
    QVERIFY( m_subject->lastVisibleItem() == indexLast );

    delete indexedModel;
}

void Ut_mlist::testLongTap()
{
    indexedModel = new MyIndexedModel;
    m_subject->setShowGroups(true);
    m_subject->setItemModel(indexedModel);
    m_subject->setSelectionMode(MList::SingleSelection);

    QCOMPARE( m_subject->itemModel(), indexedModel );

    QModelIndex index = indexedModel->index(2, 0);

    QSignalSpy spy(m_subject, SIGNAL(itemLongTapped(QModelIndex)));

    m_subject->longTapItem(index); //triggering the signal

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QModelIndex result = qvariant_cast<QModelIndex>(spy.at(0).at(0));

    QCOMPARE( result, index );

    delete indexedModel;
}


void Ut_mlist::testModelScrollTo()
{
    indexedModel = new MyIndexedModel;
    m_subject->setShowGroups(true);
    m_subject->setItemModel(indexedModel);
    m_subject->setSelectionMode(MList::SingleSelection);

    QCOMPARE( m_subject->itemModel(), indexedModel );

    QModelIndex index = indexedModel->index(2, 0);

    m_subject->model()->setScrollToIndex( index );

    QModelIndex result =  m_subject->model()->scrollToIndex();

    QCOMPARE( index, result );

}

// MyIndexedModel - indexed model
MyIndexedModel::MyIndexedModel(QObject * r) : root(r)
{
    root = new QObject;

    QObject * itemA = new QObject(root);
    itemA->setObjectName("A");

    QObject * item;
    item = new QObject(itemA);
    item->setObjectName("A1");

    item = new QObject(itemA);
    item->setObjectName("A2");

    item = new QObject(itemA);
    item->setObjectName("A3");

    QObject * itemB = new QObject(root);
    itemB->setObjectName("B1");

    item = new QObject(itemB);
    item->setObjectName("B2");

    item = new QObject(itemB);
    item->setObjectName("B3");

    item = new QObject(itemB);
    item->setObjectName("B4");

    item = new QObject(itemB);
    item->setObjectName("B5");

    QObject * itemC = new QObject(root);
    itemC->setObjectName("C1");

    item = new QObject(itemC);
    item->setObjectName("C2");
}

MyIndexedModel::~MyIndexedModel()
{
    delete root;
}

QModelIndex MyIndexedModel::index(int row, int column, const QModelIndex &parent) const
{
    QObject * parentObject;

    if(!parent.isValid())
        parentObject = root;
    else
        parentObject = static_cast<QObject *>(parent.internalPointer());

    if(row >= 0 && row < parentObject->children().size())
        return createIndex(row, column, parentObject->children().at(row));
    else
        return QModelIndex();
}

QModelIndex MyIndexedModel::parent(const QModelIndex &child) const
{
    if(child == QModelIndex() || !child.isValid())
        return QModelIndex();

    QObject * object = static_cast<QObject *>(child.internalPointer());
    QObject * parent = object->parent();

    if(parent == root)
        return QModelIndex();

    QObject * grandParent = parent->parent();

    return createIndex(grandParent->children().indexOf(parent), 0, parent);
}

int MyIndexedModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return root->children().size();

    QObject * parentObject = static_cast<QObject *>(parent.internalPointer());
    return parentObject->children().size();
}

int MyIndexedModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant MyIndexedModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
        return (static_cast<QObject *>(index.internalPointer()))->objectName();

    return QVariant();
}


//ListTesterModel class
ListTesterModel::ListTesterModel(QObject* parent)
: QAbstractListModel(parent),
  elementType(Button)
{
    theHeaderData.append("Title");
    theHeaderData.append("Subtitle");
    theHeaderData.append("Thumbnail");
}

ListTesterModel::~ListTesterModel()
{
    removeRows(0, rowCount());
}

void ListTesterModel::setListElementType(ListElementType type)
{
    elementType = type;
}

bool ListTesterModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row < 0)
    return false;
    beginInsertRows(QModelIndex(), row, row + count - 1);
    QStringList listNames;
    int firstIndex = theData.size();
    for(int i = 0; i < count; ++i) {
        theData.insert(row+i, QStringList() << (QString::number(firstIndex++)));
        endInsertRows();
    }
    return true;
}

/* It removes from index row and count represents the index till which you want ot remove
   for e.g row =1 and count =4  removes  means from index 1 till index 3 and index 4 will be there
   so if we have 1,2,3,4 and 5 then 1 and 5 should be present as index starts from 0 */
bool ListTesterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(parent, row, row + count - 1);
    theData.remove(row, count-row);
    endRemoveRows();
    return true;
}

int ListTesterModel::rowCount(const QModelIndex& parent) const {
    if(parent.isValid())
        return 0; //with a flat list groupheaders have no child items
    return theData.size();
}

int ListTesterModel::columnCount ( const QModelIndex & parent ) const {
    Q_UNUSED(parent);
    return theHeaderData.size();
}

QVariant ListTesterModel::data(const QModelIndex& index, int role) const
{


   if (role == Qt::DisplayRole) {
        if(index.isValid() && index.row() < theData.size()) {
            QStringList rowData = theData[index.row()];
            return QVariant(rowData);
        }
    }
   return QVariant();
}

QVariant ListTesterModel::headerData(int section, Qt::Orientation orientation, int role) const {
    Q_UNUSED(orientation);
    if (role == Qt::DisplayRole) {
        if (section < theHeaderData.size()) {
            return QVariant(theHeaderData.at(section));
        }
    }
    return QVariant();
}

QTEST_APPLESS_MAIN(Ut_mlist)
