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

#include "mwidgetview.h"
#include "mwidgetview_p.h"

#include "mfeedback.h"
#include "mtheme.h"
#include "mwidgetcontroller.h"
#include "mscalableimage.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLayout>
#include <QPainter>
#include "mapplicationpage.h"
#include "mwidgetmodel.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mclassfactory.h"

#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QPanGesture>
#include <QPinchGesture>
#include <QTapGesture>
#include <QSwipeGesture>

MWidgetViewPrivate::MWidgetViewPrivate() :
    q_ptr(NULL),
    controller(NULL),
    model(NULL),
    styleContainer(NULL)
{
}

MWidgetViewPrivate::~MWidgetViewPrivate()
{
}

MWidgetView::MWidgetView() :
    d_ptr(new MWidgetViewPrivate)
{
    Q_D(MWidgetView);
    d->q_ptr = this;
}

MWidgetView::MWidgetView(MWidgetViewPrivate *dd) :
    d_ptr(dd)
{
    Q_D(MWidgetView);
    d->q_ptr = this;
}

MWidgetView::MWidgetView(MWidgetController *controller) :
    d_ptr(new MWidgetViewPrivate)
{
    // old way, we shouldn't need controller in the view side.
    Q_D(MWidgetView);
    d->q_ptr = this;
    d->controller = controller;
}

MWidgetView::MWidgetView(MWidgetViewPrivate &dd, MWidgetController *controller) :
    d_ptr(&dd)
{
    // old way, we shouldn't need controller in the view side.
    Q_D(MWidgetView);
    d->q_ptr = this;
    d->controller = controller;
}

MWidgetView::~MWidgetView()
{
    Q_D(MWidgetView);

    d->model->decreaseReferenceCount();
    d->model = 0;
    delete d->styleContainer;
    delete d_ptr;
}

void MWidgetView::destroy()
{
    // TODO: animate away
    /*
        deleteLater(); // Use deleteLater() for QObjects instead of "delete this"
    */
    // TODO: replace this with something when there's a better solution available.
    // deleteLater() breaks theme change.
    delete this;
}

void MWidgetView::setActive(bool active)
{
    if (active) {
        style().setModeActive();
    } else {
        style().setModeDefault();
    }
    applyStyle();
}

void MWidgetView::setModel(MWidgetModel *model)
{
    Q_ASSERT(model);

    Q_D(MWidgetView);

    if (model == d->model)
        return;

    if (d->model) {
        disconnect(d->model, 0, this, 0);
        d->model->decreaseReferenceCount();
    }

    d->model = model;
    d->model->increaseReferenceCount();

    if (!d->styleContainer) {
        d->styleContainer = createStyleContainer();
    }

    MWidgetController *parent = NULL;
    QGraphicsItem *p = d->controller->parentItem();
    while (p && !(parent = dynamic_cast<MWidgetController *>(p))) {
        p = p->parentItem();
    }

    //The default style type is "", not "default".
    QString styleType = d->model->viewType();
    if (styleType == MWidgetController::defaultType)
        styleType.clear();

    d->styleContainer->initialize(d->model->objectName(), styleType, parent);

    if (d->controller->isActive()) {
        setActive(true);
    } else if (!d->controller->isEnabled()) {
        notifyItemChange(QGraphicsItem::ItemEnabledHasChanged, false);
    } else if (d->controller->isSelected()) {
        notifyItemChange(QGraphicsItem::ItemSelectedHasChanged, true);
    }

    // TODO: to be removed when scalable image is ready
    applyStyle();

    // notify derived classes
    setupModel();
}

void MWidgetView::updateData(const QList<const char *>& modifications)
{
    const char *member;
    foreach(member, modifications) {
        if (member == MWidgetModel::ObjectName) {
            // object name has changed, we need to update style
            style().setObjectName(model()->objectName());
            // TODO: to be removed
            applyStyle();
        } else if (member == MWidgetModel::ViewType) {
            style().setType(model()->viewType());
        } else if (member == MWidgetModel::Position) {
        }
    }
}


void MWidgetView::updateMicroFocus()
{
    Q_D(MWidgetView);

    d->controller->updateMicroFocus();
}


void MWidgetView::setupModel()
{
    Q_D(MWidgetView);
    connect(d->model, SIGNAL(modified(QList<const char *>)),
            this, SLOT(updateData(QList<const char *>)));

    update();
}

