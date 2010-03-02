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

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <DuiLabel>
#include <DuiImageWidget>
#include <DuiSeparator>
#include <DuiProgressIndicator>

#include <QCoreApplication>
#include <QSizePolicy>

#include "duicontainerview.h"
#include "duicontainerview_p.h"
#include "duicontainerheader_p.h"
#include "duicontainer.h"

#include "duiscalableimage.h"
#include "duiviewcreator.h"
#include "duitheme.h"

/*
 * layout:
 *
 *   + controller +
 *                |
 *                +- mainLayout +- header -+- headerLayout  +- icon
 *                              |                           |
 *                              |                           +- title
 *                              +- centralWidget            |
 *                                                          +- text
 *                                                          |
 *                                                          +- spinner
 */

DuiContainerViewPrivate::DuiContainerViewPrivate()
    : controller(0)
    , headerLayout(0)
    , header(0)
    , icon(0) // created only if needed
    , title(0)
    , text(0)
    , background(0)
    , headerPressed(false)
    , progressIndicator(0)
    , q_ptr(0)
{
}

DuiContainerViewPrivate::~DuiContainerViewPrivate()
{
}

void DuiContainerViewPrivate::init()
{
    controller->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Vertical);

    // pack
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addItem(controller->centralWidget());
    controller->setLayout(mainLayout);
}

/*
 * creates the header by adding a header widget, a layout and
 * labels
 */

void DuiContainerViewPrivate::createHeader()
{
    if (!header) {
        header = new DuiContainerHeader();

        headerLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        headerLayout->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
        header->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));

        headerLayout->setContentsMargins(0, 0, 0, 0);
        header->setLayout(headerLayout);

        title = new DuiLabel();
        text = new DuiLabel();

        title->setObjectName("DuiContainerTitle");
        text->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

        headerLayout->addItem(title);
        headerLayout->setAlignment(title,  Qt::AlignVCenter | Qt::AlignLeft);
        title->show();

        headerLayout->addItem(text);
        headerLayout->setAlignment(text,  Qt::AlignVCenter | Qt::AlignRight);
        text->show();

        // insert icon if available
        if (icon) {
            headerLayout->insertItem(0, icon);
            headerLayout->setAlignment(icon,  Qt::AlignVCenter | Qt::AlignLeft);
            icon->show();
        }

        QGraphicsLinearLayout* mainLayout = dynamic_cast<QGraphicsLinearLayout*>(controller->layout());
        
        if (mainLayout)
            mainLayout->insertItem(0, header);
    }
}

/*
 * delete header, mainLayout will now reuse the space previously
 * demanded by the header
 */

void DuiContainerViewPrivate::removeHeader()
{
    if (header) {
        /*
           delete header and its children, the header must be delted with
           deleteLater() as this gets called from a slot.
        */
        header->deleteLater();
        header = 0;
        title = 0;
        text = 0;
        icon = 0;
        progressIndicator = 0;
        headerLayout = 0;
    }
}

/*
 * create icon if it doesn't exist
 */

void DuiContainerViewPrivate::setupIcon(const QSize &size)
{
    if (!icon)
        icon = new DuiImageWidget;

    icon->setImage(controller->model()->icon(), size);
    icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // insert it if header is available
    if (header) {
        headerLayout->insertItem(0, icon);
        headerLayout->setAlignment(icon,  Qt::AlignVCenter | Qt::AlignLeft);
        icon->show();
    }
}

void DuiContainerViewPrivate::createProgressIndicator()
{
    if (!progressIndicator)
        progressIndicator = new DuiProgressIndicator;
    progressIndicator->setUnknownDuration(true);
}


void DuiContainerViewPrivate::layoutProgressIndicator()
{
    if (header) {
        headerLayout->addItem(progressIndicator);
        headerLayout->setAlignment(progressIndicator,  Qt::AlignVCenter | Qt::AlignRight);
        progressIndicator->show();
    }
}

DuiContainerView::DuiContainerView(DuiContainer *controller) :
    DuiWidgetView(*new DuiContainerViewPrivate, controller)
{
    Q_D(DuiContainerView);
    d->controller = controller;

    // setup main infrastructure
    d->init();

    connect(this, SIGNAL(headerClicked()), controller, SIGNAL(headerClicked()));
}

DuiContainerView::DuiContainerView(DuiContainerViewPrivate &dd, DuiContainer *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiContainerView);
    d->controller = controller;

    d->init();

    connect(this, SIGNAL(headerClicked()), controller, SIGNAL(headerClicked()));
}

DuiContainerView::~DuiContainerView()
{
}

