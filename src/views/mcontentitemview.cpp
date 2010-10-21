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
#include <MWidgetView>
#include <MCancelEvent>

#include <QGraphicsGridLayout>
#include <QGraphicsSceneEvent>
#include <QTapAndHoldGesture>
#include <QTimer>

#include "mcontentitem.h"
#include "mcontentitemview.h"
#include "mcontentitemview_p.h"
#include "mviewconstants.h"

static const int NOCONTENTITEMSTYLE = -1;
QVector<MContentItemViewPrivate::backgroundFunc> MContentItemViewPrivate::backgroundFunctions;

// Distance in pixels from the widget bounding box inside which a release is still accepted

MContentItemViewPrivate::MContentItemViewPrivate()
    : controller(NULL), titleLabel(NULL), subtitleLabel(NULL), imageWidget(NULL), configuredStyle(NOCONTENTITEMSTYLE), down(false), optionalImageWidget(0), styleModeChangeTimer(0), queuedStyleModeChange(false)
{
    layout = new QGraphicsGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    initBackgroundFunctions();
}

MContentItemViewPrivate::~MContentItemViewPrivate()
{
    clearLayout();
    delete titleLabel;
    delete subtitleLabel;
    delete imageWidget;
}

void MContentItemViewPrivate::_q_applyQueuedStyleModeChange()
{
    if (queuedStyleModeChange) {
        queuedStyleModeChange = false;
        refreshStyleMode();
    }
}

void MContentItemViewPrivate::initBackgroundFunctions()
{
    if(backgroundFunctions.size() == 0){
        backgroundFunctions.resize(MContentItem::TwoIconsTextLabelVertical + 1);
        backgroundFunctions[MContentItem::Default] = &MContentItemStyle::backgroundImage;
        backgroundFunctions[MContentItem::TopLeft] = &MContentItemStyle::backgroundImageTopLeft;
        backgroundFunctions[MContentItem::Top] = &MContentItemStyle::backgroundImageTop;
        backgroundFunctions[MContentItem::TopRight] = &MContentItemStyle::backgroundImageTopRight;
        backgroundFunctions[MContentItem::Left] = &MContentItemStyle::backgroundImageLeft;
        backgroundFunctions[MContentItem::Center] = &MContentItemStyle::backgroundImageCenter;
        backgroundFunctions[MContentItem::Right] = &MContentItemStyle::backgroundImageRight;
        backgroundFunctions[MContentItem::BottomLeft] = &MContentItemStyle::backgroundImageBottomLeft;
        backgroundFunctions[MContentItem::Bottom] = &MContentItemStyle::backgroundImageBottom;
        backgroundFunctions[MContentItem::BottomRight] = &MContentItemStyle::backgroundImageBottomRight;
        backgroundFunctions[MContentItem::Single] = &MContentItemStyle::backgroundImageSingle;
        backgroundFunctions[MContentItem::SingleRowLeft] = &MContentItemStyle::backgroundImageSinglerowLeft;
        backgroundFunctions[MContentItem::SingleRowCenter] = &MContentItemStyle::backgroundImageSinglerowCenter;
        backgroundFunctions[MContentItem::SingleRowRight] = &MContentItemStyle::backgroundImageSinglerowRight;
        backgroundFunctions[MContentItem::SingleColumnTop] = &MContentItemStyle::backgroundImageSinglecolumnTop;
        backgroundFunctions[MContentItem::SingleColumnCenter] = &MContentItemStyle::backgroundImageSinglecolumnCenter;
        backgroundFunctions[MContentItem::SingleColumnBottom] = &MContentItemStyle::backgroundImageSinglecolumnBottom;
        backgroundFunctions[MContentItem::TwoIconsTextLabelVertical] = &MContentItemStyle::backgroundImageSinglecolumnBottom;

    }
}

MLabel *MContentItemViewPrivate::title()
{
    if (!titleLabel) {
        titleLabel = new MLabel(controller);
        titleLabel->setTextElide(true);
        Q_Q(MContentItemView);
        titleLabel->setObjectName(q->style()->titleObjectName());
    }

    return titleLabel;
}

MLabel *MContentItemViewPrivate::subtitle()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(controller);
        subtitleLabel->setTextElide(true);
        Q_Q(MContentItemView);
        subtitleLabel->setObjectName(q->style()->subtitleObjectName());
    }

    return subtitleLabel;
}

MImageWidget *MContentItemViewPrivate::image()
{
    if (!imageWidget) {
        imageWidget = new MImageWidget(controller);
        Q_Q(MContentItemView);
        imageWidget->setObjectName(q->style()->imageObjectName());
    }

    return imageWidget;
}

