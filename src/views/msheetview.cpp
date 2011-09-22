/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <mscenemanager.h>
#include "mviewcreator.h"
#include <msheet.h>
#include "msheetview.h"
#include "msheetview_p.h"
#include "mslotlayout.h"
#include <mpannableviewport.h>
#include <mpositionindicator.h>
#include <mlayout.h>
#include <mlinearlayoutpolicy.h>

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsLinearLayout>

//////////////
/// MSheetSlot

MSheetSlot::MSheetSlot(QGraphicsItem *parent) : MStylableWidget(parent)
{
}

MSheetSlot::~MSheetSlot()
{
    // Widget belongs to controller. Remove it from scene graph.
    QGraphicsWidget *widget = widgetPointer.data();
    if (widget) {
        widget->setParentItem(0);
        if (scene())
            scene()->removeItem(widget);
        widgetPointer.clear();
    }
}

void MSheetSlot::setWidget(QGraphicsWidget *widget)
{
    QGraphicsWidget *currentWidget = widgetPointer.data();

    if (widget == currentWidget)
        return;

    if (currentWidget) {
        currentWidget->setParentItem(0);
        if (scene())
            scene()->removeItem(currentWidget);
    }

    if (widget) {
        widget->setParentItem(this);
        widget->setPos(0.0f, 0.0f);
        widgetPointer = widget;
        resizeChildWidget();
    } else {
        widgetPointer.clear();
    }
}

void MSheetSlot::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    resizeChildWidget();
}

void MSheetSlot::resizeChildWidget()
{
    QGraphicsWidget *widget = widgetPointer.data();
    if (widget) {
        widget->resize(size());
    }
}

void MSheetSlot::hideSlot()
{
    setVisible(false);
}

void MSheetSlot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void MSheetSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

//////////////
/// MSheetCentralSlot

MSheetCentralSlot::MSheetCentralSlot(QGraphicsItem *parent)
    : MStylableWidget(parent), pannableViewport(0), pannedSlot(0)
{
    rootLayout = new MSlotLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
}

MSheetCentralSlot::~MSheetCentralSlot()
{
    // Widget belongs to controller. Remove it from scene graph.
    QGraphicsWidget *widget = widgetPointer.data();
    if (widget) {
        if (pannableViewport) {
            pannableViewport->setWidget(0);
        }
        widget->setParentItem(0);
        scene()->removeItem(widget);
        widgetPointer.clear();
    }
}

void MSheetCentralSlot::setWidget(QGraphicsWidget *widget)
{
    QGraphicsWidget *currentWidget = widgetPointer.data();

    if (widget == currentWidget)
        return;

    if (currentWidget) {
        if (pannedSlot) {
            pannedSlot->layout()->removeAt(0);
        } else {
            rootLayout->setWidget(0);
        }
        rootLayout->setSizePolicyProxyWidget(0);
        currentWidget->setParentItem(0);
        if (scene())
            scene()->removeItem(currentWidget);
    }

    if (widget) {
        widgetPointer = widget;

        if (qobject_cast<MPannableViewport*>(widget)) {
            destroyPannableViewportAndPannedSlot();
            rootLayout->setWidget(widget);
            rootLayout->setSizePolicyProxyWidget(0);
        } else {
            createPannableViewportAndPannedSlot();
            rootLayout->setWidget(pannableViewport);
            static_cast<QGraphicsLinearLayout *>(pannedSlot->layout())
                    ->addItem(widget);
            rootLayout->setSizePolicyProxyWidget(widget);
        }
    } else {
        widgetPointer.clear();
        destroyPannableViewportAndPannedSlot();
    }
}

void MSheetCentralSlot::setPositionIndicatorStyleName(const QString& name) {
    if (!pannableViewport)
        return;

    pannableViewport->positionIndicator()->setStyleName(name);
}

void MSheetCentralSlot::setWidgetSizePolicyRespected(bool respected)
{
    rootLayout->setIgnoreSizePolicies(!respected);
}

