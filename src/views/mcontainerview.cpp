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

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <MLabel>
#include <MImageWidget>
#include <MSeparator>
#include <MProgressIndicator>

#include <QCoreApplication>
#include <QSizePolicy>

#include "mcontainerview.h"
#include "mcontainerview_p.h"
#include "mcontainerheader_p.h"
#include "mcontainer.h"

#include "mscalableimage.h"
#include "mviewcreator.h"
#include "mtheme.h"

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

MContainerViewPrivate::MContainerViewPrivate()
    : controller(0)
    , headerLayout(0)
    , header(0)
    , separator(0)
    , icon(0) // created only if needed
    , title(0)
    , text(0)
    , background(0)
    , headerPressed(false)
    , progressIndicator(0)
    , q_ptr(0)
{
}

MContainerViewPrivate::~MContainerViewPrivate()
{
    delete icon;
}

void MContainerViewPrivate::init()
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

void MContainerViewPrivate::createHeader()
{
    Q_Q(MContainerView);

    if (!header) {
        header = new MContainerHeader();

        headerLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        headerLayout->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
        header->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
        header->setStyleName(q->style()->headerStyleName());

        headerLayout->setContentsMargins(0, 0, 0, 0);
        headerLayout->setSpacing(0);
        header->setLayout(headerLayout);

        separator = new MSeparator(header, Qt::Horizontal);
        separator->setStyleName(q->style()->separatorStyleName());
        separator->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));

        title = new MLabel(header);
        title->setObjectName("MContainerTitle");
        title->setStyleName(q->style()->titleStyleName());

        headerLayout->addItem(separator);
        headerLayout->setAlignment(separator, Qt::AlignVCenter | Qt::AlignLeft);
        separator->show();

        headerLayout->addItem(title);
        headerLayout->setAlignment(title,  Qt::AlignVCenter | Qt::AlignRight);
        title->show();


        // insert icon if available
        if (icon) {
            headerLayout->insertItem(1, icon);
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

void MContainerViewPrivate::removeHeader()
{
    if (header) {
        // Remove the item from the layout. deleteLater() will NOT remove it properly but leaves the layout size incorrect and the item on the scene. See bug #231743.
        QGraphicsLinearLayout* mainLayout = dynamic_cast<QGraphicsLinearLayout*>(controller->layout());
        if (mainLayout)
            mainLayout->removeItem(header);

        /*
           Delete header and its children. The header must be deleted with
           deleteLater() as this gets called from a slot. However, the item
           MUST be removed from the layout before this.
        */
        header->deleteLater();
        header = 0;
        title = 0;
        text = 0;
        icon = 0;
        separator = 0;
        progressIndicator = 0;
        headerLayout = 0;
    }
}

void MContainerViewPrivate::setupTextLabel()
{
    if (!text)
        text = new MLabel();

    text->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    if (header) {
        headerLayout->addItem(text);
        headerLayout->setAlignment(text,  Qt::AlignVCenter | Qt::AlignRight);
        text->show();
    }
}

/*
 * create icon if it doesn't exist
 */

void MContainerViewPrivate::setupIcon(const QSize &size)
{
    if (!icon)
        icon = new MImageWidget;

    icon->setImage(controller->model()->icon(), size);
    icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // insert it if header is available
    if (header) {
        headerLayout->insertItem(1, icon);
        headerLayout->setAlignment(icon,  Qt::AlignVCenter | Qt::AlignLeft);
        icon->show();
    }
}

void MContainerViewPrivate::createProgressIndicator()
{
    Q_Q(MContainerView);

    if (!progressIndicator)
        progressIndicator = new MProgressIndicator;
    progressIndicator->setUnknownDuration(true);
    progressIndicator->setStyleName(q->style()->progressIndicatorStyleName());
}


void MContainerViewPrivate::layoutProgressIndicator()
{
    if (header) {
        headerLayout->insertItem(1, progressIndicator);
        headerLayout->setAlignment(progressIndicator,  Qt::AlignVCenter | Qt::AlignRight);
        progressIndicator->show();
    }
}

MContainerView::MContainerView(MContainer *controller) :
    MWidgetView(*new MContainerViewPrivate, controller)
{
    Q_D(MContainerView);
    d->q_ptr = this;
    d->controller = controller;

    // setup main infrastructure
    d->init();

    connect(this, SIGNAL(headerClicked()), controller, SIGNAL(headerClicked()));
}

MContainerView::MContainerView(MContainerViewPrivate &dd, MContainer *controller) :
    MWidgetView(dd, controller)
{
    Q_D(MContainerView);
    d->q_ptr = this;
    d->controller = controller;

    d->init();

    connect(this, SIGNAL(headerClicked()), controller, SIGNAL(headerClicked()));
}

MContainerView::~MContainerView()
{
}

void MContainerView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MContainerView);

    // draw the background
    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());
    if (style()->backgroundImage()) {
        const QSizeF _size = d->controller->centralWidget()->geometry().size();
        const qreal headerHeight = d->header ? d->header->size().height() : 0;
        style()->backgroundImage()->draw(0.0, headerHeight, _size.width(), _size.height(), painter);
    }
    painter->setOpacity(oldOpacity);
}

