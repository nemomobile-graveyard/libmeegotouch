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
