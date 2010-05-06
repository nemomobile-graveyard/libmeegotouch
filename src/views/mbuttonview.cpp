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

#include "mbuttonview.h"
#include "mbuttonview_p.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetricsF>

#include "mbutton.h"
#include "mbutton_p.h" // For the member indexes of the model
#include "mfeedback.h"
#include "mtheme.h"
#include "mscalableimage.h"
#include "mviewcreator.h"
#include "mscenemanager.h"
#include "mlabel.h"
#include "mdebug.h"
#include "mtimestamp.h"

// Distance in pixels from the widget bounding box inside which a release is still accepted
#define RELEASE_MISS_DELTA 30

MButtonViewPrivate::MButtonViewPrivate()
    : icon(0), toggledIcon(0), label(NULL)
{
}

MButtonViewPrivate::~MButtonViewPrivate()
{
    MTheme::releasePixmap(icon);
    MTheme::releasePixmap(toggledIcon);
}

// As the condition of text color and background change for button
bool MButtonViewPrivate::toggleState() const
{
    Q_Q(const MButtonView);
    if (q->model()->checkable()) {
        bool state = (!q->model()->checked() && q->model()->down())
                     || (q->model()->checked() && !q->model()->down());
        return state;
    } else
        return q->model()->down();
}

void MButtonViewPrivate::refreshStyleMode()
{
    Q_Q(MButtonView);

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

void MButtonViewPrivate::calcIconTextRects()
{
    Q_Q(const MButtonView);

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

void MButtonViewPrivate::loadIcon(const QPixmap*& icon, const QString &newIconId, const QSize &newIconSize)
{
    MTheme::releasePixmap(icon);
    icon = 0;
    if (!newIconId.isEmpty())
        icon = MTheme::pixmap(newIconId, newIconSize);
}


MButtonView::MButtonView(MButton *controller) :
    MWidgetView(* new MButtonViewPrivate, controller)
{
    Q_D(MButtonView);
    d->label = new MLabel(controller);
    d->label->setParentItem(controller);
    d->label->setTextElide(true);
    d->label->setObjectName("ButtonLabel");
}

MButtonView::MButtonView(MButtonViewPrivate &dd, MButton *controller) :
    MWidgetView(dd, controller)
{
    Q_D(MButtonView);
    d->label = new MLabel(controller);
    d->label->setParentItem(controller);
    d->label->setTextElide(true);
    d->label->setObjectName("ButtonLabel");
}

MButtonView::~MButtonView()
{
    Q_D(MButtonView);
    if (d->label) {
        delete d->label;
        d->label = 0;
    }
}

void MButtonView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MButtonView);

    MWidgetView::resizeEvent(event);

    d->calcIconTextRects();
}

void MButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    Q_D(const MButtonView);
    mTimestamp("MButtonView", QString("start text=%1").arg(model()->text()));
    drawIcon(painter, d->iconRect);
    mTimestamp("MButtonView", QString("end text=%1").arg(model()->text()));
}

void MButtonView::drawIcon(QPainter *painter, const QRectF &iconRect) const
{
    if (model()->iconVisible()) {
        Q_D(const MButtonView);

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

/*MLabel* MButtonView::label()
{
    Q_D(const MButtonView);
    return d->label;
}*/

void MButtonView::applyStyle()
{
    Q_D(MButtonView);

    MWidgetView::applyStyle();

    d->refreshStyleMode();

    update();
}

void MButtonView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (model()->down()) {
        return;
    }
    model()->setDown(true);

    style()->pressFeedback().play();
}

void MButtonView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MButtonView);

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-RELEASE_MISS_DELTA, -RELEASE_MISS_DELTA,
                RELEASE_MISS_DELTA, RELEASE_MISS_DELTA);

    bool pressed = rect.contains(touch);

    if ( pressed != model()->down()) {
        model()->setDown(pressed);
    }
}

void MButtonView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MButtonView);

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

void MButtonView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);

    if (!model()->down()) {
        return;
    }
    model()->setDown(false);
}

void MButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(MButtonView);

    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MButtonModel::Text) {
            d->label->setText(model()->text());
            d->calcIconTextRects();
        } else if (member == MButtonModel::TextVisible) {
            d->label->setVisible(model()->textVisible());
            d->calcIconTextRects();
        } else if (member == MButtonModel::IconID) {
            d->loadIcon(d->icon, model()->iconID(), style()->iconSize());
            d->calcIconTextRects();
        } else if (member == MButtonModel::ToggledIconID) {
            d->loadIcon(d->toggledIcon, model()->toggledIconID(), style()->iconSize());
            d->calcIconTextRects();
        } else if (member == MButtonModel::IconVisible) {
            d->calcIconTextRects();
        } else if (member == MButtonModel::Down || member == MButtonModel::Checked ||
                   member == MButtonModel::Checkable) {
            d->refreshStyleMode();
        }
    }
    update();
}

void MButtonView::setupModel()
{
    Q_D(MButtonView);

    MWidgetView::setupModel();

    d->loadIcon(d->icon, model()->iconID(), style()->iconSize());
    d->loadIcon(d->toggledIcon, model()->toggledIconID(), style()->iconSize());
    d->calcIconTextRects();

    d->label->setText(model()->text());
    d->label->setVisible(model()->textVisible());

    update();
}

QSizeF MButtonView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MButtonView);

    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return MWidgetView::sizeHint(which, constraint);

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

M_REGISTER_VIEW_NEW(MButtonView, MButton)