void MSheetCentralSlot::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    if (pannedSlot) {
        pannedSlot->setMaximumSize(size());
    }
}

void MSheetCentralSlot::destroyPannableViewportAndPannedSlot()
{
    delete pannableViewport;
    pannableViewport = 0;
    pannedSlot = 0;
}

void MSheetCentralSlot::createPannableViewportAndPannedSlot()
{
    if (pannableViewport)
        return;

    pannableViewport = new MPannableViewport(this);
    pannableViewport->setAcceptGesturesFromAnyDirection(true);
    pannableViewport->setObjectName("MSheetCentralSlotPannableViewport");
    pannableViewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);

    pannedSlot = new QGraphicsWidget;
    QGraphicsLinearLayout *pannedSlotLayout =
            new QGraphicsLinearLayout(Qt::Vertical, pannedSlot);
    pannedSlotLayout->setContentsMargins(0, 0, 0, 0);
    pannedSlotLayout->setSpacing(0);
    pannedSlot->setObjectName("MSheetCentralSlotPannedSlot");
    pannedSlot->setFlag(QGraphicsItem::ItemHasNoContents);
    pannableViewport->setWidget(pannedSlot);
}

//////////////
/// MSheetViewPrivate

MSheetViewPrivate::MSheetViewPrivate()
    : q_ptr(0),
      rootLayout(0),
      rootLayoutPolicy(0),
      rootLayoutFloatingPolicy(0),
      headerSlot(0),
      headerHidingAnimation(0),
      centralSlot(0)
{
}

MSheetViewPrivate::~MSheetViewPrivate()
{
    delete headerHidingAnimation;
    headerHidingAnimation = 0;

    //rootLayout->removeItem(headerSlot);
    delete headerSlot;
    headerSlot = 0;

    //rootLayout->removeItem(centralSlot);
    delete centralSlot;
    centralSlot = 0;

    if (qobject_cast<MSheet *>(controller)) {
        // controller is still valid.
        // causes rootLayout to be deleted
        controller->setLayout(0);
    }
    rootLayout = 0;
}

void MSheetViewPrivate::init()
{
    headerSlot = new MSheetSlot(controller);
    headerSlot->setZValue(100);

    rootLayout = new MLayout(controller);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayoutPolicy = new MLinearLayoutPolicy(rootLayout, Qt::Vertical);
    rootLayoutPolicy->setContentsMargins(0, 0, 0, 0);
    rootLayoutPolicy->setSpacing(0);
    rootLayoutFloatingPolicy = new MLinearLayoutPolicy(rootLayout, Qt::Vertical);
    rootLayoutFloatingPolicy->setContentsMargins(0, 0, 0, 0);
    rootLayoutFloatingPolicy->setSpacing(0);
    rootLayoutHeaderSpacer = new MSheetSpacer;
    rootLayoutHeaderSpacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    centralSlot = new MSheetCentralSlot(controller);
    centralSlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    centralSlot->setZValue(0);

    // normal layout
    rootLayoutPolicy->addItem(rootLayoutHeaderSpacer);
    rootLayoutPolicy->addItem(centralSlot);

    // floating header layout
    rootLayoutFloatingPolicy->addItem(centralSlot);
}

void MSheetViewPrivate::updateStyle()
{
    Q_Q(MSheetView);

    headerSlot->setStyleName(q->style()->headerSlotStyleName());
    centralSlot->setStyleName(q->style()->centralSlotStyleName());

    rootLayoutHeaderSpacer->setMinimumHeight(headerSlot->preferredHeight());

    // Since headerSlot's geometry is not being managed by a QGraphicsLayout we
    // have to set its geometry ourselves.
    headerSlot->setGeometry(QRectF(QPointF(0.0f, 0.0f), headerSlot->preferredSize()));

    centralSlot->setPositionIndicatorStyleName(
                q->style()->centralSlotPositionIndicatorStyleName());
}