void MWidgetView::applyStyle()
{
    Q_D(MWidgetView);

    if (d->controller->layoutDirection() == Qt::RightToLeft)
        d->controller->setContentsMargins(
            style()->paddingRight() + style()->marginRight(),
            style()->paddingTop() + style()->marginTop(),
            style()->paddingLeft() + style()->marginLeft(),
            style()->paddingBottom() + style()->marginBottom());
    else
        d->controller->setContentsMargins(
            style()->paddingLeft() + style()->marginLeft(),
            style()->paddingTop() + style()->marginTop(),
            style()->paddingRight() + style()->marginRight(),
            style()->paddingBottom() + style()->marginBottom());

    updateGeometry();
}

MWidgetModel *MWidgetView::model()
{
    Q_D(MWidgetView);
    return d->model;
}

const MWidgetModel *MWidgetView::model() const
{
    Q_D(const MWidgetView);
    return d->model;
}

MWidgetStyleContainer &MWidgetView::style()
{
    Q_D(MWidgetView);
    return *d->styleContainer;
}

const MWidgetStyleContainer &MWidgetView::style() const
{
    Q_D(const MWidgetView);
    return *d->styleContainer;
}

QRect MWidgetView::margins() const
{
    return QRect(style()->marginLeft(),
                 style()->marginTop(),
                 style()->marginRight(),
                 style()->marginBottom());
}

int MWidgetView::marginLeft() const
{
    return style()->marginLeft();
}

int MWidgetView::marginTop() const
{
    return style()->marginTop();
}

int MWidgetView::marginRight() const
{
    return style()->marginRight();
}

int MWidgetView::marginBottom() const
{
    return style()->marginBottom();
}

QRect MWidgetView::reactiveMargins() const
{
    return QRect(style()->reactiveMarginLeft(),
                 style()->reactiveMarginTop(),
                 style()->reactiveMarginRight(),
                 style()->reactiveMarginBottom());
}

int MWidgetView::reactiveMarginLeft() const
{
    return style()->reactiveMarginLeft();
}

int MWidgetView::reactiveMarginTop() const
{
    return style()->reactiveMarginTop();
}

int MWidgetView::reactiveMarginRight() const
{
    return style()->reactiveMarginRight();
}

int MWidgetView::reactiveMarginBottom() const
{
    return style()->reactiveMarginBottom();
}

QRectF MWidgetView::boundingRect() const
{
    return QRectF(QPointF(), size());
}

QPainterPath MWidgetView::shape() const
{
    QPainterPath path;
    QRectF br = boundingRect();
    br.setRect(br.x() - reactiveMarginLeft(),
               br.y() - reactiveMarginTop(),
               br.width()  + marginLeft() + marginRight() + reactiveMarginLeft() +  reactiveMarginRight(),
               br.height() + marginTop() + marginBottom() + reactiveMarginTop() + reactiveMarginBottom());
    path.addRect(br);
    return path;
}

void MWidgetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    style()->pressFeedback().play();

    // Ignore the event by default so it will propagate to widgets below
    event->ignore();
}

void MWidgetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    style()->releaseFeedback().play();

    // Ignore the event by default so it will propagate to widgets below
    event->ignore();
}

void MWidgetView::notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    Q_D(MWidgetView);
    Q_UNUSED(value);
    if (change == QGraphicsItem::ItemEnabledHasChanged) {
        if (d->controller->isEnabled()) { //Make it possible to style disabled widgets differently in CSS
            style().setModeDefault();
        } else {
            style().setModeDisabled();
        }
    } else if (change == QGraphicsItem::ItemSelectedHasChanged) {
        if (d->controller->isEnabled()) {
            if (d->controller->isSelected()) {
                style().setModeSelected();
            } else {
                style().setModeDefault();
            }
        }
    } else if (change == QGraphicsItem::ItemParentHasChanged) {
        MWidgetController *parent = NULL;
        QGraphicsItem *p = d->controller->parentItem();
        while (p && !(parent = dynamic_cast<MWidgetController *>(p))) {
            p = p->parentItem();
        }

        style().setParent(parent);
    }
}
void MWidgetView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MWidgetView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
}

void MWidgetView::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_UNUSED(event);
    applyStyle();
}

void MWidgetView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    event->ignore(gesture);
}

void MWidgetView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    event->ignore(gesture);
}

