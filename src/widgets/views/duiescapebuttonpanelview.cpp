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

#include "duiescapebuttonpanelview.h"
#include "duiescapebuttonpanelview_p.h"
#include "duibutton.h"
#include "duiviewcreator.h"
#include "duiescapebuttonpanel.h"

#include <QGraphicsGridLayout>
#include <QTimeLine>
#include <QDebug>

DuiEscapeButtonPanelViewPrivate::DuiEscapeButtonPanelViewPrivate() :
    opacityTimeLine(0),
    escapeMode(DuiEscapeButtonPanelModel::CloseMode)
{
}

DuiEscapeButtonPanelViewPrivate::~DuiEscapeButtonPanelViewPrivate()
{
}

void DuiEscapeButtonPanelViewPrivate::init()
{
    Q_Q(DuiEscapeButtonPanelView);

    QGraphicsGridLayout *mainLayout = new QGraphicsGridLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    backButton = new DuiButton(controller);
    closeButton = new DuiButton(controller);
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

void DuiEscapeButtonPanelViewPrivate::finalizeEscapeButtonTransition()
{
    switch (escapeMode) {
    case DuiEscapeButtonPanelModel::CloseMode:
        backButton->hide();
        closeButton->show();
        break;
    case DuiEscapeButtonPanelModel::BackMode:
        closeButton->hide();
        backButton->show();
        break;
    default:
        qCritical() << "DuiEscapeButtonPanelViewPrivate::finalizeEscapeButtonTransition: unknown mode for escape button";
        break;
    };
}

void DuiEscapeButtonPanelViewPrivate::opacityChange(qreal step)
{
    qreal opposite = 1.0 - step;
    switch (escapeMode) {
    case DuiEscapeButtonPanelModel::CloseMode:
        closeButton->setOpacity(step);
        backButton->setOpacity(opposite);
        break;
    case DuiEscapeButtonPanelModel::BackMode:
        closeButton->setOpacity(opposite);
        backButton->setOpacity(step);
        break;
    default:
        qCritical() << "DuiEscapeButtonPanelViewPrivate::opacityChange: unknown mode for escape button";
        break;
    };
}

DuiEscapeButtonPanelView::DuiEscapeButtonPanelView(DuiEscapeButtonPanel *controller) :
    DuiSceneWindowView(* new DuiEscapeButtonPanelViewPrivate, controller)
{
    Q_D(DuiEscapeButtonPanelView);
    d->controller = controller;
    d->init();
}

DuiEscapeButtonPanelView::~DuiEscapeButtonPanelView()
{
}

void DuiEscapeButtonPanelView::applyStyle()
{
    Q_D(DuiEscapeButtonPanelView);

    DuiSceneWindowView::applyStyle();

    d->backButton->setObjectName(style()->backButtonObjectName());
    d->backButton->setIconID(style()->backButtonIconId());
    d->closeButton->setObjectName(style()->closeButtonObjectName());
    d->closeButton->setIconID(style()->closeButtonIconId());
    d->opacityTimeLine->setDuration(style()->buttonAnimationLength());
}

void DuiEscapeButtonPanelView::updateData(const QList<const char *>& modifications)
{
    DuiSceneWindowView::updateData(modifications);

    Q_D(DuiEscapeButtonPanelView);

    const char *member;
    foreach(member, modifications) {
        if (member == DuiEscapeButtonPanelModel::Mode) {
            d->escapeMode = model()->mode();
            d->opacityTimeLine->start();
        }
    }
}

#include "moc_duiescapebuttonpanelview.cpp"

DUI_REGISTER_VIEW_NEW(DuiEscapeButtonPanelView, DuiEscapeButtonPanel)
