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

#include "mmenuobjectview.h"

#include <QGraphicsSceneEvent>

#include "mapplicationmenu.h"
#include "mmenuobjectview_p.h"
#include "mbutton.h"
#include "mviewcreator.h"

#include "mpannableviewport.h"
#include "mwidgetaction.h"
#include "mtheme.h"
#include "mscalableimage.h"
#include "mscenemanager.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include <QGraphicsLinearLayout>

MObjectMenuLayoutPolicy::MObjectMenuLayoutPolicy(MLayout *layout) :
    MAbstractLayoutPolicy(layout)
{ }

void MObjectMenuLayoutPolicy::setItemGeometry(QGraphicsLayoutItem *item, const QRectF &geom)
{
    geometries[item] = geom;

    QSizeF maximum = item->effectiveSizeHint(Qt::MaximumSize);
    QSizeF minimum = item->effectiveSizeHint(Qt::MinimumSize);
    QRectF new_geometry = QRectF(geom.topLeft(), geom.size().boundedTo(maximum).expandedTo(minimum));

    QRectF target(contentsArea().topLeft() + new_geometry.topLeft(), new_geometry.size());
    setItemGeometry(item, target);

    updateGeometry();
}

void MObjectMenuLayoutPolicy::removeAt(int index)
{
    QGraphicsLayoutItem *item = itemAt(index);
    if (item) {
        geometries.remove(item);
    }
    MAbstractLayoutPolicy::removeAt(index);
}

void MObjectMenuLayoutPolicy::relayout()
{
    int i = count();
    while (--i >= 0) {
        QGraphicsLayoutItem *item = itemAt(i);
        QRectF new_geometry = geometries[item];
        QSizeF maximum = item->effectiveSizeHint(Qt::MaximumSize);
        QSizeF minimum = item->effectiveSizeHint(Qt::MinimumSize);
        new_geometry = QRectF(new_geometry.topLeft(), new_geometry.size().boundedTo(maximum).expandedTo(minimum));

        QPointF topLeft = contentsArea().topLeft();
        QRectF target(topLeft + new_geometry.topLeft(), new_geometry.size());
        setItemGeometry(item, target);
    }
}

void MObjectMenuLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    MAbstractLayoutPolicy::addItem(item);
}

QSizeF MObjectMenuLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if (which == Qt::MaximumSize) {
        // maximum size is the QWIDGETSIZE_MAX
        QSizeF new_size = constraint;
        if (new_size.width() < 0)
            new_size.setWidth(QWIDGETSIZE_MAX);
        if (new_size.height() < 0)
            new_size.setHeight(QWIDGETSIZE_MAX);
        return new_size;
    }

    // minimum and preferred size of a layout is the bounding box of the children
    int i = count();
    QRectF boundingBox;
    while (--i >= 0) {
        // iterate through children
        boundingBox = boundingBox.united(itemGeometry(i));
    }

    qreal right, bottom;
    layout()->getContentsMargins(NULL, NULL, &right, &bottom);
    return QSizeF(boundingBox.right() + right - layout()->geometry().left(), boundingBox.bottom() + bottom - layout()->geometry().top());
}



MMenuObjectViewPrivate::MMenuObjectViewPrivate(MApplicationMenu *menu)
    : q_ptr(0), controller(menu), viewport(0), container(0), layout(0),
      linearPolicy(0), customPolicy(0)
{
}

MMenuObjectViewPrivate::~MMenuObjectViewPrivate()
{
    removeEventFilter(controller);
}

