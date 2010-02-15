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

#include "duihomebuttonpanelview.h"
#include "duihomebuttonpanelview_p.h"
#include "duibutton.h"
#include "duiviewcreator.h"
#include "duihomebuttonpanel.h"

#include <QGraphicsLinearLayout>

DuiHomeButtonPanelViewPrivate::DuiHomeButtonPanelViewPrivate()
    : button(0), controller(0)
{
}

DuiHomeButtonPanelViewPrivate::~DuiHomeButtonPanelViewPrivate()
{
    if (button) {
        delete button;
        button = 0;
    }
}

void DuiHomeButtonPanelViewPrivate::init()
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    button = new DuiButton(controller);

    mainLayout->addItem(button);
    controller->setLayout(mainLayout);

    QObject::connect(button, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));
}

void DuiHomeButtonPanelViewPrivate::setupButton()
{
    button->setObjectName("DuiHomeButton");
    button->setViewType("icon");
    button->setIconID("Icon-home");
}

DuiHomeButtonPanelView::DuiHomeButtonPanelView(DuiHomeButtonPanel *controller) :
    DuiSceneWindowView(* new DuiHomeButtonPanelViewPrivate, controller)
{
    Q_D(DuiHomeButtonPanelView);
    d->controller = controller;
    d->init();
    d->setupButton();
}

DuiHomeButtonPanelView::~DuiHomeButtonPanelView()
{
}

DUI_REGISTER_VIEW_NEW(DuiHomeButtonPanelView, DuiHomeButtonPanel)
