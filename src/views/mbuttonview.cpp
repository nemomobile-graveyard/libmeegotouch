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
#include <QPixmap>
#include <QIcon>
#include <QTimer>

#include "mbutton.h"
#include "mfeedback.h"
#include "mtheme.h"
#include "mscalableimage.h"
#include "mviewcreator.h"
#include "mscenemanager.h"
#include "mlabel.h"
#include "mdebug.h"
#include "mtimestamp.h"
#include "mviewconstants.h"

MButtonViewPrivate::MButtonViewPrivate()
    : icon(0), toggledIcon(0), label(NULL), styleModeChangeTimer(NULL),
    iconOrigin(IconOriginUndefined), toggledIconOrigin(IconOriginUndefined),
    queuedStyleModeChange(false)
{
}

MButtonViewPrivate::Icon::Icon()
{
    pixmap = 0;
    origin = IconOriginUndefined;
}

MButtonViewPrivate::Icon::~Icon()
{
    switch(origin) {
        case IconOriginFromModelIconId:
        case IconOriginFromStyleIconId:
            MTheme::releasePixmap(pixmap);
            break;

        case IconOriginFromModelQIcon:
            delete pixmap;
            break;

        case IconOriginUndefined:
        default:
            break;
    }

    pixmap = 0;
    origin = IconOriginUndefined;
    id.clear();
    theme.clear();
}

int MButtonViewPrivate::pressTimeout() const
{
    Q_Q(const MButtonView);

    return q->style()->pressTimeout();
}

void MButtonViewPrivate::freeIcons()
{
    if (icon) {
        delete icon;
        icon = 0;
    }

    if (toggledIcon) {
        delete toggledIcon;
        toggledIcon = 0;
    }
}

