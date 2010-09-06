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

#include "mlabelview.h"
#include "mlabelview_p.h"
#include "mlabelmodel.h"
#include "mlabel.h"
#include "mviewcreator.h"

#include <QTextDocument>
#include <QPixmapCache>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneResizeEvent>

MLabelViewPrivate::MLabelViewPrivate()
{
    impl = new MLabelViewSimple(this);
}

MLabelViewPrivate::~MLabelViewPrivate()
{
    delete impl;
}

const MLabelModel *MLabelViewPrivate::model() const
{
    Q_Q(const MLabelView);
    return q->model();
}

const MLabelStyle *MLabelViewPrivate::style() const
{
    Q_Q(const MLabelView);
    return q->style().operator ->();
}

const QRectF MLabelViewPrivate::boundingRect() const
{
    Q_Q(const MLabelView);
    return q->boundingRect();
}

bool MLabelViewPrivate::isRichText(QString text) const
{
    //Qt::mightBeRichText stops at the first line break
    text.replace("\n", " ");
    return Qt::mightBeRichText(text);
}

MLabelView::MLabelView(MLabel *controller) :
    MWidgetView(new MLabelViewPrivate)
{
    Q_D(MLabelView);
    d->controller = controller;
}

MLabelView::~MLabelView()
{
}

void MLabelView::applyStyle()
{
    MWidgetView::applyStyle();
    Q_D(MLabelView);
    d->impl->markDirty();
    d->impl->applyStyle();
    updateGeometry();
}

void MLabelView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MLabelView);
    Q_UNUSED(option);

    //give size adjusted with padding to the actual implementation class
    QSizeF padding(style()->paddingLeft() + style()->paddingRight(),
                   style()->paddingTop() + style()->paddingBottom());
    d->impl->drawContents(painter, size() - padding);
}

void MLabelView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event);

    Q_D(MLabelView);
    d->impl->markDirty();

    QSizeF padding(style()->paddingLeft() + style()->paddingRight(),
                   style()->paddingTop() + style()->paddingBottom());

    event->setOldSize(event->oldSize() - padding);
    event->setNewSize(event->newSize() - padding);

    if (d->impl->resizeEvent(event))  {
        updateGeometry();
    }
}

QFont MLabelView::font() const
{
    return style()->font();
}

QSizeF MLabelView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MLabelView);

    QSizeF size = MWidgetView::sizeHint(which, constraint);
    if (!size.isValid()) {
        QSizeF implsize = d->impl->sizeHint(which, constraint);
        if (size.width() == -1)
            size.setWidth(implsize.width() + style()->paddingLeft() + style()->paddingRight());
        if (size.height() == -1)
            size.setHeight(implsize.height() + style()->paddingTop() + style()->paddingBottom());
    }
    return size;
}

void MLabelView::setupModel()
{
    MWidgetView::setupModel();
    Q_D(MLabelView);

    bool isRichText = d->isRichText(model()->text());

    // Check has label type changed since last call to this method. Re-allocate label with correct type.
    if (d->impl->isRich() != isRichText) {
        delete d->impl;
        if (isRichText)
            d->impl = new MLabelViewRich(d);
        else
            d->impl = new MLabelViewSimple(d);
    }
    d->impl->setupModel();

    d->impl->markDirty();
}

void MLabelView::updateData(const QList<const char *>& modifications)
{

    MWidgetView::updateData(modifications);

    Q_D(MLabelView);

    if (modifications.contains(MLabelModel::Text) || modifications.contains(MLabelModel::Highlighters)) {
        bool isRichText = d->isRichText(model()->text());

        // Check has label type changed since last call to this method. Re-allocate label with correct type.
        bool shouldBeRich = isRichText || model()->highlighters().size() > 0;
        bool shouldBeSimple = !shouldBeRich;

        if ((shouldBeRich && !d->impl->isRich()) || (shouldBeSimple && d->impl->isRich())) {
            delete d->impl;
            if (shouldBeRich)
                d->impl = new MLabelViewRich(d);
            else
                d->impl = new MLabelViewSimple(d);

            d->impl->setupModel();
        }
    }

    if (d->impl->updateData(modifications))
        updateGeometry();
    d->impl->markDirty();
    update();
}

void MLabelView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MLabelView);
    d->impl->mousePressEvent(event);
}

void MLabelView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MLabelView);
    d->impl->mouseReleaseEvent(event);
}

void MLabelView::cancelEvent(MCancelEvent *event)
{
    Q_D(MLabelView);
    d->impl->cancelEvent(event);
}

void MLabelView::longPressEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->ignore();
}

void MLabelView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture)
{
    Q_D(MLabelView);
    d->impl->longPressEvent(event,gesture);
}

void MLabelView::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_D(MLabelView);
    MWidgetView::orientationChangeEvent(event);
    d->impl->orientationChangeEvent(event);
}

M_REGISTER_VIEW_NEW(MLabelView, MLabel)








