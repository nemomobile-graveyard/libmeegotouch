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
        separator(0),
        currentCentralWidget(0)
{
    layout = new QGraphicsGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    informationWidgetsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    informationWidgetsLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    informationWidgetsLayout->setContentsMargins(0, 0, 0, 0);
    informationWidgetsLayout->setSpacing(0);

    informationWidgetsSpacer = new QGraphicsWidget;
    informationWidgetsSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    innerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    innerLayout->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->setSpacing(0);

    timeStampLabel = new MLabel();
    timeStampLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    messageComposition = new MLabel();
    messageComposition->setWordWrap(true);
    messageComposition->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    messageComposition->setAlignment(Qt::AlignLeft);
    messageComposition->setTextFormat(Qt::RichText);
}

MBubbleItemViewPrivate::~MBubbleItemViewPrivate()
{
    clearLayout();
}

void MBubbleItemViewPrivate::initLayout()
{
    clearLayout();

    replaceAvatar();

    if (controller->messageType() == MBubbleItem::Incoming) {
        if (controller->avatar()) {
            layout->addItem(controller->avatar(), 0, 0);
            layout->addItem(speechBubble, 0, 1);
        } else {
            layout->addItem(speechBubble, 0, 0);
        }
    } else if(controller->messageType() == MBubbleItem::Outgoing) {
        layout->addItem(speechBubble, 0, 0);
        if (controller->avatar())
            layout->addItem(controller->avatar(), 0, 1);
    }

    updateInformationWidgetsLayout();

    innerLayout->addItem(messageComposition);
    innerLayout->addItem(informationWidgetsLayout);

    setupCentralWidget();

    controller->connect(speechBubble, SIGNAL(clicked()), controller, SIGNAL(bubbleClicked()), Qt::UniqueConnection);
}

void MBubbleItemViewPrivate::clearLayout()
{
    while (layout->count())
        layout->removeAt(0);
    while (innerLayout->count())
        innerLayout->removeAt(0);
}

void MBubbleItemViewPrivate::replaceAvatar()
{
    Q_Q(MBubbleItemView);

    if (avatar) {
        for (int i = 0; i < layout->count(); i ++) {
            if (layout->itemAt(i) == avatar)
                layout->removeAt(i);
        }
        avatar = 0;
    }

    if (controller->avatar()) {
        if (controller->avatar()->objectName().isEmpty())
            controller->avatar()->setObjectName(q->style()->avatarObjectName());
        if (controller->messageType() == MBubbleItem::Incoming) {
            layout->addItem(controller->avatar(), 0, 0);
            layout->addItem(speechBubble, 0, 1);
        }
        else
            layout->addItem(controller->avatar(), 0, 1);
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

void MBubbleItemViewPrivate::clearInformationWidgetsLayout()
{
    for (int i = 0; i < informationWidgetsLayout->count(); i++)
        informationWidgetsLayout->removeAt(0);
}

void MBubbleItemViewPrivate::updateInformationWidgetsLayout()
{
    Q_Q(MBubbleItemView);

    clearInformationWidgetsLayout();

    foreach (QGraphicsWidget *widget, q->model()->informationWidgets()) {
        informationWidgetsLayout->addItem(widget);
        informationWidgetsLayout->setAlignment(widget, Qt::AlignVCenter | Qt::AlignRight);
    }

    informationWidgetsLayout->addItem(informationWidgetsSpacer);
    informationWidgetsLayout->addItem(timeStampLabel);
}

void MBubbleItemViewPrivate::applyStyle()
{
    refreshStyleMode();
}

void MBubbleItemView::setGeometry(const QRectF &rect)
{
    MWidgetView::setGeometry(rect);

    Q_D(MBubbleItemView);
    d->updateMessageComposition();
}

void MBubbleItemViewPrivate::refreshStyleMode()
{
    Q_Q(MBubbleItemView);
    if (controller->messageType() == MBubbleItem::Incoming)
        q->style().setModeDefault();
    else
        q->style().setModeOutgoing();

    if (speechBubble)
        speechBubble->setObjectName(q->style()->bubbleObjectName());

    if (timeStampLabel) {
        timeStampLabel->setFont(q->style()->timestampFont());
        timeStampLabel->setColor(q->style()->timestampTextColor());
    }

    if (messageComposition) {
        messageComposition->setFont(q->style()->font());
        messageComposition->setColor(q->style()->textColor());
    }

    controller->setContentsMargins(q->style()->marginLeft(),
                                   q->style()->marginTop(),
                                   q->style()->marginRight(),
                                   q->style()->marginBottom());
}

void MBubbleItemViewPrivate::updateMessageComposition()
{
    Q_Q(MBubbleItemView);

    if (controller->messageType() == MBubbleItem::Incoming) {
        QString senderName = q->model()->senderName();
        if (senderName.isEmpty()) {
            messageComposition->setText(q->model()->message());
        } else {
            QFont boldFontForSendername( messageComposition->font() );
            boldFontForSendername.setBold(true);
            QFontMetrics fontMetrics(boldFontForSendername);

            if (fontMetrics.width(senderName) > messageComposition->size().width())
                senderName = fontMetrics.elidedText(senderName, Qt::ElideRight,
                                                    messageComposition->size().width()
                                                    - fontMetrics.size( Qt::TextSingleLine, "...:" ).width()
                                                    );

            messageComposition->setText( "<b>" + senderName + "</b>: " + q->model()->message());
        }
    } else if (controller->messageType() == MBubbleItem::Outgoing)
        messageComposition->setText(q->model()->message());

}
MBubbleItemView::MBubbleItemView(MBubbleItem *controller)
    : MWidgetView(* new MBubbleItemViewPrivate, controller)
{
    Q_D(MBubbleItemView);
    d->controller = controller;
    controller->setLayout(d->layout);
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
            d->refreshStyleMode();
        } else if (member == MBubbleItemModel::InformationWidgets) {
            d->updateInformationWidgetsLayout();
        }
    }
}

void MBubbleItemView::setupModel()
{
    Q_D(MBubbleItemView);

    MWidgetView::setupModel();

    d->timeStampLabel->setText(model()->timeStamp());

    connect(d->messageComposition, SIGNAL(linkActivated(QString)), d->controller, SIGNAL(linkActivated(QString)));

    d->speechBubble = new MBubbleItemBackground(d->controller);
    d->speechBubble->setMessageType(static_cast<MBubbleItem::MessageType>(model()->messageType()));
    d->speechBubble->setObjectName(style()->bubbleObjectName());
    d->speechBubble->setLayout(d->innerLayout);
    d->speechBubble->setMinimumSize(0,0); //The background must always be smaller than our current geometry

    d->initLayout();
    d->refreshStyleMode();
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

