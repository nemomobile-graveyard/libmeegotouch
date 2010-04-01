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

#include "duimenuobjectview.h"

#include <QGraphicsSceneEvent>

#include "duiapplicationmenu.h"
#include "duimenuobjectview_p.h"
#include "duibutton.h"
#include "duiviewcreator.h"

#include "duipannableviewport.h"
#include "duiwidgetaction.h"
#include "duitheme.h"
#include "duiscalableimage.h"
#include "duiscenemanager.h"
#include "duilayout.h"
#include "duilinearlayoutpolicy.h"
#include <QGraphicsLinearLayout>

DuiObjectMenuLayoutPolicy::DuiObjectMenuLayoutPolicy(DuiLayout *layout) :
    DuiAbstractLayoutPolicy(layout)
{ }

void DuiObjectMenuLayoutPolicy::setItemGeometry(QGraphicsLayoutItem *item, const QRectF &geom)
{
    geometries[item] = geom;

    QSizeF maximum = item->effectiveSizeHint(Qt::MaximumSize);
    QSizeF minimum = item->effectiveSizeHint(Qt::MinimumSize);
    QRectF new_geometry = QRectF(geom.topLeft(), geom.size().boundedTo(maximum).expandedTo(minimum));

    QRectF target(contentsArea().topLeft() + new_geometry.topLeft(), new_geometry.size());
    setItemGeometry(item, target);

    updateGeometry();
}

void DuiObjectMenuLayoutPolicy::removeAt(int index)
{
    QGraphicsLayoutItem *item = itemAt(index);
    if (item) {
        geometries.remove(item);
    }
    DuiAbstractLayoutPolicy::removeAt(index);
}

void DuiObjectMenuLayoutPolicy::relayout()
{
    QPointF topLeft = contentsArea().topLeft();

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

void DuiObjectMenuLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    DuiAbstractLayoutPolicy::addItem(item);
}

QSizeF DuiObjectMenuLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
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



DuiMenuObjectViewPrivate::DuiMenuObjectViewPrivate(DuiApplicationMenu *menu)
    : q_ptr(0), controller(menu),
      linearPolicy(0), customPolicy(0)
{
}

DuiMenuObjectViewPrivate::~DuiMenuObjectViewPrivate()
{
    removeEventFilter(controller);
}

