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

#include <MScalableImage>
#include <MImageWidget>
#include <MLabel>
#include <MWidgetController>
#include <MWidgetView>
#include <MCancelEvent>
#include <mviewcreator.h>
#include <MSeparator>

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneEvent>

#include "mbubbleitem.h"
#include "mbubbleitemview.h"
#include "mbubbleitemview_p.h"
#include "mbubbleitembackground_p.h"

MBubbleItemViewPrivate::MBubbleItemViewPrivate()
        : avatar(0),
        speechBubble(0),
        timeStampLabel(0),
        messageComposition(0),
        numberOfCommentsLabel(0),
        numberOfThumbsUpLabel(0),
        separator(0),
        currentCentralWidget(0)
{
    layout = new QGraphicsGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    innerLayout = new QGraphicsLinearLayout(Qt::Vertical);

    timeStampLabel = new MLabel();
    timeStampLabel->setObjectName("MBubbleItemTimeStamp");
    timeStampLabel->setAlignment(Qt::AlignRight);

    messageComposition = new MLabel();
    messageComposition->setWordWrap(true);
    messageComposition->setAlignment(Qt::AlignLeft);
}

MBubbleItemViewPrivate::~MBubbleItemViewPrivate()
{
    clearLayout();
}

void MBubbleItemViewPrivate::initLayout()
{
    Q_Q(MBubbleItemView);

    clearLayout();
    
    replaceAvatar();
    
    if (controller->messageType() == MBubbleItem::Incoming) {
        if(controller->avatar()) {
            layout->addItem(controller->avatar(), 0,0);
            layout->addItem(speechBubble,0,1);
        } else {
            layout->addItem(speechBubble,0,0);
        }
    } else if(controller->messageType() == MBubbleItem::Outgoing) {
        layout->addItem(speechBubble,0,0);
    }

    innerLayout = new QGraphicsLinearLayout(Qt::Vertical, speechBubble);
    messageComposition->setObjectName("MBubbleItemMessage");

    innerLayout->addItem(messageComposition);

    if (q->model()->commentsString().isEmpty() && q->model()->thumbsUpString().isEmpty()) {
        innerLayout->addItem(timeStampLabel);
        timeStampLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    } else {
        timeStampLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        QGraphicsLinearLayout* informationWidgetsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        informationWidgetsLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        MLabel* alignTimeStampToTheRight = new MLabel();
        alignTimeStampToTheRight->setAlignment(Qt::AlignLeft);
        alignTimeStampToTheRight->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
        informationWidgetsLayout->addItem(alignTimeStampToTheRight);

        if (!q->model()->commentsString().isEmpty()) {
            MImageWidget* numberOfCommentsIcon = new MImageWidget("icon-s-common-presence-away");
            numberOfCommentsIcon->setObjectName("MBubbleItemNumberOfCommentsIcon");

            numberOfCommentsLabel = new MLabel(q->model()->commentsString());
            numberOfCommentsLabel->setObjectName("MBubbleItemNumberOfCommentsLabel");

            informationWidgetsLayout->addItem(numberOfCommentsIcon);
            informationWidgetsLayout->addItem(numberOfCommentsLabel);
        }
        if (!q->model()->thumbsUpString().isEmpty()) {
            MImageWidget* numberOfThumbsIcon = new MImageWidget("icon-s-common-presence-offline");
            numberOfThumbsIcon->setObjectName("MBubbleItemNumberOfThumbsIcon");

            numberOfThumbsUpLabel = new MLabel(q->model()->thumbsUpString());
            numberOfThumbsUpLabel->setObjectName("MBubbleItemNumberofThumbsLabel");

            informationWidgetsLayout->addItem(numberOfThumbsIcon);
            informationWidgetsLayout->addItem(numberOfThumbsUpLabel);
        }

        informationWidgetsLayout->addItem(timeStampLabel);
        innerLayout->addItem(informationWidgetsLayout);
    }

    setupCentralWidget();
}

void MBubbleItemViewPrivate::clearLayout()
{
    for (int i = 0; i < layout->count(); i++)
        layout->removeAt(0);
    for (int i = 0; i < innerLayout->count(); i++)
        innerLayout->removeAt(0);
}

