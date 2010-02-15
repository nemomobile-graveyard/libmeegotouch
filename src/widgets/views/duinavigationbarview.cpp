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

#include "duinavigationbarview.h"
#include "duinavigationbarview_p.h"

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

#include "duiapplication.h"
#include "duinavigationbar.h"
#include "duinavigationbar_p.h"
#include "duiapplicationmenubutton.h"
#include "duitoolbar.h"
#include "duiviewcreator.h"
#include "duideviceprofile.h"

// --------------------------------------------------------------------------
// DuiNavigationBarViewPrivate
// --------------------------------------------------------------------------

DuiNavigationBarViewPrivate::DuiNavigationBarViewPrivate()
    : layout(new QGraphicsGridLayout()),
      applicationMenuButton(0),
      toolbarPlaceholder(0),
      toolBar(0)
{
}

DuiNavigationBarViewPrivate::~DuiNavigationBarViewPrivate()
{
}

void DuiNavigationBarViewPrivate::init()
{
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    applicationMenuButton = new DuiApplicationMenuButton(controller);
    applicationMenuButton->setObjectName("NavigationBarMenuButton");

    layout->addItem(applicationMenuButton, 0, 0);
    layout->setAlignment(applicationMenuButton, Qt::AlignVCenter);

    // panel for placing toolbar
    toolbarPlaceholder = new DuiWidget(controller);
    layout->addItem(toolbarPlaceholder, 0, 1);

    toolbarPlaceholderLayout = new QGraphicsLinearLayout(toolbarPlaceholder);
    toolbarPlaceholderLayout->setContentsMargins(0, 0, 0, 0);
    toolbarPlaceholder->setLayout(toolbarPlaceholderLayout);

    // Connects button signals
    QObject::connect(applicationMenuButton, SIGNAL(clicked()), controller, SIGNAL(viewmenuTriggered()));

}

void DuiNavigationBarViewPrivate::setMenuButtonwidth()
{
    Q_Q(DuiNavigationBarView);

    /* FIXME: Its there because the UI specs contains the following forumla:
      width of view menu button = width of portrait view - width of home button - width of close button.
      This needs to be removed when there exists support for reading CSS constants in the code
     */
    int widthofPortraitMode = DuiDeviceProfile::instance()->resolution().height();
    qreal width = widthofPortraitMode - (qreal)q->style()->paddingLeft() - (qreal)q->style()->paddingRight();
    applicationMenuButton->setPreferredWidth(width);
    applicationMenuButton->setMinimumWidth(width);
    applicationMenuButton->setMaximumWidth(width);
}

void DuiNavigationBarViewPrivate::notificationFlagChanged()
{
    // FIXME: Add notification support!
}

void DuiNavigationBarViewPrivate::toolBarChanged()
{
    Q_Q(DuiNavigationBarView);

    DuiToolBar *nextToolBar = q->model()->toolBar();

    // Make sure the last toolbar is deleted first...
    if (toolBar) {
        if (nextToolBar == toolBar) return;

        toolbarPlaceholderLayout->removeAt(0);
        toolBar->setParentItem(NULL);
    }

    if (nextToolBar) {
        toolbarPlaceholderLayout->addItem(nextToolBar);
        nextToolBar->show();
    }
    toolBar = nextToolBar;
}

// --------------------------------------------------------------------------
// DuiNavigationBarView
// --------------------------------------------------------------------------

DuiNavigationBarView::DuiNavigationBarView(DuiNavigationBar *controller) :
    DuiSceneWindowView(*(new DuiNavigationBarViewPrivate()), controller)
{
    Q_D(DuiNavigationBarView);
    d->init();
}

DuiNavigationBarView::~DuiNavigationBarView()
{
}

void DuiNavigationBarView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiNavigationBarView);
    DuiSceneWindowView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiNavigationBarModel::NotifyUser) {
            d->notificationFlagChanged();
        } else if (member == DuiNavigationBarModel::ViewMenuDescription) {
            d->applicationMenuButton->setText(model()->viewMenuDescription());
        } else if (member == DuiNavigationBarModel::ViewMenuIconID) {
            d->applicationMenuButton->setIconID(model()->viewMenuIconID());
        } else if (member == DuiNavigationBarModel::ProgressIndicatorVisible) {
            d->applicationMenuButton->setProgressIndicatorVisible(model()->progressIndicatorVisible());
        } else if (member == DuiNavigationBarModel::ArrowIconVisible) {
            d->applicationMenuButton->setArrowIconVisible(model()->arrowIconVisible());
        } else if (member == DuiNavigationBarModel::ToolBar) {
            d->toolBarChanged();
        }
    }
}

void DuiNavigationBarView::setupModel()
{
    DuiSceneWindowView::setupModel();

    Q_D(DuiNavigationBarView);
    d->setMenuButtonwidth();
    d->applicationMenuButton->setText(model()->viewMenuDescription());
    d->applicationMenuButton->setIconID(model()->viewMenuIconID());
    d->applicationMenuButton->setProgressIndicatorVisible(model()->progressIndicatorVisible());
    d->applicationMenuButton->setArrowIconVisible(model()->arrowIconVisible());
}

void DuiNavigationBarView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    DuiWidgetView::mousePressEvent(event);

    // Don't let it propagate to widgets below
    event->accept();
}

void DuiNavigationBarView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    DuiWidgetView::mouseReleaseEvent(event);

    // Don't let it propagate to widgets below
    event->accept();
}

DUI_REGISTER_VIEW_NEW(DuiNavigationBarView, DuiNavigationBar)
