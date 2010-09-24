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

#include "mnavigationbarview.h"
#include "mnavigationbarview_p.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

#include "mapplication.h"
#include "mnavigationbar.h"
#include "mnavigationbar_p.h"
#include "mapplicationmenubutton.h"
#include "mtoolbar.h"
#include "mviewcreator.h"
#include "mdeviceprofile.h"
#include "mscalableimage.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"

// --------------------------------------------------------------------------
// MNavigationBarViewPrivate
// --------------------------------------------------------------------------

MNavigationBarViewPrivate::MNavigationBarViewPrivate()
    : layout(0),
      policyTitle(0),
      policySimple(0),
      toolBarLayout(0),
      applicationMenuButton(0),
      toolBar(0)
{
}

MNavigationBarViewPrivate::~MNavigationBarViewPrivate()
{
    if (toolBar)
        toolBarLayout->removeItem(toolBar);

    delete applicationMenuButton;
}

void MNavigationBarViewPrivate::init()
{
    applicationMenuButton = new MApplicationMenuButton(controller);
    applicationMenuButton->setObjectName("NavigationBarMenuButton");

    layout = new MLayout;
    controller->setLayout(layout);

    toolBarLayout = new QGraphicsLinearLayout(Qt::Horizontal, layout);

    policyTitle = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policyTitle->setContentsMargins(0, 0, 0, 0);
    policyTitle->setSpacing(0);
    policyTitle->addItem(applicationMenuButton);
    policyTitle->addItem(toolBarLayout);
    policyTitle->setAlignment(applicationMenuButton, Qt::AlignCenter);

    policySimple = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policySimple->setContentsMargins(0, 0, 0, 0);
    policySimple->setSpacing(0);
    policySimple->addItem(toolBarLayout);
    policySimple->addStretch();
    policySimple->addItem(applicationMenuButton);
    policySimple->setAlignment(applicationMenuButton, Qt::AlignRight);

    // Connects button signals
    QObject::connect(applicationMenuButton, SIGNAL(clicked()), controller, SIGNAL(viewmenuTriggered()));

}

void MNavigationBarViewPrivate::setMenuButtonwidth()
{
    Q_Q(MNavigationBarView);

    if (q->style()->hasTitle()) {
        /* FIXME: Its there because the UI specs contains the following formula:
          width of application menu button = width of portrait view - width of home button - width of close button.
          This needs to be removed when there exists support for reading CSS constants in the code
         */
        int widthofPortraitMode = MDeviceProfile::instance()->resolution().height();
        qreal width = widthofPortraitMode - (qreal)q->style()->paddingLeft() - (qreal)q->style()->paddingRight();
        applicationMenuButton->setPreferredWidth(width);
        applicationMenuButton->setMinimumWidth(width);
        applicationMenuButton->setMaximumWidth(width);
    } else {
        applicationMenuButton->setPreferredWidth(-1);
        applicationMenuButton->setMinimumWidth(-1);
        applicationMenuButton->setMaximumWidth(-1);
    }
}

void MNavigationBarViewPrivate::notificationFlagChanged()
{
    // FIXME: Add notification support!
}

void MNavigationBarViewPrivate::toolBarChanged()
{
    Q_Q(MNavigationBarView);

    MToolBar *nextToolBar = q->model()->toolBar();

    // Make sure the last toolbar is removed first...
    if (toolBar) {
        if (nextToolBar == toolBar) return;

        toolBarLayout->removeItem(toolBar);
        toolBar->setParentItem(NULL);
    }

    if (nextToolBar) {
        toolBarLayout->addItem(nextToolBar);
        nextToolBar->show();
    }
    toolBar = nextToolBar;
}

// --------------------------------------------------------------------------
// MNavigationBarView
// --------------------------------------------------------------------------

MNavigationBarView::MNavigationBarView(MNavigationBar *controller) :
    MSceneWindowView(*(new MNavigationBarViewPrivate()), controller)
{
    Q_D(MNavigationBarView);
    d->init();
}

MNavigationBarView::~MNavigationBarView()
{
}

QRectF MNavigationBarView::boundingRect() const
{
    QRectF br = MWidgetView::boundingRect();
    if( style()->dropShadowImage() ) {
        br.setHeight(br.height() + style()->dropShadowImage()->pixmap()->size().height());
    }
    return br;
}

QPainterPath MNavigationBarView::shape() const
{
    QPainterPath path;
    // Get the base bounding rect, which excludes the shadow
    QRectF br = MWidgetView::boundingRect();

    // The shape is not expanded by the margins+reactiveMargins, as the bar is
    // non-interactive by design
    path.addRect(br);
    return path;
}

void MNavigationBarView::updateData(const QList<const char *>& modifications)
{
    Q_D(MNavigationBarView);
    MSceneWindowView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MNavigationBarModel::NotifyUser) {
            d->notificationFlagChanged();
        } else if (member == MNavigationBarModel::ViewMenuDescription) {
            d->applicationMenuButton->setText(model()->viewMenuDescription());
        } else if (member == MNavigationBarModel::ViewMenuIconID) {
            d->applicationMenuButton->setIconID(model()->viewMenuIconID());
        } else if (member == MNavigationBarModel::ProgressIndicatorVisible) {
            d->applicationMenuButton->setProgressIndicatorVisible(model()->progressIndicatorVisible());
        } else if (member == MNavigationBarModel::ArrowIconVisible) {
            d->applicationMenuButton->setArrowIconVisible(model()->arrowIconVisible());
        } else if (member == MNavigationBarModel::ToolBar) {
            d->toolBarChanged();
        }
    }
}

void MNavigationBarView::setupModel()
{
    MSceneWindowView::setupModel();

    Q_D(MNavigationBarView);
    d->applicationMenuButton->setText(model()->viewMenuDescription());
    d->applicationMenuButton->setIconID(model()->viewMenuIconID());
    d->applicationMenuButton->setProgressIndicatorVisible(model()->progressIndicatorVisible());
    d->applicationMenuButton->setArrowIconVisible(model()->arrowIconVisible());
}

void MNavigationBarView::applyStyle()
{
    MSceneWindowView::applyStyle();

    Q_D(MNavigationBarView);

    d->applicationMenuButton->setTextVisible(style()->hasTitle());
    d->applicationMenuButton->setIconVisible(style()->hasTitle());

    d->layout->setPolicy(style()->hasTitle() ? d->policyTitle : d->policySimple);

    d->setMenuButtonwidth();
}

void MNavigationBarView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MWidgetView::mousePressEvent(event);

    // Don't let it propagate to widgets below
    event->accept();
}

void MNavigationBarView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MWidgetView::mouseReleaseEvent(event);

    // Don't let it propagate to widgets below
    event->accept();
}

void MNavigationBarView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    //draw shadow under the actual navigationbar    
    if( style()->dropShadowImage() ) {
        style()->dropShadowImage()->draw(0, size().height(), boundingRect().width(),  style()->dropShadowImage()->pixmap()->size().height(), painter);
    }
    
    MWidgetView::drawBackground(painter, option);
}

M_REGISTER_VIEW_NEW(MNavigationBarView, MNavigationBar)
