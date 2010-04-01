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

#include "duiapplicationmenubuttonview.h"
#include "duiapplicationmenubuttonview_p.h"
#include "duiapplicationmenubutton.h"
#include "duitheme.h"
#include "duiviewcreator.h"
#include "duiimagewidget.h"
#include "duilabel.h"
#include "duiprogressindicator.h"

#include <QFontMetricsF>
#include <QGraphicsGridLayout>

DuiApplicationMenuButtonViewPrivate::DuiApplicationMenuButtonViewPrivate()
    : iconImage(0),  arrowIconImage(0), spinner(0), layout(0)
{
}

DuiApplicationMenuButtonViewPrivate::~DuiApplicationMenuButtonViewPrivate()
{
}

void DuiApplicationMenuButtonViewPrivate::init()
{
    iconImage = new DuiImageWidget(controller);
    arrowIconImage = new DuiImageWidget(controller);

    iconImage->setObjectName("NavigationBarMenuButtonIconImage");
    arrowIconImage->setObjectName("NavigationBarMenuButtonArrowImage");
    label->setObjectName("NavigationBarMenuButtonLabel");

    layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(layout);
}

void DuiApplicationMenuButtonViewPrivate::refreshStyleMode()
{
    Q_Q(DuiApplicationMenuButtonView);

    if (q->model()->down())
        q->style().setModePressed();
    else if (q->model()->checked())
        q->style().setModeSelected();
    else
        q->style().setModeDefault();

    label->setAlignment(q->style()->horizontalTextAlign() | q->style()->verticalTextAlign());

    refreshIconImage();
}

void DuiApplicationMenuButtonViewPrivate::refreshLayout()
{
    Q_Q(DuiApplicationMenuButtonView);

    while (layout->count()) {
        layout->removeAt(0);
    }

    int colIndex = 0;
    DuiApplicationMenuButton *buttonController = (DuiApplicationMenuButton *)controller;
    if (buttonController->isIconVisible() && !buttonController->iconID().isEmpty()) {
        iconImage->show();
        layout->addItem(iconImage, 0, colIndex++);
        layout->setAlignment(iconImage, Qt::AlignCenter);
    } else {
        iconImage->hide();
    }
    layout->addItem(label, 0, colIndex++);
    layout->setAlignment(label, Qt::AlignCenter);

    arrowIconImage->hide();
    if (q->model()->progressIndicatorVisible() && spinner) {
        spinner->show();
        spinner->setUnknownDuration(true);
        layout->addItem(spinner, 0, colIndex++);
        layout->setAlignment(spinner, Qt::AlignCenter);
    } else {
        if (spinner) {
            spinner->hide();
            spinner->setUnknownDuration(false);
        }
        if (buttonController->isArrowIconVisible()) {
            arrowIconImage->show();
            layout->addItem(arrowIconImage, 0, colIndex++);
            layout->setAlignment(arrowIconImage, Qt::AlignCenter);
        }
    }

}

void DuiApplicationMenuButtonViewPrivate::refreshIconImage()
{
    Q_Q(DuiApplicationMenuButtonView);

    QSize size = q->style()->iconSize();
    if (toggleState()) {
        if (!q->model()->toggledIconID().isEmpty())
            iconImage->setImage(q->model()->toggledIconID(), size);
    }  else {
        iconImage->setImage(q->model()->iconID(), size);
    }

    refreshLayout();
}

DuiApplicationMenuButtonView::DuiApplicationMenuButtonView(DuiApplicationMenuButton *controller) :
    DuiButtonView(* new DuiApplicationMenuButtonViewPrivate, controller)
{
    Q_D(DuiApplicationMenuButtonView);
    d->init();
}

DuiApplicationMenuButtonView::DuiApplicationMenuButtonView(DuiApplicationMenuButtonViewPrivate &dd, DuiApplicationMenuButton *controller) :
    DuiButtonView(dd, controller)
{
    Q_D(DuiApplicationMenuButtonView);
    d->init();
}

DuiApplicationMenuButtonView::~DuiApplicationMenuButtonView()
{
}

// must override DuiButtonView::resizeEvent
void DuiApplicationMenuButtonView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    DuiWidgetView::resizeEvent(event);
}

void DuiApplicationMenuButtonView::applyStyle()
{
    Q_D(DuiApplicationMenuButtonView);

    DuiWidgetView::applyStyle();

    d->arrowIconImage->setImage(style()->arrowIcon(), style()->arrowIconSize());
    d->refreshStyleMode();
    d->iconImage->setZoomFactor(1.0);
    d->arrowIconImage->setZoomFactor(1.0);

    update();
}

void DuiApplicationMenuButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiApplicationMenuButtonView);

    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiButtonModel::Text) {
            d->label->setText(model()->text());
        } else if (member == DuiButtonModel::TextVisible) {
            d->label->setVisible(model()->textVisible());
        } else if (member == DuiButtonModel::IconID || member == DuiButtonModel::ToggledIconID) {
            d->refreshIconImage();
        } else if (member == DuiButtonModel::IconVisible) {
            d->iconImage->setVisible(model()->iconVisible());
            d->refreshLayout();
        } else if (member == DuiApplicationMenuButtonModel::ProgressIndicatorVisible) {
            // to reduce the application startup time,  only create spinner when it is needed.
            if (d->spinner == 0) {
                d->spinner = new DuiProgressIndicator(d->controller, DuiProgressIndicator::spinnerType);
            }
            d->refreshLayout();
        } else if (member == DuiApplicationMenuButtonModel::ArrowIconVisible) {
            d->refreshLayout();
        } else if (member == DuiButtonModel::Down || member == DuiButtonModel::Checked ||
                   member == DuiButtonModel::Checkable) {
            d->refreshStyleMode();
        }
    }
    update();
}

void DuiApplicationMenuButtonView::setupModel()
{
    Q_D(DuiApplicationMenuButtonView);

    d->label->setText(model()->text());
    d->label->setVisible(model()->textVisible());

    d->refreshIconImage();

    DuiWidgetView::setupModel();
}

QSizeF DuiApplicationMenuButtonView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return DuiWidgetView::sizeHint(which, constraint);

    if (style()->preferredSize().isValid())
        return style()->preferredSize();

    Q_D(const DuiApplicationMenuButtonView);

    qreal width = 0, height = 0;
    QSizeF iconSize(0, 0);
    QSizeF labelSize(0, 0);

    if (model()->iconVisible())
        iconSize = style()->iconSize();

    if (model()->textVisible() && model()->text().isEmpty() == false) {
        labelSize = d->label->sizeHint(which, constraint);
    }

    width  = iconSize.width() * 2 + labelSize.width();
    height = qMax(iconSize.height(), labelSize.height());

    return QSizeF(width, height);
}

DUI_REGISTER_VIEW_NEW(DuiApplicationMenuButtonView, DuiApplicationMenuButton)
