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

#include "duicombobox.h"
#include "duicombobox_p.h"
#include "duilistnamespace.h"

#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiComboBox)

DuiComboBoxPrivate::DuiComboBoxPrivate()
{
}

DuiComboBoxPrivate::~DuiComboBoxPrivate()
{
}

void DuiComboBoxPrivate::init()
{
    Q_Q(DuiComboBox);
    q->setItemModel(new QStandardItemModel(0, 1, q));
}

void DuiComboBoxPrivate::_q_modelDestroyed()
{
    Q_Q(DuiComboBox);
    q->model()->setItemModel(duiEmptyModel());
}

void DuiComboBoxPrivate::_q_selectionModelCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    Q_Q(DuiComboBox);
    emit q->currentIndexChanged(current.row());
    emit q->currentIndexChanged(q->itemText(current.row()));
}

void DuiComboBoxPrivate::_q_selectionModelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    Q_Q(DuiComboBox);
    if (selected.isEmpty()) {
        emit q->currentIndexChanged(-1);
        emit q->currentIndexChanged(QString());
    }
}

void DuiComboBoxPrivate::_q_itemModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_Q(DuiComboBox);
    int curRow = q->currentIndex();
    if (curRow == -1) {
        emit q->currentIndexChanged(-1);
        emit q->currentIndexChanged(QString());
    } else if (curRow >= topLeft.row() && curRow <= bottomRight.row()) {
        emit q->currentIndexChanged(curRow);
        emit q->currentIndexChanged(q->itemText(curRow));
    }
}

void DuiComboBoxPrivate::_q_itemModelReset()
{
    Q_Q(DuiComboBox);
    emit q->currentIndexChanged(-1);
    emit q->currentIndexChanged(QString());
}

void DuiComboBoxPrivate::_q_itemClicked(const QModelIndex &index)
{
    Q_Q(DuiComboBox);

    int row = index.row();

    emit q->activated(row);
    emit q->activated(q->itemText(row));
}


DuiComboBox::DuiComboBox(QGraphicsItem *parent)
    : DuiWidgetController(new DuiComboBoxPrivate(), new DuiComboBoxModel(), parent)
{
    Q_D(DuiComboBox);
    d->init();
}

DuiComboBox::DuiComboBox(DuiComboBoxPrivate *dd, DuiComboBoxModel *model, QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
    Q_D(DuiComboBox);
    d->init();
}