void MBubbleItemViewPrivate::replaceAvatar()
{
    if (avatar) {
        for (int i = 0; i < layout->count(); i ++) {
            if (layout->itemAt(i) == avatar)
                layout->removeAt(i);
        }
        delete avatar;
        avatar = 0;
    }
    
    if (controller->avatar() && controller->messageType() == MBubbleItem::Incoming) {
        if (controller->avatar()->objectName().isEmpty())
            controller->avatar()->setObjectName("MBubbleItemAvatar");
        layout->addItem(controller->avatar(), 0, 0);
        avatar = controller->avatar();
    }
}

void MBubbleItemViewPrivate::setupCentralWidget()
{
    Q_Q(MBubbleItemView);

    if (q->model()->centralWidget() &&
        q->model()->centralWidget() != currentCentralWidget) {
        separator = new MSeparator;
        separator->setOrientation(Qt::Horizontal);
        innerLayout->addItem(separator);

        currentCentralWidget = q->model()->centralWidget();
        innerLayout->addItem(currentCentralWidget);
    } else {
        if (separator) {
            innerLayout->removeItem(separator);
            delete separator;
            separator = 0;
        }

        if (currentCentralWidget) {
            innerLayout->removeItem(currentCentralWidget);
            currentCentralWidget = 0;
        }
    }
}

void MBubbleItemViewPrivate::applyStyle()
{
}

void MBubbleItemViewPrivate::updateMessageComposition()
{
    Q_Q(MBubbleItemView);

    if (controller->messageType() == MBubbleItem::Incoming)
        messageComposition->setText("<b>" + q->model()->senderName() + "</b>: " + q->model()->message());
    else if (controller->messageType() == MBubbleItem::Outgoing)
        messageComposition->setText(q->model()->message() + "<b></b>");
}

MBubbleItemView::MBubbleItemView(MBubbleItem *controller)
    : MWidgetView(* new MBubbleItemViewPrivate, controller)
{
    Q_D(MBubbleItemView);
    d->controller = controller;
    controller->setLayout(d->layout);
    d->layout->setParentLayoutItem(controller);
}

MBubbleItemView::MBubbleItemView(MBubbleItemViewPrivate &dd, MBubbleItem *controller)
    : MWidgetView(dd, controller)
{
    Q_D(MBubbleItemView);
    d->controller = controller;
    controller->setLayout(d->layout);
}

MBubbleItemView::~MBubbleItemView()
{
}

void MBubbleItemView::applyStyle()
{
    MWidgetView::applyStyle();

    Q_D(MBubbleItemView);
    d->applyStyle();
}

void MBubbleItemView::updateData(const QList<const char *> &modifications)
{
    Q_D(MBubbleItemView);

    MWidgetView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == MBubbleItemModel::Avatar) {
            d->replaceAvatar();
        } else if (member == MBubbleItemModel::SenderName) {
            d->updateMessageComposition();
        } else if (member == MBubbleItemModel::TimeStamp) {
            d->timeStampLabel->setText(model()->timeStamp());
        } else if (member == MBubbleItemModel::Message) {
            d->updateMessageComposition();
        } else if (member == MBubbleItemModel::CentralWidget) {
            d->setupCentralWidget();
        } else if (member == MBubbleItemModel::MessageType) {
            d->speechBubble->setMessageType(static_cast<MBubbleItem::MessageType>(model()->messageType()));
            d->updateMessageComposition();
            d->initLayout();
        } else if(member == MBubbleItemModel::CommentsString) {
            if (d->numberOfCommentsLabel)
                d->numberOfCommentsLabel->setText(model()->commentsString());
        } else if(member == MBubbleItemModel::ThumbsUpString) {
            if (d->numberOfCommentsLabel)
                d->numberOfCommentsLabel->setText(model()->thumbsUpString());
        }
    }
}

void MBubbleItemView::setupModel()
{
    Q_D(MBubbleItemView);

    MWidgetView::setupModel();

    d->timeStampLabel->setText(model()->timeStamp());

    connect(d->messageComposition, SIGNAL(linkActivated(QString)), d->controller, SIGNAL(linkActivated(QString)));
    d->updateMessageComposition();

    d->speechBubble = new MBubbleItemBackground(d->controller);
    d->speechBubble->setMessageType(static_cast<MBubbleItem::MessageType>(model()->messageType()));

    d->initLayout();
}

void MBubbleItemView::setSelected(bool selected)
{
    Q_UNUSED(selected);
}

void MBubbleItemView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MBubbleItemView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}


M_REGISTER_VIEW_NEW(MBubbleItemView, MBubbleItem)

