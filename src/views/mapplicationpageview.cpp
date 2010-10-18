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

#include "mapplicationpageview.h"
#include "mapplicationpageview_p.h"

#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mscalableimage.h"
#include "mapplicationpage.h"
#include "mlabel.h"
#include "mscenemanager.h"
#include "mviewcreator.h"
#include "mpannableviewport.h"
#include "mondisplaychangeevent.h"
#include "mpositionindicator.h"

#include <QGraphicsLinearLayout>

const QString MApplicationPageViewPrivate::TopSpacerName = "__mt_top_spacer";
const QString MApplicationPageViewPrivate::BottomSpacerName = "__mt_bottom_spacer";

MApplicationPageViewPrivate::MApplicationPageViewPrivate()
    : q_ptr(0),
    controller(0),
    topSpacer(0),
    bottomSpacer(0),
    mainWidget(0),
    titleLabel(0),
    mainLayout(0),
    centralWidget(0)
{
}

MApplicationPageViewPrivate::~MApplicationPageViewPrivate()
{
}

void MApplicationPageViewPrivate::init()
{
    mainWidget = new MWidget;

    controller->pannableViewport()->setWidget(mainWidget);

    mainLayout = createLayout();
    mainWidget->setLayout(mainLayout);

    topSpacer = createSpacer(mainWidget);
    topSpacer->setObjectName(MApplicationPageViewPrivate::TopSpacerName);
    bottomSpacer = createSpacer(mainWidget);
    bottomSpacer->setObjectName(MApplicationPageViewPrivate::BottomSpacerName);

    setWidgetHeight(topSpacer, 0);
    setWidgetHeight(bottomSpacer, 0);

    mainLayout->addItem(topSpacer);
    mainLayout->addItem(bottomSpacer);
}


QGraphicsLinearLayout *MApplicationPageViewPrivate::createLayout()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0.0);
    return layout;
}

MWidget *MApplicationPageViewPrivate::createSpacer(QGraphicsItem *parent)
{
    MWidget *spacer = new MWidget(parent);
    return spacer;
}

void MApplicationPageViewPrivate::setWidgetHeight(MWidget *w, qreal height)
{
    w->setMinimumHeight(height);
    w->setMaximumHeight(height);
    w->setPreferredHeight(height);
}

void MApplicationPageViewPrivate::propagateOnDisplayChangeEvent(bool visible)
{
    Q_Q(MApplicationPageView);

    if (controller->scene() == 0 || controller->sceneManager() == 0)
        return;

    if (q->model()->centralWidget()) {
        QRectF viewRect(QPointF(0, 0), controller->sceneManager()->visibleSceneSize());
        if (visible) {
            MOnDisplayChangeEvent ev(MOnDisplayChangeEvent::MustBeResolved, viewRect);
            controller->scene()->sendEvent(q->model()->centralWidget(), &ev);
        } else {
            MOnDisplayChangeEvent ev(false, viewRect);
            controller->scene()->sendEvent(q->model()->centralWidget(), &ev);
        }
    }
}

void MApplicationPageViewPrivate::setCentralWidget(QGraphicsWidget *newCentralWidget)
{
    if (centralWidget)
        mainLayout->removeItem(centralWidget);

    // Place the new one
    if (newCentralWidget) {
        mainLayout->insertItem(mainLayout->count()-1, newCentralWidget);
        centralWidget = newCentralWidget;
        propagateOnDisplayChangeEvent(controller->isOnDisplay());
    }
}

void MApplicationPageViewPrivate::updateAutoMarginsForComponents()
{
    Q_Q(MApplicationPageView);

    qreal topMargin = 0.0;
    qreal bottomMargin = 0.0;

    QRectF exposedContentRect = q->model()->exposedContentRect();

    if (q->model()->autoMarginsForComponentsEnabled()) {
        topMargin = exposedContentRect.y();

        qreal exposedContentRectBottomEdge = exposedContentRect.y() + exposedContentRect.height();
        bottomMargin = q->boundingRect().height() - exposedContentRectBottomEdge;
    }

    controller->pannableViewport()->positionIndicator()->setPos(exposedContentRect.topLeft());
    setWidgetHeight(controller->pannableViewport()->positionIndicator(), exposedContentRect.height());

    setWidgetHeight(topSpacer, topMargin);
    setWidgetHeight(bottomSpacer, bottomMargin);

    // FIXME: This activation is needed so input widget relocator can correctly calculate
    // positions after decorations hide. Correct place for this would be after all
    // decorations have been hidden. Only one activation is required.
    controller->pannableViewport()->widget()->layout()->activate();
}

void MApplicationPageViewPrivate::_q_onPageStateChanged()
{
    if (controller->sceneWindowState() == MSceneWindow::Disappeared)
        resetPannableViewportPositionIfNeeded();
}

void MApplicationPageViewPrivate::resetPannableViewportPositionIfNeeded()
{
    Q_Q(MApplicationPageView);

    if (!q->model()->rememberPosition())
        controller->pannableViewport()->setPosition(QPointF(0, 0));
}

void MApplicationPageViewPrivate::updateTitleLabel()
{
    Q_Q(MApplicationPageView);

    if (q->style()->hasTitleLabel() &&
        !q->model()->title().isEmpty())
    {
        if (titleLabel)
            delete titleLabel;

        titleLabel = new MLabel;
        titleLabel->setStyleName("titleLabel");
        titleLabel->setText(q->model()->title());
        mainLayout->insertItem(1,titleLabel);
    } else {
        delete titleLabel;
        titleLabel = 0;
    }
}

MApplicationPageView::MApplicationPageView(MApplicationPage *controller) :
    MSceneWindowView(controller),
    d_ptr(new MApplicationPageViewPrivate)
{
    Q_D(MApplicationPageView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
    connect(d->controller, SIGNAL(sceneWindowStateChanged(MSceneWindow::SceneWindowState,MSceneWindow::SceneWindowState)),
            SLOT(_q_onPageStateChanged()));
}

MApplicationPageView::~MApplicationPageView()
{
    Q_D(MApplicationPageView);

    if (model()->centralWidget())
        model()->centralWidget()->setParentItem(0);
    d->setCentralWidget(0);
    d->controller->pannableViewport()->setWidget(0);

    delete d->mainWidget;
    delete d_ptr;
}

void MApplicationPageView::applyStyle()
{
    Q_D(MApplicationPageView);

    MSceneWindowView::applyStyle();
    d->updateAutoMarginsForComponents();
    d->updateTitleLabel();
}

void MApplicationPageView::setupModel()
{
    Q_D(MApplicationPageView);

    MSceneWindowView::setupModel();

    d->setCentralWidget(model()->centralWidget());
    d->updateAutoMarginsForComponents();
}

void MApplicationPageView::updateData(const QList<const char *> &modifications)
{
    Q_D(MApplicationPageView);

    MSceneWindowView::updateData(modifications);

    const char *member;

    foreach(member, modifications) {
        if (member == MApplicationPageModel::CentralWidget) {
            d->setCentralWidget(model()->centralWidget());
        } else if (member == MApplicationPageModel::AutoMarginsForComponentsEnabled ||
                   member == MApplicationPageModel::ExposedContentRect)
        {
            d->updateAutoMarginsForComponents();
        } else if (member == MApplicationPageModel::Title) {
            d->updateTitleLabel();
        }
    }
}

M_REGISTER_VIEW_NEW(MApplicationPageView, MApplicationPage)
#include "moc_mapplicationpageview.cpp"
