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

#include "duilabelview.h"
#include "duilabelview_p.h"
#include "duilabelmodel.h"
#include "duilabel.h"
#include "duiviewcreator.h"

#include <QTextDocument>
#include <QPixmapCache>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneResizeEvent>

DuiLabelViewPrivate::DuiLabelViewPrivate()
{
    impl = new DuiLabelViewSimple(this);
    cacheKey.sprintf("%p", (void *)(this));
}

DuiLabelViewPrivate::~DuiLabelViewPrivate()
{
    delete impl;
}

const DuiLabelModel *DuiLabelViewPrivate::model() const
{
    Q_Q(const DuiLabelView);
    return q->model();
}

const DuiLabelStyle *DuiLabelViewPrivate::style() const
{
    Q_Q(const DuiLabelView);
    return q->style().operator ->();
}

const QRectF DuiLabelViewPrivate::boundingRect() const
{
    Q_Q(const DuiLabelView);
    return q->boundingRect();
}

DuiLabelView::DuiLabelView(DuiLabel *controller) :
    DuiWidgetView(new DuiLabelViewPrivate)
{
    Q_D(DuiLabelView);
    d->controller = controller;
}

DuiLabelView::~DuiLabelView()
{
}

void DuiLabelView::applyStyle()
{
    DuiWidgetView::applyStyle();
    Q_D(DuiLabelView);
    QPixmapCache::remove(d->cacheKey);
    d->impl->dirty = true;
    d->impl->applyStyle();
    updateGeometry();
}

void DuiLabelView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const DuiLabelView);
    Q_UNUSED(option);

    //give size adjusted with padding to the actual implementation class
    QSizeF padding(style()->paddingLeft() + style()->paddingRight(),
                   style()->paddingTop() + style()->paddingBottom());
    d->impl->drawContents(painter, size() - padding);
}

void DuiLabelView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    DuiWidgetView::resizeEvent(event);

    Q_D(DuiLabelView);
    QPixmapCache::remove(d->cacheKey);
    d->impl->dirty = true;

    QSizeF padding(style()->paddingLeft() + style()->paddingRight(),
                   style()->paddingTop() + style()->paddingBottom());

    event->setOldSize(event->oldSize() - padding);
    event->setNewSize(event->newSize() - padding);

    if (d->impl->resizeEvent(event))  {
        updateGeometry();
    }
}

QFont DuiLabelView::font() const
{
    return style()->font();
}

QSizeF DuiLabelView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiLabelView);

    QSizeF size = DuiWidgetView::sizeHint(which, constraint);
    if (!size.isValid()) {
        QSizeF implsize = d->impl->sizeHint(which, constraint);
        if (size.width() == -1)
            size.setWidth(implsize.width() + style()->paddingLeft() + style()->paddingRight());
        if (size.height() == -1)
            size.setHeight(implsize.height() + style()->paddingTop() + style()->paddingBottom());
    }
    return size;
}

void DuiLabelView::setupModel()
{
    DuiWidgetView::setupModel();
    Q_D(DuiLabelView);

    bool isRichText = Qt::mightBeRichText(model()->text());
    // Check has label type changed since last call to this method. Re-allocate label with correct type.
    if (d->impl->isRich() != isRichText) {
        delete d->impl;
        if (isRichText)
            d->impl = new DuiLabelViewRich(d);
        else
            d->impl = new DuiLabelViewSimple(d);
    }
    d->impl->setupModel();

    QPixmapCache::remove(d->cacheKey);
    d->impl->dirty = true;
}


void DuiLabelView::updateData(const QList<const char *>& modifications)
{

    DuiWidgetView::updateData(modifications);

    Q_D(DuiLabelView);

    if (modifications.contains(DuiLabelModel::Text) || modifications.contains(DuiLabelModel::Highlighters)) {
        bool isRichText = Qt::mightBeRichText(model()->text());
        
        // Check has label type changed since last call to this method. Re-allocate label with correct type.
        bool shouldBeRich = isRichText || model()->highlighters().size() > 0;
        bool shouldBeSimple = !shouldBeRich;

        if ((shouldBeRich && !d->impl->isRich()) || (shouldBeSimple && d->impl->isRich())) {
            delete d->impl;
            if (shouldBeRich) 
                d->impl = new DuiLabelViewRich(d);
            else 
                d->impl = new DuiLabelViewSimple(d);
        }
    }

    if (d->impl->updateData(modifications))
        updateGeometry();
    QPixmapCache::remove(d->cacheKey);
    d->impl->dirty = true;
    update();
}

void DuiLabelView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiLabelView);
    d->impl->mousePressEvent(event);
}

void DuiLabelView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiLabelView);
    d->impl->mouseReleaseEvent(event);
}

void DuiLabelView::cancelEvent(DuiCancelEvent *event)
{
    Q_D(DuiLabelView);
    d->impl->cancelEvent(event);
}

void DuiLabelView::longPressEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_D(DuiLabelView);
    event->ignore();
    d->impl->longPressEvent(event);
}


DUI_REGISTER_VIEW_NEW(DuiLabelView, DuiLabel)








