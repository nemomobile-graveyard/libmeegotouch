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
#include <DuiSceneManager>
#include "duiscenewindowmodel.h"
#include "duioverlay.h"
#include "duioverlay_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiOverlay)

void DuiOverlayPrivate::init()
{
    Q_Q(DuiOverlay);

    mainLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    widget = new DuiWidget;

    mainLayout->addItem(widget);
    q->setLayout(mainLayout);
}

void DuiOverlayPrivate::deleteWidget()
{
    if (widget) {
        mainLayout->removeItem(widget);
        widget->deleteLater();
        widget = 0;
    }
}

void DuiOverlayPrivate::placeWidget(DuiWidget *widget)
{
    if (widget) {
        mainLayout->addItem(widget);
        this->widget = widget;
    }
}

DuiOverlay::DuiOverlay(QGraphicsItem *parent) :
    DuiSceneWindow(new DuiOverlayPrivate, new DuiSceneWindowModel, DuiSceneWindow::Overlay, "overlay", parent)
{
    Q_D(DuiOverlay);

    d->init();
}

DuiOverlay::~DuiOverlay()
{
}

DuiWidget *DuiOverlay::widget() const
{
    Q_D(const DuiOverlay);

    return d->widget;
}

void DuiOverlay::setWidget(DuiWidget *widget)
{
    Q_D(DuiOverlay);

    d->deleteWidget();
    d->placeWidget(widget);
}
