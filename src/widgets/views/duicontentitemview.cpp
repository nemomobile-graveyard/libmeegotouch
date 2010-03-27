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

#include <DuiScalableImage>
#include <DuiImageWidget>
#include <DuiLabel>
#include <DuiWidgetView>
#include <DuiCancelEvent>

#include <QGraphicsGridLayout>
#include <QGraphicsSceneEvent>

#include "duicontentitem.h"
#include "duicontentitemview.h"
#include "duicontentitemview_p.h"

static const int NOCONTENTITEMSTYLE = -1;
QVector<DuiContentItemViewPrivate::backgroundFunc> DuiContentItemViewPrivate::backgroundFunctions;

DuiContentItemViewPrivate::DuiContentItemViewPrivate()
    : controller(NULL), titleLabel(NULL), subtitleLabel(NULL), imageWidget(NULL), configuredStyle(NOCONTENTITEMSTYLE), down(false), optionalImageWidget(0)
{
    layout = new QGraphicsGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    initBackgroundFunctions();
}

DuiContentItemViewPrivate::~DuiContentItemViewPrivate()
{
    clearLayout();
    delete titleLabel;
    delete subtitleLabel;
    delete imageWidget;
}

void DuiContentItemViewPrivate::initBackgroundFunctions()
{
    if(backgroundFunctions.size() == 0){
        backgroundFunctions.resize(DuiContentItem::TwoIconsTextLabelVertical + 1);
        backgroundFunctions[DuiContentItem::Default] = &DuiContentItemStyle::backgroundImage;
        backgroundFunctions[DuiContentItem::TopLeft] = &DuiContentItemStyle::backgroundImageTopLeft;
        backgroundFunctions[DuiContentItem::Top] = &DuiContentItemStyle::backgroundImageTop;
        backgroundFunctions[DuiContentItem::TopRight] = &DuiContentItemStyle::backgroundImageTopRight;
        backgroundFunctions[DuiContentItem::Left] = &DuiContentItemStyle::backgroundImageLeft;
        backgroundFunctions[DuiContentItem::Center] = &DuiContentItemStyle::backgroundImageCenter;
        backgroundFunctions[DuiContentItem::Right] = &DuiContentItemStyle::backgroundImageRight;
        backgroundFunctions[DuiContentItem::BottomLeft] = &DuiContentItemStyle::backgroundImageBottomLeft;
        backgroundFunctions[DuiContentItem::Bottom] = &DuiContentItemStyle::backgroundImageBottom;
        backgroundFunctions[DuiContentItem::BottomRight] = &DuiContentItemStyle::backgroundImageBottomRight;
        backgroundFunctions[DuiContentItem::Single] = &DuiContentItemStyle::backgroundImageSingle;
        backgroundFunctions[DuiContentItem::SingleRowLeft] = &DuiContentItemStyle::backgroundImageSinglerowLeft;
        backgroundFunctions[DuiContentItem::SingleRowCenter] = &DuiContentItemStyle::backgroundImageSinglerowCenter;
        backgroundFunctions[DuiContentItem::SingleRowRight] = &DuiContentItemStyle::backgroundImageSinglerowRight;
        backgroundFunctions[DuiContentItem::SingleColumnTop] = &DuiContentItemStyle::backgroundImageSinglecolumnTop;
        backgroundFunctions[DuiContentItem::SingleColumnCenter] = &DuiContentItemStyle::backgroundImageSinglecolumnCenter;
        backgroundFunctions[DuiContentItem::SingleColumnBottom] = &DuiContentItemStyle::backgroundImageSinglecolumnBottom;
        backgroundFunctions[DuiContentItem::TwoIconsTextLabelVertical] = &DuiContentItemStyle::backgroundImageSinglecolumnBottom;

    }
}

DuiLabel *DuiContentItemViewPrivate::title()
{
    if (!titleLabel) {
        titleLabel = new DuiLabel(controller);
        titleLabel->setTextElide(true);
        Q_Q(DuiContentItemView);
        titleLabel->setObjectName(q->style()->titleObjectName());
    }

    return titleLabel;
}

DuiLabel *DuiContentItemViewPrivate::subtitle()
{
    if (!subtitleLabel) {
        subtitleLabel = new DuiLabel(controller);
        subtitleLabel->setTextElide(true);
        Q_Q(DuiContentItemView);
        subtitleLabel->setObjectName(q->style()->subtitleObjectName());
    }

    return subtitleLabel;
}

