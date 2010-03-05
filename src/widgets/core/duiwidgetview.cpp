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

#include "duiwidgetview.h"
#include "duiwidgetview_p.h"

#include "duifeedback.h"
#include "duitheme.h"
#include "duiwidgetcontroller.h"
#include "duiscalableimage.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLayout>
#include <QPainter>
#include "duiapplicationpage.h"
#include "duiwidgetmodel.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiabstractwidgetanimation.h"
#include "duiclassfactory.h"

DuiWidgetViewPrivate::DuiWidgetViewPrivate() :
    controller(NULL),
    model(NULL),
    styleContainer(NULL),
    showAnimation(NULL),
    hideAnimation(NULL)
{
}

DuiWidgetViewPrivate::~DuiWidgetViewPrivate()
{
}

void DuiWidgetViewPrivate::orientationChanged()
{
    // notify views that the style has changed
    q_ptr->applyStyle();
}

DuiWidgetView::DuiWidgetView() :
    d_ptr(new DuiWidgetViewPrivate)
{
    Q_D(DuiWidgetView);
    d->q_ptr = this;
}

DuiWidgetView::DuiWidgetView(DuiWidgetViewPrivate *dd) :
    d_ptr(dd)
{
    Q_D(DuiWidgetView);
    d->q_ptr = this;
}

DuiWidgetView::DuiWidgetView(DuiWidgetController *controller) :
    d_ptr(new DuiWidgetViewPrivate)
{
    // old way, we shouldn't need controller in the view side.
    Q_D(DuiWidgetView);
    d->q_ptr = this;
    d->controller = controller;
}

DuiWidgetView::DuiWidgetView(DuiWidgetViewPrivate &dd, DuiWidgetController *controller) :
    d_ptr(&dd)
{
    // old way, we shouldn't need controller in the view side.
    Q_D(DuiWidgetView);
    d->q_ptr = this;
    d->controller = controller;
}

DuiWidgetView::~DuiWidgetView()
{
    Q_D(DuiWidgetView);

    d->model->decreaseReferenceCount();
    d->model = 0;
    delete d->styleContainer;
    delete d->showAnimation;
    delete d->hideAnimation;
    delete d_ptr;
}

void DuiWidgetView::destroy()
{
    // TODO: animate away
    /*
        deleteLater(); // Use deleteLater() for QObjects instead of "delete this"
    */
    // TODO: replace this with something when there's a better solution available.
    // deleteLater() breaks theme change.
    delete this;
}

DuiAbstractWidgetAnimation *DuiWidgetView::showAnimation()
{
    Q_D(DuiWidgetView);
    return d->showAnimation;
}

DuiAbstractWidgetAnimation *DuiWidgetView::hideAnimation()
{
    Q_D(DuiWidgetView);
    return d->hideAnimation;
}

void DuiWidgetView::setActive(bool active)
{
    if (active) {
        style().setModeActive();
    } else {
        style().setModeDefault();
    }
    applyStyle();
}

