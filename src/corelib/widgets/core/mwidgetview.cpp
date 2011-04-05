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
#include <mscene.h>

#include "mdebug.h"

#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QPanGesture>
#include <QPinchGesture>
#include <QTapGesture>
#include <QSwipeGesture>
#include <QStyleOptionGraphicsItem>

MWidgetViewPrivate::MWidgetViewPrivate() :
    q_ptr(NULL),
    controller(NULL),
    model(NULL),
    styleContainer(NULL),
    appliedStyle(NULL),
    margins(),
    reactiveMargins()
{
}

MWidgetViewPrivate::~MWidgetViewPrivate()
{
}

void MWidgetViewPrivate::setActive(bool active)
{
    if (active) {
        styleContainer->setModeActive();
    } else {
        styleContainer->setModeDefault();
    }

    const_cast<MWidgetView*>(controller->view())->applyStyle();
}

void MWidgetViewPrivate::setEnabled(bool enabled)
{
    if (enabled)
        styleContainer->setModeDefault();
    else
        styleContainer->setModeDisabled();

    // Apply style mode recursively to child items
    foreach(QGraphicsItem *item, controller->childItems()) {
        MWidgetController *widget = dynamic_cast<MWidgetController*>(item);
        if (widget) {
            MWidgetViewPrivate *viewPrivate = const_cast<MWidgetViewPrivate*>(widget->view()->d_func());
            if (enabled && widget->isEnabled())
                viewPrivate->setEnabled(true);
            else
                viewPrivate->setEnabled(false);
        }
    }

    const_cast<MWidgetView*>(controller->view())->applyStyle();
}

void MWidgetViewPrivate::setSelected(bool selected)
{
    if (selected)
        styleContainer->setModeSelected();
    else
        styleContainer->setModeDefault();

    // Apply style mode recursively to child items
    foreach(QGraphicsItem *item, controller->childItems()) {
        MWidgetController *widget = dynamic_cast<MWidgetController*>(item);
        if (widget) {
            MWidgetViewPrivate *viewPrivate = const_cast<MWidgetViewPrivate*>(widget->view()->d_func());
            if (selected || widget->isSelected())
                viewPrivate->setSelected(true);
            else
                viewPrivate->setSelected(false);
        }
    }

    const_cast<MWidgetView*>(controller->view())->applyStyle();
}

void MWidgetViewPrivate::setPressed(bool pressed)
{
    if (pressed)
        styleContainer->setModePressed();
    else
        styleContainer->setModeDefault();

    // Apply style mode recursively to child items
    foreach(QGraphicsItem *item, controller->childItems()) {
        MWidgetController *widget = dynamic_cast<MWidgetController*>(item);
        if (widget) {
            MWidgetViewPrivate *viewPrivate = const_cast<MWidgetViewPrivate*>(widget->view()->d_func());
            viewPrivate->setPressed(pressed);
        }
    }

    const_cast<MWidgetView*>(controller->view())->applyStyle();
}

const MWidgetStyle *MWidgetViewPrivate::currentStyle() const
{
    return static_cast<const MWidgetStyle *>(styleContainer->currentStyle());
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
    Q_D(MWidgetView);

    d->setActive(active);
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
    while (p && !(p->isWidget() &&
                  (parent = qobject_cast<MWidgetController *>(static_cast<QGraphicsWidget *>(p))))) {
        p = p->parentItem();
    }

    //The default style type is "", not "default".
    QString styleType = d->model->viewType();
    if (styleType == MWidgetController::defaultType)
        styleType.clear();

    QString styleName;
    if (d->model->styleName().isNull())
        styleName = d->model->objectName(); // fallback to old behavior
    else
        styleName = d->model->styleName();
    d->styleContainer->setSceneManager(d->controller->sceneManager());
    d->styleContainer->initialize(styleName, styleType, parent);

    if (d->controller->isActive()) {
        setActive(true);
    } else if (!d->controller->isEnabled()) {
        notifyItemChange(QGraphicsItem::ItemEnabledHasChanged, false);
    } else if (d->controller->isSelected()) {
        notifyItemChange(QGraphicsItem::ItemSelectedHasChanged, true);
    } else {
        applyStyle();
    }

    // notify derived classes
    setupModel();
}

