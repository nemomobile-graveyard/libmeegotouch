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
#include <MSceneManager>
#include "mscenewindowmodel.h"
#include "moverlay.h"
#include "moverlay_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MOverlay)

void MOverlayPrivate::init()
{
    Q_Q(MOverlay);

    mainLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    widget = new MWidget;

    mainLayout->addItem(widget);
    q->setLayout(mainLayout);
}

void MOverlayPrivate::deleteWidget()
{
    if (widget) {
        mainLayout->removeItem(widget);
        widget->deleteLater();
        widget = 0;
    }
}

void MOverlayPrivate::placeWidget(MWidget *widget)
{
    if (widget) {
        mainLayout->addItem(widget);
        this->widget = widget;
    }
}

MOverlay::MOverlay(QGraphicsItem *parent) :
    MSceneWindow(new MOverlayPrivate, new MSceneWindowModel, MSceneWindow::Overlay, "overlay", parent)
{
    Q_D(MOverlay);

    d->init();
}

MOverlay::~MOverlay()
{
}

MWidget *MOverlay::widget() const
{
    Q_D(const MOverlay);

    return d->widget;
}

void MOverlay::setWidget(MWidget *widget)
{
    Q_D(MOverlay);

    d->deleteWidget();
    d->placeWidget(widget);
}
