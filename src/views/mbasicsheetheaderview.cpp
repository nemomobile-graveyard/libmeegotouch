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


#include "mbasicsheetheaderview.h"
#include "mbasicsheetheaderview_p.h"

#include <MBasicSheetHeader>
#include <MButton>
#include <MProgressIndicator>
#include <MViewCreator>

#include <QGraphicsAnchorLayout>
#include <QAction>

MBasicSheetHeaderViewPrivate::MBasicSheetHeaderViewPrivate()
    : q_ptr(0),
    controller(0),
    layout(0),
    positiveButton(0),
    negativeButton(0),
    progressIndicator(0)
{
}

MBasicSheetHeaderViewPrivate::~MBasicSheetHeaderViewPrivate()
{
    if (controller) {
        controller->setLayout(0);
    }
    delete positiveButton;
    delete negativeButton;
    delete progressIndicator;
}

void MBasicSheetHeaderViewPrivate::createLayout()
{
    layout = new QGraphicsAnchorLayout(controller);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
}

void MBasicSheetHeaderViewPrivate::setPositiveAction(QAction *action)
{
    Q_Q(MBasicSheetHeaderView);

    if (action) {
        if (!positiveButton) {
            positiveButton = new MButton(controller);
            positiveButton->setStyleName(q->style()->positiveActionStyleName());
            layout->addAnchor(positiveButton, Qt::AnchorRight, layout, Qt::AnchorRight);
            layout->addAnchor(positiveButton, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
        }
        positiveButton->setText(action->text());
        positiveButton->setEnabled(action->isEnabled());
        positiveButton->connect(positiveButton, SIGNAL(clicked()), action, SLOT(trigger()));
        q->connect(action, SIGNAL(changed()), q, SLOT(_q_refreshPositiveAction()));
    } else {
        delete positiveButton;
        positiveButton = 0;
    }
    layout->invalidate();
}

void MBasicSheetHeaderViewPrivate::setNegativeAction(QAction *action)
{
    Q_Q(MBasicSheetHeaderView);

    if (action) {
        if (!negativeButton) {
            negativeButton = new MButton(controller);
            negativeButton->setStyleName(q->style()->negativeActionStyleName());
            layout->addAnchor(negativeButton, Qt::AnchorLeft, layout, Qt::AnchorLeft);
            layout->addAnchor(negativeButton, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
        }
        negativeButton->setText(action->text());
        negativeButton->setEnabled(action->isEnabled());
        negativeButton->connect(negativeButton, SIGNAL(clicked()), action, SLOT(trigger()));
        q->connect(action, SIGNAL(changed()), q, SLOT(_q_refreshNegativeAction()));
    } else {
        delete negativeButton;
        negativeButton = 0;
    }
    layout->invalidate();
}

void MBasicSheetHeaderViewPrivate::setProgressIndicatorVisible(bool visible)
{
    Q_Q(MBasicSheetHeaderView);

    if (visible) {
        if (!progressIndicator) {
            progressIndicator = new MProgressIndicator(controller);
            progressIndicator->setViewType(MProgressIndicator::spinnerType);
            progressIndicator->setUnknownDuration(true);
            progressIndicator->setStyleName(q->style()->progressIndicatorStyleName());
            layout->addAnchor(progressIndicator, Qt::AnchorHorizontalCenter, layout, Qt::AnchorHorizontalCenter);
            layout->addAnchor(progressIndicator, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
        }
    }
    if (progressIndicator)
        progressIndicator->setVisible(visible);

    layout->invalidate();
}

void MBasicSheetHeaderViewPrivate::_q_refreshPositiveAction()
{
    Q_Q(MBasicSheetHeaderView);
    QAction *positiveAction = q->model()->positiveAction();

    positiveButton->setText(positiveAction->text());
    positiveButton->setEnabled(positiveAction->isEnabled());
}

void MBasicSheetHeaderViewPrivate::_q_refreshNegativeAction()
{
    Q_Q(MBasicSheetHeaderView);
    QAction *negativeAction = q->model()->negativeAction();

    negativeButton->setText(negativeAction->text());
    negativeButton->setEnabled(negativeAction->isEnabled());
}

void MBasicSheetHeaderViewPrivate::refreshStyles()
{
    Q_Q(MBasicSheetHeaderView);

    if (positiveButton)
        positiveButton->setStyleName(q->style()->positiveActionStyleName());

    if (negativeButton)
        negativeButton->setStyleName(q->style()->negativeActionStyleName());

    if (progressIndicator)
        progressIndicator->setStyleName(q->style()->progressIndicatorStyleName());
}

MBasicSheetHeaderView::MBasicSheetHeaderView(MBasicSheetHeader *controller)
    : MWidgetView(controller),
    d_ptr(new MBasicSheetHeaderViewPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->controller = controller;
    d_ptr->createLayout();
}

MBasicSheetHeaderView::~MBasicSheetHeaderView()
{
    delete d_ptr;
}

void MBasicSheetHeaderView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char*> modifications;
    modifications << MBasicSheetHeaderModel::PositiveAction;
    modifications << MBasicSheetHeaderModel::NegativeAction;
    modifications << MBasicSheetHeaderModel::ProgressIndicatorVisible;

    updateData(modifications);
}

void MBasicSheetHeaderView::updateData(const QList<const char *> &modifications)
{
    Q_D(MBasicSheetHeaderView);

    MWidgetView::updateData(modifications);

    foreach (const char* member, modifications) {
        if (member == MBasicSheetHeaderModel::PositiveAction) {
            d->setPositiveAction(model()->positiveAction());
        } else if (member == MBasicSheetHeaderModel::NegativeAction) {
            d->setNegativeAction(model()->negativeAction());
        } else if (member == MBasicSheetHeaderModel::ProgressIndicatorVisible) {
            d->setProgressIndicatorVisible(model()->progressIndicatorVisible());
        }
    }
    update();
}

void MBasicSheetHeaderView::applyStyle()
{
    Q_D(MBasicSheetHeaderView);

    d->refreshStyles();
}

#include "moc_mbasicsheetheaderview.cpp"

M_REGISTER_VIEW_NEW(MBasicSheetHeaderView, MBasicSheetHeader)