void DuiWidgetView::setModel(DuiWidgetModel *model)
{
    Q_ASSERT(model);

    Q_D(DuiWidgetView);

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

    DuiWidgetController *parent = NULL;
    QGraphicsItem *p = d->controller->parentItem();
    while (p && !(parent = dynamic_cast<DuiWidgetController *>(p))) {
        p = p->parentItem();
    }

    //The default style type is "", not "default".
    QString styleType = d->model->viewType();
    if (styleType == DuiWidgetController::defaultType)
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

void DuiWidgetView::updateData(const QList<const char *>& modifications)
{
    const char *member;
    foreach(member, modifications) {
        if (member == DuiWidgetModel::ObjectName) {
            // object name has changed, we need to update style
            style().setObjectName(model()->objectName());
            // TODO: to be removed
            applyStyle();
        } else if (member == DuiWidgetModel::ViewType) {
            style().setType(model()->viewType());
        } else if (member == DuiWidgetModel::Position) {
        }
    }
}


void DuiWidgetView::updateMicroFocus()
{
    Q_D(DuiWidgetView);

    d->controller->updateMicroFocus();
}


void DuiWidgetView::setupModel()
{
    Q_D(DuiWidgetView);
    connect(d->model, SIGNAL(modified(QList<const char *>)),
            this, SLOT(updateData(QList<const char *>)));

    update();
}

void DuiWidgetView::applyStyle()
{
    Q_D(DuiWidgetView);

    if (style()->showAnimation() == "none") {
        delete d->showAnimation;
        d->showAnimation = NULL;
    } else {
        if (!d->showAnimation || style()->showAnimation() != d->showAnimation->metaObject()->className()) {
            delete d->showAnimation;
            d->showAnimation = dynamic_cast<DuiAbstractWidgetAnimation *>(DuiClassFactory::instance()->createAnimation(style()->showAnimation().toStdString().c_str()));
            if (d->showAnimation)
                d->showAnimation->setView(this);
        }
    }

    if (style()->hideAnimation() == "none") {
        delete d->hideAnimation;
        d->hideAnimation = NULL;
    } else {
        if (!d->hideAnimation || style()->hideAnimation() != d->hideAnimation->metaObject()->className()) {
            delete d->hideAnimation;
            d->hideAnimation = dynamic_cast<DuiAbstractWidgetAnimation *>(DuiClassFactory::instance()->createAnimation(style()->hideAnimation().toStdString().c_str()));
            if (d->hideAnimation)
                d->hideAnimation->setView(this);
        }
    }

    if (d->controller->layoutDirection() == Qt::RightToLeft)
        d->controller->setContentsMargins(
            style()->paddingRight() + style()->marginRight() + d->contentPosition.x(),
            style()->paddingTop() + style()->marginTop() + d->contentPosition.y(),
            style()->paddingLeft() + style()->marginLeft() - d->contentPosition.x(),
            style()->paddingBottom() + style()->marginBottom() - d->contentPosition.y());
    else
        d->controller->setContentsMargins(
            style()->paddingLeft() + style()->marginLeft() + d->contentPosition.x(),
            style()->paddingTop() + style()->marginTop() + d->contentPosition.y(),
            style()->paddingRight() + style()->marginRight() - d->contentPosition.x(),
            style()->paddingBottom() + style()->marginBottom() - d->contentPosition.y());

    updateGeometry();
}

DuiWidgetModel *DuiWidgetView::model()
{
    Q_D(DuiWidgetView);
    return d->model;
}

const DuiWidgetModel *DuiWidgetView::model() const
{
    Q_D(const DuiWidgetView);
    return d->model;
}

DuiWidgetStyleContainer &DuiWidgetView::style()
{
    Q_D(DuiWidgetView);
    return *d->styleContainer;
}

const DuiWidgetStyleContainer &DuiWidgetView::style() const
{
    Q_D(const DuiWidgetView);
    return *d->styleContainer;
}

QRect DuiWidgetView::margins() const
{
    return QRect(style()->marginLeft(),
                 style()->marginTop(),
                 style()->marginRight(),
                 style()->marginBottom());
}

int DuiWidgetView::marginLeft() const
{
    return style()->marginLeft();
}

int DuiWidgetView::marginTop() const
{
    return style()->marginTop();
}

int DuiWidgetView::marginRight() const
{
    return style()->marginRight();
}

int DuiWidgetView::marginBottom() const
{
    return style()->marginBottom();
}

QRect DuiWidgetView::reactiveMargins() const
{
    return QRect(style()->reactiveMarginLeft(),
                 style()->reactiveMarginTop(),
                 style()->reactiveMarginRight(),
                 style()->reactiveMarginBottom());
}

int DuiWidgetView::reactiveMarginLeft() const
{
    return style()->reactiveMarginLeft();
}

int DuiWidgetView::reactiveMarginTop() const
{
    return style()->reactiveMarginTop();
}

int DuiWidgetView::reactiveMarginRight() const
{
    return style()->reactiveMarginRight();
}

int DuiWidgetView::reactiveMarginBottom() const
{
    return style()->reactiveMarginBottom();
}

QRectF DuiWidgetView::boundingRect() const
{
    return QRectF(QPointF(), size());
}

QPainterPath DuiWidgetView::shape() const
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

void DuiWidgetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    style()->pressFeedback().play();

    // Ignore the event by default so it will propagate to widgets below
    event->ignore();
}

void DuiWidgetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    style()->releaseFeedback().play();

    // Ignore the event by default so it will propagate to widgets below
    event->ignore();
}

void DuiWidgetView::notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    Q_D(DuiWidgetView);
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
        DuiWidgetController *parent = NULL;
        QGraphicsItem *p = d->controller->parentItem();
        while (p && !(parent = dynamic_cast<DuiWidgetController *>(p))) {
            p = p->parentItem();
        }

        style().setParent(parent);
    }
}
void DuiWidgetView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void DuiWidgetView::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
}

void DuiWidgetView::orientationChangeEvent(DuiOrientationChangeEvent *event)
{
    Q_UNUSED(event);
    applyStyle();
}

void DuiWidgetView::setGeometry(const QRectF &rect)
{
    Q_UNUSED(rect);
}

