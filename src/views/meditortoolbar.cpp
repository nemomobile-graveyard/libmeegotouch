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
#include "meditortoolbar.h"
#include "mtopleveloverlay.h"

#include <MButton>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MSceneManager>
#include <MWidget>
#include <QAction>

#include <QApplication>
#include <QGraphicsLinearLayout>

MEditorToolbar::MEditorToolbar(const MWidget &followWidget)
    : overlay(new MTopLevelOverlay(followWidget.sceneManager())),
      followWidget(followWidget),
      buttonLayoutPolicy(new MLinearLayoutPolicy(new MLayout(this),
                                                 Qt::Horizontal)),
      arrow(new MEditorToolbarArrow(this)),
      buttonUpdateQueued(false),
      temporarilyDisappeared(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, true);
    overlay->hide();
    hide();
    followWidget.scene()->addItem(overlay);
    setParentItem(overlay);

    // Set z value for arrow higher than default since
    // it needs to float on top of buttons.
    arrow->setZValue(1.0f);

    // The policy notifies the widgets of their relative position inside the layout,
    // this causes the buttons to be rendered with different backgrounds for each position
    buttonLayoutPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);

    // Don't add extra margins or spacings for buttons.
    buttonLayoutPolicy->setContentsMargins(0.0f, 0.0f, 0.0f, 0.0f);
    buttonLayoutPolicy->setSpacing(0.0f);

    QObject::connect(sceneManager(), SIGNAL(orientationChanged(M::Orientation)),
                     this, SLOT(updateGeometry()));

    updateArrow(MEditorToolbarArrow::ArrowDown);
}

MEditorToolbar::~MEditorToolbar()
{
    hideEditorItem();
    // Before destroying parent, detach so it doesn't try to destroy us.
    setParentItem(0);
    if (overlay) {
        overlay->deleteLater();
    }
}

void MEditorToolbar::setPosition(const QPointF &pos,
                                 MEditorToolbarArrow::ArrowDirection direction)
{
    setPos(followWidget.mapToItem(overlay, pos));
    updateArrow(direction);
}

void MEditorToolbar::appear()
{
    overlay->show();
    updateEditorItemVisibility();
    temporarilyDisappeared = false;
}

void MEditorToolbar::disappear()
{
    hideEditorItem();
    overlay->hide();
    temporarilyDisappeared = false;
}

void MEditorToolbar::disappearTemporarily()
{
    if (isAppeared()) {
        disappear();
        temporarilyDisappeared = true;
    }
}

void MEditorToolbar::removeTemporaryDisappearance()
{
    if (temporarilyDisappeared) {
        appear();
    }
}

bool MEditorToolbar::isAppeared() const
{
    return overlay->isVisible();
}

void MEditorToolbar::updateArrow(MEditorToolbarArrow::ArrowDirection direction)
{
    // Clear local transforms.
    setTransform(QTransform());

    // Style mode is different with regarding to top and bottom margins.
    if (direction == MEditorToolbarArrow::ArrowUp) {
        style().setModeEditorUnderCursor();
    } else {
        style().setModeDefault();
    }
    MStylableWidget::applyStyle();

    const QRectF contentsRectangle = contentsRect();

    // Update horizontally, make sure widget is inside screen.
    qreal center = contentsRectangle.center().x();
    QRectF mappedSceneRect = mapRectFromScene(
        QRectF(QPointF(), sceneManager()->visibleSceneSize(M::Landscape)));
    mappedSceneRect.translate(center, 0.0f);

    qreal offscreenLeft = qMax<qreal>(0.0f, mappedSceneRect.left());
    qreal offscreenRight = qMax<qreal>(0.0f, (effectiveSizeHint(Qt::PreferredSize).width()
                                              - mappedSceneRect.right()));

    // The widget won't be off the screen from both ends at the same time.
    // Width is restricted to screen width.

    qreal x = center - arrow->size().width() / 2.0f
              - offscreenLeft + offscreenRight;
    x = qBound<qreal>(contentsRectangle.left(),
                      x,
                      contentsRectangle.right() - arrow->size().width());

    // Update vertically. Arrow graphics are designed to be aligned to either
    // top or bottom of buttons, completely overlapping them.
    arrow->setDirection(direction);

    switch (arrow->direction()) {
    case MEditorToolbarArrow::ArrowUp:
        arrow->setPos(x, contentsRectangle.top());
        break;
    case MEditorToolbarArrow::ArrowDown:
        arrow->setPos(x, contentsRectangle.bottom() - arrow->size().height());
        break;
    }

    // Arrow has changed position, update widget origin.
    updateWidgetOrigin();
}

