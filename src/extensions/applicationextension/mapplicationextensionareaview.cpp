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

#include "mviewcreator.h"
#include <QGraphicsLinearLayout>
#include <MContainer>
#include "mextensionareaview.h"
#include "mapplicationextensionareaview.h"
#include "mapplicationextensionareaview_p.h"
#include "mapplicationextensionarea.h"


MApplicationExtensionAreaViewPrivate::MApplicationExtensionAreaViewPrivate()
{
}

MApplicationExtensionAreaViewPrivate::~MApplicationExtensionAreaViewPrivate()
{
}

void MApplicationExtensionAreaViewPrivate::init(MApplicationExtensionArea *controller)
{
    this->controller = controller;

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);
    this->layout = layout;
}

void MApplicationExtensionAreaViewPrivate::addToLayout(QGraphicsWidget *widget, int index)
{
    QGraphicsLinearLayout *linearLayout = static_cast<QGraphicsLinearLayout *>(layout);
    if (index >= 0) {
        linearLayout->insertItem(index, widget);
    } else {
        linearLayout->addItem(widget);
    }

}

void MApplicationExtensionAreaViewPrivate::connectContainerToWidget(MContainer *container, QGraphicsWidget *widget) const
{
    const QMetaObject *mob = widget->metaObject();

    // use widget properties to fill header of the container
    int iconProperty  = mob->indexOfProperty("applicationExtensionIcon");
    int titleProperty = mob->indexOfProperty("applicationExtensionTitle");
    int textProperty  = mob->indexOfProperty("applicationExtensionText");

    if (iconProperty != -1) {
        container->setIconID((mob->property(iconProperty).read(widget)).toString());
    }

    if (titleProperty != -1) {
        container->setTitle((mob->property(titleProperty).read(widget)).toString());
        container->setHeaderVisible(true);
    }

    if (textProperty != -1) {
        container->setText((mob->property(textProperty).read(widget)).toString());
    }

    // connect signals from widget to the container
    if (mob->indexOfSignal("applicationExtensionIconChanged(QString)") != -1) {
        QObject::connect(widget, SIGNAL(applicationExtensionIconChanged(QString)), container, SLOT(setIconID(QString)));
    }

    if (mob->indexOfSignal("applicationExtensionTitleChanged(QString)") != -1) {
        QObject::connect(widget, SIGNAL(applicationExtensionTitleChanged(QString)), container, SLOT(setTitle(QString)));
    }

    if (mob->indexOfSignal("applicationExtensionTextChanged(QString)") != -1) {
        QObject::connect(widget, SIGNAL(applicationExtensionTextChanged(QString)), container, SLOT(setText(QString)));
    }

    MExtensionAreaViewPrivate::connectContainerToWidget(container, widget);
}


MApplicationExtensionAreaView::MApplicationExtensionAreaView(MApplicationExtensionArea *controller) :
    MExtensionAreaView(new MApplicationExtensionAreaViewPrivate, controller)
{
    Q_D(MApplicationExtensionAreaView);
    d->q_ptr = this;
    d->init(controller);
}

MApplicationExtensionAreaView::MApplicationExtensionAreaView(MApplicationExtensionAreaViewPrivate &dd, MApplicationExtensionArea *controller) :
    MExtensionAreaView(&dd, controller)
{
    Q_D(MApplicationExtensionAreaView);
    d->init(controller);
}

MApplicationExtensionAreaView::MApplicationExtensionAreaView(MApplicationExtensionAreaViewPrivate *dd, MApplicationExtensionArea *controller) :
    MExtensionAreaView(dd, controller)
{
    Q_D(MApplicationExtensionAreaView);
    d->init(controller);
}

MApplicationExtensionAreaView::~MApplicationExtensionAreaView()
{
}

void MApplicationExtensionAreaView::applyStyle()
{
    Q_D(MApplicationExtensionAreaView);
    MExtensionAreaView::applyStyle();

    QGraphicsLinearLayout *linearLayout = static_cast<QGraphicsLinearLayout *>(d->layout);
    linearLayout->setOrientation(style()->layoutOrientation());
}

M_REGISTER_VIEW_NEW(MApplicationExtensionAreaView, MApplicationExtensionArea)