void DuiWidgetView::update(const QRectF &rect)
{
    Q_D(DuiWidgetView);
    d->controller->update(rect);
}

void DuiWidgetView::update(qreal x, qreal y, qreal width, qreal height)
{
    Q_D(DuiWidgetView);
    d->controller->update(x, y, width, height);
}

void DuiWidgetView::updateGeometry()
{
    Q_D(DuiWidgetView);
    d->controller->updateGeometry();
}

QRectF DuiWidgetView::geometry() const
{
    Q_D(const DuiWidgetView);
    QRectF controllerGeom = d->controller->geometry();
    return QRectF(controllerGeom.topLeft() + QPointF(marginLeft(), marginTop()),
                  controllerGeom.bottomRight() - QPointF(marginRight(), marginBottom()));
}

QSizeF DuiWidgetView::size() const
{
    Q_D(const DuiWidgetView);
    return d->controller->size() - QSizeF(marginLeft() + marginRight(), marginTop() + marginBottom());
}

QPointF DuiWidgetView::pos() const
{
    Q_D(const DuiWidgetView);
    return d->controller->pos() + QPointF(marginLeft(), marginTop());
}

QRectF DuiWidgetView::rect() const
{
    return QRectF(QPointF(0, 0), size());
}

QVariant DuiWidgetView::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_UNUSED(query);
    return QVariant();
}

void DuiWidgetView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
}


void DuiWidgetView::changeEvent(QEvent *event)
{
    // styles can depend on right-to-left e.g. left and right margins
    // get switched.
    if (event->type() == QEvent::LayoutDirectionChange)
        applyStyle();
}

void DuiWidgetView::show()
{
    Q_D(DuiWidgetView);
    d->controller->show();
}

void DuiWidgetView::hide()
{
    Q_D(DuiWidgetView);
    d->controller->hide();
}

QPointF DuiWidgetView::contentPosition() const
{
    Q_D(const DuiWidgetView);
    return d->contentPosition;
}

void DuiWidgetView::setContentPosition(const QPointF &p)
{
    Q_D(DuiWidgetView);
    d->contentPosition = p;

    if (d->controller->layoutDirection() == Qt::RightToLeft)
        d->controller->setContentsMargins(
            style()->paddingRight() + style()->marginRight() + p.x(),
            style()->paddingTop() + style()->marginTop() + p.y(),
            style()->paddingLeft() + style()->marginLeft() - p.x(),
            style()->paddingBottom() + style()->marginBottom() - p.y());
    else
        d->controller->setContentsMargins(
            style()->paddingLeft() + style()->marginLeft() + p.x(),
            style()->paddingTop() + style()->marginTop() + p.y(),
            style()->paddingRight() + style()->marginRight() - p.x(),
            style()->paddingBottom() + style()->marginBottom() - p.y());

//    update();
}


qreal DuiWidgetView::effectiveOpacity() const
{
    Q_D(const DuiWidgetView);
    return d->controller->effectiveOpacity();
}

qreal DuiWidgetView::opacity() const
{
    Q_D(const DuiWidgetView);
    return d->controller->opacity();
}

qreal DuiWidgetView::scale() const
{
    Q_D(const DuiWidgetView);
    return d->controller->scale();
}


QFont DuiWidgetView::font() const
{
    return QFont();
}

void DuiWidgetView::setOpacity(qreal opacity)
{
    Q_D(DuiWidgetView);
    d->controller->setOpacity(opacity);
}

void DuiWidgetView::setScale(qreal scale)
{
    Q_D(DuiWidgetView);
    d->controller->setScale(scale);
}


bool DuiWidgetView::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    Q_UNUSED(watched);
    Q_UNUSED(event);
    return false;  //by default, don't catch the event.  Allow it to continue to propagate
}

QSizeF DuiWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
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

void DuiWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->translate(marginLeft() + contentPosition().x(), marginTop() + contentPosition().y());
    drawBackground(painter, option);
    drawContents(painter, option);
    painter->translate(-marginLeft(), -marginTop());
    drawForeground(painter, option);
}

void DuiWidgetView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());

    if (style()->backgroundImage()) {
        // TODO Use tiled bitmap drawing when it becomes available.
        style()->backgroundImage()->draw(0, 0, size().width(), size().height(), painter);
    } else if (style()->backgroundColor().isValid()) {
        painter->fillRect(boundingRect(), QBrush(style()->backgroundColor()));
    }
    painter->setOpacity(oldOpacity);
}

void DuiWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void DuiWidgetView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

DUI_REGISTER_VIEW_NEW(DuiWidgetView, DuiApplicationPage)
#include "moc_duiwidgetview.cpp"
