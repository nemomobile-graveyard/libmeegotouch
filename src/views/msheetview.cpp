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
#include <mpannableviewport.h>
#include <mpositionindicator.h>

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

//////////////
/// MSheetViewPrivate

MSheetViewPrivate::MSheetViewPrivate()
    : q_ptr(0),
      rootLayout(0),
      headerSlot(0),
      centralSlot(0),
      centralSlotPannableViewport(0),
      animationGroup(0)
{
}

MSheetViewPrivate::~MSheetViewPrivate()
{
    delete animationGroup;
    animationGroup = 0;

    //rootLayout->removeItem(headerSlot);
    delete headerSlot;
    headerSlot = 0;

    //rootLayout->removeItem(centralSlot);
    delete centralSlotPannableViewport;
    centralSlotPannableViewport = 0;

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

    rootLayout = new QGraphicsLinearLayout(Qt::Vertical, controller);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayoutHeaderSpacer = new MSheetSpacer;
    rootLayoutHeaderSpacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // The sole purpose of this internal pannable is to guarantee proper input widget relocation
    // if the central widget doesn't have a pannable viewport.
    centralSlotPannableViewport = new MPannableViewport(controller);
    centralSlotPannableViewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    centralSlotPannableViewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
    centralSlotPannableViewport->setObjectName("MSheetCentralSlotPannableViewport");
    rootLayout->addItem(rootLayoutHeaderSpacer);
    rootLayout->addItem(centralSlotPannableViewport);

    centralSlot = new MSheetSlot(centralSlotPannableViewport);
    centralSlotPannableViewport->setWidget(centralSlot);
}

void MSheetViewPrivate::updateStyle()
{
    Q_Q(MSheetView);

    headerSlot->setStyleName(q->style()->headerSlotStyleName());
    centralSlot->setStyleName(q->style()->centralSlotStyleName());

    headerSlot->setMinimumWidth(centralSlot->geometry().width());
    rootLayoutHeaderSpacer->setMinimumHeight(headerSlot->preferredHeight());

    centralSlotPannableViewport->positionIndicator()->setStyleName(
                q->style()->centralSlotPositionIndicatorStyleName());
}

void MSheetViewPrivate::updateHeaderVisibility()
{
    Q_Q(MSheetView);

    if (controller->sceneWindowState() != MSceneWindow::Appeared)
        return;

    if (!animationGroup)
        setupAnimation();

    if (q->model()->headerVisible())
        animationGroup->setDirection(QAbstractAnimation::Backward);
    else
        animationGroup->setDirection(QAbstractAnimation::Forward);

    animationGroup->start();
}

void MSheetViewPrivate::setupAnimation()
{
    animationGroup = new QParallelAnimationGroup;

    //TODO: create animation class so that these parameters are stylable
    QPropertyAnimation* headerAnimation = new QPropertyAnimation;
    headerAnimation->setTargetObject(headerSlot);
    headerAnimation->setPropertyName("y");
    headerAnimation->setStartValue(0);
    headerAnimation->setEndValue(-headerSlot->geometry().height());
    headerAnimation->setEasingCurve(QEasingCurve::Linear);
    headerAnimation->setDuration(300);

    QPropertyAnimation* centralAnimation = new QPropertyAnimation;
    centralAnimation->setTargetObject(rootLayoutHeaderSpacer);
    centralAnimation->setPropertyName("maximumHeight");
    centralAnimation->setStartValue(headerSlot->minimumHeight());
    centralAnimation->setEndValue(0);
    centralAnimation->setEasingCurve(QEasingCurve::Linear);
    centralAnimation->setDuration(300);

    animationGroup->addAnimation(centralAnimation);
    animationGroup->addAnimation(headerAnimation);
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
            if (qobject_cast<MPannableViewport*>(model()->centralWidget()))
                // our internal pannable is not needed at all in this case
                // TODO: consider removing centralSlotPannableViewport from the widget hierarchy altogether
                //       if there's any performance benefit in doing so
                d->centralSlotPannableViewport->setVerticalPanningPolicy(MPannableViewport::PanningAlwaysOff);
            else
                d->centralSlotPannableViewport->setVerticalPanningPolicy(MPannableViewport::PanningAsNeeded);
            d->centralSlot->setWidget(model()->centralWidget());
        } else if (member == MSheetModel::HeaderWidget)
            d->headerSlot->setWidget(model()->headerWidget());
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