DuiImageWidget *DuiContentItemViewPrivate::image()
{
    if (!imageWidget) {
        imageWidget = new DuiImageWidget(controller);
        Q_Q(DuiContentItemView);
        imageWidget->setObjectName(q->style()->imageObjectName());
    }

    return imageWidget;
}

DuiImageWidget * DuiContentItemViewPrivate::optionalImage()
{
    if(!optionalImageWidget){
        optionalImageWidget = new DuiImageWidget(controller);
        Q_Q(DuiContentItemView);
        optionalImageWidget->setObjectName(q->style()->optionalImageObjectName());
    }

    return optionalImageWidget;
}


void DuiContentItemViewPrivate::initLayout(DuiContentItem::ContentItemStyle style)
{
    if (configuredStyle == style)
        return;

    clearLayout();

    configuredStyle = style;

    switch (style) {
    case DuiContentItem::IconAndTwoTextLabels:
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

    case DuiContentItem::SingleTextLabel:
        layout->addItem(title(), 0, 0, Qt::AlignCenter);

        if (subtitleLabel)
            subtitleLabel->setVisible(false);
        if (imageWidget)
            imageWidget->setVisible(false);
        break;

    case DuiContentItem::IconAndSingleTextLabel:
        layout->addItem(image(), 0, 0);
        layout->addItem(title(), 0, 1, Qt::AlignCenter);
        image()->setVisible(true);
        title()->setVisible(true);

        if (subtitleLabel)
            subtitleLabel->setVisible(false);
        break;

    case DuiContentItem::TwoTextLabels:
        layout->addItem(new QGraphicsWidget(controller), 0, 0);
        layout->addItem(title(), 1, 0, Qt::AlignBottom | Qt::AlignCenter);
        layout->addItem(subtitle(), 2, 0, Qt::AlignTop | Qt::AlignCenter);
        layout->addItem(new QGraphicsWidget(controller), 3, 0);

        if (imageWidget)
            imageWidget->setVisible(false);
        break;

    case DuiContentItem::SingleIcon:
        layout->addItem(image(), 0, 0, Qt::AlignCenter);
        image()->setVisible(true);

        if (titleLabel)
            title()->setVisible(false);
        if (subtitleLabel)
            subtitle()->setVisible(false);

        break;

    case DuiContentItem::IconAndSingleTextLabelVertical:
        layout->addItem(image(), 0, 0, Qt::AlignCenter);
        layout->addItem(title(), 1, 0, Qt::AlignCenter);

        title()->setAlignment(Qt::AlignCenter);

        image()->setVisible(true);
        title()->setVisible(true);

        if (subtitleLabel)
            subtitleLabel->setVisible(false);
        break;

    case DuiContentItem::IconAndTwoTextLabelsVertical:
        layout->addItem(image(), 0, 0, Qt::AlignCenter);

        layout->addItem(title(), 1, 0, Qt::AlignCenter);
        layout->addItem(subtitle(), 2, 0, Qt::AlignCenter);


        title()->setAlignment(Qt::AlignCenter);
        subtitle()->setAlignment(Qt::AlignCenter);

        image()->setVisible(true);
        title()->setVisible(true);
        subtitle()->setVisible(true);

        break;

    case DuiContentItem::TwoIconsTwoWidgets:

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
            subtitle()->setVisible( false );
        } else
            layout->addItem( subtitle(), 1,1, 1,columnSpan, Qt::AlignLeft );

        layout->addItem( optionalImage(), 0,2, Qt::AlignRight );
        optionalImage()->setVisible(true);

        if( controller->smallItem() )
            layout->addItem( controller->smallItem(), 1,2, Qt::AlignRight );

        break;
    };
}

void DuiContentItemViewPrivate::clearLayout()
{
    for (int i = 0; i < layout->count(); i++)
        layout->removeAt(0);
}

void DuiContentItemViewPrivate::setTitle(const QString &string)
{
    title()->setText(string);
}

void DuiContentItemViewPrivate::setSubtitle(const QString &string)
{
    subtitle()->setText(string);
}

void DuiContentItemViewPrivate::setPixmap(const QPixmap &pixmap)
{
    image()->setPixmap(pixmap);
}

void DuiContentItemViewPrivate::setOptionalPixmap(const QPixmap& pixmap)
{
    optionalImage()->setPixmap(pixmap);
}

