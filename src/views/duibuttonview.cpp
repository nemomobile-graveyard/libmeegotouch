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

#include "duibuttonview.h"
#include "duibuttonview_p.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetricsF>

#include "duibutton.h"
#include "duibutton_p.h" // For the member indexes of the model
#include "duifeedback.h"
#include "duitheme.h"
#include "duiscalableimage.h"
#include "duiviewcreator.h"
#include "duiscenemanager.h"
#include "duilabel.h"
#include "duidebug.h"
#include "duitimestamp.h"

// Distance in pixels from the widget bounding box inside which a release is still accepted
#define RELEASE_MISS_DELTA 30

DuiButtonViewPrivate::DuiButtonViewPrivate()
    : icon(0), toggledIcon(0), label(NULL)
{
}

DuiButtonViewPrivate::~DuiButtonViewPrivate()
{
    DuiTheme::releasePixmap(icon);
    DuiTheme::releasePixmap(toggledIcon);
}

// As the condition of text color and background change for button
bool DuiButtonViewPrivate::toggleState() const
{
    Q_Q(const DuiButtonView);
    if (q->model()->checkable()) {
        bool state = (!q->model()->checked() && q->model()->down())
                     || (q->model()->checked() && !q->model()->down());
        return state;
    } else
        return q->model()->down();
}

void DuiButtonViewPrivate::refreshStyleMode()
{
    Q_Q(DuiButtonView);

    if (q->model()->down())
        q->style().setModePressed();
    else if (q->model()->checked())
        q->style().setModeSelected();
    else
        q->style().setModeDefault();

    label->setAlignment(q->style()->horizontalTextAlign() | q->style()->verticalTextAlign());
    label->setFont(q->style()->font());
    label->setColor(q->style()->textColor());

    //update the icons only if the iconSize in the style has changed
    QSize size = q->style()->iconSize();
    if (icon && icon->size() != size) {
        loadIcon(icon, q->model()->iconID(), size);
    }
    if (toggledIcon && toggledIcon->size() != size) {
        loadIcon(toggledIcon, q->model()->toggledIconID(), size);
    }

    calcIconTextRects();
}

void DuiButtonViewPrivate::calcIconTextRects()
{
    Q_Q(const DuiButtonView);

    //total horizontal and vertical text margins
    int hTextMargin = q->style()->textMarginLeft() + q->style()->textMarginRight();
    int vTextMargin = q->style()->textMarginTop() + q->style()->textMarginBottom();

    //total horizontal and vertical padding
    int hPadding = q->style()->paddingLeft() + q->style()->paddingRight();
    int vPadding = q->style()->paddingTop() + q->style()->paddingBottom();

    //area for the content (icon and text)
    QRect contentRect(q->style()->paddingLeft(), q->style()->paddingTop(),
                      q->size().width() - hPadding,
                      q->size().height() - vPadding);

    //text rect when there is no icon
    QRect textRect = QRect(contentRect.left() + q->style()->textMarginLeft(),
                           contentRect.top() + q->style()->textMarginTop(),
                           contentRect.width() - hTextMargin,
                           contentRect.height() - vTextMargin);

    //icon visible and valid?
    if (q->model()->iconVisible() && (icon || toggledIcon)) {

        int iconWidth = q->style()->iconSize().width();
        int iconHeight = q->style()->iconSize().height();

        //text visible and valid?
        if (q->model()->textVisible() && !q->model()->text().isEmpty()) {

            switch (q->style()->iconAlign()) {
                //icon on left and text on right
            case Qt::AlignLeft: {
                iconRect = QRectF(contentRect.left(), contentRect.center().y() - (iconHeight / 2), iconWidth, iconHeight);
                textRect.setX(iconRect.right() + q->style()->textMarginLeft());
                textRect.setWidth(contentRect.width() - iconWidth - hTextMargin);
                break;
            }

            //icon on right and text on left
            case Qt::AlignRight: {
                iconRect = QRectF(contentRect.right() - iconWidth, contentRect.center().y() - (iconHeight / 2), iconWidth, iconHeight);
                textRect.setWidth(contentRect.width() - iconWidth - hTextMargin);
                break;
            }

            //icon on bottom and text on top
            case Qt::AlignBottom: {
                iconRect = QRectF(contentRect.center().x() - (iconWidth / 2), contentRect.bottom() - iconHeight, iconWidth, iconHeight);
                textRect.setHeight(contentRect.height() - iconHeight - vTextMargin);
                break;
            }

            //icon on top and text on bottom
            default: {
                iconRect = QRectF(contentRect.center().x() - (iconWidth / 2), contentRect.top(), iconWidth, iconHeight);
                textRect.setY(iconRect.bottom() + q->style()->textMarginTop());
                textRect.setHeight(contentRect.height() - iconHeight - vTextMargin);
                break;
            }
            }
        }
        // no text
        else {
            //icon on center
            iconRect = QRectF(contentRect.center().x() - (iconWidth / 2), contentRect.center().y() - (iconHeight / 2), iconWidth, iconHeight);
        }
    }

    //adjust label with button margins
    label->setGeometry(textRect.translated(q->marginLeft(), q->marginTop()));
}

void DuiButtonViewPrivate::loadIcon(const QPixmap*& icon, const QString &newIconId, const QSize &newIconSize)
{
    DuiTheme::releasePixmap(icon);
    icon = 0;
    if (!newIconId.isEmpty())
        icon = DuiTheme::pixmap(newIconId, newIconSize);
}


