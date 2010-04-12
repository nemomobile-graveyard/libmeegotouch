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

#include "mwidgetcontroller.h"
#include "mwidgetcontroller_p.h"

#include "mwidgetmodel.h"
#include "mwidgetview.h"
#include "mwidgetview_p.h"
#include "mtheme.h"

#include <MDebug>
#include <QSet>
#include <QGraphicsSceneMouseEvent>
#include <QInputContext>
#include <QApplication>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MWidgetController)

const MTheme::ViewType MWidgetController::defaultType = "default";

// TODO: if model needs to be shared, a version of constructor is
// needed which takes model as a parameter. Some consideration is also
// needed of how the shared model is managed outside the controllers
// (e.g. deleting after detached from the last controller). Reference
// count maybe?

///////////////////
// PRIVATE CLASS //
///////////////////

QSet<MWidgetController *> MWidgetControllerPrivate::allSystemWidgets;

MWidgetControllerPrivate::MWidgetControllerPrivate() :
    model(0),
    view(0),
    viewSetManually(false),
    active(false),
    constructingView(false),
    modelSetup(false)
{
}

MWidgetControllerPrivate::~MWidgetControllerPrivate()
{
}

//////////////////
// PUBLIC CLASS //
//////////////////

MWidgetController::MWidgetController(QGraphicsItem *parent) :
    MWidget(*new MWidgetControllerPrivate, parent)
{
    setModel(new MWidgetModel);

    MWidgetControllerPrivate::allSystemWidgets.insert(this);
}

MWidgetController::MWidgetController(MWidgetModel *model, QGraphicsItem *parent) :
    MWidget(*new MWidgetControllerPrivate, parent)
{
    setModel(model == NULL ? new MWidgetModel : model);

    MWidgetControllerPrivate::allSystemWidgets.insert(this);
}

// protected constructor for derived classes
MWidgetController::MWidgetController(MWidgetControllerPrivate *dd, MWidgetModel *model, QGraphicsItem *parent) :
    MWidget(*dd, parent)
{
    //The parent class must set a model
    Q_ASSERT(model);
    setModel(model);
    MWidgetControllerPrivate::allSystemWidgets.insert(this);
}

// destructor
MWidgetController::~MWidgetController()
{
    Q_D(MWidgetController);

    // let the model know that we're not attached to it anymore.
    if (d->model) {
        d->model->decreaseReferenceCount();
        d->model = 0;
    }

    // let the view know that it can now animate away.
    if (d->view) {
        // TODO: later this should probably be done by MSceneManager.
        d->view->destroy();
        d->view = 0;
    }

    MWidgetControllerPrivate::allSystemWidgets.remove(this);
}

// const getter for model
const MWidgetModel *MWidgetController::model() const
{
    MWidgetControllerPrivate *d = (MWidgetControllerPrivate *)d_ptr;

    if (d->modelSetup == false) {
        // Clear the flag first to avoid recursion in case a derived setupModel calls model()
        d->modelSetup = true;
        const_cast<MWidgetController &>(*this).setupModel();
    }

    return d->model;
}

// getter for model
MWidgetModel *MWidgetController::model()
{
    return const_cast<MWidgetModel *>
           (static_cast<const MWidgetController &>(*this).model());
}

// const getter for view
const MWidgetView *MWidgetController::view() const
{
    Q_D(const MWidgetController);

    if (!d->view) {
        Q_ASSERT_X(d->model, "MWidgetController", "You should not call MWidgetController::view() before the widget has a model!");
        const_cast<MWidgetControllerPrivate *>(d)->createView();
    }

    return d->view;
}

// notification of model modifications
void MWidgetController::updateData(const QList<const char *>& modifications)
{
    Q_UNUSED(modifications);
}

// notification of model reset
void MWidgetController::setupModel()
{
    Q_D(MWidgetController);
    connect(d->model, SIGNAL(modified(QList<const char *>)),
            this, SLOT(updateData(QList<const char *>)));
}

void MWidgetController::updateMicroFocus()
{
    QInputContext *ic = qApp->inputContext();

    if (ic != 0) {
        ic->update();
    }
}

