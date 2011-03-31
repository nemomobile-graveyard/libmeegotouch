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
#include "mbuttontransition.h"
#include "mbuttondefaulttransition.h"
#include "mbuttonexpandingbackgroundtransition.h"
#include "mbuttonimplodingbackgroundtransition.h"


MButtonViewPrivate::MButtonViewPrivate()
    : icon(0), toggledIcon(0), label(NULL),
    iconOrigin(IconOriginUndefined), toggledIconOrigin(IconOriginUndefined),
      transition(NULL), eventCancelled(false), expectMouseReleaseEvent(false)
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

void MButtonViewPrivate::updateItemsAfterModeChange()
{
    updateLabelStyle();
    updateIcon();
    updateToggledIcon();

    calcIconTextRects();
}

void MButtonViewPrivate::calcIconTextRects()
{
    Q_Q(const MButtonView);

    const MButtonStyle *s = static_cast<const MButtonStyle *>(q->style().operator ->());

    //total horizontal and vertical text margins
    int hTextMargin = s->textMarginLeft() + s->textMarginRight();
    int vTextMargin = s->textMarginTop() + s->textMarginBottom();

    //total horizontal and vertical padding
    int hPadding = s->paddingLeft() + s->paddingRight();
    int vPadding = s->paddingTop() + s->paddingBottom();

    //area for the content (icon and text)
    QRectF contentRect(s->paddingLeft(), s->paddingTop(),
                      q->size().width() - hPadding,
                      q->size().height() - vPadding);

    //text rect when there is no icon
    QRectF textRect(contentRect.left() + s->textMarginLeft(),
                    contentRect.top() + s->textMarginTop(),
                    contentRect.width() - hTextMargin,
                    contentRect.height() - vTextMargin);

    QSizeF textSize(0,0);
    if (label)
        textSize = label->sizeHint(Qt::PreferredSize);

    //icon visible and valid?
    if (q->model()->iconVisible() && (icon || toggledIcon)) {
        int iconWidth = s->iconSize().width();
        int iconHeight = s->iconSize().height();

        QSizeF iconSize(iconWidth, iconHeight);
        QPointF iconPosition(0, 0);

        //text visible and valid?
        if (q->model()->textVisible() && !q->model()->text().isEmpty()) {
            textSize.setWidth(qMin(textSize.width(), contentRect.width() - iconWidth - hTextMargin));
            switch (q->style()->iconAlign()) {
                //icon on left and text on right
            case Qt::AlignLeft: {
                if (s->horizontalTextAlign() == Qt::AlignHCenter)
                    iconPosition.setX(contentRect.center().x() - iconWidth - (textSize.width() / 2));
                else if (s->horizontalTextAlign() == Qt::AlignRight)
                    iconPosition.setX(contentRect.right() - textSize.width() - iconWidth - hTextMargin);
                else if (s->horizontalTextAlign() == Qt::AlignLeft)
                    iconPosition.setX(contentRect.left());

                iconPosition.setY(contentRect.center().y() - (iconHeight / 2));
                textRect.setX(contentRect.left() + s->textMarginLeft() + iconWidth);
                textRect.setWidth(contentRect.width() - iconWidth - hTextMargin);
                break;
            }

            //icon on right and text on left
            case Qt::AlignRight: {
                if (s->horizontalTextAlign() == Qt::AlignHCenter)
                    iconPosition.setX(contentRect.center().x() + (textSize.width() / 2));
                else if (s->horizontalTextAlign() == Qt::AlignRight)
                    iconPosition.setX(contentRect.right() - iconWidth);
                else if (s->horizontalTextAlign() == Qt::AlignLeft)
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
    if (label) {
        if (controller->layoutDirection() == Qt::LeftToRight)
            label->setGeometry(textRect.translated(q->marginLeft(), q->marginTop()));
        else
            label->setGeometry(textRect.translated(q->marginRight(), q->marginTop()));
    }
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

void MButtonViewPrivate::setText(const QString &text)
{
    Q_Q(MButtonView);

    if (q->model()->textVisible() && !text.isEmpty()) {
        if (!label) {
            label = new MLabel(controller);
            label->setTextElide(true);
            label->setObjectName("ButtonLabel");
            label->setMinimumSize(0,0);
            updateLabelStyle();
        }
        if (!label->isVisible())
            label->setVisible(true);
        label->setText(text);
    } else {
        if (label)
            label->setVisible(false);
    }
}

void MButtonViewPrivate::updateLabelStyle()
{
    Q_Q(MButtonView);

    if (label) {
        const MButtonStyle *s = static_cast<const MButtonStyle *>(q->style().operator ->());
        label->setAlignment(s->horizontalTextAlign() | s->verticalTextAlign());
        label->setFont(s->font());
        label->setColor(s->textColor());
        label->setOpacity(s->contentOpacity());
    }
}

MButtonView::MButtonView(MButton *controller) :
    MWidgetView(* new MButtonViewPrivate, controller)
{
}

MButtonView::MButtonView(MButtonViewPrivate &dd, MButton *controller) :
    MWidgetView(dd, controller)
{
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

void MButtonView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MButtonView);
    painter->save();
    d->transition->modifyBackgroundPainter(painter);
    MWidgetView::drawBackground(painter,option);
    painter->restore();
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
        if( pixmap && style()->contentOpacity() >= 0.0 ) {
            qreal formerOpacity = painter->opacity();
            painter->setOpacity(d->controller->effectiveOpacity() * style()->contentOpacity());
            painter->drawPixmap(iconRect, *pixmap, QRectF(pixmap->rect()));
            painter->setOpacity(formerOpacity);
        }
    }
}

void MButtonView::applyStyle()
{
    Q_D(MButtonView);

    MWidgetView::applyStyle();

    if (style()->transition() == "default") {
        if (!qobject_cast<MButtonDefaultTransition*>(d->transition)) {
            delete d->transition;
            d->transition = new MButtonDefaultTransition(style(), model(), d->controller,d);
            d->transition->setParent(this);
        }
    } else if (style()->transition() == "expanding-background") {
        if (!qobject_cast<MButtonExpandingBackgroundTransition*>(d->transition)) {
            delete d->transition;
            d->transition = new MButtonExpandingBackgroundTransition(style(), model(), d->controller,d);
            d->transition->setParent(this);
        }
    } else if (style()->transition() == "imploding-background") {
        if (!qobject_cast<MButtonImplodingBackgroundTransition*>(d->transition)) {
            delete d->transition;
            d->transition = new MButtonImplodingBackgroundTransition(style(), model(), d->controller,d);
            d->transition->setParent(this);
        }
    } else {
        if (!qobject_cast<MButtonDefaultTransition*>(d->transition)) {
            delete d->transition;
            d->transition = new MButtonDefaultTransition(style(), model(), d->controller,d);
            d->transition->setParent(this);
        }
    }

    d->transition->refreshStyle();

    update();
}

void MButtonView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MButtonView);

    d->expectMouseReleaseEvent = true;

    if (model()->down()) {
        return;
    }
    model()->setDown(true);
}

void MButtonView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MButtonView);
    if (!d->expectMouseReleaseEvent) {
        // The usual mouse-press -> mouse-move -> mouse-release cycle
        // has been interrupted by a cancel event.
        return;
    }

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);

    if (rect.contains(touch)) {
        if (!model()->down()) {
            model()->setDown(true);
        }
    } else {
        if (model()->down()) {
            d->eventCancelled = true;
            model()->setDown(false);
        }
    }

}

