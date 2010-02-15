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

#include "duipopuplistview.h"
#include "duipopuplistview_p.h"

#include "duipopuplist.h"
#include "duipopuplist_p.h"
#include "duilistnamespace.h"
#include "duipannableviewport.h"
#include "duitheme.h"
#include "duiviewcreator.h"
#include "duigriditem.h"
#include "duilabel.h"
#include "duiscenemanager.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsLayoutItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include <QTimer>

#include <QDebug>


DuiPopupListViewPrivate::DuiPopupListViewPrivate()
    : controller(0), viewport(0),
      layout(0), container(0), spacer(0),
      itemModel(0), selectionModel(0),
      itemDirty(0), singlePass(true), batchSize(0), gridItemMode(true),
      itemStartIndex(0), itemHeight(0), arrangingWidget(false)
{
}

DuiPopupListViewPrivate::~DuiPopupListViewPrivate()
{
}

void DuiPopupListViewPrivate::init()
{
    Q_Q(DuiPopupListView);

    viewport = q->contentsViewport();

    // container which contain the gridItems
    container = new QGraphicsWidget();
    q->contentsLayout()->insertItem(0, container);

    layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setOrientation(Qt::Vertical);
    container->setLayout(layout);

    // add spacer
    spacer = new QGraphicsWidget();
    layout->addItem(spacer);

    QObject::connect(controller, SIGNAL(itemModelChanged(QAbstractItemModel *)),
                     q, SLOT(setItemModel(QAbstractItemModel *)));
    QObject::connect(controller, SIGNAL(selectionModelChanged(QItemSelectionModel *)),
                     q, SLOT(setSelectionModel(QItemSelectionModel *)));
    QObject::connect(controller, SIGNAL(scrollToIndex(QModelIndex)),
                     q, SLOT(scrollTo(QModelIndex)));

    if (controller->itemModel() != NULL) {
        q->setItemModel(controller->itemModel());
        q->setSelectionModel(controller->selectionModel());
    }

}

void DuiPopupListViewPrivate::_q_modelDestroyed()
{
    itemModel = duiEmptyModel();
}

void DuiPopupListViewPrivate::_q_modelReset()
{
    doItemsLayout();
}

void DuiPopupListViewPrivate::_q_layoutChanged()
{
    doItemsLayout();
}

void DuiPopupListViewPrivate::_q_arrangeWidget()
{
    int i;
    int layoutItemCount = layout->count();
    int modelItemCount = itemModel->rowCount();

    qreal spacerHeight = layout->itemAt(0)->geometry().height();
    qreal sum = spacerHeight;

    QGraphicsLayoutItem *item;
    DuiWidget *widget;

    qreal y = pannedPos.y();

    if (y - lastPosition.y() >= 0) {
        // pan down

        // calculate viewport top skipped items
        int removeItemNumber = 0;
        qreal spacerAddHeight = 0;
        for (i = 1; i < layoutItemCount; ++i) {
            item = layout->itemAt(i);

            // save item Height
            if (itemHeight == 0)
                setViewportRange(item->geometry());

            if (sum + itemHeight >= y)
                break;

            removeItemNumber++;
            spacerAddHeight += itemHeight;
            sum += itemHeight;

            // stop moving canvas when batches overlap
            if (removeItemNumber >= modelItemCount - batchSize) break;
        }

        if (removeItemNumber > 0) {

            // append items to bottom, make widgetsList have batchSize items anytime
            int start = itemStartIndex + batchSize;
            if (start < modelItemCount) {

                // remove items from top
                for (i = removeItemNumber; i > 0; --i) {
                    layout->removeAt(i);

                    widget = widgetsList.takeFirst();
                    recycleWidget(widget);
                }

                setSpacerFixedHeight(spacerHeight + spacerAddHeight);
                itemStartIndex += removeItemNumber;

                for (i = 0; i < removeItemNumber; ++i) {
                    if (i + start >= modelItemCount) break;

                    QModelIndex index = itemModel->index(i + start, 0);
                    widget = buildItem(index);

                    layout->addItem(widget);
                    widgetsList.append(widget);
                }
            }
        }

    } else {
        // pan up
        y += viewportSize.height();

        // calculate viewport bottom skipped items
        int skip = 0;
        for (i = 1; i < layoutItemCount; ++i) {
            item = layout->itemAt(i);

            if (itemHeight == 0)
                setViewportRange(item->geometry());

            skip++;
            if (sum + itemHeight >= y)
                break;

            sum += itemHeight;
        }

        int removeItemNumber = layoutItemCount - 1 - skip;
        if (removeItemNumber > 0) {

            // insert items to top
            qreal spacerRemoveHeight = 0;
            int start = itemStartIndex - 1;
            for (i = 0; i < removeItemNumber; ++i) {
                if (start - i < 0) break;

                QModelIndex index = itemModel->index(start - i, 0);
                widget = buildItem(index);

                layout->insertItem(1, widget);
                widgetsList.prepend(widget);

                spacerRemoveHeight += itemHeight;
            }

            // remove items from bottom, make widgetsList have batchSize items anytime
            layoutItemCount = layout->count();
            for (i = layoutItemCount - 1; i > batchSize; --i) {
                layout->removeAt(i);
                widget = widgetsList.takeLast();
                recycleWidget(widget);
            }

            itemStartIndex -= removeItemNumber;
            if (itemStartIndex < 0)
                itemStartIndex = 0;

            if (spacerHeight - spacerRemoveHeight < 0)
                setSpacerFixedHeight(0);
            else
                setSpacerFixedHeight(spacerHeight - spacerRemoveHeight);
        }
    }

    lastPosition = pannedPos;
    arrangingWidget = false;
}