MButtonViewPrivate::~MButtonViewPrivate()
{
    freeIcons();
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

void MButtonViewPrivate::_q_applyQueuedStyleModeChange()
{
    Q_Q(MButtonView);

    if (queuedStyleModeChange) {
        queuedStyleModeChange = false;
        q->applyStyle();
    }
}

void MButtonViewPrivate::_q_finishBlinkEffect()
{
    Q_Q(MButtonView);

    q->style().setModePressed();
    updateItemsAfterModeChange();
    q->update();

    styleModeChangeTimer->start(pressTimeout());
}

void MButtonViewPrivate::refreshStyleMode()
{
    Q_Q(MButtonView);

    if (controller->isEnabled()) {
        if (q->model()->down()) {
            if (blinkTimer->isActive()) {
                // very unlikely to happen, but check it anyway
                return;
            }
            if (styleModeChangeTimer->isActive()) {
                styleModeChangeTimer->stop();
                q->style().setModeDefault();
                updateItemsAfterModeChange();
                q->update();

                blinkTimer->start(M_PRESS_BLINK_TIMEOUT);
                return;
            }
            styleModeChangeTimer->start(pressTimeout());
            q->style().setModePressed();
        } else if (q->model()->checked()) {
            q->style().setModeSelected();
        } else {
            if (styleModeChangeTimer->isActive()) {
                queuedStyleModeChange = true;
                return;
            }
            q->style().setModeDefault();
        }
    } else {
        if (styleModeChangeTimer->isActive()) {
            queuedStyleModeChange = true;
            return;
        }
        q->style().setModeDisabled();
    }

    updateItemsAfterModeChange();
}

void MButtonViewPrivate::updateItemsAfterModeChange()
{
    Q_Q(MButtonView);

    label->setAlignment(q->style()->horizontalTextAlign() | q->style()->verticalTextAlign());
    label->setFont(q->style()->font());
    label->setColor(q->style()->textColor());

    updateIcon();
    updateToggledIcon();

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
    QRectF contentRect(q->style()->paddingLeft(), q->style()->paddingTop(),
                      q->size().width() - hPadding,
                      q->size().height() - vPadding);

    //text rect when there is no icon
    QRectF textRect(contentRect.left() + q->style()->textMarginLeft(),
                    contentRect.top() + q->style()->textMarginTop(),
                    contentRect.width() - hTextMargin,
                    contentRect.height() - vTextMargin);

    QSizeF textSize = label->sizeHint(Qt::PreferredSize);

    //icon visible and valid?
    if (q->model()->iconVisible() && (icon || toggledIcon)) {
        int iconWidth = q->style()->iconSize().width();
        int iconHeight = q->style()->iconSize().height();

        QSizeF iconSize(iconWidth, iconHeight);
        QPointF iconPosition(0, 0);

        //text visible and valid?
        if (q->model()->textVisible() && !q->model()->text().isEmpty()) {
            textSize.setWidth(qMin(textSize.width(), contentRect.width() - iconWidth - hTextMargin));
            switch (q->style()->iconAlign()) {
                //icon on left and text on right
            case Qt::AlignLeft: {
                if (q->style()->horizontalTextAlign() == Qt::AlignHCenter)
                    iconPosition.setX(contentRect.center().x() - iconWidth - (textSize.width() / 2));
                else if (q->style()->horizontalTextAlign() == Qt::AlignRight)
                    iconPosition.setX(contentRect.right() - textSize.width() - iconWidth - hTextMargin);
                else if (q->style()->horizontalTextAlign() == Qt::AlignLeft)
                    iconPosition.setX(contentRect.left());

                iconPosition.setY(contentRect.center().y() - (iconHeight / 2));
                textRect.setX(contentRect.left() + q->style()->textMarginLeft() + iconWidth);
                textRect.setWidth(contentRect.width() - iconWidth - hTextMargin);
                break;
            }

            //icon on right and text on left
            case Qt::AlignRight: {
                if (q->style()->horizontalTextAlign() == Qt::AlignHCenter)
                    iconPosition.setX(contentRect.center().x() + (textSize.width() / 2));
                else if (q->style()->horizontalTextAlign() == Qt::AlignRight)
                    iconPosition.setX(contentRect.right() - iconWidth);
                else if (q->style()->horizontalTextAlign() == Qt::AlignLeft)
                    iconPosition.setX(contentRect.left() + textSize.width() + hTextMargin);

                iconPosition.setY(contentRect.center().y() - (iconHeight / 2));
                textRect.setWidth(contentRect.width() - iconWidth - hTextMargin);
                break;
            }

            //icon on bottom and text on top
            case Qt::AlignBottom: {
                iconPosition = QPointF(contentRect.center().x() - (iconWidth / 2), contentRect.bottom() - iconHeight);
                textRect.setHeight(contentRect.height() - iconHeight - vTextMargin);
                break;
            }

            //icon on top and text on bottom
            default: {
                iconPosition = QPointF(contentRect.center().x() - (iconWidth / 2), contentRect.top());
                textRect.setY(iconRect.bottom() + q->style()->textMarginTop());
                textRect.setHeight(contentRect.height() - iconHeight - vTextMargin);
                break;
            }
            }
        }
        // no text
        else {
            //icon on center
            iconPosition = QPointF(contentRect.center().x() - (iconWidth / 2), contentRect.center().y() - (iconHeight / 2));
        }

        iconRect = QRectF(iconPosition, iconSize);
    }

    //adjust label with button margins
    if (controller->layoutDirection() == Qt::LeftToRight)
        label->setGeometry(textRect.translated(q->marginLeft(), q->marginTop()));
    else
        label->setGeometry(textRect.translated(q->marginRight(), q->marginTop()));
}

bool MButtonViewPrivate::isCurrentIconObsolete(const QString &newIconId) const
{
    Q_Q(const MButtonView);
    bool result = false;

    if (icon) {
        result = icon->id != newIconId // it's a different icon
                 || icon->theme != MTheme::currentTheme() // current icon is from a previous theme.
                                                          // In the new theme the id might point
                                                          // to a different graphical asset.
                 || icon->pixmap->size() != q->style()->iconSize(); // icon has changed its size
    } else {
        // We don't even have an icon to compare with.
        result = true;
    }

    return result;
}

void MButtonViewPrivate::updateIcon(Icon **iconPtr,
        QIcon::Mode mode, const QString &iconIdFromModel,
        const QString &iconIdFromStyle)
{
    Q_Q(MButtonView);
    Icon *icon = *iconPtr;

    /*
     Fallback order for chosing where to take the icon from:
     1 - MButtonModel::icon(), a QIcon
     2 - MButtonModel::[toggled]iconID(), a QString
     3 - MButtonStyle::[toggled]iconId(),  a QString
     4 - No icon at all
     */

    /*
        We only reload an icon if it has really changed.
     */

    if (!q->model()->icon().isNull()) {

        if (icon)
            delete icon;

        icon = new Icon;
        *iconPtr = icon;

        icon->pixmap = new QPixmap(q->model()->icon().pixmap(q->style()->iconSize(), mode));
        icon->origin = IconOriginFromModelQIcon;

    } else if (!iconIdFromModel.isEmpty() && isCurrentIconObsolete(iconIdFromModel)) {

        if (icon)
            delete icon;

        icon = new Icon;
        *iconPtr = icon;

        icon->pixmap = MTheme::pixmap(iconIdFromModel, q->style()->iconSize());
        icon->origin = IconOriginFromModelIconId;
        icon->id = iconIdFromModel;
        icon->theme = MTheme::currentTheme();

    } else if (!iconIdFromStyle.isEmpty() && isCurrentIconObsolete(iconIdFromStyle)) {

        if (icon)
            delete icon;

        icon = new Icon;
        *iconPtr = icon;

        icon->pixmap = MTheme::pixmap(iconIdFromStyle, q->style()->iconSize());
        icon->origin = IconOriginFromStyleIconId;
        icon->id = iconIdFromStyle;
        icon->theme = MTheme::currentTheme();

    } else if (iconIdFromModel.isEmpty() && iconIdFromStyle.isEmpty()) {
        delete icon;
        icon = NULL;
        *iconPtr = icon;
    }
}

void MButtonViewPrivate::updateIcon()
{
    Q_Q(MButtonView);
    updateIcon(&icon, QIcon::Normal, q->model()->iconID(), q->style()->iconId());
}

void MButtonViewPrivate::updateToggledIcon()
{
    Q_Q(MButtonView);
    updateIcon(&toggledIcon, QIcon::Selected, q->model()->toggledIconID(), q->style()->toggledIconId());
}

MButtonView::MButtonView(MButton *controller) :
    MWidgetView(* new MButtonViewPrivate, controller)
{
    Q_D(MButtonView);
    d->label = new MLabel(controller);
    d->label->setParentItem(controller);
    d->label->setTextElide(true);
    d->label->setObjectName("ButtonLabel");
    d->label->setMinimumSize(0,0);

    d->styleModeChangeTimer = new QTimer(this);
    d->styleModeChangeTimer->setSingleShot(true);
    connect(d->styleModeChangeTimer, SIGNAL(timeout()), SLOT(_q_applyQueuedStyleModeChange()));

    d->blinkTimer = new QTimer(this);
    d->blinkTimer->setSingleShot(true);
    connect(d->blinkTimer, SIGNAL(timeout()), SLOT(_q_finishBlinkEffect()));
}

MButtonView::MButtonView(MButtonViewPrivate &dd, MButton *controller) :
    MWidgetView(dd, controller)
{
    Q_D(MButtonView);
    d->label = new MLabel(controller);
    d->label->setParentItem(controller);
    d->label->setTextElide(true);
    d->label->setObjectName("ButtonLabel");
    d->label->setMinimumSize(0,0);

    d->styleModeChangeTimer = new QTimer(this);
    d->styleModeChangeTimer->setSingleShot(true);
    connect(d->styleModeChangeTimer, SIGNAL(timeout()), SLOT(_q_applyQueuedStyleModeChange()));

    d->blinkTimer = new QTimer(this);
    d->blinkTimer->setSingleShot(true);
    connect(d->blinkTimer, SIGNAL(timeout()), SLOT(_q_finishBlinkEffect()));
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
    drawIcon(painter, d->iconRect);
}

void MButtonView::drawIcon(QPainter *painter, const QRectF &iconRect) const
{
    if (model()->iconVisible()) {
        Q_D(const MButtonView);

        bool toggleState = d->toggleState();

        const QPixmap *pixmap = NULL;
        if (toggleState && d->toggledIcon)
            pixmap = d->toggledIcon->pixmap;
        else if (d->icon)
            pixmap = d->icon->pixmap;

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
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);

    if (rect.contains(touch)) {
        if (!model()->down()) {
            model()->setDown(true);
            style()->pressFeedback().play();
        }
    } else {
        if (model()->down()) {
            model()->setDown(false);
            style()->cancelFeedback().play();
        }
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
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    if (rect.contains(touch))
        model()->click();
}

void MButtonView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);

    if (!model()->down()) {
        return;
    }

    style()->cancelFeedback().play();

    model()->setDown(false);
}

void MButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(MButtonView);

    MWidgetView::updateData(modifications);
    const char *member;

    bool mustCalcIconTextRects = false;
    bool mustUpdateIcon = false;
    bool mustUpdateToggledIcon = false;

    foreach(member, modifications) {
        if (member == MButtonModel::Text) {
            d->label->setText(model()->text());
            mustCalcIconTextRects = true;
        } else if (member == MButtonModel::TextVisible) {
            d->label->setVisible(model()->textVisible());
            mustCalcIconTextRects = true;
        } else if (member == MButtonModel::IconID) {
            mustUpdateIcon = true;
            mustCalcIconTextRects = true;
        } else if (member == MButtonModel::ToggledIconID) {
            mustUpdateToggledIcon = true;
            mustCalcIconTextRects = true;
        } else if (member == MButtonModel::Icon) {
            mustUpdateIcon = true;
            mustUpdateToggledIcon = true;
            mustCalcIconTextRects = true;
        } else if (member == MButtonModel::IconVisible) {
            mustCalcIconTextRects = true;
        } else if (member == MButtonModel::Down || member == MButtonModel::Checked ||
                   member == MButtonModel::Checkable) {
            d->refreshStyleMode();
        }
    }

    if (mustUpdateIcon) {
        d->updateIcon();
    }

    if (mustUpdateToggledIcon) {
        d->updateToggledIcon();
    }

    if (mustCalcIconTextRects) {
        d->calcIconTextRects();
    }

    update();
}

void MButtonView::setupModel()
{
    Q_D(MButtonView);

    MWidgetView::setupModel();
    QList<const char *> members;
    if (model()->icon().isNull())
        members << MButtonModel::IconID;
    else
        members << MButtonModel::Icon;
    if (!model()->toggledIconID().isEmpty())
        members << MButtonModel::ToggledIconID;

    updateData(members);

    d->label->setText(model()->text());
    d->label->setVisible(model()->textVisible());

    d->calcIconTextRects();

    update();
}

QSizeF MButtonView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MButtonView);

    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return MWidgetView::sizeHint(which, constraint);

    QSizeF iconSize(0, 0);
    if (model()->iconVisible() && d->icon)
        iconSize = d->icon->pixmap->size();

    QSizeF textSize(0, 0);
    if (model()->textVisible() && !model()->text().isEmpty()) {
        textSize = d->label->sizeHint(which, constraint);
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

#include "moc_mbuttonview.cpp"
