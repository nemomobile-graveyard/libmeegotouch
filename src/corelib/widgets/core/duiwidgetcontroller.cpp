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

#include "duiwidgetcontroller.h"
#include "duiwidgetcontroller_p.h"

#include "duiwidgetmodel.h"
#include "duiwidgetview.h"
#include "duiwidgetview_p.h"
#include "duitheme.h"

#include <DuiDebug>
#include <QSet>
#include <QGraphicsSceneMouseEvent>
#include <QInputContext>
#include <QApplication>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiWidgetController)

const DuiTheme::ViewType DuiWidgetController::defaultType = "default";

// TODO: if model needs to be shared, a version of constructor is
// needed which takes model as a parameter. Some consideration is also
// needed of how the shared model is managed outside the controllers
// (e.g. deleting after detached from the last controller). Reference
// count maybe?

///////////////////
// PRIVATE CLASS //
///////////////////

QSet<DuiWidgetController *> DuiWidgetControllerPrivate::allSystemWidgets;

DuiWidgetControllerPrivate::DuiWidgetControllerPrivate() :
    model(0),
    view(0),
    viewSetManually(false),
    active(false),
    constructingView(false),
    modelSetup(false)
{
}

DuiWidgetControllerPrivate::~DuiWidgetControllerPrivate()
{
}

//////////////////
// PUBLIC CLASS //
//////////////////

DuiWidgetController::DuiWidgetController(QGraphicsItem *parent) :
    DuiWidget(*new DuiWidgetControllerPrivate, parent)
{
    setModel(new DuiWidgetModel);

    DuiWidgetControllerPrivate::allSystemWidgets.insert(this);
}

DuiWidgetController::DuiWidgetController(DuiWidgetModel *model, QGraphicsItem *parent) :
    DuiWidget(*new DuiWidgetControllerPrivate, parent)
{
    setModel(model == NULL ? new DuiWidgetModel : model);

    DuiWidgetControllerPrivate::allSystemWidgets.insert(this);
}

// protected constructor for derived classes
DuiWidgetController::DuiWidgetController(DuiWidgetControllerPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent) :
    DuiWidget(*dd, parent)
{
    //The parent class must set a model
    Q_ASSERT(model);
    setModel(model);
    DuiWidgetControllerPrivate::allSystemWidgets.insert(this);
}

// destructor
DuiWidgetController::~DuiWidgetController()
{
    Q_D(DuiWidgetController);

    // let the model know that we're not attached to it anymore.
    if (d->model) {
        d->model->decreaseReferenceCount();
        d->model = 0;
    }

    // let the view know that it can now animate away.
    if (d->view) {
        // TODO: later this should probably be done by DuiSceneManager.
        d->view->destroy();
        d->view = 0;
    }

    DuiWidgetControllerPrivate::allSystemWidgets.remove(this);
}

// const getter for model
const DuiWidgetModel *DuiWidgetController::model() const
{
    DuiWidgetControllerPrivate *d = (DuiWidgetControllerPrivate *)d_ptr;

    if (d->modelSetup == false) {
        // Clear the flag first to avoid recursion in case a derived setupModel calls model()
        d->modelSetup = true;
        const_cast<DuiWidgetController &>(*this).setupModel();
    }

    return d->model;
}

// getter for model
DuiWidgetModel *DuiWidgetController::model()
{
    return const_cast<DuiWidgetModel *>
           (static_cast<const DuiWidgetController &>(*this).model());
}

// const getter for view
const DuiWidgetView *DuiWidgetController::view() const
{
    Q_D(const DuiWidgetController);

    if (!d->view) {
        Q_ASSERT_X(d->model, "DuiWidgetController", "You should not call DuiWidgetController::view() before the widget has a model!");
        const_cast<DuiWidgetControllerPrivate *>(d)->createView();
    }

    return d->view;
}

// notification of model modifications
void DuiWidgetController::updateData(const QList<const char *>& modifications)
{
    Q_UNUSED(modifications);
}

// notification of model reset
void DuiWidgetController::setupModel()
{
    Q_D(DuiWidgetController);
    connect(d->model, SIGNAL(modified(QList<const char *>)),
            this, SLOT(updateData(QList<const char *>)));
}

void DuiWidgetController::updateMicroFocus()
{
    QInputContext *ic = qApp->inputContext();

    if (ic != 0) {
        ic->update();
    }
}

// method for setting new model for this controller + view
void DuiWidgetController::setModel(DuiWidgetModel *model)
{
    Q_ASSERT_X(model, "DuiWidgetController", "DuiWidgetController::setModel() parameter model has to be valid!");

    Q_D(DuiWidgetController);

    if (model == d->model)
        return;

    if (d->model) {
        disconnect(d->model, 0, this, 0);
        d->model->decreaseReferenceCount();
    }

    d->model = model;
    d->model->increaseReferenceCount();

    // setupModel() will be called on the first model access to finalize the model switch
    d->modelSetup = false;

    // set the model also to view if we already have one
    if (d->view) {
        d->view->setModel(d->model);
    }
}

