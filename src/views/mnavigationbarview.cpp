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

#include <QGraphicsGridLayout>
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

// --------------------------------------------------------------------------
// MNavigationBarViewPrivate
// --------------------------------------------------------------------------

MNavigationBarViewPrivate::MNavigationBarViewPrivate()
    : layout(new QGraphicsLinearLayout()),
      applicationMenuButton(0),
      toolBar(0)
{
}

MNavigationBarViewPrivate::~MNavigationBarViewPrivate()
{
}

void MNavigationBarViewPrivate::init()
{
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    applicationMenuButton = new MApplicationMenuButton(controller);
    applicationMenuButton->setObjectName("NavigationBarMenuButton");

    layout->addItem(applicationMenuButton);
    layout->setAlignment(applicationMenuButton, Qt::AlignCenter);

    // Connects button signals
    QObject::connect(applicationMenuButton, SIGNAL(clicked()), controller, SIGNAL(viewmenuTriggered()));

}

void MNavigationBarViewPrivate::setMenuButtonwidth()
{
    Q_Q(MNavigationBarView);

    /* FIXME: Its there because the UI specs contains the following formula:
      width of application menu button = width of portrait view - width of home button - width of close button.
      This needs to be removed when there exists support for reading CSS constants in the code
     */
    int widthofPortraitMode = MDeviceProfile::instance()->resolution().height();
    qreal width = widthofPortraitMode - (qreal)q->style()->paddingLeft() - (qreal)q->style()->paddingRight();
    applicationMenuButton->setPreferredWidth(width);
    applicationMenuButton->setMinimumWidth(width);
    applicationMenuButton->setMaximumWidth(width);
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

        layout->removeAt(1);
        toolBar->setParentItem(NULL);
    }

    if (nextToolBar) {
        layout->addItem(nextToolBar);
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
        br.setHeight(style()->dropShadowImage()->pixmap()->size().height());
    }
    return br;
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
    d->setMenuButtonwidth();
    d->applicationMenuButton->setText(model()->viewMenuDescription());
    d->applicationMenuButton->setIconID(model()->viewMenuIconID());
    d->applicationMenuButton->setProgressIndicatorVisible(model()->progressIndicatorVisible());
    d->applicationMenuButton->setArrowIconVisible(model()->arrowIconVisible());
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
        style()->dropShadowImage()->draw(0, 0, boundingRect().width(),  style()->dropShadowImage()->pixmap()->size().height(), painter);
    }
    
    MWidgetView::drawBackground(painter, option);
}

M_REGISTER_VIEW_NEW(MNavigationBarView, MNavigationBar)
