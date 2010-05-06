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

#include <QGraphicsGridLayout>
#include <QTimeLine>
#include <QDebug>

MEscapeButtonPanelViewPrivate::MEscapeButtonPanelViewPrivate() :
    q_ptr(0),
    opacityTimeLine(0),
    escapeMode(MEscapeButtonPanelModel::CloseMode),
    controller(0)
{
}

MEscapeButtonPanelViewPrivate::~MEscapeButtonPanelViewPrivate()
{
}

void MEscapeButtonPanelViewPrivate::init()
{
    Q_Q(MEscapeButtonPanelView);

    QGraphicsGridLayout *mainLayout = new QGraphicsGridLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    backButton = new MButton(controller);
    closeButton = new MButton(controller);
    backButton->setViewType("icon");
    closeButton->setViewType("icon");

    mainLayout->addItem(closeButton, 0, 0);
    mainLayout->addItem(backButton, 0, 0);
    controller->setLayout(mainLayout);

    QObject::connect(backButton, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(buttonClicked()));

    opacityTimeLine = new QTimeLine(500, q);
    QObject::connect(opacityTimeLine, SIGNAL(finished()),
                     q, SLOT(finalizeEscapeButtonTransition()));

    QObject::connect(opacityTimeLine, SIGNAL(valueChanged(qreal)),
                     q, SLOT(opacityChange(qreal)));

    finalizeEscapeButtonTransition();
}

void MEscapeButtonPanelViewPrivate::finalizeEscapeButtonTransition()
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
        qCritical() << "MEscapeButtonPanelViewPrivate::finalizeEscapeButtonTransition: unknown mode for escape button";
        break;
    };
}

void MEscapeButtonPanelViewPrivate::opacityChange(qreal step)
{
    qreal opposite = 1.0 - step;
    switch (escapeMode) {
    case MEscapeButtonPanelModel::CloseMode:
        closeButton->setOpacity(step);
        backButton->setOpacity(opposite);
        break;
    case MEscapeButtonPanelModel::BackMode:
        closeButton->setOpacity(opposite);
        backButton->setOpacity(step);
        break;
    default:
        qCritical() << "MEscapeButtonPanelViewPrivate::opacityChange: unknown mode for escape button";
        break;
    };
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
    d->opacityTimeLine->setDuration(style()->buttonAnimationLength());
}

void MEscapeButtonPanelView::updateData(const QList<const char *>& modifications)
{
    MSceneWindowView::updateData(modifications);

    Q_D(MEscapeButtonPanelView);

    const char *member;
    foreach(member, modifications) {
        if (member == MEscapeButtonPanelModel::Mode) {
            d->escapeMode = model()->mode();
            d->opacityTimeLine->start();
        }
    }
}

#include "moc_mescapebuttonpanelview.cpp"

M_REGISTER_VIEW_NEW(MEscapeButtonPanelView, MEscapeButtonPanel)