void MButtonView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MButtonView);
    d->expectMouseReleaseEvent = false;

    if (!model()->down()) {
        return;
    }
    model()->setDown(false);

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
    Q_D(MButtonView);

    d->expectMouseReleaseEvent = false;

    if (!model()->down()) {
        return;
    }

    d->eventCancelled = true;
    model()->setDown(false);
}

void MButtonView::setGeometry(const QRectF &rect)
{
    MWidgetView::setGeometry(rect);
}

void MButtonView::updateData(const QList<const char *>& modifications)
{
    Q_D(MButtonView);

    MWidgetView::updateData(modifications);
    const char *member;

    bool mustUpdateGeometry = false;
    bool mustUpdateIcon = false;
    bool mustUpdateToggledIcon = false;
    bool mustRelocateIconText = false;
    bool mustUpdateButtonText = false;

    foreach(member, modifications) {
        if (member == MButtonModel::Text || member == MButtonModel::TextVisible) {
            mustUpdateButtonText = true;
            mustUpdateGeometry = true;
            mustRelocateIconText = true;
        } else if (member == MButtonModel::IconID) {
            mustUpdateIcon = true;
            mustUpdateGeometry = true;
            mustRelocateIconText = true;
        } else if (member == MButtonModel::ToggledIconID) {
            mustUpdateToggledIcon = true;
            mustUpdateGeometry = true;
            mustRelocateIconText = true;
        } else if (member == MButtonModel::Icon) {
            mustUpdateIcon = true;
            mustUpdateToggledIcon = true;
            mustUpdateGeometry = true;
            mustRelocateIconText = true;
        } else if (member == MButtonModel::IconVisible) {
            mustUpdateGeometry = true;
            mustRelocateIconText = true;
        } else if (member == MButtonModel::Checked ||
                   member == MButtonModel::Checkable) {
            d->transition->refreshStyle();
        } else if (member == MButtonModel::Down) {
            if (model()->down()) {
                d->transition->onPress();
                style()->pressFeedback().play();
            } else {
                if (!d->eventCancelled) {
                    d->transition->onRelease();
                    style()->releaseFeedback().play();
                } else {
                    d->transition->onCancel();
                    style()->cancelFeedback().play();
                    d->eventCancelled = false;
                }
            }
        }
    }

    if (mustUpdateButtonText) {
        d->setText(model()->text());
    }

    if (mustUpdateIcon) {
        d->updateIcon();
    }

    if (mustUpdateToggledIcon) {
        d->updateToggledIcon();
    }

    if (mustRelocateIconText) {
        d->calcIconTextRects();
    }

    if (mustUpdateGeometry) {
        updateGeometry();
    }

    update();
}