void DuiContentItemViewPrivate::setImage(const QImage &i)
{
    image()->setImage(i);
}

void DuiContentItemViewPrivate::setOptionalImage(const QImage& i)
{
    optionalImage()->setImage(i);
}

void DuiContentItemViewPrivate::applyStyle()
{
    Q_Q(DuiContentItemView);

    if (titleLabel)
        titleLabel->setObjectName(q->style()->titleObjectName());

    if (subtitleLabel)
        subtitleLabel->setObjectName(q->style()->subtitleObjectName());

    if (imageWidget)
        imageWidget->setObjectName(q->style()->imageObjectName());
}

DuiContentItemView::DuiContentItemView(DuiContentItem *controller)
    : DuiWidgetView(* new DuiContentItemViewPrivate, controller)
{
    Q_D(DuiContentItemView);
    d->controller = controller;
    controller->setLayout(d->layout);
}

DuiContentItemView::DuiContentItemView(DuiContentItemViewPrivate &dd, DuiContentItem *controller)
    : DuiWidgetView(dd, controller)
{
    Q_D(DuiContentItemView);
    d->controller = controller;
    controller->setLayout(d->layout);
}

DuiContentItemView::~DuiContentItemView()
{

}

void DuiContentItemView::applyStyle()
{
    DuiWidgetView::applyStyle();

    Q_D(DuiContentItemView);
    d->applyStyle();
}

void DuiContentItemView::updateData(const QList<const char *> &modifications)
{
    Q_D(DuiContentItemView);

    DuiWidgetView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == DuiContentItemModel::Title) {
            d->setTitle(model()->title());
        } else if (member == DuiContentItemModel::Subtitle) {
            d->setSubtitle(model()->subtitle());
        } else if (member == DuiContentItemModel::ItemPixmap) {
            d->setPixmap(model()->itemPixmap());
        } else if (member == DuiContentItemModel::Selected) {
            setSelected(model()->selected());
        } else if(member == DuiContentItemModel::OptionalPixmap){
            d->setOptionalImage(d->controller->optionalPixmap());
        } else if(member == DuiContentItemModel::OptionalImage){
            d->setOptionalImage(model()->optionalImage());
        } else if(member == DuiContentItemModel::ItemImage) {
            d->setImage(model()->itemImage());
        }
    }
}

void DuiContentItemView::setupModel()
{
    Q_D(DuiContentItemView);

    DuiWidgetView::setupModel();

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
    if(!d->controller->image().isNull())
        d->setImage(d->controller->image());

    d->initLayout(static_cast<DuiContentItem::ContentItemStyle>(model()->itemStyle()));
}

void DuiContentItemView::cancelEvent(DuiCancelEvent *event)
{
    Q_D(DuiContentItemView);
    d->down = false;

    if (d->controller->isSelected()) {
        style().setModeSelected();
    } else {
        style().setModeDefault();
    }

    applyStyle();
    update();

    event->accept();
}

void DuiContentItemView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiContentItemView);

    /*
    if(d->controller->isSelectable())
    {
    */
    d->down = true;
    style().setModePressed();
    applyStyle();
    update();

    event->accept();
    /*
    } else {
        event->ignore();
    }
    event->ignore();
    */
}

void DuiContentItemView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiContentItemView);

    if (d->down) {
        d->down = false;
        event->accept();
        if(d->controller->isSelected())
            style().setModeSelected();
        else
            style().setModeDefault();
        applyStyle();
        update();

        d->controller->click();
    } else {
        event->ignore();
    }
}

void DuiContentItemView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());

    if (style()->backgroundImage()) {
        DuiContentItemViewPrivate::backgroundFunc func = DuiContentItemViewPrivate::backgroundFunctions[model()->itemMode()];
        const DuiContentItemStyle *itemStyle = style().operator ->();
        const DuiScalableImage *itemBackground = ((*itemStyle).*func)();

        if( itemBackground )
            itemBackground->draw(0, 0, size().width(), size().height(), painter);
    } else if (style()->backgroundColor().isValid()) {
        painter->fillRect(boundingRect(), QBrush(style()->backgroundColor()));
    }

    painter->setOpacity(oldOpacity);

}

void DuiContentItemView::setSelected(bool selected)
{
    if (selected)
        style().setModeSelected();
    else
        style().setModeDefault();

    applyStyle();
    updateGeometry();
}

DUI_REGISTER_VIEW_NEW(DuiContentItemView, DuiContentItem)