MImageWidget * MContentItemViewPrivate::optionalImage()
{
    if(!optionalImageWidget){
        optionalImageWidget = new MImageWidget(controller);
        Q_Q(MContentItemView);
        optionalImageWidget->setObjectName(q->style()->optionalImageObjectName());
    }

    return optionalImageWidget;
}


void MContentItemViewPrivate::initLayout(MContentItem::ContentItemStyle style)
{
    if (configuredStyle == style)
        return;

    clearLayout();

    configuredStyle = style;

    if (optionalImageWidget)
        optionalImageWidget->setVisible(false);
    switch (style) {
    case MContentItem::IconAndTwoTextLabels:
        image()->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        layout->addItem(image(), 0, 0, 4, 1);
        layout->addItem(new QGraphicsWidget(controller), 0, 1);
        layout->addItem(title(), 1, 1);
        layout->addItem(subtitle(), 2, 1);
        layout->addItem(new QGraphicsWidget(controller), 3, 1);

        image()->setVisible(true);
        title()->setVisible(true);
        subtitle()->setVisible(true);
        break;

    case MContentItem::SingleTextLabel:
        layout->addItem(title(), 0, 0, Qt::AlignCenter);

        if (subtitleLabel)
            subtitleLabel->setVisible(false);
        if (imageWidget)
            imageWidget->setVisible(false);
        break;

    case MContentItem::IconAndSingleTextLabel:
        layout->addItem(image(), 0, 0);
        layout->addItem(title(), 0, 1, Qt::AlignCenter);
        image()->setVisible(true);
        title()->setVisible(true);

        if (subtitleLabel)
            subtitleLabel->setVisible(false);
        break;

    case MContentItem::TwoTextLabels:
        layout->addItem(new QGraphicsWidget(controller), 0, 0);
        layout->addItem(title(), 1, 0, Qt::AlignBottom | Qt::AlignCenter);
        layout->addItem(subtitle(), 2, 0, Qt::AlignTop | Qt::AlignCenter);
        layout->addItem(new QGraphicsWidget(controller), 3, 0);

        if (imageWidget)
            imageWidget->setVisible(false);
        break;

    case MContentItem::SingleIcon:
        layout->addItem(image(), 0, 0, Qt::AlignCenter);
        image()->setVisible(true);

        if (titleLabel)
            title()->setVisible(false);
        if (subtitleLabel)
            subtitle()->setVisible(false);

        break;

    case MContentItem::IconAndSingleTextLabelVertical:
        layout->addItem(image(), 0, 0, Qt::AlignCenter);
        layout->addItem(title(), 1, 0, Qt::AlignCenter);

        title()->setAlignment(Qt::AlignCenter);

        image()->setVisible(true);
        title()->setVisible(true);

        if (subtitleLabel)
            subtitleLabel->setVisible(false);
        break;

    case MContentItem::IconAndTwoTextLabelsVertical:
        layout->addItem(image(), 0, 0, Qt::AlignCenter);

        layout->addItem(title(), 1, 0, Qt::AlignCenter);
        layout->addItem(subtitle(), 2, 0, Qt::AlignCenter);


        title()->setAlignment(Qt::AlignCenter);
        subtitle()->setAlignment(Qt::AlignCenter);

        image()->setVisible(true);
        title()->setVisible(true);
        subtitle()->setVisible(true);

        break;

    case MContentItem::TwoIconsTwoWidgets:
        initTwoIconsTwoWidgetsLayout();
        break;
    };
}

void MContentItemViewPrivate::initTwoIconsTwoWidgetsLayout()
{
    layout->addItem( image(), 0,0, 2,1, Qt::AlignCenter );
    image()->setVisible(true);
    image()->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    layout->addItem( title(), 0,1, Qt::AlignLeft );

    // first free widget or subtitle
    int columnSpan = 1;
    if( !controller->smallItem() )
        columnSpan = 2;

    if( controller->additionalItem() ) {
        layout->addItem( controller->additionalItem(), 1,1, 1,columnSpan, Qt::AlignLeft );
        subtitle()->setVisible(false);
    } else {
        layout->addItem( subtitle(), 1,1, 1,columnSpan, Qt::AlignLeft );
        subtitle()->setVisible(true);
    }

    layout->addItem( optionalImage(), 0,2, Qt::AlignRight );
    optionalImage()->setVisible(true);

    if( controller->smallItem() )
        layout->addItem( controller->smallItem(), 1,2, Qt::AlignRight );
}