void MWidgetView::pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture)
{
    event->ignore(gesture);
}

void MWidgetView::tapGestureEvent(QGestureEvent *event, QTapGesture* gesture)
{
    event->ignore(gesture);
}

void MWidgetView::swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture)
{
    event->ignore(gesture);
}

void MWidgetView::setGeometry(const QRectF &rect)
{
    Q_UNUSED(rect);
}

void MWidgetView::update(const QRectF &rect)
{
    Q_D(MWidgetView);
    d->controller->update(rect);
}

void MWidgetView::update(qreal x, qreal y, qreal width, qreal height)
{
    Q_D(MWidgetView);
    d->controller->update(x, y, width, height);
}

void MWidgetView::updateGeometry()
{
    Q_D(MWidgetView);
    d->controller->updateGeometry();
}

QRectF MWidgetView::geometry() const
{
    Q_D(const MWidgetView);
    QRectF controllerGeom = d->controller->geometry();
    return QRectF(controllerGeom.topLeft() + QPointF(marginLeft(), marginTop()),
                  controllerGeom.bottomRight() - QPointF(marginRight(), marginBottom()));
}

QSizeF MWidgetView::size() const
{
    Q_D(const MWidgetView);
    return d->controller->size() - QSizeF(marginLeft() + marginRight(), marginTop() + marginBottom());
}

QPointF MWidgetView::pos() const
{
    Q_D(const MWidgetView);
    return d->controller->pos() + QPointF(marginLeft(), marginTop());
}

QRectF MWidgetView::rect() const
{
    return QRectF(QPointF(0, 0), size());
}

QVariant MWidgetView::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_UNUSED(query);
    return QVariant();
}

void MWidgetView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
}


void MWidgetView::changeEvent(QEvent *event)
{
    // styles can depend on right-to-left e.g. left and right margins
    // get switched.
    if (event->type() == QEvent::LayoutDirectionChange)
        applyStyle();
}

void MWidgetView::show()
{
    Q_D(MWidgetView);
    d->controller->show();
}

void MWidgetView::hide()
{
    Q_D(MWidgetView);
    d->controller->hide();
}

qreal MWidgetView::effectiveOpacity() const
{
    Q_D(const MWidgetView);
    return d->controller->effectiveOpacity();
}

qreal MWidgetView::opacity() const
{
    Q_D(const MWidgetView);
    return d->controller->opacity();
}

qreal MWidgetView::scale() const
{
    Q_D(const MWidgetView);
    return d->controller->scale();
}


QFont MWidgetView::font() const
{
    return QFont();
}

void MWidgetView::setOpacity(qreal opacity)
{
    Q_D(MWidgetView);
    d->controller->setOpacity(opacity);
}

void MWidgetView::setScale(qreal scale)
{
    Q_D(MWidgetView);
    d->controller->setScale(scale);
}


bool MWidgetView::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    Q_UNUSED(watched);
    Q_UNUSED(event);
    return false;  //by default, don't catch the event.  Allow it to continue to propagate
}

QSizeF MWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);
    QSizeF sh;

    switch (which) {
    case Qt::MinimumSize:
        sh = style()->minimumSize();
        break;

    case Qt::MaximumSize:
        sh = style()->maximumSize();
        break;
    case Qt::PreferredSize:
        sh = style()->preferredSize();
        break;
    default:
        break;
    }

    return sh;
}

void MWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->translate(marginLeft(), marginTop());
    drawBackground(painter, option);
    drawContents(painter, option);
    painter->translate(-marginLeft(), -marginTop());
    drawForeground(painter, option);
}

void MWidgetView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    QSizeF currentSize = size();

    if (currentSize.width() == 0 || currentSize.height() == 0)
        return;

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());

    if (style()->backgroundTiles().isValid()) {
        style()->backgroundTiles()[model()->layoutPosition()]->draw(0,0, currentSize.width(), currentSize.height(), painter);
    }
    else if (style()->backgroundImage()) {
        style()->backgroundImage()->draw(0, 0, currentSize.width(), currentSize.height(), painter);
    } else if (style()->backgroundColor().isValid()) {
        painter->fillRect(boundingRect(), QBrush(style()->backgroundColor()));
    }
    painter->setOpacity(oldOpacity);
}

void MWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MWidgetView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

M_REGISTER_VIEW(MWidgetView, MApplicationPage)
#include "moc_mwidgetview.cpp"