void MMenuObjectViewPrivate::refreshLayout()
{
    Q_Q(MMenuObjectView);

    if (controller->sceneManager() == 0)
        return;

    // refresh layout items
    int i;
    for (i = customPolicy->count() - 1; i >= 0; --i) {
        customPolicy->removeAt(i);
        linearPolicy->removeAt(i);
    }

    QList<QAction *> actions = controller->actions();
    const int actionsCount = actions.count();
    for (i = 0; i < actionsCount; ++i) {
        QAction *action = actions[i];
        MAction *mAction = qobject_cast<MAction *>(action);
        if (!mAction) continue;
        if ((mAction->location() & MAction::ObjectMenuLocation) == 0) continue;
        if (!mAction->isVisible()) continue;

        // add to both layouts
        MWidget *widget = widgets.value(mAction);
        if (!widget) {
            addWidget(action);
            widget = widgets.value(mAction);
        }

        if (widget) {
            linearPolicy->addItem(widget);
            customPolicy->addItem(widget);
        }
    }

    const int width = q->size().width();

    // if item count < 4, make it linear
    const int customPolicyCount = customPolicy->count();
    if (customPolicy->count() < 4) {
        for (i = 0; i < customPolicyCount; ++i) {
            QGraphicsLayoutItem *item = customPolicy->itemAt(i);
            QRectF rect(0, 0, 0, 0);
            rect.setLeft(width / 4);
            rect.setTop(i * q->style()->itemHeight());
            rect.setWidth(width / 2);
            rect.setHeight(q->style()->itemHeight());
            customPolicy->setItemGeometry(item, rect);
        }
    } else { // otherwise make it two columns so that if the item count in both columns are not equal, the other is vertically centered
        const bool even_count = (customPolicyCount % 2) == 0 ? true : false;
        int row_count = customPolicyCount / 2;

        if (!even_count)
            row_count++;

        // first column
        for (i = 0; i < row_count; ++i) {
            QGraphicsLayoutItem *item = customPolicy->itemAt(i);
            QRectF rect(0, 0, 0, 0);
            rect.setTop(i * q->style()->itemHeight());
            rect.setWidth(width / 2);
            rect.setHeight(q->style()->itemHeight());
            customPolicy->setItemGeometry(item, rect);
        }

        // second column
        for (i = row_count; i < customPolicyCount; ++i) {
            QGraphicsLayoutItem *item = customPolicy->itemAt(i);

            int row = i - row_count;
            QRectF rect(0, 0, 0, 0);
            rect.setTop(row * q->style()->itemHeight());
            rect.setLeft((width / 2));

            if (!even_count) {
                rect.setTop(rect.top() + (q->style()->itemHeight() / 2));
            }

            rect.setWidth(width / 2);
            rect.setHeight(q->style()->itemHeight());
            customPolicy->setItemGeometry(item, rect);
        }
    }

    QSize visibleSceneSize = controller->sceneManager()->visibleSceneSize(M::Portrait);
    // center the widgets, first the linear layout
    qreal height = visibleSceneSize.height();
    qreal offset = height - ((qreal)linearPolicy->count() * (qreal)q->style()->itemHeight());
    linearPolicy->setContentsMargins(0, offset / 2.f, 0, offset / 2.f);

    visibleSceneSize.transpose();

    // and the custom one
    height = visibleSceneSize.height();
    offset = height - ((qreal)customPolicyCount / 2.f * (qreal)q->style()->itemHeight());
    customPolicy->setContentsMargins(0, offset / 2.f, 0, offset / 2.f);
}

void MMenuObjectViewPrivate::addWidget(QAction *action)
{
    Q_Q(MMenuObjectView);

    // check whether it is a m action, TODO: Figure out if we need to support QActions
    MAction *mAction = qobject_cast<MAction *>(action);
    if (!mAction)
        return;

    // is this an object menu item?
    if ((mAction->location() & MAction::ObjectMenuLocation) == 0)
        return;

    // show only if it is visible
    if (mAction->isVisible()) {

        // Check whether a widget was submitted for this action, if not, create button
        MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(mAction);

        MWidget *widget = NULL;

        if (widgetAction) {
            widget = widgetAction->requestWidget(container);
        } else {
            // create button for this action
            MButton *button = new MButton(mAction->text(), container);
            button->setViewType("objectmenuitem");
            button->setIconID(mAction->iconID());

            QObject::connect(button, SIGNAL(clicked(bool)), controller, SLOT(hideMenu()));
            QObject::connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));

            // store this separately so we can later on access it
            buttons.insert(mAction, button);
            widget = button;
        }

        if (widget) {
            // store widget so we can easily access it later on if needed
            widgets.insert(mAction, widget);

            // TODO: This could be better
            QSize preferredSize(q->size().width() / 2, q->style()->itemHeight());
            widget->setPreferredSize(preferredSize);
            widget->setMinimumHeight(q->style()->itemHeight());
            widget->setMaximumHeight(q->style()->itemHeight());
            widget->setVisible(true);
            widget->setEnabled(action->isEnabled());

        }
    }
}

