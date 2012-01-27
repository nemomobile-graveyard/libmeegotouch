/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mpopuplistview.h"
#include "mpopuplistview_p.h"

#include "mpopuplist.h"
#include "mlist.h"
#include "mpannableviewport.h"
#include "mlabel.h"
#include "mimagewidget.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"

#include <QGraphicsLinearLayout>
#include <QIcon>
#include <QTimer>

MPopupListItem::MPopupListItem(QGraphicsItem* parent)
    : MListItem(parent),
      icon(0),
      title(0),
      itemStyle(SingleTitle)
{
    memset(policy, 0, sizeof(policy));
    setObjectName("PopupListItem");
    setLayout(new MLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);
}

void MPopupListItem::updateLayout()
{
    MLayout* mlayout = static_cast<MLayout*>(layout());
    if (!policy[itemStyle]) {
        policy[itemStyle] = new MLinearLayoutPolicy(mlayout, Qt::Horizontal);
        if (itemStyle == IconTitle) {
            policy[itemStyle]->addItem(icon);
            policy[itemStyle]->addItem(title);
        } else {
            policy[itemStyle]->addItem(title);
        }
    }
    mlayout->setPolicy(policy[itemStyle]);
}

void MPopupListItem::setTitle(const QString &text)
{
    titleLabel()->setText(text);
    updateLayout();
}

void MPopupListItem::setTitleForeground(const QColor &color)
{
    // Invalid QColor resets the label to use color from style.
    titleLabel()->setColor(color);
}

void MPopupListItem::setWordWrap(bool wordWrap) {
    if (!title)
        return;
    if (wordWrap) {
        title->setTextElide(false);
        title->setWordWrap(true);
    }
    else {
        title->setTextElide(true);
        title->setWordWrap(false);
    }
}

MImageWidget* MPopupListItem::iconWidget()
{
    if (!icon) {
        icon = new MImageWidget(this);
        icon->setStyleName("CommonMainIcon");
    }
    return icon;
}

void MPopupListItem::setIconID(const QString& id)
{
    if (id.isEmpty()) {
        itemStyle = SingleTitle;
        if (icon)
            icon->setPixmap(QPixmap());
    } else {
        itemStyle = IconTitle;
        iconWidget()->setImage(id);
    }
    updateLayout();
}

void MPopupListItem::setPixmap(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        itemStyle = SingleTitle;
        if (icon)
            icon->setPixmap(QPixmap());
    } else {
        itemStyle = IconTitle;
        iconWidget()->setPixmap(pixmap);
    }
    updateLayout();
}

MLabel *MPopupListItem::titleLabel()
{
    if (!title) {
        title = new MLabel(this);
        title->setTextElide(true);
        title->setStyleName("CommonSingleTitle");
    }
    return title;
}

MPopupListViewPrivate::MPopupListViewPrivate()
    : controller(0), list(0), cellCreator(0)
{
}

MPopupListViewPrivate::~MPopupListViewPrivate()
{
}

void MPopupListViewPrivate::init()
{
    Q_Q(MPopupListView);

    list = new MList();
    cellCreator = new MPopupListCellCreator(list);
    list->setCellCreator(cellCreator);
    list->setSelectionMode(MList::SingleSelection);
    q->contentsLayout()->insertItem(0, list);

    QObject::connect(list, SIGNAL(itemClicked(QModelIndex)), controller, SLOT(click(QModelIndex)));
}

MPopupListCellCreator::MPopupListCellCreator(MList *list)
    : list(list), wordWrap(false)
{}

void MPopupListCellCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
    MPopupListItem* item = static_cast<MPopupListItem*>(cell);

    QVariant variant = list->itemModel()->data(index, Qt::DisplayRole);
    item->setTitle(variant.toString());

    item->setWordWrap(wordWrap);

    variant = list->itemModel()->data(index, Qt::DecorationRole);
    // Accepting QString as Qt::DecorationRole is against Qt documentation
    // and brings ambiguity between types QString and QColor: QColor can always
    // be converted into a QString and QString can potentially be converted to
    // QColor. QVariant::type() should probably be used here to check but could
    // cause regressions if we already rely on such conversions.
    if (variant.canConvert<QString>()) {
        item->setIconID(variant.toString());
    } else if (variant.canConvert<QIcon>()) {
        QIcon icon = variant.value<QIcon>();
        const QList<QSize>& sizes = icon.availableSizes();
        item->setPixmap(sizes.isEmpty() ? QPixmap() : icon.pixmap(sizes[0]));
    } else {
        item->setPixmap(variant.value<QPixmap>());
    }

    QColor foregroundColor; // Title foreground color
    variant = list->itemModel()->data(index, Qt::ForegroundRole);
    if (variant.isValid()) {
        // By Qt documention the type should be QBrush. We also accept QColor.
        // Color is the only thing used here, eventually.
        if (variant.type() == static_cast<QVariant::Type>(QMetaType::QBrush)) {
            foregroundColor = variant.value<QBrush>().color();
        } else if (variant.type() == static_cast<QVariant::Type>(QMetaType::QColor)) {
            foregroundColor =variant.value<QColor>();
        }
    }
    // If the model didn't provide ForegroundRole foregroundColor
    // will be invalid here. Still set it as existing one may need
    // to be cleared.
    item->setTitleForeground(foregroundColor);

    if (list->selectionModel()->isSelected(index))
        item->setSelected(true);

    if (index.row() == 0)
        item->setLayoutPosition(M::VerticalTopPosition);
    else if (index.row() == index.model()->rowCount() - 1)
        item->setLayoutPosition(M::VerticalBottomPosition);
    else
        item->setLayoutPosition(M::VerticalCenterPosition);
}

void MPopupListCellCreator::setWordWrap(bool wordWrap)
{
    this->wordWrap = wordWrap;
}

MPopupListView::MPopupListView(MPopupList *controller)
    : MDialogView(* new MPopupListViewPrivate, controller)
{
    Q_D(MPopupListView);
    d->controller = controller;

    d->init();
}

MPopupListView::MPopupListView(MPopupListViewPrivate &dd, MPopupList *controller) :
    MDialogView(dd, controller)
{
    Q_D(MPopupListView);
    d->controller = controller;

    d->init();
}

MPopupListView::~MPopupListView()
{
}

void MPopupListView::updateData(const QList<const char *>& modifications)
{
    MDialogView::updateData(modifications);

    Q_D(MPopupListView);
    foreach(const char* member, modifications) {
        if (member == MPopupListModel::ItemModel)
            d->list->setItemModel(model()->itemModel());
        else if (member == MPopupListModel::SelectionModel)
            d->list->setSelectionModel(model()->selectionModel());
        else if (member == MPopupListModel::ScrollToIndex)
            d->list->scrollTo(model()->scrollToIndex());
    }
}

void MPopupListView::setupModel()
{
    Q_D(MPopupListView);

    MDialogView::setupModel();

    d->list->setItemModel(model()->itemModel());
    d->list->setSelectionModel(model()->selectionModel());
    d->list->scrollTo(model()->scrollToIndex());
}

void MPopupListView::applyStyle()
{
    Q_D(MPopupListView);

    MDialogView::applyStyle();

    d->cellCreator->setCellObjectName(style()->itemStyleName());
    d->cellCreator->setWordWrap(style()->itemWordWrap());
    contentsViewport()->setStyleName(style()->contentsViewportStyleName());
}

void MPopupListView::scrollTo(const QModelIndex &index) const
{
    const_cast<MPopupListModel*>(model())->setScrollToIndex(index);
}

M_REGISTER_VIEW_NEW(MPopupListView, MPopupList)

#include "moc_mpopuplistview.cpp"