void MWidgetView::updateData(const QList<const char *>& modifications)
{
    const char *member;
    foreach(member, modifications) {
        if (member == MWidgetModel::ObjectName ||
            member == MWidgetModel::StyleName) {
            // style name has changed, we need to update the style
            QString styleName(QString::null);
            if (model()->styleName().isNull())
                styleName = model()->objectName(); // fallback to old behavior
            else
                styleName = model()->styleName();
            style().setObjectName(styleName);
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

    style().updateCurrentStyle();
    const MWidgetStyle *s = d->currentStyle();

    d->margins = QMargins(s->marginLeft(), s->marginTop(), s->marginRight(), s->marginBottom());
    d->reactiveMargins = QMargins(s->reactiveMarginLeft(), s->reactiveMarginTop(), s->reactiveMarginRight(), s->reactiveMarginBottom());

    if (d->controller->layoutDirection() == Qt::RightToLeft)
        d->controller->setContentsMargins(
            s->paddingRight() + d->margins.right(),
            s->paddingTop() + d->margins.top(),
            s->paddingLeft() + d->margins.left(),
            s->paddingBottom() + d->margins.bottom());
    else
        d->controller->setContentsMargins(
            s->paddingLeft() + d->margins.left(),
            s->paddingTop() + d->margins.top(),
            s->paddingRight() + d->margins.right(),
            s->paddingBottom() + d->margins.bottom());

    updateGeometry();
    update();

    d->appliedStyle = s;
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
    Q_D(const MWidgetView);
    return QRect(d->margins.left(),
                 d->margins.top(),
                 d->margins.right(),
                 d->margins.bottom());
}

int MWidgetView::marginLeft() const
{
    Q_D(const MWidgetView);
    return d->margins.left();
}

int MWidgetView::marginTop() const
{
    Q_D(const MWidgetView);
    return d->margins.top();
}

int MWidgetView::marginRight() const
{
    Q_D(const MWidgetView);
    return d->margins.right();
}

int MWidgetView::marginBottom() const
{
    Q_D(const MWidgetView);
    return d->margins.bottom();
}

QRect MWidgetView::reactiveMargins() const
{
    Q_D(const MWidgetView);
    return QRect(d->reactiveMargins.left(),
                 d->reactiveMargins.top(),
                 d->reactiveMargins.right(),
                 d->reactiveMargins.bottom());
}

int MWidgetView::reactiveMarginLeft() const
{
    Q_D(const MWidgetView);
    return d->reactiveMargins.left();
}

int MWidgetView::reactiveMarginTop() const
{
    Q_D(const MWidgetView);
    return d->reactiveMargins.top();
}

int MWidgetView::reactiveMarginRight() const
{
    Q_D(const MWidgetView);
    return d->reactiveMargins.right();
}

int MWidgetView::reactiveMarginBottom() const
{
    Q_D(const MWidgetView);
    return d->reactiveMargins.bottom();
}

QRectF MWidgetView::boundingRect() const
{
    return QRectF(QPointF(), size());
}

QPainterPath MWidgetView::shape() const
{
    Q_D(const MWidgetView);
    QPainterPath path;
    QRectF br = boundingRect();
    br.setRect(br.x() - d->reactiveMargins.left(),
               br.y() - d->reactiveMargins.top(),
               br.width()  + d->margins.left() + d->margins.right() + d->reactiveMargins.left() +  d->reactiveMargins.right(),
               br.height() + d->margins.top() + d->margins.bottom() + d->reactiveMargins.top() + d->reactiveMargins.bottom());
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

void MWidgetViewPrivate::reloadChildItemsStyles(QGraphicsItem* item)
{
    foreach(QGraphicsItem* child, item->childItems()) {
        reloadChildItemsStyles(child);

        if (!child->isWidget()) {
            continue;
        }
        QGraphicsWidget *graphicsWidget = static_cast<QGraphicsWidget*>(child);
        MWidgetController *widget = qobject_cast<MWidgetController*>(graphicsWidget);

        MWidgetView *view = widget ? const_cast<MWidgetView*>(widget->view()) : NULL;
        if (view) {
            MWidgetStyleContainer& style = view->style();
            style.reloadStyles();
            view->applyStyle();
        }
    }
}

void MWidgetView::notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    Q_D(MWidgetView);
    if (change == QGraphicsItem::ItemEnabledHasChanged) {
        d->setEnabled(d->controller->isEnabled());
    } else if (change == QGraphicsItem::ItemSelectedHasChanged) {
        if (d->controller->isEnabled()) {
            d->setSelected(d->controller->isSelected());
        }
    } else if (change == QGraphicsItem::ItemParentHasChanged) {
        MWidgetController *parent = NULL;
        QGraphicsItem *p = d->controller->parentItem();
        while (p && !(parent = dynamic_cast<MWidgetController *>(p))) {
            p = p->parentItem();
        }

        style().setParent(parent);
        if (value.value<QGraphicsItem*>() != NULL) {
            mPerformanceWarning("MWidgetView::reloadChildItemsStyles") << "Styles are recursively reloaded due to parent change :" << d->controller;
            d->reloadChildItemsStyles(d->controller);
            applyStyle();
        }
    } else if (change == QGraphicsItem::ItemSceneChange) {
        QGraphicsScene *scene = value.value<QGraphicsScene*>();
        MScene *mscene = 0;
        if (scene)
            mscene = qobject_cast<MScene*>(scene);

        if (mscene)
            d->styleContainer->setSceneManager(mscene->sceneManager());
        else
            d->styleContainer->setSceneManager(0);
    }
}
void MWidgetView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MWidgetView::cancelEvent(MCancelEvent *event)
{
    style()->cancelFeedback().play();

    Q_UNUSED(event);
}

void MWidgetView::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_UNUSED(event);
    Q_D(MWidgetView);

    style().updateCurrentStyle();
    if (d->currentStyle() != d->appliedStyle) {
        applyStyle();
        d->appliedStyle = d->currentStyle();
    }
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
    return QRectF(controllerGeom.topLeft() + QPointF(d->margins.left(), d->margins.top()),
                  controllerGeom.bottomRight() - QPointF(d->margins.right(), d->margins.bottom()));
}

QSizeF MWidgetView::size() const
{
    Q_D(const MWidgetView);
    return d->controller->size() - QSizeF(d->margins.left() + d->margins.right(), d->margins.top() + d->margins.bottom());
}

QPointF MWidgetView::pos() const
{
    Q_D(const MWidgetView);
    return d->controller->pos() + QPointF(d->margins.left(), d->margins.top());
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
    if (event->type() == QEvent::LayoutDirectionChange
        || event->type() == QEvent::StyleChange) {
        applyStyle();
    }
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
    Q_UNUSED(which);
    Q_UNUSED(constraint);
    return QSizeF(-1,-1);
}

void MWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(MWidgetView);
    Q_UNUSED(widget);

    int horizontalMargin = 0;
    if (d->controller->layoutDirection() == Qt::LeftToRight)
        horizontalMargin = d->margins.left();
    else
        horizontalMargin = d->margins.right();

    painter->translate(horizontalMargin, d->margins.top());

    if (option) {
        QStyleOptionGraphicsItem translatedOption(*option);
        translatedOption.exposedRect.translate(-horizontalMargin,
                                               -d->margins.top());
        drawBackground(painter, &translatedOption);
        drawContents(painter, &translatedOption);
    } else {
        drawBackground(painter, 0);
        drawContents(painter, 0);
    }

    painter->translate(-horizontalMargin, -d->margins.top());
    drawForeground(painter, option);
}

void MWidgetView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    QSizeF currentSize = size();

    if (currentSize.width() == 0 || currentSize.height() == 0)
        return;

    const MWidgetStyle *s = style().operator ->();

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(oldOpacity * s->backgroundOpacity() * effectiveOpacity());

    if (s->backgroundTiles().isValid()) {
        s->backgroundTiles()[model()->layoutPosition()]->draw(0.0,0.0, currentSize.width(), currentSize.height(), painter);
    }
    else if (s->backgroundImage()) {
        s->backgroundImage()->draw(0.0, 0.0, currentSize.width(), currentSize.height(), painter);
    } else if (s->backgroundColor().isValid()) {
        painter->fillRect(boundingRect(), QBrush(s->backgroundColor()));
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
