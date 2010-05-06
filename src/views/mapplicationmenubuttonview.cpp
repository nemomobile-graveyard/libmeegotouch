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

#include <QFontMetricsF>
#include <QGraphicsGridLayout>

MApplicationMenuButtonViewPrivate::MApplicationMenuButtonViewPrivate()
    : iconImage(0),  arrowIconImage(0), spinner(0), layout(0)
{
}

MApplicationMenuButtonViewPrivate::~MApplicationMenuButtonViewPrivate()
{
}

void MApplicationMenuButtonViewPrivate::init()
{
    iconImage = new MImageWidget(controller);
    arrowIconImage = new MImageWidget(controller);

    iconImage->setObjectName("NavigationBarMenuButtonIconImage");
    arrowIconImage->setObjectName("NavigationBarMenuButtonArrowImage");
    label->setObjectName("NavigationBarMenuButtonLabel");

    layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
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

    label->setAlignment(q->style()->horizontalTextAlign() | q->style()->verticalTextAlign());

    refreshIconImage();
}

void MApplicationMenuButtonViewPrivate::refreshLayout()
{
    Q_Q(MApplicationMenuButtonView);

    while (layout->count()) {
        layout->removeAt(0);
    }

    int colIndex = 0;
    MApplicationMenuButton *buttonController = (MApplicationMenuButton *)controller;
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
    q->updateGeometry();
}

void MApplicationMenuButtonViewPrivate::refreshIconImage()
{
    Q_Q(MApplicationMenuButtonView);

    QSize size = q->style()->iconSize();
    if (toggleState()) {
        if (!q->model()->toggledIconID().isEmpty())
            iconImage->setImage(q->model()->toggledIconID(), size);
    }  else {
        iconImage->setImage(q->model()->iconID(), size);
    }

    refreshLayout();
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
    MWidgetView::resizeEvent(event);
}

void MApplicationMenuButtonView::applyStyle()
{
    Q_D(MApplicationMenuButtonView);

    MWidgetView::applyStyle();

    d->arrowIconImage->setImage(style()->arrowIcon(), style()->arrowIconSize());
    d->refreshStyleMode();
    d->iconImage->setZoomFactor(1.0);
    d->arrowIconImage->setZoomFactor(1.0);

    update();
}

void MApplicationMenuButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(MApplicationMenuButtonView);

    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MButtonModel::Text) {
            d->label->setText(model()->text());
        } else if (member == MButtonModel::TextVisible) {
            d->label->setVisible(model()->textVisible());
        } else if (member == MButtonModel::IconID || member == MButtonModel::ToggledIconID) {
            d->refreshIconImage();
        } else if (member == MButtonModel::IconVisible) {
            d->iconImage->setVisible(model()->iconVisible());
            d->refreshLayout();
        } else if (member == MApplicationMenuButtonModel::ProgressIndicatorVisible) {
            // to reduce the application startup time,  only create spinner when it is needed.
            if (d->spinner == 0) {
                d->spinner = new MProgressIndicator(d->controller, MProgressIndicator::spinnerType);
            }
            d->refreshLayout();
        } else if (member == MApplicationMenuButtonModel::ArrowIconVisible) {
            d->refreshLayout();
        } else if (member == MButtonModel::Down || member == MButtonModel::Checked ||
                   member == MButtonModel::Checkable) {
            d->refreshStyleMode();
        }
    }
    update();
}

void MApplicationMenuButtonView::setupModel()
{
    Q_D(MApplicationMenuButtonView);

    d->label->setText(model()->text());
    d->label->setVisible(model()->textVisible());

    d->refreshIconImage();

    MWidgetView::setupModel();
}

QSizeF MApplicationMenuButtonView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return MWidgetView::sizeHint(which, constraint);

    if (style()->preferredSize().isValid())
        return style()->preferredSize();

    Q_D(const MApplicationMenuButtonView);

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

M_REGISTER_VIEW_NEW(MApplicationMenuButtonView, MApplicationMenuButton)
