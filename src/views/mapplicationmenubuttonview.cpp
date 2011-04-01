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
#include <QGraphicsLinearLayout>

MApplicationMenuButtonViewPrivate::MApplicationMenuButtonViewPrivate()
    : iconImage(0), arrowIconImage(0), spinner(0), layout(0), labelSlot(0), labelLayout(0)
{
}

MApplicationMenuButtonViewPrivate::~MApplicationMenuButtonViewPrivate()
{
    delete iconImage;
    delete arrowIconImage;
    delete spinner;
    delete labelSlot;
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

    labelSlot = new QGraphicsWidget;
    labelSlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    labelLayout = new QGraphicsLinearLayout;
    labelLayout->setContentsMargins(0, 0, 0, 0);
    labelLayout->setSpacing(0);
    labelSlot->setLayout(labelLayout);
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

    if (label && labelLayout) {
        if (labelLayout->count() == 0)
            labelLayout->addItem(label);
        policy->addItem(labelSlot, 0, colIndex++);
        policy->setAlignment(labelSlot, Qt::AlignCenter);
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

void MApplicationMenuButtonViewPrivate::updateLabelStyle()
{
    Q_Q(MApplicationMenuButtonView);

    MButtonViewPrivate::updateLabelStyle();

    if (labelLayout) {
        const MApplicationMenuButtonStyle *s = static_cast<const MApplicationMenuButtonStyle *>(q->style().operator ->());
        labelLayout->setContentsMargins(s->textMarginLeft(), s->textMarginTop(),
                                        s->textMarginRight(), s->textMarginBottom());
    }
}

void MApplicationMenuButtonViewPrivate::updateItemsAfterModeChange()
{
    updateLabelStyle();
    refreshIconImage();
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

void MApplicationMenuButtonView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event); //skip MButtonView::resizeEvent as we don't like calcIconTextRects() call there
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

    d->updateLabelStyle();
    d->refreshIconImage();
    d->refreshLayout();
}

void MApplicationMenuButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(MApplicationMenuButtonView);

    MButtonView::updateData(modifications);
    bool mustRefreshLayout = false;
    bool mustRefreshIconImage = false;
    const char *member;
    foreach(member, modifications) {
        if (member == MButtonModel::Text || member == MButtonModel::TextVisible) {
            mustRefreshLayout = true;
        } else if (member == MButtonModel::IconID || member == MButtonModel::ToggledIconID) {
            mustRefreshIconImage = true;
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
        }
    }

    if (mustRefreshIconImage)
        d->refreshIconImage();

    if (mustRefreshLayout)
        d->refreshLayout();
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

void MApplicationMenuButtonView::drawIcon(QPainter *painter, const QRectF &iconRect) const
{
    Q_UNUSED(painter);
    Q_UNUSED(iconRect);
}

M_REGISTER_VIEW_NEW(MApplicationMenuButtonView, MApplicationMenuButton)
