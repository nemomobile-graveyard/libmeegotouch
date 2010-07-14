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

#include "mmessageboxview.h"
#include "mmessageboxview_p.h"

#include "mmessagebox.h"
#include "mlabel.h"

#include <QGraphicsLinearLayout>

MMessageBoxViewPrivate::MMessageBoxViewPrivate()
  : label(0)
{
}

MMessageBoxViewPrivate::~MMessageBoxViewPrivate()
{
}

MMessageBoxView::MMessageBoxView(MMessageBox *controller) :
    MDialogView(*new MMessageBoxViewPrivate, controller)
{
}

MMessageBoxView::~MMessageBoxView()
{
}

void MMessageBoxView::updateData(const QList<const char *>& modifications)
{
    MDialogView::updateData(modifications);

    Q_D(MMessageBoxView);
    const char *member;

    foreach(member, modifications) {
        if (member == MMessageBoxModel::Text) {
            d->label->setText(model()->text());
        }
    }
    if (model()->buttons().count() >= 3 && style()->maximumHorizontalButtons() >= 3)
        d->dialogBox->setPreferredWidth(style()->maximumSize().width());
    else
        d->dialogBox->setPreferredWidth(style()->dialogPreferredSize().width());

    updateGeometry();
}

void MMessageBoxView::setupModel()
{
    MDialogView::setupModel();

    Q_D(MMessageBoxView);
    model()->setAlwaysPannable(false);

    //update text
    d->label->setText(model()->text());
    if (model()->buttons().count() >= 3 && style()->maximumHorizontalButtons() >= 3)
        d->dialogBox->setPreferredWidth(style()->maximumSize().width());
    else
        d->dialogBox->setPreferredWidth(style()->dialogPreferredSize().width());


    updateGeometry();
}

void MMessageBoxView::applyStyle() {
    MDialogView::applyStyle();
    Q_D(MMessageBoxView);
    if (!d->label) {
        d->label = new MLabel;
        d->label->setAlignment(Qt::AlignCenter);
        d->label->setWordWrap(true);
        contentsLayout()->insertItem(0, d->label);
    }
}

M_REGISTER_VIEW_NEW(MMessageBoxView, MMessageBox)
