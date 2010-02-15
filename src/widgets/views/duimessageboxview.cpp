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

#include "duimessageboxview.h"
#include "duimessageboxview_p.h"

#include "duimessagebox.h"
#include "duilabel.h"

#include <QGraphicsLinearLayout>

DuiMessageBoxViewPrivate::DuiMessageBoxViewPrivate()
{
}

DuiMessageBoxViewPrivate::~DuiMessageBoxViewPrivate()
{
}

DuiMessageBoxView::DuiMessageBoxView(DuiMessageBox *controller) :
    DuiDialogView(*new DuiMessageBoxViewPrivate, controller)
{
    Q_D(DuiMessageBoxView);
    d->label = new DuiLabel;
    d->label->setAlignment(Qt::AlignCenter);
    contentsLayout()->insertItem(0, d->label);
}

DuiMessageBoxView::~DuiMessageBoxView()
{
}

void DuiMessageBoxView::updateData(const QList<const char *>& modifications)
{
    DuiDialogView::updateData(modifications);

    Q_D(DuiMessageBoxView);
    const char *member;

    foreach(member, modifications) {
        if (member == DuiMessageBoxModel::Text) {
            d->label->setText(model()->text());
        }
    }

    updateGeometry();
}

void DuiMessageBoxView::setupModel()
{
    DuiDialogView::setupModel();

    Q_D(DuiMessageBoxView);

    //update text
    d->label->setText(model()->text());

    updateGeometry();
}

DUI_REGISTER_VIEW_NEW(DuiMessageBoxView, DuiMessageBox)
