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

#include "mescapebuttonpanelview.h"
#include "mescapebuttonpanelview_p.h"
#include "mbutton.h"
#include "mviewcreator.h"
#include "mescapebuttonpanel.h"
#include "mdebug.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPauseAnimation>

MEscapeButtonPanelViewPrivate::MEscapeButtonPanelViewPrivate() :
    q_ptr(0),
    backButton(0),
    closeButton(0),
    escapeMode(MEscapeButtonPanelModel::CloseMode),
    controller(0)
{
}

MEscapeButtonPanelViewPrivate::~MEscapeButtonPanelViewPrivate()
{
    delete warpInAnimation;
    delete warpOutAnimation;
}

void MEscapeButtonPanelViewPrivate::init()
{
    backButton = new MButton(controller);
    closeButton = new MButton(controller);
    backButton->setViewType("icon");
    closeButton->setViewType("icon");

    QObject::connect(backButton, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));

    setupEscapeButtonTransition();
}

void MEscapeButtonPanelViewPrivate::animatedEscapeButtonTransition()
{
    if (warpInAnimation && warpInAnimation->state() == QAbstractAnimation::Running) {
        warpInAnimation->setCurrentTime(warpInAnimation->duration());
        delete warpInAnimation;
    }
    if (warpOutAnimation && warpOutAnimation->state() == QAbstractAnimation::Running) {
        warpOutAnimation->setCurrentTime(warpOutAnimation->duration());
        delete warpOutAnimation;
    }

    switch (escapeMode) {
    case MEscapeButtonPanelModel::CloseMode:
        warpInAnimation = new MWarpAnimation(closeButton, MWarpAnimation::InFromLeft, controller);
        warpOutAnimation = new MWarpAnimation(backButton, MWarpAnimation::OutFromRight, controller);
        break;
    case MEscapeButtonPanelModel::BackMode:
        warpInAnimation = new MWarpAnimation(backButton, MWarpAnimation::InFromRight, controller);
        warpOutAnimation = new MWarpAnimation(closeButton, MWarpAnimation::OutFromLeft, controller);
        break;
    default:
        mWarning("MEscapeButtonPanelView") << "Unknown mode for escape button transition";
        break;
    };

    if (warpInAnimation && warpOutAnimation) {
        warpInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        warpOutAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MEscapeButtonPanelViewPrivate::immediateEscapeButtonTransition()
{
    switch (escapeMode) {
    case MEscapeButtonPanelModel::CloseMode:
        backButton->hide();
        closeButton->show();
        break;
    case MEscapeButtonPanelModel::BackMode:
        closeButton->hide();
        backButton->show();
        break;
    default:
        mWarning("MEscapeButtonPanelView") << "Unknown mode for escape button transition";
        break;
    };
}

void MEscapeButtonPanelViewPrivate::setupEscapeButtonTransition()
{
    if (controller->isOnDisplay()) {
        animatedEscapeButtonTransition();
    } else {
        immediateEscapeButtonTransition();
    }
}

MEscapeButtonPanelView::MEscapeButtonPanelView(MEscapeButtonPanel *controller) :
    MSceneWindowView(controller),
    d_ptr(new MEscapeButtonPanelViewPrivate)
{
    Q_D(MEscapeButtonPanelView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MEscapeButtonPanelView::~MEscapeButtonPanelView()
{
    delete d_ptr;
}

void MEscapeButtonPanelView::applyStyle()
{
    Q_D(MEscapeButtonPanelView);

    MSceneWindowView::applyStyle();

    d->backButton->setObjectName(style()->backButtonObjectName());
    d->backButton->setIconID(style()->backButtonIconId());
    d->closeButton->setObjectName(style()->closeButtonObjectName());
    d->closeButton->setIconID(style()->closeButtonIconId());
}

void MEscapeButtonPanelView::updateData(const QList<const char *>& modifications)
{
    MSceneWindowView::updateData(modifications);

    Q_D(MEscapeButtonPanelView);

    const char *member;
    foreach(member, modifications) {
        if (member == MEscapeButtonPanelModel::Mode) {
            d->escapeMode = model()->mode();
            d->setupEscapeButtonTransition();
        }
    }
}

#include "moc_mescapebuttonpanelview.cpp"

M_REGISTER_VIEW_NEW(MEscapeButtonPanelView, MEscapeButtonPanel)