// setter for view
// manually sets the view to some specific view
// after calling this the view won't change with theme
void DuiWidgetController::setView(DuiWidgetView *view)
{
    Q_D(DuiWidgetController);

    // check if we already had a view and destroy it.
    if (d->view != NULL)
        d->view->destroy();

    // set the new view
    d->view = view;

    // if the user gave us a view
    if (d->view) {
        // set the flag that user has manually set the view, so theme won't override it.
        d->viewSetManually = true;

        d->configureView(d->view);

        // TODO: check if this is really needed.
        prepareGeometryChange();
    } else {
        // user set a NULL view

        // unset the flag so now the view will be again controlled by the theme.
        d->viewSetManually = false;
    }
    updateGeometry();
}

// this method is called by DuiTheme when theme is changing.
// deprecates the current view, if the user has not manually set the view
void DuiWidgetControllerPrivate::deprecateView()
{
    if (viewSetManually)
        return;

    if (view != NULL) {
        view->destroy();
        view = 0;
    }
}

// creates a view for this controller
void DuiWidgetControllerPrivate::createView()
{
    Q_Q(DuiWidgetController);

    // check that we don't have a view yet
    Q_ASSERT(!view);

    if (!constructingView) {
        constructingView = true;
        view = DuiTheme::view(q);
        constructingView = false;
    }

    if (view) {
        configureView(view);
        q->updateGeometry();
    }
}

void DuiWidgetControllerPrivate::configureView(DuiWidgetView *view)
{
    Q_Q(DuiWidgetController);

    // set this as the controller of the view
    view->d_ptr->controller = q;

    // give our model to the view also
    view->setModel(model);
}

void DuiWidgetController::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(DuiWidgetController);

    if (view()) {
        d->view->paint(painter, option, widget);
    }
}

QRectF DuiWidgetController::boundingRect() const
{
    if (const DuiWidgetView *v = view()) {
        QRect margins = v->margins();
        QRect reactiveMargins = v->reactiveMargins();
        QRectF br = v->boundingRect();

        // height contains bottom margin and width contains the right margin
        br.adjust(-margins.left() - reactiveMargins.left(), -margins.top() - reactiveMargins.top(),
                  margins.width() + reactiveMargins.width(), margins.height() + reactiveMargins.height());
        br.translate(margins.left(), margins.top());
        return br;
    } else {
        return QRectF();
    }
}

QPainterPath DuiWidgetController::shape() const
{
    Q_D(const DuiWidgetController);
    if (view()) {
        QPainterPath path;
        path.moveTo(d->view->marginLeft(), d->view->marginTop());
        path.addPath(d->view->shape());
        return path;
    } else {
        return QPainterPath();
    }
}

// this function takes a mouse event and translates every local(not scene) coordinate
// of it by offset p.
void translateMouseEvent(QGraphicsSceneMouseEvent *event, const QPointF &p)
{
    event->setPos(event->pos() - p);
    event->setButtonDownPos(Qt::LeftButton, event->buttonDownPos(Qt::LeftButton) - p);
    event->setButtonDownPos(Qt::RightButton, event->buttonDownPos(Qt::RightButton) - p);
    event->setButtonDownPos(Qt::MidButton, event->buttonDownPos(Qt::MidButton) - p);
    event->setButtonDownPos(Qt::XButton1, event->buttonDownPos(Qt::XButton1) - p);
    event->setButtonDownPos(Qt::XButton2, event->buttonDownPos(Qt::XButton2) - p);
    event->setLastPos(event->lastPos() - p);
}


void DuiWidgetController::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiWidgetController);
    // check if we have, or if we can create a view
    if (view()) {
        // forward the event to the view
        translateMouseEvent(event, QPointF(d->view->marginLeft(), d->view->marginTop()));
        d->view->mousePressEvent(event);
    }
}

void DuiWidgetController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiWidgetController);
    // check if we have, or if we can create a view
    if (view()) {
        // forward the event to the view
        translateMouseEvent(event, QPointF(d->view->marginLeft(), d->view->marginTop()));
        d->view->mouseReleaseEvent(event);
    }
}

void DuiWidgetController::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiWidgetController);
    // check if we have, or if we can create a view
    if (view()) {
        // forward the event to the view
        translateMouseEvent(event, QPointF(d->view->marginLeft(), d->view->marginTop()));
        d->view->mouseMoveEvent(event);
    }
}

void DuiWidgetController::cancelEvent(DuiCancelEvent *event)
{
    Q_D(DuiWidgetController);
    if (view()) {
        d->view->cancelEvent(event);
    }
}

void DuiWidgetController::orientationChangeEvent(DuiOrientationChangeEvent *event)
{
    Q_D(DuiWidgetController);
    if (view())
        d->view->orientationChangeEvent(event);
}

void DuiWidgetController::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    Q_D(DuiWidgetController);
    if (view())
        d->view->tapAndHoldGestureEvent(event,gesture);
}

void DuiWidgetController::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    Q_D(DuiWidgetController);
    if (view())
        d->view->panGestureEvent(event,gesture);
}

