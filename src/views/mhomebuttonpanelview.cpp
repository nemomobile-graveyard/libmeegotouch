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

#include "mhomebuttonpanelview.h"
#include "mhomebuttonpanelview_p.h"
#include "mbutton.h"
#include "mviewcreator.h"
#include "mhomebuttonpanel.h"

#include <QGraphicsLinearLayout>

MHomeButtonPanelViewPrivate::MHomeButtonPanelViewPrivate()
    : q_ptr(0), button(0), controller(0)
{
}

MHomeButtonPanelViewPrivate::~MHomeButtonPanelViewPrivate()
{
    delete button;
    button = 0;
}

void MHomeButtonPanelViewPrivate::init()
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    button = new MButton(controller);
    button->setViewType("icon");

    mainLayout->addItem(button);
    controller->setLayout(mainLayout);

    QObject::connect(button, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));
}

MHomeButtonPanelView::MHomeButtonPanelView(MHomeButtonPanel *controller) :
    MSceneWindowView(controller),
    d_ptr(new MHomeButtonPanelViewPrivate)
{
    Q_D(MHomeButtonPanelView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MHomeButtonPanelView::~MHomeButtonPanelView()
{
}

void MHomeButtonPanelView::applyStyle()
{
    Q_D(MHomeButtonPanelView);

    MSceneWindowView::applyStyle();

    d->button->setObjectName(style()->homeButtonObjectName());
    d->button->setIconID(style()->homeButtonIconId());
}

M_REGISTER_VIEW_NEW(MHomeButtonPanelView, MHomeButtonPanel)
