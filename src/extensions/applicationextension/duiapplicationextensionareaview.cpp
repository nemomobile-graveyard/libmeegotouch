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

#include "duiviewcreator.h"
#include <QGraphicsLinearLayout>
#include <DuiContainer>
#include "duiextensionareaview.h"
#include "duiapplicationextensionareaview.h"
#include "duiapplicationextensionareaview_p.h"
#include "duiapplicationextensionarea.h"


DuiApplicationExtensionAreaViewPrivate::DuiApplicationExtensionAreaViewPrivate()
{
}

DuiApplicationExtensionAreaViewPrivate::~DuiApplicationExtensionAreaViewPrivate()
{
}

void DuiApplicationExtensionAreaViewPrivate::init(DuiApplicationExtensionArea *controller)
{
    this->controller = controller;

    layout = new QGraphicsLinearLayout(Qt::Vertical);
    controller->setLayout(layout);
}

void DuiApplicationExtensionAreaViewPrivate::addToLayout(DuiWidget *widget, int index)
{
    QGraphicsLinearLayout *linearLayout = static_cast<QGraphicsLinearLayout *>(layout);
    if (index >= 0) {
        linearLayout->insertItem(index, widget);
    } else {
        linearLayout->addItem(widget);
    }

}

void DuiApplicationExtensionAreaViewPrivate::connectContainerToWidget(DuiContainer *container, DuiWidget *widget) const
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

    DuiExtensionAreaViewPrivate::connectContainerToWidget(container, widget);
}


DuiApplicationExtensionAreaView::DuiApplicationExtensionAreaView(DuiApplicationExtensionArea *controller) :
    DuiExtensionAreaView(* new DuiApplicationExtensionAreaViewPrivate, controller)
{
    Q_D(DuiApplicationExtensionAreaView);
    d->q_ptr = this;
    d->init(controller);
}

DuiApplicationExtensionAreaView::DuiApplicationExtensionAreaView(DuiApplicationExtensionAreaViewPrivate &dd, DuiApplicationExtensionArea *controller) :
    DuiExtensionAreaView(dd, controller)
{
    Q_D(DuiApplicationExtensionAreaView);
    d->init(controller);
}

DuiApplicationExtensionAreaView::~DuiApplicationExtensionAreaView()
{
}

DUI_REGISTER_VIEW_NEW(DuiApplicationExtensionAreaView, DuiApplicationExtensionArea)