void MMenuObjectViewPrivate::removeWidget(QAction *action)
{
    // check whether it is a m action, TODO: Figure out if we need to support QActions
    MAction *mAction = qobject_cast<MAction *>(action);
    if (!mAction)
        return;

    MWidget *widget = widgets.value(mAction);
    if (widget) {
        // Remove the widget from both layouts
        linearPolicy->removeItem(widget);
        customPolicy->removeItem(widget);

        // Check whether we did the widget for this action and clean up
        MButton *button = buttons.value(mAction, NULL);
        buttons.remove(mAction);
        widgets.remove(mAction);

        MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(mAction);
        if (widgetAction)
            widgetAction->releaseWidget(widget);

        if (button)
            delete button;
    }
}

void MMenuObjectViewPrivate::actionChanged(QAction *action)
{
    MAction *da = qobject_cast<MAction *>(action);

    // check whether we did the button for this action
    if (da) {
        MButton *button = buttons.value(da, NULL);
        if (button) {
            // update button data accordingly
            button->setText(da->text());
            button->setIconID(da->iconID());
            button->setEnabled(da->isEnabled());
        }
    }
}

void MMenuObjectViewPrivate::init()
{
    // create pannable viewport
    viewport = new MPannableViewport(controller);
    viewport->setAcceptGesturesFromAnyDirection(true);

    // create container widget for pannable viewport
    container = new MWidget;
    viewport->setWidget(container);

    // create layout for controller, make it pannable
    QGraphicsLinearLayout *controllerLayout = new QGraphicsLinearLayout();
    controllerLayout->setContentsMargins(0, 0, 0, 0);
    controllerLayout->addItem(viewport);
    controller->setLayout(controllerLayout);

    // create layout policies for portrait & landscape orientation
    layout = new MLayout(container);
    linearPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    linearPolicy->setContentsMargins(0, 0, 0, 0);
    customPolicy = new MObjectMenuLayoutPolicy(layout);
    customPolicy->setContentsMargins(0, 0, 0, 0);

    controller->installEventFilter(this);
}

bool MMenuObjectViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
    QActionEvent *actionEvent = dynamic_cast<QActionEvent *>(e);

    if (actionEvent) {
        QAction *action = actionEvent->action();
        switch (e->type()) {
        case QEvent::ActionAdded:
            addWidget(action);
            refreshLayout();
            break;
        case QEvent::ActionRemoved:
            removeWidget(action);
            break;
        case QEvent::ActionChanged:
            actionChanged(action);
            break;
        default:
            break;
        }
    }

    return QObject::eventFilter(obj, e);
}


MMenuObjectView::MMenuObjectView(MApplicationMenu *controller) :
    MSceneWindowView(controller),
    d_ptr(new MMenuObjectViewPrivate(controller))
{
    Q_D(MMenuObjectView);
    d->q_ptr = this;
    d->init();
}

MMenuObjectView::MMenuObjectView(MMenuObjectViewPrivate &dd, MApplicationMenu *controller) :
    MSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(MMenuObjectView);
    d->q_ptr = this;
    d->init();
}

MMenuObjectView::~MMenuObjectView()
{
    delete d_ptr;
}

void MMenuObjectView::applyStyle()
{
    Q_D(MMenuObjectView);

    MSceneWindowView::applyStyle();

    d->refreshLayout();

    // Fix item heights, according to style
    QSize preferredSize(size().width() / 2, style()->itemHeight());
    foreach(MWidget * widget, d->widgets) {
        widget->setPreferredSize(preferredSize);
        widget->setMinimumHeight(style()->itemHeight());
        widget->setMaximumHeight(style()->itemHeight());
    }

    // Activate proper policy
    M::Orientation orientation = M::Landscape;
    if (d->controller->sceneManager()) {
        orientation = d->controller->sceneManager()->orientation();
    }
    (orientation == M::Landscape) ?
    d->customPolicy->activate() :
    d->linearPolicy->activate();
}

void MMenuObjectView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //Do not pass the events through the menu window
    event->accept();
}

void MMenuObjectView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    MSceneWindowView::drawBackground(painter, option);

    Q_D(const MMenuObjectView);

    if (style()->canvasOpacity() > 0.0) {
        // draw canvas
        painter->setOpacity(d->controller->effectiveOpacity() * style()->canvasOpacity());

        QRectF canvasRect(QPoint(0, 0), size());
        if (style()->canvasImage())
            style()->canvasImage()->draw(canvasRect.toRect(), painter);
        else {
            QColor color = style()->canvasColor();
            painter->fillRect(canvasRect, QBrush(color));
        }
    }
}

M_REGISTER_VIEW_NEW(MMenuObjectView, MApplicationMenu)