void DuiPopupListViewPrivate::setLayoutDirty()
{
    itemDirty = 1;
    viewport->setEnabled(false);
}

void DuiPopupListViewPrivate::setViewportRange(const QRectF &rect) const
{
    itemHeight = rect.height();
    viewport->setAutoRange(false);

    int modelItemCount = itemModel->rowCount();
    QRectF range = QRectF(0, 0, rect.width(), modelItemCount * itemHeight);

    viewport->setRange(range);
}

void DuiPopupListViewPrivate::setSpacerFixedHeight(qreal height) const
{
    if (spacer == 0) return;

    spacer->setMaximumHeight(height);
    spacer->setMinimumHeight(height);
}

void DuiPopupListViewPrivate::doItemsLayout() const
{
    Q_Q(const DuiPopupListView);

    // remove old items
    int i;
    DuiWidget *widget;
    for (i = layout->count() - 1; i > 0; --i)
        layout->removeAt(i);

    for (i = widgetsList.count() - 1; i >= 0; --i) {
        widget = widgetsList.takeAt(i);
        recycleWidget(widget);
    }
    setSpacerFixedHeight(0);
    gridItemMode = true;

    // decide item create mode
    int count = 0;
    if (itemModel)
        count = itemModel->rowCount();

    viewport->setAutoRange(true);
    itemHeight = 0;

    QObject::disconnect(viewport, SIGNAL(sizePosChanged(QSizeF, QRectF, QPointF)),
                        q, SLOT(sizePosChanged(QSizeF, QRectF, QPointF)));

    if (count <= q->model()->batchSize()) {
        singlePass = true;
        batchSize = count;
    } else {
        singlePass = false;
        batchSize = q->model()->batchSize();
        arrangingWidget = false;

        QObject::connect(viewport, SIGNAL(sizePosChanged(QSizeF, QRectF, QPointF)),
                         q, SLOT(sizePosChanged(QSizeF, QRectF, QPointF)));
    }

    // create first batch items
    if (itemModel) {
        for (i = 0; i < batchSize; ++i) {
            QModelIndex index = itemModel->index(i, 0);

            widget = buildItem(index);
            layout->addItem(widget);

            widgetsList.append(widget);
        }
    }

    lastPosition = QPointF(0, 0);
    viewport->setEnabled(true);
    viewport->setPosition(QPointF(0, 0));

    itemStartIndex = 0;
    itemDirty = 0;
}

DuiWidget *DuiPopupListViewPrivate::buildItem(const QModelIndex &index) const
{
    Q_UNUSED(index);
    QVariant itemType = itemModel->data(index, DuiListNameSpace::ItemTypeRole);
    // custom
    if (itemType != QVariant()) {
        gridItemMode = false;
        int i = itemType.toInt();
        if (i == DuiListNameSpace::Custom) {
            QVariant modelItem =  itemModel->data(index, Qt::DisplayRole);
            if (modelItem.canConvert<DuiWidget *>()) {
                DuiWidget *w = modelItem.value<DuiWidget *>();
                if (selectionModel->isSelected(index))
                    w->setSelected(true);
                else
                    w->setSelected(false);
                return w;
            }
        }
    }

    // griditem
    DuiGridItem *item;
    if (objectsPool.count() == 0) {
        item = new DuiGridItem();
    } else {
        item = (DuiGridItem *)(objectsPool.takeLast());
        item->show();
        item->setSelected(false);
    }

    QString title, image, subtitle;
    QVariant value;

    value = itemModel->data(index, Qt::DisplayRole);
    if (value != QVariant())
        title = value.toString();

    value = itemModel->data(index, Qt::DecorationRole);
    if (value != QVariant())
        image = value.toString();

    value = itemModel->data(index, DuiListNameSpace::SubtitleRole);
    if (value != QVariant())
        subtitle = value.toString();

    if (image.isEmpty()) {
        item->setTitle(title);
        item->setTitleVisible(true);
        item->setImageVisible(false);
        item->setSubtitleVisible(false);
    } else {
        item->setImage(image);
        item->setImageVisible(true);
        item->setTitle(title);
        item->setTitleVisible(true);
        item->setSubtitle(subtitle);
        item->setSubtitleVisible(true);

        if (title.isEmpty() && subtitle.isEmpty()) {
            item->setTitleVisible(false);
            item->setSubtitleVisible(false);
        } else if (subtitle.isEmpty()) {
            item->setSubtitleVisible(false);
        }
    }

    if (selectionModel->isSelected(index))
        item->setSelected(true);

    return item;
}

