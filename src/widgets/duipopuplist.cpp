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

#include "duipopuplist.h"
#include "duipopuplist_p.h"

#include <QItemSelectionModel>
#include <QEventLoop>

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiScene>

#define MINBATCHSIZE 10

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiPopupList)

DuiPopupListPrivate::DuiPopupListPrivate()
    : itemModel(0), selectionModel(0)
{
}

DuiPopupListPrivate::~DuiPopupListPrivate()
{
}

DuiPopupList::DuiPopupList()
    : DuiDialog(new DuiPopupListPrivate(), new DuiPopupListModel(), DuiSceneWindow::PopupList)
{
    setCentralWidget(0);
}

DuiPopupList::DuiPopupList(DuiPopupListPrivate *dd, DuiPopupListModel *model, DuiSceneWindow::WindowType windowType)
    : DuiDialog(dd, model, windowType)
{
    setCentralWidget(0);
}

DuiPopupList::~DuiPopupList()
{
}

void DuiPopupList::setItemModel(QAbstractItemModel *itemModel)
{
    Q_D(DuiPopupList);

    if (d->itemModel == itemModel)
        return;

    d->itemModel = itemModel;

    emit itemModelChanged(d->itemModel);

    if (d->itemModel) {
        setSelectionModel(new QItemSelectionModel(d->itemModel, this));
    } else {
        setSelectionModel(0);
    }
}

QAbstractItemModel *DuiPopupList::itemModel() const
{
    Q_D(const DuiPopupList);
    return d->itemModel;
}

void DuiPopupList::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (selectionModel && selectionModel->model() != itemModel()) {
        qWarning("DuiPopupList::setSelectionModel() failed: "
                 "Trying to set a selection model, which works on "
                 "a different model than the view.");
        return;
    }

    Q_D(DuiPopupList);

    d->selectionModel = selectionModel;
    emit selectionModelChanged(selectionModel);
}

QItemSelectionModel *DuiPopupList::selectionModel() const
{
    Q_D(const DuiPopupList);
    return d->selectionModel;
}

int DuiPopupList::batchSize() const
{
    return model()->batchSize();
}

QModelIndex DuiPopupList::currentIndex() const
{
    Q_D(const DuiPopupList);
    if (!d->selectionModel || !d->selectionModel->hasSelection())
        return QModelIndex();

    return d->selectionModel->currentIndex();
}

void DuiPopupList::scrollTo(const QModelIndex &index)
{
    Q_D(DuiPopupList);
    if (index.model() != d->itemModel) return;
    emit scrollToIndex(index);
}

void DuiPopupList::click(const QModelIndex &index)
{
    Q_D(DuiPopupList);
    if (index.model() != d->itemModel) return;

    if (index == currentIndex()) {
        emit clicked(index);
    } else {
        d->selectionModel->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

        emit clicked(index);
        emit currentIndexChanged(index);
    }

    accept();
}

void DuiPopupList::setCurrentIndex(const QModelIndex &index)
{
    Q_D(DuiPopupList);
    if (d->itemModel == NULL)
        return;

    if (index.isValid() && d->selectionModel->model() != index.model()) {
        qWarning("DuiPopupList::selectItem() failed: "
                 "Trying to select an item that is for"
                 " a different model than the view ");
        return;
    }

    // Since we didn't change the current index, no new signals should not send
    if (index == currentIndex()) return;

    if (index.isValid())
        d->selectionModel->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    else
        d->selectionModel->clear();

    emit currentIndexChanged(index);
}

void DuiPopupList::setBatchSize(int size)
{
    if (size < MINBATCHSIZE) return;
    model()->setBatchSize(size);
}