void DuiWidgetController::pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture)
{
    Q_D(DuiWidgetController);
    if (view())
        d->view->pinchGestureEvent(event,gesture);
}

void DuiWidgetController::tapGestureEvent(QGestureEvent *event, QTapGesture* gesture)
{
    Q_D(DuiWidgetController);
    if (view())
        d->view->tapGestureEvent(event,gesture);
}

void DuiWidgetController::swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture)
{
    Q_D(DuiWidgetController);
    if (view())
        d->view->swipeGestureEvent(event,gesture);
}

QVariant DuiWidgetController::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(DuiWidgetController);
    if (d->view) {
        // forward the event to the view
        d->view->notifyItemChange(change, value);
    }
    return DuiWidget::itemChange(change, value);
}


QVariant DuiWidgetController::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_D(const DuiWidgetController);
    if (view()) {
        return d->view->inputMethodQuery(query);
    } else {
        return QVariant();
    }
}

void DuiWidgetController::setActive(bool targetState)
{
    Q_D(DuiWidgetController);
    if (d->active != targetState) {
        d->active = targetState;
        if (view())
            d->view->setActive(targetState);
    }
}

bool DuiWidgetController::isActive() const
{
    Q_D(const DuiWidgetController);
    return d->active;
}

void DuiWidgetController::setViewType(const DuiTheme::ViewType &type)
{
    Q_D(DuiWidgetController);
    d->viewSetManually = false;
    d->deprecateView();
    model()->setViewType(type);
}

DuiTheme::ViewType DuiWidgetController::viewType() const
{
    return model()->viewType();
}

void DuiWidgetController::setGeometry(const QRectF &rect)
{
    Q_D(DuiWidgetController);
    DuiWidget::setGeometry(rect);
    // check if we have, or if we can create a view
    if (view()) {
        QRect margins = d->view->margins();
        // create a new smaller rect with topLeft-corner translated towards center
        // and bottomRight corner translated also towards center.
        QRectF viewGeom(rect.topLeft() + margins.topLeft(),
                        rect.bottomRight() - QPointF(margins.width(), margins.height()));
        d->view->setGeometry(viewGeom);
    }
}

void DuiWidgetController::setObjectName(const QString &name)
{
    DuiWidget::setObjectName(name);

    model()->setObjectName(name);
}

QSizeF DuiWidgetController::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiWidgetController);
    QSizeF sh(-1, -1);
    // check if we have, or if we can create a view
    if (view()) {
        QRect margins = d->view->margins();
        qreal widthMargin = margins.x() + margins.width();
        qreal heightMargin = margins.top() + margins.height();

        if (constraint.width() >= 0 || constraint.height() >= 0) {
            QSizeF adjustedConstraint(constraint);
            if (adjustedConstraint.width() >= 0) {
                adjustedConstraint.rwidth() -= widthMargin;
                if (adjustedConstraint.width() < 0) //what to do if the margins are bigger than the constraint?
                    adjustedConstraint.rwidth() = 0;
            }
            if (adjustedConstraint.height() >= 0) {
                adjustedConstraint.rheight() -= heightMargin;
                if (adjustedConstraint.height() < 0) //what to do if the margins are bigger than the constraint?
                    adjustedConstraint.rheight() = 0;
            }
            // fetch the size hint from the view
            sh = d->view->sizeHint(which, adjustedConstraint);
        } else
            sh = d->view->sizeHint(which, constraint);

        // unless the final size dimensions are unconstrained, we need to add margins to them
        // so the layouting system reserves enough space for the content + margins.
        if (sh.width() >= 0)
            sh.rwidth() += widthMargin;
        if (sh.height() >= 0)
            sh.rheight() += heightMargin;
    }

    if (sh.width() < 0 || sh.height() < 0) {
        QSizeF baseSizeHint = DuiWidget::sizeHint(which, constraint);
        if (sh.width() < 0) {
            sh.rwidth() = baseSizeHint.width();
        }
        if (sh.height() < 0) {
            sh.rheight() = baseSizeHint.height();
        }
    }

    return sh;
}

void DuiWidgetController::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(DuiWidgetController);
    // check if we have, or if we can create a view
    if (view()) {
        QRect margins = d->view->margins();
        QSizeF marginSize(margins.x() + margins.width(),
                          margins.top() + margins.height());

        event->setOldSize(event->oldSize() - marginSize);
        event->setNewSize(event->newSize() - marginSize);

        // forward the event to the view
        d->view->resizeEvent(event);
    }
}


void DuiWidgetController::changeEvent(QEvent *event)
{
    Q_D(DuiWidgetController);

    DuiWidget::changeEvent(event);

    if (view()) {
        d->view->changeEvent(event);
    }
}


bool DuiWidgetController::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    Q_D(DuiWidgetController);
    // check if we have, or if we can create a view
    if (view())
        return d->view->sceneEventFilter(watched, event);
    return false;
}

DuiWidgetStyleContainer &DuiWidgetController::style()
{
    return const_cast<DuiWidgetStyleContainer &>(view()->style());
}

const DuiWidgetStyleContainer &DuiWidgetController::style() const
{
    return view()->style();
}