void DuiPopupListViewPrivate::recycleWidget(DuiWidget *widget) const
{
    // Only provide recycle when use DuiGridItem
    if (gridItemMode) {
        objectsPool.append(widget);
        widget->hide();
    } else
        widget->deleteLater();
}

QModelIndex DuiPopupListViewPrivate::indexAt(const QPointF &pos)
{
    int count = widgetsList.count();

    for (int i = 0; i < count; ++i) {
        QRectF rect = widgetsList[i]->sceneBoundingRect();
        if (rect.contains(pos))
            return itemModel->index(itemStartIndex + i, 0);
    }

    return QModelIndex();
}

DuiWidget *DuiPopupListViewPrivate::indexWidget(const QModelIndex &index)
{
    int row = index.row();

    // not create yet
    if (row < itemStartIndex || row >= itemStartIndex + batchSize)
        return NULL;

    int i = row - itemStartIndex;
    int count = widgetsList.count();
    if (count > 0 && count > i)
        return widgetsList[i];

    return NULL;
}

DuiPopupListView::DuiPopupListView(DuiPopupList *controller)
    : DuiDialogView(* new DuiPopupListViewPrivate, controller)
{
    Q_D(DuiPopupListView);
    d->controller = controller;

    d->init();
}

DuiPopupListView::DuiPopupListView(DuiPopupListViewPrivate &dd, DuiPopupList *controller) :
    DuiDialogView(dd, controller)
{
    Q_D(DuiPopupListView);
    d->controller = controller;

    d->init();
}

DuiPopupListView::~DuiPopupListView()
{
}

void DuiPopupListView::setItemModel(QAbstractItemModel *itemModel)
{
    Q_D(DuiPopupListView);

    if (d->itemModel && d->itemModel != duiEmptyModel()) {
        disconnect(d->itemModel, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        disconnect(d->itemModel, SIGNAL(modelReset()), this, SLOT(_q_modelReset()));
        disconnect(d->itemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                   this, SLOT(dataChanged(QModelIndex, QModelIndex)));

        disconnect(d->itemModel, SIGNAL(layoutChanged()), this, SLOT(_q_layoutChanged()));

        disconnect(d->itemModel, SIGNAL(rowsInserted(QModelIndex, int, int)),
                   this, SLOT(rowsInserted(QModelIndex, int, int)));

        disconnect(d->itemModel, SIGNAL(rowsRemoved(QModelIndex, int, int)),
                   this, SLOT(rowsRemoved(QModelIndex, int, int)));
    }

    d->itemModel = itemModel;

    if (d->itemModel && d->itemModel != duiEmptyModel()) {
        connect(d->itemModel, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        connect(d->itemModel, SIGNAL(modelReset()), this, SLOT(_q_modelReset()));
        connect(d->itemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                this, SLOT(dataChanged(QModelIndex, QModelIndex)));

        // when sort in model will emit this signal
        connect(d->itemModel, SIGNAL(layoutChanged()), this, SLOT(_q_layoutChanged()));

        connect(d->itemModel, SIGNAL(rowsInserted(QModelIndex, int, int)),
                this, SLOT(rowsInserted(QModelIndex, int, int)));

        connect(d->itemModel, SIGNAL(rowsRemoved(QModelIndex, int, int)),
                this, SLOT(rowsRemoved(QModelIndex, int, int)));
    }

    // need rebuild layout
    d->setLayoutDirty();
}

void DuiPopupListView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    Q_D(DuiPopupListView);

    if (d->selectionModel) {
        disconnect(d->selectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                   this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    }

    d->selectionModel = selectionModel;

    if (d->selectionModel) {
        connect(d->selectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    }
}

void DuiPopupListView::sizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos)
{
    Q_UNUSED(pannedRange);

    Q_D(DuiPopupListView);
    if (d->itemDirty) return;

//    qDebug() << "pannedPos: " << pannedPos.y() << "lastPosition: " << d->lastPosition.y();
    if (qAbs(pannedPos.y() - d->lastPosition.y()) < 10.0) return;    // skip little change to speed up
    if (d->arrangingWidget) return;

    // In order to not block panning, use timer to arrange widget
    d->viewportSize = viewportSize;
    d->pannedPos = pannedPos;
    d->arrangingWidget = true;
    QTimer::singleShot(0, this, SLOT(_q_arrangeWidget()));
}

void DuiPopupListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_D(DuiPopupListView);

    // test if the widgets have been created.
    int count = d->layout->count();
    if (count == 1)
        return;

    int top = topLeft.row();
    int bottom = bottomRight.row();

    DuiWidget *widget;
    for (int i = top; i <= bottom; ++i) {
        if (i < d->itemStartIndex || i >= d->itemStartIndex + d->batchSize)
            continue;

        // remove old widget
        int offset = i - d->itemStartIndex;
        d->layout->removeAt(offset + 1);
        widget = d->widgetsList.takeAt(offset);
        d->recycleWidget(widget);

        // create new widget and insert it into layout
        QModelIndex index = d->itemModel->index(i, 0);
        widget = d->buildItem(index);

        d->layout->insertItem(offset + 1, widget);
        d->widgetsList.insert(offset, widget);
    }
}

void DuiPopupListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);

    Q_D(DuiPopupListView);
    int i;
    DuiWidget *widget;

    if (d->singlePass) {

        d->batchSize += end - start + 1;
        if (d->batchSize > model()->batchSize()) {
            d->setLayoutDirty();
            return;
        }

        // insert new items into layout
        for (i = end; i >= start; --i) {
            QModelIndex index = d->itemModel->index(i, 0);
            widget = d->buildItem(index);

            // +1 for skip spacer
            d->layout->insertItem(start + 1, widget);
            d->widgetsList.insert(i, widget);
        }

    } else {
        d->setLayoutDirty();
    }
}

