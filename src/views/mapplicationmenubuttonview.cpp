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

#include "mapplicationmenubuttonview.h"
#include "mapplicationmenubuttonview_p.h"
#include "mapplicationmenubutton.h"
#include "mtheme.h"
#include "mviewcreator.h"
#include "mimagewidget.h"
#include "mlabel.h"
#include "mprogressindicator.h"
#include "mlayout.h"
#include "mgridlayoutpolicy.h"

#include <QFontMetricsF>

MApplicationMenuButtonViewPrivate::MApplicationMenuButtonViewPrivate()
    : iconImage(0),  arrowIconImage(0), spinner(0), layout(0)
{
}

MApplicationMenuButtonViewPrivate::~MApplicationMenuButtonViewPrivate()
{
    delete iconImage;
    delete arrowIconImage;
    delete spinner;
}

void MApplicationMenuButtonViewPrivate::init()
{
    iconImage = new MImageWidget(controller);
    arrowIconImage = new MImageWidget(controller);

    iconImage->setObjectName("NavigationBarMenuButtonIconImage");
    arrowIconImage->setObjectName("NavigationBarMenuButtonArrowImage");
    arrowIconImage->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    layout = new MLayout();
    policy = new MGridLayoutPolicy(layout);

    policy->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(layout);
}

void MApplicationMenuButtonViewPrivate::refreshStyleMode()
{
    Q_Q(MApplicationMenuButtonView);

    if (q->model()->down())
        q->style().setModePressed();
    else if (q->model()->checked())
        q->style().setModeSelected();
    else
        q->style().setModeDefault();

    if (label)
        label->setAlignment(q->style()->horizontalTextAlign() | q->style()->verticalTextAlign());

    refreshIconImage();
}

void MApplicationMenuButtonViewPrivate::refreshLayout()
{
    Q_Q(MApplicationMenuButtonView);

    while (policy->count()) {
        policy->removeAt(0);
    }

    int colIndex = 0;
    MApplicationMenuButton *buttonController = (MApplicationMenuButton *)controller;
    if (buttonController->isIconVisible() && !buttonController->iconID().isEmpty()) {
        policy->addItem(iconImage, 0, colIndex++);
        policy->setAlignment(iconImage, Qt::AlignCenter);
        iconImage->show();
    } else {
        iconImage->hide();
    }

    if (label) {
        policy->addItem(label, 0, colIndex++);
        policy->setAlignment(label, Qt::AlignCenter);
    }

    arrowIconImage->hide();
    if (q->model()->progressIndicatorVisible() && spinner) {
        spinner->setUnknownDuration(true);
        policy->addItem(spinner, 0, colIndex++);
        policy->setAlignment(spinner, Qt::AlignCenter);
        spinner->show();
    } else {
        if (spinner) {
            spinner->hide();
            spinner->setUnknownDuration(false);
        }
        if (buttonController->isArrowIconVisible()) {
            policy->addItem(arrowIconImage, 0, colIndex++);
            policy->setAlignment(arrowIconImage, Qt::AlignLeft | Qt::AlignVCenter);
            arrowIconImage->show();
        }
    }
    q->updateGeometry();
}

void MApplicationMenuButtonViewPrivate::refreshIconImage()
{
    Q_Q(MApplicationMenuButtonView);

    QSize size = q->style()->iconSize();
    QString iconID = q->model()->iconID();

    if (toggleState() && !q->model()->toggledIconID().isEmpty())
        iconID = q->model()->toggledIconID();

    if (iconID != iconImage->image() ||
        (!iconID.isEmpty() && iconID == iconImage->image() && size != iconImage->imageSize())) {
        //Apply image and refresh layout only if image itself or size of the existing image
        //should be changed (avoid some flickering).
        iconImage->setImage(iconID, size);
        refreshLayout();
    }
}

MApplicationMenuButtonView::MApplicationMenuButtonView(MApplicationMenuButton *controller) :
    MButtonView(* new MApplicationMenuButtonViewPrivate, controller)
{
    Q_D(MApplicationMenuButtonView);
    d->init();
}

MApplicationMenuButtonView::MApplicationMenuButtonView(MApplicationMenuButtonViewPrivate &dd, MApplicationMenuButton *controller) :
    MButtonView(dd, controller)
{
    Q_D(MApplicationMenuButtonView);
    d->init();
}

MApplicationMenuButtonView::~MApplicationMenuButtonView()
{
}

// must override MButtonView::resizeEvent
void MApplicationMenuButtonView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MButtonView::resizeEvent(event);
}

void MApplicationMenuButtonView::applyStyle()
{
    Q_D(MApplicationMenuButtonView);

    MButtonView::applyStyle();

    d->arrowIconImage->setImage(style()->arrowIcon(), style()->arrowIconSize());
    d->iconImage->setZoomFactor(1.0);
    d->arrowIconImage->setZoomFactor(1.0);

    if (d->label) {
        d->label->setObjectName("NavigationBarMenuButtonLabel");
    }

    d->refreshStyleMode();
    d->refreshLayout();
}

void MApplicationMenuButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(MApplicationMenuButtonView);

    MButtonView::updateData(modifications);
    bool mustRefreshLayout = false;
    bool mustRefreshStyleMode = false;
    const char *member;
    foreach(member, modifications) {
        if (member == MButtonModel::Text || member == MButtonModel::TextVisible) {
            mustRefreshLayout = true;
        } else if (member == MButtonModel::IconID || member == MButtonModel::ToggledIconID) {
            d->refreshIconImage();
        } else if (member == MButtonModel::IconVisible) {
            d->iconImage->setVisible(model()->iconVisible());
            mustRefreshLayout = true;
        } else if (member == MApplicationMenuButtonModel::ProgressIndicatorVisible) {
            // to reduce the application startup time,  only create spinner when it is needed.
            if (d->spinner == 0) {
                d->spinner = new MProgressIndicator(d->controller, MProgressIndicator::spinnerType);
            }
            mustRefreshLayout = true;
        } else if (member == MApplicationMenuButtonModel::ArrowIconVisible) {
            mustRefreshLayout = true;
        } else if (member == MButtonModel::Down || member == MButtonModel::Checked ||
                   member == MButtonModel::Checkable) {
            mustRefreshStyleMode = true;
        }
    }

    if (mustRefreshLayout)
        d->refreshLayout();

    if (mustRefreshStyleMode)
        d->refreshStyleMode();
}

void MApplicationMenuButtonView::setupModel()
{
    MButtonView::setupModel();

    QList<const char*> modifications;
    modifications << MButtonModel::IconID;
    modifications << MButtonModel::IconVisible;
    modifications << MApplicationMenuButtonModel::ProgressIndicatorVisible;
    modifications << MApplicationMenuButtonModel::ArrowIconVisible;

    updateData(modifications);
    update();
}

M_REGISTER_VIEW_NEW(MApplicationMenuButtonView, MApplicationMenuButton)
