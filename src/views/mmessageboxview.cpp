/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mimagewidget.h"
#include "mlabel.h"

#include <QGraphicsLinearLayout>

MMessageBoxViewPrivate::MMessageBoxViewPrivate()
  : iconImage(0),
  titleLabel(0),
  textLabel(0)
{
}

MMessageBoxViewPrivate::~MMessageBoxViewPrivate()
{
    clearLayout();

    delete iconImage;
    delete titleLabel;
    delete textLabel;
}

MImageWidget *MMessageBoxViewPrivate::iconImageWidget()
{
    Q_Q(MMessageBoxView);

    if (!iconImage) {
        iconImage = new MImageWidget(contents);
        iconImage->setStyleName(q->style()->iconStyleName());
    }
    return iconImage;
}

MLabel *MMessageBoxViewPrivate::titleLabelWidget()
{
    Q_Q(MMessageBoxView);

    if (!titleLabel) {
        titleLabel = new MLabel(contents);
        titleLabel->setStyleName(q->style()->titleStyleName());
        titleLabel->setTextElide(false);
        titleLabel->setWordWrap(true);
        titleLabel->setAlignment(Qt::AlignCenter);
    }
    return titleLabel;
}

MLabel *MMessageBoxViewPrivate::textLabelWidget()
{
    Q_Q(MMessageBoxView);

    if (!textLabel) {
        textLabel = new MLabel(contents);
        textLabel->setStyleName(q->style()->textStyleName());
        textLabel->setWordWrap(true);
        textLabel->setAlignment(Qt::AlignCenter);
    }
    return textLabel;
}

void MMessageBoxViewPrivate::prepareLayout()
{
    updateIconWidget();
    updateTitleWidget();
    updateTextWidget();
}

void MMessageBoxViewPrivate::updateLayout()
{
    clearLayout();
    prepareLayout();

    if (centralWidget) {
        contentsLayout->insertItem(0, centralWidget);
    }

    if (textLabel) {
        contentsLayout->insertItem(0, textLabel);
        contentsLayout->setAlignment(textLabel, Qt::AlignCenter);
    }

    if (titleLabel) {
        contentsLayout->insertItem(0, titleLabel);
        contentsLayout->setAlignment(titleLabel, Qt::AlignCenter);
    }

    if (iconImage) {
        contentsLayout->insertItem(0, iconImage);
        contentsLayout->setAlignment(iconImage, Qt::AlignCenter);
    }

    if (!QGraphicsLayout::instantInvalidatePropagation()) {
        /* Update the geometry of the parents immediately. This simply makes the layout have the correct sizehint
         * immediately instead of one frame later, removing a single frame of 'flicker'.
         */
        QGraphicsLayoutItem *item = contents;
        do {
            item->updateGeometry();
        } while( (item = item->parentLayoutItem()) );
    }
}

void MMessageBoxViewPrivate::clearLayout()
{
    contentsLayout->removeItem(iconImage);
    contentsLayout->removeItem(titleLabel);
    contentsLayout->removeItem(textLabel);
    contentsLayout->removeItem(centralWidget);
}

void MMessageBoxViewPrivate::updateIconWidget()
{
    Q_Q(MMessageBoxView);

    if (q->model()->iconId().isEmpty() && q->model()->iconPixmap().isNull()) {
        if (iconImage)
            delete iconImage;
        iconImage = NULL;
    } else {
        if (!q->model()->iconPixmap().isNull())
            iconImageWidget()->setPixmap(q->model()->iconPixmap());
        else
            iconImageWidget()->setImage(q->model()->iconId());
    }
}

void MMessageBoxViewPrivate::updateTitleWidget()
{
    Q_Q(MMessageBoxView);

    if (q->model()->title().isEmpty() || q->style()->hasTitleBar()) {
        if (titleLabel)
            delete titleLabel;
        titleLabel = NULL;
    } else {
        titleLabelWidget()->setText(q->model()->title());
    }
}

void MMessageBoxViewPrivate::updateTextWidget()
{
    Q_Q(MMessageBoxView);

    if (q->model()->text().isEmpty()) {
        if (textLabel)
            delete textLabel;
        textLabel = NULL;
    } else {
        textLabelWidget()->setText(q->model()->text());
    }
}

MMessageBoxView::MMessageBoxView(MMessageBox *controller) :
    MDialogView(*new MMessageBoxViewPrivate, controller)
{
    Q_D(MMessageBoxView);
    d->controller = qobject_cast<MMessageBox*>(controller);
    d->buttonBox->setStyleName("MMessageBoxButtonBox");
}

MMessageBoxView::~MMessageBoxView()
{
}

void MMessageBoxView::updateData(const QList<const char *>& modifications)
{
    MDialogView::updateData(modifications);

    Q_D(MMessageBoxView);
    const char *member;

    bool updateLayout = false;
    foreach(member, modifications) {
        if (member == MMessageBoxModel::Text || member == MMessageBoxModel::Title ||
            member == MMessageBoxModel::IconId || member == MMessageBoxModel::IconPixmap ||
            member == MDialogModel::CentralWidget) {
            updateLayout = true;
            break;
        }
    }

    if (updateLayout)
        d->updateLayout();

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

    if (model()->buttons().count() >= 3 && style()->maximumHorizontalButtons() >= 3)
        d->dialogBox->setPreferredWidth(style()->maximumSize().width());
    else
        d->dialogBox->setPreferredWidth(style()->dialogPreferredSize().width());

    // update the layout if MDialogView::setupModel() inserted us a central widget
    bool updateLayout = false;
    for (int i = 0; i < d->contentsLayout->count(); i++) {
        if (d->contentsLayout->itemAt(i) == d->centralWidget) {
            updateLayout = true;
            break;
        }
    }

    if (updateLayout)
        d->updateLayout();

    updateGeometry();
}

void MMessageBoxView::applyStyle() {
    Q_D(MMessageBoxView);

    MDialogView::applyStyle();

    d->updateLayout();

    if (d->iconImage)
        d->iconImage->setStyleName(style()->iconStyleName());

    if (d->titleLabel)
        d->titleLabel->setStyleName(style()->titleStyleName());

    if (d->textLabel)
        d->textLabel->setStyleName(style()->textStyleName());
}

M_REGISTER_VIEW_NEW(MMessageBoxView, MMessageBox)