void MButtonView::setupModel()
{
    MWidgetView::setupModel();
    QList<const char *> members;
    if (model()->icon().isNull())
        members << MButtonModel::IconID;
    else
        members << MButtonModel::Icon;
    if (!model()->toggledIconID().isEmpty())
        members << MButtonModel::ToggledIconID;
    members << MButtonModel::TextVisible;
    members << MButtonModel::Text;

    updateData(members);
}

QSizeF MButtonView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MButtonView);

    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return MWidgetView::sizeHint(which, constraint);

    const MButtonStyle *s = static_cast<const MButtonStyle *>(style().operator ->());

    QSizeF iconSize(0, 0);
    if (model()->iconVisible() && d->icon)
        iconSize = d->icon->pixmap->size();

    QSizeF textSize(0, 0);
    if (model()->textVisible() && !model()->text().isEmpty() && d->label) {
        textSize = d->label->sizeHint(which, constraint);
        textSize += QSizeF(s->textMarginLeft() + s->textMarginRight(), s->textMarginTop() + s->textMarginBottom());
    }

    qreal width = 0, height = 0;
    if (s->iconAlign() == Qt::AlignTop || style()->iconAlign() == Qt::AlignBottom) {
        width  = qMax(iconSize.width(), textSize.width());
        height = iconSize.height() + textSize.height();
    } else {
        width  = iconSize.width() + textSize.width();
        height = qMax(iconSize.height(), textSize.height());
    }

    return QSizeF(width + s->paddingLeft() + s->paddingRight(), height + s->paddingTop() + s->paddingBottom());
}


void MButtonView::notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    MWidgetView::notifyItemChange(change, value);

    if (change == QGraphicsItem::ItemEnabledHasChanged) {
        Q_D(MButtonView);
        if (!d->controller->isEnabled() && model()->checked()) {
            style().setModeDisabledSelected();
            applyStyle();
        }
    }
}

M_REGISTER_VIEW_NEW(MButtonView, MButton)