void MSheetViewPrivate::updateHeaderVisibility()
{
    Q_Q(MSheetView);

    if (controller->sceneWindowState() != MSceneWindow::Appeared)
        return;

    if (!headerHidingAnimation)
        setupHeaderHidingAnimation();

    if (q->model()->headerVisible()) {
        headerHidingAnimation->setDirection(QAbstractAnimation::Backward);
        q->disconnect(headerHidingAnimation, SIGNAL(finished()), headerSlot, SLOT(hideSlot()));
    } else {
        headerHidingAnimation->setDirection(QAbstractAnimation::Forward);
        q->connect(headerHidingAnimation, SIGNAL(finished()), headerSlot, SLOT(hideSlot()));
    }

    if (!headerSlot->isVisible())
        headerSlot->setVisible(true);
    headerHidingAnimation->start();
}

void MSheetViewPrivate::setupHeaderHidingAnimation()
{
    headerHidingAnimation = new QParallelAnimationGroup;

    //TODO: create animation class so that these parameters are stylable
    QPropertyAnimation* headerAnimation = new QPropertyAnimation;
    headerAnimation->setTargetObject(headerSlot);
    headerAnimation->setPropertyName("y");
    headerAnimation->setStartValue(0);
    headerAnimation->setEndValue(-headerSlot->preferredHeight());
    headerAnimation->setEasingCurve(QEasingCurve::Linear);
    headerAnimation->setDuration(300);

    QPropertyAnimation* centralAnimation = new QPropertyAnimation;
    centralAnimation->setTargetObject(rootLayoutHeaderSpacer);
    centralAnimation->setPropertyName("maximumHeight");
    centralAnimation->setStartValue(headerSlot->minimumHeight());
    centralAnimation->setEndValue(0);
    centralAnimation->setEasingCurve(QEasingCurve::Linear);
    centralAnimation->setDuration(300);

    headerHidingAnimation->addAnimation(centralAnimation);
    headerHidingAnimation->addAnimation(headerAnimation);
}

void MSheetViewPrivate::updateRootLayoutActivePolicy()
{
    Q_Q(MSheetView);

    if (q->model()->headerFloating())
        rootLayoutFloatingPolicy->activate();
    else
        rootLayoutPolicy->activate();
}

//////////////
/// MSheetView

MSheetView::MSheetView(MSheet *controller) :
    MSceneWindowView(*(new MSheetViewPrivate), controller)
{
    Q_D(MSheetView);
    d->q_ptr = this;
    d->init();
}

MSheetView::~MSheetView()
{
}

void MSheetView::setupModel()
{
    MSceneWindowView::setupModel();

    QList<const char*> modifications;
    modifications << MSheetModel::HeaderWidget;
    modifications << MSheetModel::CentralWidget;
    modifications << MSheetModel::HeaderVisible;
    modifications << MSheetModel::CentralWidgetSizePolicyRespected;
    modifications << MSheetModel::HeaderFloating;

    updateData(modifications);
}

void MSheetView::updateData(const QList<const char *> &modifications)
{
    Q_D(MSheetView);
    MSceneWindowView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == MSheetModel::HeaderVisible)
            d->updateHeaderVisibility();
        else if (member == MSheetModel::CentralWidget) {
            d->centralSlot->setWidget(model()->centralWidget());
        } else if (member == MSheetModel::HeaderWidget)
            d->headerSlot->setWidget(model()->headerWidget());
        else if (member == MSheetModel::CentralWidgetSizePolicyRespected) {
            d->centralSlot->setWidgetSizePolicyRespected(
                model()->centralWidgetSizePolicyRespected());
        } else if (member == MSheetModel::HeaderFloating) {
            d->updateRootLayoutActivePolicy();
        }
    }
}

void MSheetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't let it propagate to widgets below
    event->accept();
}

void MSheetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Don't let it propagate to widgets below
    event->accept();
}

void MSheetView::applyStyle()
{
    Q_D(MSheetView);

    MSceneWindowView::applyStyle();
    d->updateStyle();
}

M_REGISTER_VIEW_NEW(MSheetView, MSheet)
