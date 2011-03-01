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


#include "mbasicsheetheader.h"
#include "mbasicsheetheader_p.h"
#include "mbutton.h"
#include "mprogressindicator.h"

#include <QGraphicsAnchorLayout>
#include <QAction>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MBasicSheetHeader)

MBasicSheetHeaderPrivate::MBasicSheetHeaderPrivate()
    : q_ptr(0)
{
}

MBasicSheetHeaderPrivate::~MBasicSheetHeaderPrivate()
{
}

void MBasicSheetHeaderPrivate::_q_removePositiveActionFromModel()
{
    Q_Q(MBasicSheetHeader);
    q->model()->setPositiveAction(0);
}

void MBasicSheetHeaderPrivate::_q_removeNegativeActionFromModel()
{
    Q_Q(MBasicSheetHeader);
    q->model()->setNegativeAction(0);
}

void MBasicSheetHeaderPrivate::init()
{
}

MBasicSheetHeader::MBasicSheetHeader(QGraphicsItem *parent)
    : MWidgetController(new MBasicSheetHeaderModel, parent),
    d_ptr(new MBasicSheetHeaderPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->init();
}

MBasicSheetHeader::~MBasicSheetHeader()
{
    if (model()->positiveAction()) {
        QObject::disconnect(model()->positiveAction(), SIGNAL(destroyed()),
                            this, SLOT(_q_removePositiveActionFromModel()));
    }

    if (model()->negativeAction()) {
        QObject::disconnect(model()->negativeAction(), SIGNAL(destroyed()),
                            this, SLOT(_q_removeNegativeActionFromModel()));
    }

    delete d_ptr;
}

void MBasicSheetHeader::setPositiveAction(QAction *newAction)
{
    QAction *currentAction = model()->positiveAction();

    if (newAction) {
        connect(newAction, SIGNAL(destroyed()), SLOT(_q_removePositiveActionFromModel()));
    }
    model()->setPositiveAction(newAction);

    if (currentAction) {
        QObject::disconnect(currentAction, SIGNAL(destroyed()),
                            this, SLOT(_q_removePositiveActionFromModel()));

        if (currentAction->parent() == this)
            delete currentAction;
    }
}

void MBasicSheetHeader::setNegativeAction(QAction *newAction)
{
    QAction *currentAction = model()->negativeAction();

    if (newAction) {
        connect(newAction, SIGNAL(destroyed()), SLOT(_q_removeNegativeActionFromModel()));
    }
    model()->setNegativeAction(newAction);

    if (currentAction) {
        QObject::disconnect(currentAction, SIGNAL(destroyed()),
                            this, SLOT(_q_removeNegativeActionFromModel()));

        if (currentAction->parent() == this)
            delete currentAction;
    }
}

QAction *MBasicSheetHeader::positiveAction() const
{
    return model()->positiveAction();
}

QAction *MBasicSheetHeader::negativeAction() const
{
    return model()->negativeAction();
}

void MBasicSheetHeader::setProgressIndicatorVisible(bool visibile)
{
    model()->setProgressIndicatorVisible(visibile);
}

bool MBasicSheetHeader::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

#include "moc_mbasicsheetheader.cpp"
