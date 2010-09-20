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
    delete fadeAnimation;
    delete backButton;
    delete closeButton;
}

void MEscapeButtonPanelViewPrivate::init()
{
    Q_Q(MEscapeButtonPanelView);

    backButton = new MButton(controller);
    closeButton = new MButton(controller);
    backButton->setViewType("icon");
    closeButton->setViewType("icon");

    QObject::connect(backButton, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));

    QObject::connect(backButton, SIGNAL(pressed()), q, SLOT(_q_buttonInteracted()));
    QObject::connect(backButton, SIGNAL(released()), q, SLOT(_q_buttonInteracted()));
    QObject::connect(closeButton, SIGNAL(pressed()), q, SLOT(_q_buttonInteracted()));
    QObject::connect(closeButton, SIGNAL(released()), q, SLOT(_q_buttonInteracted()));

    setupEscapeButtonTransition();
}

void MEscapeButtonPanelViewPrivate::animatedEscapeButtonTransition()
{
    Q_Q(MEscapeButtonPanelView);

    if (warpInAnimation && warpInAnimation->state() == QAbstractAnimation::Running) {
        warpInAnimation->setCurrentTime(warpInAnimation->duration());
        delete warpInAnimation;
    }
    if (warpOutAnimation && warpOutAnimation->state() == QAbstractAnimation::Running) {
        warpOutAnimation->setCurrentTime(warpOutAnimation->duration());
        delete warpOutAnimation;
    }
    if (fadeAnimation && fadeAnimation->state() == QAbstractAnimation::Running) {
        fadeAnimation->setCurrentTime(fadeAnimation->duration());
        delete fadeAnimation;
    }

    if (q->style()->hasCloseButton()) {
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
        warpInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        warpOutAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        fadeAnimation = new QPropertyAnimation(backButton, "opacity", controller);
        fadeAnimation->setDuration(q->style()->buttonAnimationLength());

        if (escapeMode == MEscapeButtonPanelModel::BackMode) {
            fadeAnimation->setStartValue(0);
            fadeAnimation->setEndValue(1);
        } else {
            fadeAnimation->setStartValue(1);
            fadeAnimation->setEndValue(0);
            q->connect(fadeAnimation, SIGNAL(finished()), SLOT(_q_fadeOutAnimationFinished()));
        }
        backButton->show();
        fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MEscapeButtonPanelViewPrivate::immediateEscapeButtonTransition()
{
    Q_Q(MEscapeButtonPanelView);

    switch (escapeMode) {
    case MEscapeButtonPanelModel::CloseMode:
        backButton->hide();
        if (&q->style()) {
            if (q->style()->hasCloseButton())
                closeButton->show();
        } else {
            closeButton->show();
        }
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

void MEscapeButtonPanelViewPrivate::setCloseButtonVisible(bool visible)
{
    Q_Q(MEscapeButtonPanelView);

    if (!controller->isOnDisplay()) {
        closeButton->setVisible(visible);
        return;
    }

    if (fadeAnimation && fadeAnimation->state() == QAbstractAnimation::Running) {
        fadeAnimation->setCurrentTime(fadeAnimation->duration());
        delete fadeAnimation;
    }

    fadeAnimation = new QPropertyAnimation(closeButton, "opacity", controller);
    fadeAnimation->setDuration(q->style()->buttonAnimationLength());
    closeButton->show();

    if (visible) {
        fadeAnimation->setStartValue(0);
        fadeAnimation->setEndValue(1);
    } else {
        fadeAnimation->setStartValue(1);
        fadeAnimation->setEndValue(0);
        q->connect(fadeAnimation, SIGNAL(finished()), SLOT(_q_fadeOutAnimationFinished()));
    }

    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MEscapeButtonPanelViewPrivate::_q_buttonInteracted()
{
    Q_Q(MEscapeButtonPanelView);
    q->applyStyle();
}

void MEscapeButtonPanelViewPrivate::_q_fadeOutAnimationFinished()
{
    QGraphicsWidget *button = qobject_cast<QGraphicsWidget*>(fadeAnimation->targetObject());
    if (button) {
        button->hide();
        button->setOpacity(1);
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

    MButton* button = d->escapeMode == MEscapeButtonPanelModel::BackMode ? d->backButton : d->closeButton;
    if (button->isDown())
        style().setModePressed();
    else
        style().setModeDefault();

    MSceneWindowView::applyStyle();

    d->backButton->setObjectName(style()->backButtonObjectName());
    d->backButton->setIconID(style()->backButtonIconId());
    d->closeButton->setObjectName(style()->closeButtonObjectName());
    d->closeButton->setIconID(style()->closeButtonIconId());

    if (d->escapeMode == MEscapeButtonPanelModel::CloseMode) {
        if (style()->hasCloseButton() && !d->closeButton->isVisible())
            d->setCloseButtonVisible(true);
        else if (!style()->hasCloseButton() && d->closeButton->isVisible())
            d->setCloseButtonVisible(false);
    }

    d->controller->setProperty("hasCloseButton", style()->hasCloseButton());
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