void MContentItemViewPrivate::clearLayout()
{
    for (int i = layout->count()-1; i>=0; i--) {
        layout->removeAt(i);
    }
}

void MContentItemViewPrivate::setTitle(const QString &string)
{
    title()->setText(string);
}

void MContentItemViewPrivate::setSubtitle(const QString &string)
{
    subtitle()->setText(string);
}

void MContentItemViewPrivate::setPixmap(const QPixmap &pixmap)
{
    image()->setPixmap(pixmap);
}

void MContentItemViewPrivate::setOptionalPixmap(const QPixmap& pixmap)
{
    optionalImage()->setPixmap(pixmap);
}

void MContentItemViewPrivate::setImage(const QImage &i)
{
    image()->setImage(i);
}

void MContentItemViewPrivate::setImage(const QString &i, const QSize &s)
{
    QSize imageSize = s;
    if (imageSize.isNull()) {
        imageSize = preferredImageSize(image());
        controller->model()->setItemImageSize(imageSize);
    }

    image()->setImage(i, imageSize);
}

void MContentItemViewPrivate::setOptionalImage(const QImage& i)
{
    optionalImage()->setImage(i);
}

void MContentItemViewPrivate::setOptionalImage(const QString &i, const QSize &s)
{
    QSize imageSize = s;
    if (imageSize.isNull()) {
        imageSize = preferredImageSize(optionalImage());
        controller->model()->setOptionalImageSize(imageSize);
    }

    optionalImage()->setImage(i, imageSize);
}

QSize MContentItemViewPrivate::preferredImageSize(MImageWidget *image)
{
    if (!image)
        return QSize();

    qreal marginLeft, marginTop, marginRight, marginBottom;
    image->getContentsMargins(&marginLeft, &marginTop, &marginRight, &marginBottom);
    QSize imageSize = image->preferredSize().toSize();
    imageSize.setWidth(imageSize.width() - (marginLeft + marginRight));
    imageSize.setHeight(imageSize.height() - (marginTop + marginBottom));

    return imageSize;
}

void MContentItemViewPrivate::applyStyle()
{
    Q_Q(MContentItemView);

    if (titleLabel)
        titleLabel->setObjectName(q->style()->titleObjectName());

    if (subtitleLabel)
        subtitleLabel->setObjectName(q->style()->subtitleObjectName());

    if (imageWidget)
        imageWidget->setObjectName(q->style()->imageObjectName());
}

void MContentItemViewPrivate::refreshStyleMode()
{
    Q_Q(MContentItemView);

    if (down) {
        if (styleModeChangeTimer->isActive()) {
            styleModeChangeTimer->start(M_PRESS_STYLE_TIMEOUT);
            return;
        }
        styleModeChangeTimer->start(M_PRESS_STYLE_TIMEOUT);
        q->style().setModePressed();
    } else {
        if(controller->isSelected()) {
            q->style().setModeSelected();
        } else {
            if (styleModeChangeTimer->isActive()) {
                queuedStyleModeChange = true;
                return;
            }
            q->style().setModeDefault();
        }
    }

    q->applyStyle();
    q->update();
}

void MContentItemViewPrivate::longTap(const QPointF &pos)
{
    controller->longTap(pos);
}

MContentItemView::MContentItemView(MContentItem *controller)
    : MWidgetView(* new MContentItemViewPrivate, controller)
{
    Q_D(MContentItemView);
    d->controller = controller;
    controller->setLayout(d->layout);
    d->styleModeChangeTimer = new QTimer(this);
    d->styleModeChangeTimer->setSingleShot(true);
    connect(d->styleModeChangeTimer, SIGNAL(timeout()), SLOT(_q_applyQueuedStyleModeChange()));
}

MContentItemView::MContentItemView(MContentItemViewPrivate &dd, MContentItem *controller)
    : MWidgetView(dd, controller)
{
    Q_D(MContentItemView);
    d->controller = controller;
    controller->setLayout(d->layout);
    d->styleModeChangeTimer = new QTimer(this);
    d->styleModeChangeTimer->setSingleShot(true);
    connect(d->styleModeChangeTimer, SIGNAL(timeout()), SLOT(_q_applyQueuedStyleModeChange()));
}

MContentItemView::~MContentItemView()
{

}

void MContentItemView::applyStyle()
{
    MWidgetView::applyStyle();

    Q_D(MContentItemView);
    d->applyStyle();
}