void DuiMenuObjectViewPrivate::refreshLayout()
{
    Q_Q(DuiMenuObjectView);

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
        DuiAction *duiAction = qobject_cast<DuiAction *>(action);
        if (!duiAction) continue;
        if ((duiAction->location() & DuiAction::ObjectMenuLocation) == 0) continue;
        if (!duiAction->isVisible()) continue;

        // add to both layouts
        DuiWidget *widget = widgets.value(duiAction);
        if (!widget) {
            addWidget(action);
            widget = widgets.value(duiAction);
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

    QSize visibleSceneSize = controller->sceneManager()->visibleSceneSize(Dui::Portrait);
    // center the widgets, first the linear layout
    qreal height = visibleSceneSize.height();
    qreal offset = height - ((qreal)linearPolicy->count() * (qreal)q->style()->itemHeight());
    linearPolicy->setContentsMargins(0, offset / 2.0, 0, offset / 2.0);

    visibleSceneSize.transpose();

    // and the custom one
    height = visibleSceneSize.height();
    offset = height - ((qreal)customPolicyCount / 2.0 * (qreal)q->style()->itemHeight());
    customPolicy->setContentsMargins(0, offset / 2.0, 0, offset / 2.0);
}

void DuiMenuObjectViewPrivate::addWidget(QAction *action)
{
    Q_Q(DuiMenuObjectView);

    // check whether it is a dui action, TODO: Figure out if we need to support QActions
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (!duiAction)
        return;

    // is this an object menu item?
    if ((duiAction->location() & DuiAction::ObjectMenuLocation) == 0)
        return;

    // show only if it is visible
    if (duiAction->isVisible()) {

        // Check whether a widget was submitted for this action, if not, create button
        DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(duiAction);

        DuiWidget *widget = NULL;

        if (widgetAction) {
            widget = widgetAction->requestWidget(container);
        } else {
            // create button for this action
            DuiButton *button = new DuiButton(duiAction->text(), container);
            button->setViewType("objectmenuitem");
            button->setIconID(duiAction->iconID());

            QObject::connect(button, SIGNAL(clicked(bool)), controller, SLOT(hideMenu()));
            QObject::connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));

            // store this separately so we can later on access it
            buttons.insert(duiAction, button);
            widget = button;
        }

        if (widget) {
            // store widget so we can easily access it later on if needed
            widgets.insert(duiAction, widget);

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

void DuiMenuObjectViewPrivate::removeWidget(QAction *action)
{
    // check whether it is a dui action, TODO: Figure out if we need to support QActions
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (!duiAction)
        return;

    DuiWidget *widget = widgets.value(duiAction);
    if (widget) {
        // Remove the widget from both layouts
        linearPolicy->removeItem(widget);
        customPolicy->removeItem(widget);

        // Check whether we did the widget for this action and clean up
        DuiButton *button = buttons.value(duiAction, NULL);
        buttons.remove(duiAction);
        widgets.remove(duiAction);

        DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(duiAction);
        if (widgetAction)
            widgetAction->releaseWidget(widget);

        if (button)
            delete button;
    }
}

void DuiMenuObjectViewPrivate::actionChanged(QAction *action)
{
    DuiAction *da = qobject_cast<DuiAction *>(action);

    // check whether we did the button for this action
    if (da) {
        DuiButton *button = buttons.value(da, NULL);
        if (button) {
            // update button data accordingly
            button->setText(da->text());
            button->setIconID(da->iconID());
            button->setEnabled(da->isEnabled());
        }
    }
}

void DuiMenuObjectViewPrivate::init()
{
    // create pannable viewport
    viewport = new DuiPannableViewport(controller);

    // create container widget for pannable viewport
    container = new DuiWidget;
    viewport->setWidget(container);

    // create layout for controller, make it pannable
    QGraphicsLinearLayout *controllerLayout = new QGraphicsLinearLayout();
    controllerLayout->setContentsMargins(0, 0, 0, 0);
    controllerLayout->addItem(viewport);
    controller->setLayout(controllerLayout);

    // create layout policies for portrait & landscape orientation
    layout = new DuiLayout(container);
    linearPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    linearPolicy->setContentsMargins(0, 0, 0, 0);
    customPolicy = new DuiObjectMenuLayoutPolicy(layout);
    customPolicy->setContentsMargins(0, 0, 0, 0);

    controller->installEventFilter(this);
}

bool DuiMenuObjectViewPrivate::eventFilter(QObject *obj, QEvent *e)
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


DuiMenuObjectView::DuiMenuObjectView(DuiApplicationMenu *controller) :
    DuiSceneWindowView(controller),
    d_ptr(new DuiMenuObjectViewPrivate(controller))
{
    Q_D(DuiMenuObjectView);
    d->q_ptr = this;
    d->init();
}

DuiMenuObjectView::DuiMenuObjectView(DuiMenuObjectViewPrivate &dd, DuiApplicationMenu *controller) :
    DuiSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(DuiMenuObjectView);
    d->q_ptr = this;
    d->init();
}

DuiMenuObjectView::~DuiMenuObjectView()
{
    delete d_ptr;
}

void DuiMenuObjectView::applyStyle()
{
    Q_D(DuiMenuObjectView);

    DuiSceneWindowView::applyStyle();

    d->refreshLayout();

    // Fix item heights, according to style
    QSize preferredSize(size().width() / 2, style()->itemHeight());
    foreach(DuiWidget * widget, d->widgets) {
        widget->setPreferredSize(preferredSize);
        widget->setMinimumHeight(style()->itemHeight());
        widget->setMaximumHeight(style()->itemHeight());
    }

    // Activate proper policy
    Dui::Orientation orientation = Dui::Landscape;
    if (d->controller->sceneManager()) {
        orientation = d->controller->sceneManager()->orientation();
    }
    (orientation == Dui::Landscape) ?
    d->customPolicy->activate() :
    d->linearPolicy->activate();
}

void DuiMenuObjectView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //Do not pass the events through the menu window
    event->accept();
}

void DuiMenuObjectView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    DuiSceneWindowView::drawBackground(painter, option);

    Q_D(const DuiMenuObjectView);

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

DUI_REGISTER_VIEW_NEW(DuiMenuObjectView, DuiApplicationMenu)