void MContainerView::applyStyle()
{
    Q_D(MContainerView);

    MWidgetView::applyStyle();

    if (d->separator) {
        d->separator->setStyleName(style()->separatorStyleName());
    }

    if (d->title) {
        d->title->setStyleName(style()->titleStyleName());
    }

    if (d->progressIndicator) {
        d->progressIndicator->setStyleName(style()->progressIndicatorStyleName());
    }

    if (d->header) {
        d->header->setStyleName(style()->headerStyleName());
    }

    //  get background from style
    d->background = style()->backgroundImage();
}

void MContainerView::updateData(const QList<const char *>& modifications)
{
    Q_D(MContainerView);
    MWidgetView::updateData(modifications);

    const MContainerModel *model = MContainerView::model();

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

        // create the text label if necessary
        if (!model->text().isEmpty()) {
            d->setupTextLabel();
            d->text->setText(model->text());
        }

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
        if (member == MContainerModel::CentralWidget) {
            QGraphicsLinearLayout* mainLayout = dynamic_cast<QGraphicsLinearLayout*>(d->controller->layout());
            if (mainLayout) {
                // Remove the old central widget but don't destroy since the controller does it
                mainLayout->removeAt(mainLayout->count() - 1);
                // Add the new central widget
                mainLayout->addItem(d->controller->centralWidget());
            }
            // icon value changed
        } else if (member == MContainerModel::Icon) {
            // set new icon
            if (!model->icon().isEmpty()) {
                d->setupIcon(style()->iconSize());
            } else {
                d->headerLayout->removeItem(d->icon);
                d->icon->hide();
            }
        } else if (member == MContainerModel::ProgressIndicatorVisible) {
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

void MContainerView::setupModel()
{
    Q_D(MContainerView);
    MWidgetView::setupModel();

    // initially creating the header if defined in the model
    if (model()->headerVisible()) {
        d->createHeader();

        QObject::connect(d->header, SIGNAL(pressed()), this, SLOT(headerPressed()));
        QObject::connect(d->header, SIGNAL(released()), this, SLOT(headerReleased()));

        d->title->setText(model()->title());
        if (!model()->text().isEmpty())
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

void MContainerView::headerMoved()
{
    Q_D(MContainerView);

    if (d->headerPressed) {
        d->headerPressed = false;
        update();
    }
}

void MContainerView::headerCanceled()
{
    Q_D(MContainerView);

    if (d->headerPressed) {
        d->headerPressed = false;
        update();
    }
}

void MContainerView::headerPressed()
{
    Q_D(MContainerView);
    d->headerPressed = true;
}

void MContainerView::headerReleased()
{
    Q_D(MContainerView);

    if (d->headerPressed) {
        emit headerClicked();
        d->headerPressed = false;
        update();
    }
}

// bind view and controller together
M_REGISTER_VIEW_NEW(MContainerView, MContainer)