void DuiContainerView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const DuiContainerView);

    // draw the background
    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());
    if (style()->backgroundImage()) {
        const qreal headerHeight = d->header ? d->header->size().height() : 0;
        style()->backgroundImage()->draw(0, headerHeight, size().width(), size().height() - headerHeight, painter);
    }
    painter->setOpacity(oldOpacity);
}

void DuiContainerView::applyStyle()
{
    Q_D(DuiContainerView);

    DuiWidgetView::applyStyle();

    //  get background from style
    d->background = style()->backgroundImage();
}

void DuiContainerView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiContainerView);
    DuiWidgetView::updateData(modifications);

    const DuiContainerModel *model = DuiContainerView::model();

    // make sure the header exists before icons or centralWidget are set
    // or modified
    if (model->headerVisible()) {
        d->createHeader();

        QObject::connect(d->header, SIGNAL(pressed()), this, SLOT(headerPressed()));
        QObject::connect(d->header, SIGNAL(released()), this, SLOT(headerReleased()));
        QObject::connect(d->header, SIGNAL(moved()), this, SLOT(headerMoved()));
        QObject::connect(d->header, SIGNAL(canceled()), this, SLOT(headerCanceled()));

        // update labels
        d->title->setText(model->title());
        d->text->setText(model->text());

        // make sure the icon is present if headerVisible() changed at runtime
        if (!model->icon().isEmpty())
            d->setupIcon(style()->iconSize());

        // same for the progress indicator
        if (model->progressIndicatorVisible()) {
            d->createProgressIndicator();
            d->layoutProgressIndicator();
        }

    } else {
        d->removeHeader();
    }

    const char *member;
    foreach(member, modifications) {
        if (member == DuiContainerModel::CentralWidget) {
            QGraphicsLinearLayout* mainLayout = dynamic_cast<QGraphicsLinearLayout*>(d->controller->layout());
            if (mainLayout) {
                // Remove the old central widget but don't destroy since the controller does it
                mainLayout->removeAt(mainLayout->count() - 1);
                // Add the new central widget
                mainLayout->addItem(d->controller->centralWidget());
            }
            // icon value changed
        } else if (member == DuiContainerModel::Icon) {
            // set new icon
            if (!model->icon().isEmpty()) {
                d->setupIcon(style()->iconSize());
            } else {
                d->headerLayout->removeItem(d->icon);
                d->icon->hide();
            }
        } else if (member == DuiContainerModel::ProgressIndicatorVisible) {
            if (model->progressIndicatorVisible()) {
                d->createProgressIndicator();
                d->layoutProgressIndicator();
            } else {
                delete d->progressIndicator;
                d->progressIndicator = 0;
            }
        }
    }

    // update visuals
    update();
}

void DuiContainerView::setupModel()
{
    Q_D(DuiContainerView);
    DuiWidgetView::setupModel();

    // initially creating the header if defined in the model
    if (model()->headerVisible()) {
        d->createHeader();

        QObject::connect(d->header, SIGNAL(pressed()), this, SLOT(headerPressed()));
        QObject::connect(d->header, SIGNAL(released()), this, SLOT(headerReleased()));

        d->title->setText(model()->title());
        d->text->setText(model()->text());

        // create icon if it is set to be shown by the model
        if (!model()->icon().isEmpty())
            d->setupIcon(style()->iconSize());

    }

    int spacing = style()->internalItemSpacing();
    int margin = style()->internalMargins();

    // In case of strange or missing values while reading css
    if (spacing < 0 || spacing > 100)
        spacing = 0;
    if (margin < 0 || margin > 100)
        margin = 0;

    QGraphicsLinearLayout* mainLayout = dynamic_cast<QGraphicsLinearLayout*>(d->controller->layout());
    if (mainLayout) {
        mainLayout->setContentsMargins(margin, margin, margin, margin);
        mainLayout->setSpacing(spacing);
        mainLayout->addItem(d->controller->centralWidget());
    }

    if (model()->progressIndicatorVisible()) {
        d->createProgressIndicator();
        d->layoutProgressIndicator();
    }
}

void DuiContainerView::headerMoved()
{
    Q_D(DuiContainerView);

    if (d->headerPressed) {
        d->headerPressed = false;
        update();
    }
}

void DuiContainerView::headerCanceled()
{
    Q_D(DuiContainerView);

    if (d->headerPressed) {
        d->headerPressed = false;
        update();
    }
}

void DuiContainerView::headerPressed()
{
    Q_D(DuiContainerView);
    d->headerPressed = true;
}

void DuiContainerView::headerReleased()
{
    Q_D(DuiContainerView);

    if (d->headerPressed) {
        emit headerClicked();
        d->headerPressed = false;
        update();
    }
}

// bind view and controller together
DUI_REGISTER_VIEW_NEW(DuiContainerView, DuiContainer)