void DuiPopupListView::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);

    Q_D(DuiPopupListView);
    int i;
    DuiWidget *widget;

    if (d->singlePass) {

        // test if the widgets have been created.
        int count = d->layout->count();
        if (count == 1)
            return;

        // remove items from layout
        for (i = end; i >= start; --i) {
            d->layout->removeAt(i + 1);
            widget = d->widgetsList.takeAt(i);
            d->recycleWidget(widget);
        }

        d->batchSize -= end - start + 1;

    } else {
        d->setLayoutDirty();
    }
}

void DuiPopupListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_D(DuiPopupListView);

    int i;
    QModelIndexList list = deselected.indexes();
    DuiWidget *w;

    int count = list.count();
    for (i = 0; i < count; ++i) {
        w = d->indexWidget(list[i]);
        if (w != NULL)
            w->setSelected(false);
    }

    list = selected.indexes();
    count = list.count();
    for (i = 0; i < count; ++i) {
        w = d->indexWidget(list[i]);
        if (w != NULL)
            w->setSelected(true);
    }
}

void DuiPopupListView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const DuiPopupListView);

    if (d->itemDirty)
        d->doItemsLayout();

    DuiWidgetView::drawBackground(painter, option);
}

void DuiPopupListView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiPopupListView);

    QPersistentModelIndex index = d->indexAt(event->scenePos());
    d->pressedIndex = index;
}

void DuiPopupListView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiPopupListView);

    QPersistentModelIndex index = d->indexAt(event->scenePos());

    if (!index.isValid())
        d->controller->accept();
    else if (index == d->pressedIndex)
        d->controller->click(index);
}

void DuiPopupListView::scrollTo(const QModelIndex &index) const
{
    Q_D(const DuiPopupListView);

    int modelItemCount = d->itemModel->rowCount();
    if (index.row() >= modelItemCount) return;
    if (d->layout->count() == 0) return;

    QRectF rect = d->layout->itemAt(1)->geometry();
    qreal height = rect.height();

    int offset = index.row() - d->itemStartIndex;
    qreal y = rect.y() + offset * height;

    d->viewport->setPosition(QPointF(0, y));
}

void DuiPopupListView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiPopupListView);

    DuiDialogView::updateData(modifications);

    const char *member;
    const int count = modifications.count();
    for (int i = 0; i < count; ++i) {
        member = modifications[i];

        if (member == DuiPopupListModel::BatchSize) {
            d->setLayoutDirty();
        }
    }
}

void DuiPopupListView::setupModel()
{
    Q_D(DuiPopupListView);
    DuiDialogView::setupModel();

    if (d->batchSize != model()->batchSize())
        d->setLayoutDirty();
}

DUI_REGISTER_VIEW_NEW(DuiPopupListView, DuiPopupList)

#include "moc_duipopuplistview.cpp"