DuiComboBox::~DuiComboBox()
{
    disconnect(model()->itemModel(), SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
}

void DuiComboBox::setItemModel(QAbstractItemModel *itemModel)
{
    QAbstractItemModel *oldModel = model()->itemModel();
    if (itemModel == NULL || oldModel == itemModel)
        return;

    QItemSelectionModel *oldSelModel = model()->selectionModel();

    if (oldSelModel)
        oldSelModel->clear();

    if (oldModel) {
        disconnect(oldModel, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        disconnect(oldModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(_q_itemModelDataChanged(QModelIndex, QModelIndex)));
        disconnect(oldModel, SIGNAL(modelReset()), this, SLOT(_q_itemModelReset()));
    }

    QItemSelectionModel *itemSelectionModel = new QItemSelectionModel(itemModel, this);

    model()->setItemModel(itemModel);
    model()->setSelectionModel(itemSelectionModel);

    if (oldSelModel && oldSelModel->QObject::parent() == this)
        delete oldSelModel;
    if (oldModel && oldModel->QObject::parent() == this)
        delete oldModel;


    connect(itemModel, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
    connect(itemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(_q_itemModelDataChanged(QModelIndex, QModelIndex)));
    connect(itemModel, SIGNAL(modelReset()), this, SLOT(_q_itemModelReset()));
    connect(itemSelectionModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(_q_selectionModelCurrentChanged(QModelIndex, QModelIndex)));
    connect(itemSelectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(_q_selectionModelSelectionChanged(QItemSelection, QItemSelection)));
}

QAbstractItemModel *DuiComboBox::itemModel() const
{
    QAbstractItemModel *itemModel = model()->itemModel();
    if (itemModel == duiEmptyModel()) {
        DuiComboBox *that = const_cast<DuiComboBox *>(this);
        that->setItemModel(new QStandardItemModel(0, 1, that));
    }

    return itemModel;
}

void DuiComboBox::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (selectionModel == NULL) return;
    if (selectionModel->model() != itemModel()) {
        qWarning("DuiComboBox::setSelectionModel() failed: "
                 "Trying to set a selection model, which works on "
                 "a different model than the view.");
        return;
    }

    model()->setSelectionModel(selectionModel);
}

QItemSelectionModel *DuiComboBox::selectionModel() const
{
    return model()->selectionModel();
}

QString DuiComboBox::iconID() const
{
    return model()->iconID();
}

bool DuiComboBox::isIconVisible() const
{
    return model()->iconVisible();
}

QString DuiComboBox::title() const
{
    return model()->title();
}

void DuiComboBox::addItem(const QString &text)
{
    insertItem(count(), text);
}

void DuiComboBox::addItem(const QString &iconID, const QString &text)
{
    insertItem(count(), iconID, text);
}

void DuiComboBox::addItems(const QStringList &texts)
{
    insertItems(count(), texts);
}

int DuiComboBox::count() const
{
    return model()->itemModel()->rowCount();
}

int DuiComboBox::currentIndex() const
{
    QItemSelectionModel *itemSelectionModel = model()->selectionModel();

    if (itemSelectionModel == NULL || !itemSelectionModel->hasSelection())
        return -1;

    return itemSelectionModel->currentIndex().row();
}

QString DuiComboBox::currentText() const
{
    int index = currentIndex();
    if (index != -1)
        return itemText(index);
    else
        return QString();
}

QString DuiComboBox::itemText(int index) const
{
    QAbstractItemModel *itemModel = model()->itemModel();

    QModelIndex mi = itemModel->index(index, 0);
    if (mi.isValid())
        return itemModel->data(mi, Qt::DisplayRole).toString();
    else
        return QString();
}

QString DuiComboBox::itemIconID(int index) const
{
    QAbstractItemModel *itemModel = model()->itemModel();

    QModelIndex mi = itemModel->index(index, 0);
    if (mi.isValid())
        return itemModel->data(mi, Qt::DecorationRole).toString();
    else
        return QString();
}

bool DuiComboBox::isDown() const
{
    return model()->down();
}

void DuiComboBox::setDown(bool status)
{
    model()->setDown(status);
}

void DuiComboBox::insertItem(int index, const QString &text)
{
    insertItem(index, QString(), text);
}

void DuiComboBox::insertItem(int index, const QString &iconID, const QString &text)
{
    int itemCount = count();
    index = qBound(0, index, itemCount);

    // For the common case where we are using the built in QStandardItemModel
    // construct a QStandardItem, reducing the number of expensive signals from the model
    QAbstractItemModel *itemModel = model()->itemModel();

    if (QStandardItemModel *m = qobject_cast<QStandardItemModel *>(itemModel)) {
        QStandardItem *item = new QStandardItem(text);
        if (!iconID.isNull()) item->setData(iconID, Qt::DecorationRole);
        m->insertRow(index, item);
    } else {
        if (itemModel->insertRows(index, 1)) {
            QModelIndex item = itemModel->index(index, 0);
            itemModel->setData(item, text, Qt::DisplayRole);

            if (!iconID.isEmpty())
                itemModel->setData(item, iconID, Qt::DecorationRole);
        }
    }
}

void DuiComboBox::insertItems(int index, const QStringList &list)
{
    if (list.isEmpty())
        return;

    index = qBound(0, index, count());
    int insertCount = list.count();

    // For the common case where we are using the built in QStandardItemModel
    // construct a QStandardItem, reducing the number of expensive signals from the model
    QAbstractItemModel *itemModel = model()->itemModel();

    if (QStandardItemModel *m = qobject_cast<QStandardItemModel *>(itemModel)) {

        QList<QStandardItem *> items;
        QStandardItem *hiddenRoot = m->invisibleRootItem();
        for (int i = 0; i < insertCount; ++i)
            items.append(new QStandardItem(list.at(i)));
        hiddenRoot->insertRows(index, items);

    } else {
        if (itemModel->insertRows(index, insertCount)) {
            QModelIndex item;
            for (int i = 0; i < insertCount; ++i) {
                item = itemModel->index(i + index, 0);
                itemModel->setData(item, list.at(i), Qt::DisplayRole);
            }
        }
    }
}

void DuiComboBox::removeItem(int index)
{
    if (index >= 0 && index < count()) {
        QAbstractItemModel *itemModel = model()->itemModel();
        itemModel->removeRows(index, 1);
    }
}

void DuiComboBox::setItemText(int index, const QString &text)
{
    QAbstractItemModel *itemModel = model()->itemModel();
    QModelIndex item = itemModel->index(index, 0);
    if (item.isValid()) {
        itemModel->setData(item, text, Qt::DisplayRole);
    }
}

void DuiComboBox::setItemIconID(int index, const QString &iconID)
{
    QAbstractItemModel *itemModel = model()->itemModel();
    QModelIndex item = itemModel->index(index, 0);
    if (item.isValid()) {
        itemModel->setData(item, iconID, Qt::DecorationRole);
    }
}

void DuiComboBox::clear()
{
    QAbstractItemModel *itemModel = model()->itemModel();
    itemModel->removeRows(0, itemModel->rowCount());
}

void DuiComboBox::setIconID(const QString &id)
{
    model()->setIconID(id);
}

void DuiComboBox::setIconVisible(bool visible)
{
    model()->setIconVisible(visible);
}

void DuiComboBox::setTitle(const QString &title)
{
    model()->setTitle(title);
}

void DuiComboBox::setCurrentIndex(int index)
{
    if (index == currentIndex()) return;

    QItemSelectionModel *itemSelectionModel = model()->selectionModel();
    if (itemSelectionModel == 0) return;

    QModelIndex item = model()->itemModel()->index(index, 0);

    if (item.isValid()) {
        itemSelectionModel->select(item, QItemSelectionModel::ClearAndSelect);
        itemSelectionModel->setCurrentIndex(item, QItemSelectionModel::Current);
    } else
        itemSelectionModel->clear();
}

void DuiComboBox::click()
{
    emit clicked();
}

#include "moc_duicombobox.cpp"