bool MEditorToolbar::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::ActionAdded:
    {
        QActionEvent *actionEvent(static_cast<QActionEvent *>(event));
        Q_ASSERT(actionEvent);
        Q_ASSERT(!actionEvent->before()); // we support appending only
        QAction *action(qobject_cast<QAction *>(actionEvent->action()));
        Q_ASSERT(action);

        buttons << new MButton(action->text());
        QObject::connect(buttons.back(), SIGNAL(clicked(bool)), action, SLOT(trigger()));
        buttons.back()->setStyleName(action->objectName());
        if (action->isVisible()) {
            visibilityUpdated();
        }
        break;
    }
    case QEvent::ActionRemoved:
    {
        QActionEvent *actionEvent(static_cast<QActionEvent *>(event));
        const int actionIndex = actions().indexOf(actionEvent->action());
        if (actionIndex >= 0) {
            // Actions list is in sync with buttons list so we can
            // use the same index to delete the corresponding button.
            Q_ASSERT(actionIndex < buttons.count());
            delete buttons.at(actionIndex);
            buttons.removeAt(actionIndex);
        }

        if (actionEvent->action()->isVisible()) {
            // Action was visible before removal, need to update widget.
            visibilityUpdated();
        }
        break;
    }
    case QEvent::ActionChanged:
    {
        QActionEvent *actionEvent(static_cast<QActionEvent *>(event));

        // Name of action might have been changed.
        const int actionIndex = actions().indexOf(actionEvent->action());
        Q_ASSERT(actionIndex >= 0 && actionIndex < buttons.count());
        MButton *button(buttons.at(actionIndex));
        if (button->text() != actionEvent->action()->text()) {
            button->setText(actionEvent->action()->text());
        }

        // Update visibility of buttons to match visibility of actions.
        visibilityUpdated();
        break;
    }
    default:
        return MStylableWidget::event(event);
        break;
    }

    event->accept();
    return true;
}

void MEditorToolbar::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    // Stop mouse event propagation.
}

void MEditorToolbar::updateAvailableButtons()
{
    buttonUpdateQueued = false;

    while (buttonLayoutPolicy->count() > 0) {
        buttonLayoutPolicy->removeAt(buttonLayoutPolicy->count() - 1);
    }

    QList<QAction *> actionList(actions());
    Q_ASSERT(actionList.count() == buttons.count());

    for (int i = 0; i < buttons.count(); ++i) {
        MButton *button = buttons.at(i);

        if (actionList.at(i)->isCheckable()) {
            button->setCheckable(true);
            button->setChecked(actionList.at(i)->isChecked());
        }

        if (actionList.at(i)->isVisible()) {
            buttonLayoutPolicy->addItem(button);
            button->show();
        } else {
            button->hide();
        }
    }

    // Resize manually since this widget is not managed by layout.
    resize(preferredSize());

    // Hide if there is no buttons.
    updateEditorItemVisibility();
}

void MEditorToolbar::visibilityUpdated()
{
    if (!buttonUpdateQueued) {
        buttonUpdateQueued = true;
        QMetaObject::invokeMethod(this, "updateAvailableButtons", Qt::QueuedConnection);
    }
}

void MEditorToolbar::updateGeometry()
{
    MStylableWidget::updateGeometry();
}

QSizeF MEditorToolbar::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QSizeF hint;
    switch (which) {
    case Qt::MinimumSize:
    case Qt::PreferredSize: {
            qreal width = 0;
            qreal height = 0;
            for (int i = 0; i < buttonLayoutPolicy->count(); ++i) {
                QSizeF buttonHint = buttonLayoutPolicy->itemAt(i)->effectiveSizeHint(Qt::PreferredSize);
                width += buttonHint.width();
                height = qMax<qreal>(height, buttonHint.height());
            }
            width += style()->marginLeft() + style()->marginRight();
            height += style()->marginTop() + style()->marginBottom();
            hint.setWidth(width);
            hint.setHeight(height);
        }
        break;
    case Qt::MaximumSize:
        hint = QSizeF(sceneManager() ? sceneManager()->visibleSceneSize().width() : QWIDGETSIZE_MAX,
                      QWIDGETSIZE_MAX);
        if (constraint.width() > 0.0f) {
            hint.setWidth(constraint.width());
        } else {
            hint.setWidth(QWIDGETSIZE_MAX);
        }
        if (constraint.height() > 0.0f) {
            hint.setHeight(constraint.height());
        } else {
            hint.setHeight(QWIDGETSIZE_MAX);
        }
        break;
    default:
        break;
    }

    return hint;
}

void MEditorToolbar::resizeEvent(QGraphicsSceneResizeEvent *)
{
    emit sizeChanged();
}

void MEditorToolbar::updateWidgetOrigin()
{
    // We include margin to arrow tip position.
    QPointF arrowTip(arrow->size().width() / 2.0f, 0);
    QPointF widgetOrigin = QPointF(mapFromItem(arrow, arrowTip).x(),
                                   arrow->direction() == MEditorToolbarArrow::ArrowUp
                                       ? 0.0f : size().height());

    setTransform(QTransform::fromTranslate(-widgetOrigin.x(),
                                           -widgetOrigin.y()));
}

void MEditorToolbar::updateEditorItemVisibility()
{
    // Visibility of editor item is determined solely by existence of buttons.
    if (buttonLayoutPolicy->count() > 0) {
        showEditorItem();
    } else {
        hideEditorItem();
    }
}

void MEditorToolbar::showEditorItem()
{
    // Set focus proxy so that input widget doesn't lose focus.
    setFocusPolicy(Qt::ClickFocus);
    setFocusProxy(&const_cast<MWidget &>(followWidget));
    show();
}

void MEditorToolbar::hideEditorItem()
{
    setFocusProxy(0);
    setFocusPolicy(Qt::NoFocus);
    hide();
}
