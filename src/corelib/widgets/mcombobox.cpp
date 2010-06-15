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

#include "mcombobox.h"
#include "mcombobox_p.h"

#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MComboBox)

MComboBoxPrivate::MComboBoxPrivate()
{
}

MComboBoxPrivate::~MComboBoxPrivate()
{
}

void MComboBoxPrivate::init()
{
    Q_Q(MComboBox);
    q->setItemModel(new QStandardItemModel(0, 1, q));
}

void MComboBoxPrivate::_q_modelDestroyed()
{
    Q_Q(MComboBox);
    q->model()->setItemModel(NULL);
}

void MComboBoxPrivate::_q_selectionModelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    Q_Q(MComboBox);
    if (!selected.isEmpty()) {
        QModelIndex current = selected.indexes().first();
        q->selectionModel()->setCurrentIndex(current, QItemSelectionModel::NoUpdate);
        emit q->currentIndexChanged(current.row());
        emit q->currentIndexChanged(q->itemText(current.row()));
    } else if (q->currentIndex() == -1) {
        emit q->currentIndexChanged(-1);
        emit q->currentIndexChanged(QString());
    }
}

void MComboBoxPrivate::_q_itemModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_Q(MComboBox);
    int curRow = q->currentIndex();
    if (curRow >= topLeft.row() && curRow <= bottomRight.row()) {
        emit q->currentIndexChanged(curRow);
        emit q->currentIndexChanged(q->itemText(curRow));
    }
}

void MComboBoxPrivate::_q_itemModelReset()
{
    Q_Q(MComboBox);
    emit q->currentIndexChanged(-1);
    emit q->currentIndexChanged(QString());
}

void MComboBoxPrivate::_q_itemClicked(const QModelIndex &index)
{
    Q_Q(MComboBox);

    int row = index.row();

    emit q->activated(row);
    emit q->activated(q->itemText(row));
}

MComboBox::MComboBox(QGraphicsItem *parent)
    : MWidgetController(new MComboBoxPrivate(), new MComboBoxModel(), parent)
{
    Q_D(MComboBox);
    d->init();
}

MComboBox::MComboBox(MComboBoxPrivate *dd, MComboBoxModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MComboBox);
    d->init();
}

MComboBox::~MComboBox()
{
    disconnect(model()->itemModel(), SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
}

void MComboBox::setItemModel(QAbstractItemModel *itemModel)
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
    connect(itemSelectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(_q_selectionModelSelectionChanged(QItemSelection, QItemSelection)));
}

QAbstractItemModel *MComboBox::itemModel() const
{
    if (model()->itemModel() == NULL) {
        MComboBox *that = const_cast<MComboBox *>(this);
        that->setItemModel(new QStandardItemModel(0, 1, that));
    }
    return model()->itemModel();
}

void MComboBox::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (selectionModel == NULL) return;
    if (selectionModel->model() != itemModel()) {
        qWarning("MComboBox::setSelectionModel() failed: "
                 "Trying to set a selection model, which works on "
                 "a different model than the view.");
        return;
    }

    model()->setSelectionModel(selectionModel);
}

QItemSelectionModel *MComboBox::selectionModel() const
{
    return model()->selectionModel();
}

QString MComboBox::iconID() const
{
    return model()->iconID();
}

bool MComboBox::isIconVisible() const
{
    return model()->iconVisible();
}

QString MComboBox::title() const
{
    return model()->title();
}

bool MComboBox::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

void MComboBox::addItem(const QString &text)
{
    insertItem(count(), text);
}

void MComboBox::addItem(const QString &iconID, const QString &text)
{
    insertItem(count(), iconID, text);
}

void MComboBox::addItems(const QStringList &texts)
{
    insertItems(count(), texts);
}

int MComboBox::count() const
{
    return model()->itemModel()->rowCount();
}

int MComboBox::currentIndex() const
{
    QItemSelectionModel *itemSelectionModel = model()->selectionModel();

    if (itemSelectionModel == NULL || !itemSelectionModel->hasSelection())
        return -1;

    return itemSelectionModel->currentIndex().row();
}

QString MComboBox::currentText() const
{
    int index = currentIndex();
    if (index != -1)
        return itemText(index);
    else
        return QString();
}

QString MComboBox::itemText(int index) const
{
    QAbstractItemModel *itemModel = model()->itemModel();

    QModelIndex mi = itemModel->index(index, 0);
    if (mi.isValid())
        return itemModel->data(mi, Qt::DisplayRole).toString();
    else
        return QString();
}

QString MComboBox::itemIconID(int index) const
{
    QAbstractItemModel *itemModel = model()->itemModel();

    QModelIndex mi = itemModel->index(index, 0);
    if (mi.isValid())
        return itemModel->data(mi, Qt::DecorationRole).toString();
    else
        return QString();
}

void MComboBox::insertItem(int index, const QString &text)
{
    insertItem(index, QString(), text);
}

void MComboBox::insertItem(int index, const QString &iconID, const QString &text)
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

void MComboBox::insertItems(int index, const QStringList &list)
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

void MComboBox::removeItem(int index)
{
    if (index >= 0 && index < count()) {
        QAbstractItemModel *itemModel = model()->itemModel();
        itemModel->removeRows(index, 1);
    }
}

void MComboBox::setItemText(int index, const QString &text)
{
    QAbstractItemModel *itemModel = model()->itemModel();
    QModelIndex item = itemModel->index(index, 0);
    if (item.isValid()) {
        itemModel->setData(item, text, Qt::DisplayRole);
    }
}

void MComboBox::setItemIconID(int index, const QString &iconID)
{
    QAbstractItemModel *itemModel = model()->itemModel();
    QModelIndex item = itemModel->index(index, 0);
    if (item.isValid()) {
        itemModel->setData(item, iconID, Qt::DecorationRole);
    }
}

void MComboBox::clear()
{
    QAbstractItemModel *itemModel = model()->itemModel();
    itemModel->removeRows(0, itemModel->rowCount());
}

void MComboBox::setIconID(const QString &id)
{
    model()->setIconID(id);
}

void MComboBox::setIconVisible(bool visible)
{
    model()->setIconVisible(visible);
}

void MComboBox::setTitle(const QString &title)
{
    model()->setTitle(title);
}

void MComboBox::setProgressIndicatorVisible(bool enable)
{
    model()->setProgressIndicatorVisible(enable);
}

void MComboBox::setCurrentIndex(int index)
{
    if (index == currentIndex()) return;

    QItemSelectionModel *itemSelectionModel = model()->selectionModel();
    if (itemSelectionModel == 0) return;

    QModelIndex item = model()->itemModel()->index(index, 0);

    if (item.isValid())
        itemSelectionModel->setCurrentIndex(item, QItemSelectionModel::ClearAndSelect);
    else
        itemSelectionModel->clear();
}

void MComboBox::click()
{
    emit clicked();
}

void MComboBox::dismiss()
{
    emit dismissed();
}

#include "moc_mcombobox.cpp"
