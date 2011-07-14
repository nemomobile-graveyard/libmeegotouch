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

//////////////
/// MSheetCentralSlot

MSheetCentralSlot::MSheetCentralSlot(QGraphicsItem *parent)
    : MStylableWidget(parent), pannableViewport(0)
{
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
            // slot will take care of removing widget from scene
            pannedSlot->setWidget(0);
        } else {
            // we have to remove widget from scene ourselves
            currentWidget->setParentItem(0);
            if (scene())
                scene()->removeItem(currentWidget);
        }
    }

    if (widget) {
        widgetPointer = widget;

        if (qobject_cast<MPannableViewport*>(widget)) {
            destroyPannableViewportAndPannedSlot();
            widget->setParentItem(this);
            widget->setPos(0.0f, 0.0f);
        } else {
            createPannableViewportAndPannedSlot();
            pannedSlot->setWidget(widget);
        }

        resizeChildWidget();
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

void MSheetCentralSlot::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    resizeChildWidget();
}

void MSheetCentralSlot::resizeChildWidget()
{
    QGraphicsWidget *widget;

    if (pannableViewport) {
        widget = pannableViewport;
    } else {
        widget = widgetPointer.data();
    }

    if (widget) {
        widget->resize(size());
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
    pannableViewport->setPos(0.0f, 0.0f);
    pannableViewport->setObjectName("MSheetCentralSlotPannableViewport");
    pannableViewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);

    pannedSlot = new MSheetSlot;
    pannedSlot->setObjectName("MSheetCentralSlotPannedSlot");
    pannedSlot->setFlag(QGraphicsItem::ItemHasNoContents);
    pannableViewport->setWidget(pannedSlot);
}

//////////////
/// MSheetViewPrivate

MSheetViewPrivate::MSheetViewPrivate()
    : q_ptr(0),
      rootLayout(0),
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

    rootLayout = new QGraphicsLinearLayout(Qt::Vertical, controller);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayoutHeaderSpacer = new MSheetSpacer;
    rootLayoutHeaderSpacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    centralSlot = new MSheetCentralSlot(controller);
    centralSlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    rootLayout->addItem(rootLayoutHeaderSpacer);
    rootLayout->addItem(centralSlot);
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

    headerHidingAnimation->addAnimation(centralAnimation);
    headerHidingAnimation->addAnimation(headerAnimation);
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