DuiButtonView::DuiButtonView(DuiButton *controller) :
    DuiWidgetView(* new DuiButtonViewPrivate, controller)
{
    Q_D(DuiButtonView);
    d->label = new DuiLabel(controller);
    d->label->setParentItem(controller);
    d->label->setTextElide(true);
    d->label->setObjectName("ButtonLabel");
}

DuiButtonView::DuiButtonView(DuiButtonViewPrivate &dd, DuiButton *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiButtonView);
    d->label = new DuiLabel(controller);
    d->label->setParentItem(controller);
    d->label->setTextElide(true);
    d->label->setObjectName("ButtonLabel");
}

DuiButtonView::~DuiButtonView()
{
    Q_D(DuiButtonView);
    if (d->label) {
        delete d->label;
        d->label = 0;
    }
}

void DuiButtonView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(DuiButtonView);

    DuiWidgetView::resizeEvent(event);

    d->calcIconTextRects();
}

void DuiButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    Q_D(const DuiButtonView);
    duiTimestamp("DuiButtonView", QString("start text=%1").arg(model()->text()));
    drawIcon(painter, d->iconRect);
    duiTimestamp("DuiButtonView", QString("end text=%1").arg(model()->text()));
}

void DuiButtonView::drawIcon(QPainter *painter, const QRectF &iconRect) const
{
    if (model()->iconVisible()) {
        Q_D(const DuiButtonView);

        bool toggleState = d->toggleState();

        const QPixmap *pixmap = NULL;
        if (toggleState && d->toggledIcon)
            pixmap = d->toggledIcon;
        else
            pixmap = d->icon;

        if (pixmap)
            painter->drawPixmap(iconRect, *pixmap, QRectF(pixmap->rect()));
    }
}

/*DuiLabel* DuiButtonView::label()
{
    Q_D(const DuiButtonView);
    return d->label;
}*/

void DuiButtonView::applyStyle()
{
    Q_D(DuiButtonView);

    DuiWidgetView::applyStyle();

    d->refreshStyleMode();

    update();
}

void DuiButtonView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (model()->down()) {
        return;
    }
    model()->setDown(true);

    style()->pressFeedback().play();
}

void DuiButtonView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiButtonView);

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-RELEASE_MISS_DELTA, -RELEASE_MISS_DELTA,
                RELEASE_MISS_DELTA, RELEASE_MISS_DELTA);

    bool pressed = rect.contains(touch);

    if ( pressed != model()->down()) {
        model()->setDown(pressed);
    }
}

void DuiButtonView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiButtonView);

    if (!model()->down()) {
        return;
    }
    model()->setDown(false);

    style()->releaseFeedback().play();

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-RELEASE_MISS_DELTA, -RELEASE_MISS_DELTA,
                RELEASE_MISS_DELTA, RELEASE_MISS_DELTA);
    if (rect.contains(touch))
        model()->click();
}

void DuiButtonView::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);

    if (!model()->down()) {
        return;
    }
    model()->setDown(false);
}

void DuiButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiButtonView);

    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiButtonModel::Text) {
            d->label->setText(model()->text());
            d->calcIconTextRects();
        } else if (member == DuiButtonModel::TextVisible) {
            d->label->setVisible(model()->textVisible());
            d->calcIconTextRects();
        } else if (member == DuiButtonModel::IconID) {
            d->loadIcon(d->icon, model()->iconID(), style()->iconSize());
            d->calcIconTextRects();
        } else if (member == DuiButtonModel::ToggledIconID) {
            d->loadIcon(d->toggledIcon, model()->toggledIconID(), style()->iconSize());
            d->calcIconTextRects();
        } else if (member == DuiButtonModel::IconVisible) {
            d->calcIconTextRects();
        } else if (member == DuiButtonModel::Down || member == DuiButtonModel::Checked ||
                   member == DuiButtonModel::Checkable) {
            d->refreshStyleMode();
        }
    }
    update();
}

void DuiButtonView::setupModel()
{
    Q_D(DuiButtonView);

    DuiWidgetView::setupModel();

    d->loadIcon(d->icon, model()->iconID(), style()->iconSize());
    d->loadIcon(d->toggledIcon, model()->toggledIconID(), style()->iconSize());
    d->calcIconTextRects();

    d->label->setText(model()->text());
    d->label->setVisible(model()->textVisible());

    update();
}

QSizeF DuiButtonView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiButtonView);

    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return DuiWidgetView::sizeHint(which, constraint);

    if (style()->preferredSize().isValid())
        return style()->preferredSize();


    QSizeF iconSize(0, 0);
    if (model()->iconVisible() && d->icon)
        iconSize = d->icon->size();

    QSizeF textSize(0, 0);
    if (model()->textVisible() && !model()->text().isEmpty()) {
        QFontMetricsF fm(style()->font());
        textSize = fm.size(0, model()->text());
        textSize += QSizeF(style()->textMarginLeft() + style()->textMarginRight(), style()->textMarginTop() + style()->textMarginBottom());
    }

    qreal width = 0, height = 0;
    if (style()->iconAlign() == Qt::AlignTop || style()->iconAlign() == Qt::AlignBottom) {
        width  = qMax(iconSize.width(), textSize.width());
        height = iconSize.height() + textSize.height();
    } else {
        width  = iconSize.width() + textSize.width();
        height = qMax(iconSize.height(), textSize.height());
    }

    return QSizeF(width + style()->paddingLeft() + style()->paddingRight(), height + style()->paddingTop() + style()->paddingBottom());
}

DUI_REGISTER_VIEW_NEW(DuiButtonView, DuiButton)