// method for setting new model for this controller + view
void MWidgetController::setModel(MWidgetModel *model)
{
    Q_ASSERT_X(model, "MWidgetController", "MWidgetController::setModel() parameter model has to be valid!");

    Q_D(MWidgetController);

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
void MWidgetController::setView(MWidgetView *view)
{
    Q_D(MWidgetController);

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

// this method is called by MTheme when theme is changing.
// deprecates the current view, if the user has not manually set the view
void MWidgetControllerPrivate::deprecateView()
{
    if (viewSetManually)
        return;

    if (view != NULL) {
        view->destroy();
        view = 0;
    }
}

// creates a view for this controller
void MWidgetControllerPrivate::createView()
{
    Q_Q(MWidgetController);

    // check that we don't have a view yet
    Q_ASSERT(!view);

    if (!constructingView) {
        constructingView = true;
        view = MTheme::view(q);
        constructingView = false;
    }

    if (view) {
        configureView(view);
        q->updateGeometry();
    }
}

void MWidgetControllerPrivate::configureView(MWidgetView *view)
{
    Q_Q(MWidgetController);

    // set this as the controller of the view
    view->d_ptr->controller = q;

    // give our model to the view also
    view->setModel(model);
}

void MWidgetController::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(MWidgetController);

    if (view()) {
        d->view->paint(painter, option, widget);
    }
}

QRectF MWidgetController::boundingRect() const
{
    if (const MWidgetView *v = view()) {
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

QPainterPath MWidgetController::shape() const
{
    Q_D(const MWidgetController);
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


void MWidgetController::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MWidgetController);
    // check if we have, or if we can create a view
    if (view()) {
        // forward the event to the view
        translateMouseEvent(event, QPointF(d->view->marginLeft(), d->view->marginTop()));
        d->view->mousePressEvent(event);
    }
}

void MWidgetController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MWidgetController);
    // check if we have, or if we can create a view
    if (view()) {
        // forward the event to the view
        translateMouseEvent(event, QPointF(d->view->marginLeft(), d->view->marginTop()));
        d->view->mouseReleaseEvent(event);
    }
}

void MWidgetController::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MWidgetController);
    // check if we have, or if we can create a view
    if (view()) {
        // forward the event to the view
        translateMouseEvent(event, QPointF(d->view->marginLeft(), d->view->marginTop()));
        d->view->mouseMoveEvent(event);
    }
}

void MWidgetController::cancelEvent(MCancelEvent *event)
{
    Q_D(MWidgetController);
    if (view()) {
        d->view->cancelEvent(event);
    }
}

void MWidgetController::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_D(MWidgetController);
    if (view())
        d->view->orientationChangeEvent(event);
}

void MWidgetController::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    Q_D(MWidgetController);
    if (view())
        d->view->tapAndHoldGestureEvent(event,gesture);
}

void MWidgetController::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    Q_D(MWidgetController);
    if (view())
        d->view->panGestureEvent(event,gesture);
}

void MWidgetController::pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture)
{
    Q_D(MWidgetController);
    if (view())
        d->view->pinchGestureEvent(event,gesture);
}

void MWidgetController::tapGestureEvent(QGestureEvent *event, QTapGesture* gesture)
{
    Q_D(MWidgetController);
    if (view())
        d->view->tapGestureEvent(event,gesture);
}

void MWidgetController::swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture)
{
    Q_D(MWidgetController);
    if (view())
        d->view->swipeGestureEvent(event,gesture);
}

QVariant MWidgetController::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(MWidgetController);
    if (d->view) {
        // forward the event to the view
        d->view->notifyItemChange(change, value);
    }
    return MWidget::itemChange(change, value);
}


QVariant MWidgetController::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_D(const MWidgetController);
    if (view()) {
        return d->view->inputMethodQuery(query);
    } else {
        return QVariant();
    }
}

void MWidgetController::setActive(bool targetState)
{
    Q_D(MWidgetController);
    if (d->active != targetState) {
        d->active = targetState;
        if (view())
            d->view->setActive(targetState);
    }
}

bool MWidgetController::isActive() const
{
    Q_D(const MWidgetController);
    return d->active;
}

void MWidgetController::setViewType(const MTheme::ViewType &type)
{
    Q_D(MWidgetController);
    d->viewSetManually = false;
    d->deprecateView();
    model()->setViewType(type);
}

MTheme::ViewType MWidgetController::viewType() const
{
    return model()->viewType();
}

void MWidgetController::setGeometry(const QRectF &rect)
{
    Q_D(MWidgetController);
    MWidget::setGeometry(rect);
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

void MWidgetController::setObjectName(const QString &name)
{
    MWidget::setObjectName(name);

    model()->setObjectName(name);
}

QSizeF MWidgetController::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MWidgetController);
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
        QSizeF baseSizeHint = MWidget::sizeHint(which, constraint);
        if (sh.width() < 0) {
            sh.rwidth() = baseSizeHint.width();
        }
        if (sh.height() < 0) {
            sh.rheight() = baseSizeHint.height();
        }
    }

    return sh;
}

void MWidgetController::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MWidgetController);
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


void MWidgetController::changeEvent(QEvent *event)
{
    Q_D(MWidgetController);

    MWidget::changeEvent(event);

    if (view()) {
        d->view->changeEvent(event);
    }
}


bool MWidgetController::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    Q_D(MWidgetController);
    // check if we have, or if we can create a view
    if (view())
        return d->view->sceneEventFilter(watched, event);
    return false;
}

MWidgetStyleContainer &MWidgetController::style()
{
    return const_cast<MWidgetStyleContainer &>(view()->style());
}

const MWidgetStyleContainer &MWidgetController::style() const
{
    return view()->style();
}