void MContentItemView::updateData(const QList<const char *> &modifications)
{
    Q_D(MContentItemView);

    MWidgetView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == MContentItemModel::Title) {
            d->setTitle(model()->title());
        } else if (member == MContentItemModel::Subtitle) {
            d->setSubtitle(model()->subtitle());
        } else if (member == MContentItemModel::ItemPixmap) {
            d->setPixmap(model()->itemPixmap());
        } else if (member == MContentItemModel::Selected) {
            setSelected(model()->selected());
        } else if(member == MContentItemModel::OptionalPixmap){
            d->setOptionalPixmap(d->controller->optionalPixmap());
        } else if(member == MContentItemModel::OptionalImage){
            d->setOptionalImage(model()->optionalImage());
        } else if(member == MContentItemModel::OptionalImageID ||
                  member == MContentItemModel::OptionalImageSize) {
            d->setOptionalImage(model()->optionalImageID(), model()->optionalImageSize());
        } else if(member == MContentItemModel::ItemImage) {
            d->setImage(model()->itemImage());
        } else if(member == MContentItemModel::ItemImageID ||
                  member == MContentItemModel::ItemImageSize) {
            d->setImage(model()->itemImageID(), model()->itemImageSize());
        } else if(member == MContentItemModel::AdditionalItem ||
                  member == MContentItemModel::SmallItem) {
            if (d->configuredStyle == MContentItem::TwoIconsTwoWidgets) {
                d->clearLayout();
                d->initTwoIconsTwoWidgetsLayout();
            }
        }
    }
}

void MContentItemView::setupModel()
{
    Q_D(MContentItemView);

    MWidgetView::setupModel();

    if (!model()->title().isEmpty())
        d->setTitle(model()->title());
    if (!model()->subtitle().isEmpty())
        d->setSubtitle(model()->subtitle());
    if (!d->controller->pixmap().isNull())
        d->setPixmap(d->controller->pixmap());
    if(!d->controller->optionalPixmap().isNull())
        d->setOptionalPixmap(d->controller->optionalPixmap());
    if(!d->controller->optionalImage().isNull())
        d->setOptionalImage(d->controller->optionalImage());
    if(!model()->optionalImageID().isNull())
        d->setOptionalImage(model()->optionalImageID(), model()->optionalImageSize());
    if(!d->controller->image().isNull())
        d->setImage(d->controller->image());
    if(!model()->itemImageID().isNull())
        d->setImage(model()->itemImageID(), model()->itemImageSize());

    d->initLayout(static_cast<MContentItem::ContentItemStyle>(model()->itemStyle()));
}

void MContentItemView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MContentItemView);

    if (!d->down)
        return;

    style()->cancelFeedback().play();
    d->down = false;
    d->refreshStyleMode();
}

void MContentItemView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MContentItemView);

    if (d->down)
        return;

    style()->pressFeedback().play();
    d->down = true;
    d->refreshStyleMode();
}

void MContentItemView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MContentItemView);

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    bool pressed = rect.contains(touch);

    if (pressed != d->down) {
        if (pressed) {
            style()->pressFeedback().play();
        } else {
            style()->cancelFeedback().play();
        }
        d->down = pressed;
        d->refreshStyleMode();
    }
}

void MContentItemView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    Q_D(MContentItemView);

    if (gesture->state() == Qt::GestureFinished)
        d->longTap(gesture->position());

    event->accept(gesture);
}

void MContentItemView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MContentItemView);

    if (!d->down)
        return;
    d->down = false;
    d->refreshStyleMode();

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    bool pressed = rect.contains(touch);

    if (pressed) {
        style()->releaseFeedback().play();
        d->controller->click();
    }
}

void MContentItemView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());

    if (style()->backgroundImage()) {
        MContentItemViewPrivate::backgroundFunc func = MContentItemViewPrivate::backgroundFunctions[model()->itemMode()];
        const MContentItemStyle *itemStyle = style().operator ->();
        const MScalableImage *itemBackground = ((*itemStyle).*func)();

        if( itemBackground )
            itemBackground->draw(0.0, 0.0, size().width(), size().height(), painter);
    } else if (style()->backgroundColor().isValid()) {
        painter->fillRect(boundingRect(), QBrush(style()->backgroundColor()));
    }

    painter->setOpacity(oldOpacity);

}

void MContentItemView::setSelected(bool selected)
{
    if (selected)
        style().setModeSelected();
    else
        style().setModeDefault();

    applyStyle();
    updateGeometry();
}

M_REGISTER_VIEW_NEW(MContentItemView, MContentItem)

#include "moc_mcontentitemview.cpp"
