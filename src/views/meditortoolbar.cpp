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
#include "meditortoolbar.h"
#include "meditortoolbar_p.h"
#include "mtopleveloverlay.h"
#include <mtexteditstyle.h>

#include <MButton>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MSceneManager>
#include <MWidget>
#include <QAction>
#include <MInputMethodState>

#include <QtAlgorithms>
#include <QApplication>
#include <QGraphicsLinearLayout>

namespace {
    // Any value above 1.0 will put our overlay on top of any other MTopLevelOverlay
    const qreal OverlayZValue = 10.0f;
}

MEditorToolbar::MEditorToolbar(MWidget *followWidget)
    : d_ptr(new MEditorToolbarPrivate(this, followWidget))
{
    Q_D(MEditorToolbar);
    d->init();
}

MEditorToolbar::~MEditorToolbar()
{
}

void MEditorToolbar::setForbiddenRegion(const QRegion &forbiddenRegion)
{
    Q_D(MEditorToolbar);
    QRegion overlayRegion;

    if (d->followWidget && d->overlay) {
        foreach (const QRect &rect, forbiddenRegion.rects()) {
            overlayRegion |= d->overlay->mapRectFromItem(d->followWidget, rect).toRect();
        }
    }
    d->forbiddenRegion = overlayRegion;
}

void MEditorToolbar::setPosition(const QPointF &pos,
                                 ToolbarPlacement placement)
{
    Q_D(MEditorToolbar);
    d->setPosition(pos, placement == BelowPointOfInterest ? MEditorToolbarArrow::ArrowUp
                                                          : MEditorToolbarArrow::ArrowDown);
}

void MEditorToolbar::appear()
{
    Q_D(MEditorToolbar);
    d->appear(MEditorToolbarPrivate::Animated);
}

void MEditorToolbar::disappear()
{
    Q_D(MEditorToolbar);
    d->disappear(MEditorToolbarPrivate::Animated);
}

bool MEditorToolbar::isAppeared() const
{
    Q_D(const MEditorToolbar);
    return d->isAppeared();
}

bool MEditorToolbar::isAutoHideEnabled() const
{
    Q_D(const MEditorToolbar);
    return d->autoHideEnabled;
}

void MEditorToolbar::setAutoHideEnabled(bool enable)
{
    Q_D(MEditorToolbar);
    if (d->autoHideEnabled == enable) {
        return;
    }

    d->autoHideEnabled = enable;

    if (enable && isAppeared()) {
        d->startAutoHideTimer();
    } else if (!enable) {
        d->stopAutoHideTimer();
    }
}

bool MEditorToolbar::event(QEvent *event)
{
    Q_D(MEditorToolbar);

    switch (event->type()) {
    case QEvent::ActionAdded:
        d->handleActionAdded(static_cast<QActionEvent *>(event));
        break;
    case QEvent::ActionRemoved:
        d->handleActionRemoved(static_cast<QActionEvent *>(event));
        break;
    case QEvent::ActionChanged:
        d->handleActionChanged(static_cast<QActionEvent *>(event));
        break;
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

QSizeF MEditorToolbar::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MEditorToolbar);
    return d->sizeHint(which, constraint);
}

void MEditorToolbar::resizeEvent(QGraphicsSceneResizeEvent *)
{
    emit sizeChanged();
}

void MEditorToolbar::updateGeometry()
{
    MStylableWidget::updateGeometry();
}

// Private class implementation

MEditorToolbarPrivate::MEditorToolbarPrivate(MEditorToolbar *qq, MWidget *followWidget)
    : q_ptr(qq),
      overlay(new MTopLevelOverlay(followWidget->sceneManager())),
      followWidget(followWidget),
      buttonLayoutPolicy(new MLinearLayoutPolicy(new MLayout(qq),
                                                 Qt::Horizontal)),
      arrow(new MEditorToolbarArrow(qq)),
      buttonUpdateQueued(false),
      hideAnimation(qq, "opacity"),
      autoHideEnabled(false),
      positionUpdatePending(false),
      pendingArrowDirection(MEditorToolbarArrow::ArrowDown) // Initial value doesn't matter.
{
}

MEditorToolbarPrivate::~MEditorToolbarPrivate()
{
    Q_Q(MEditorToolbar);

    hideEditorItem();
    // Before destroying parent, detach so it doesn't try to destroy us.
    q->setParentItem(0);
    if (overlay) {
        overlay->deleteLater();
    }
    qDeleteAll(buttons);
}

void MEditorToolbarPrivate::init()
{
    Q_Q(MEditorToolbar);

    q->setFlag(QGraphicsItem::ItemHasNoContents, true);
    overlay->hide();
    overlay->setZValue(OverlayZValue); // put toolbar on top of any other MTopLevelOverlay
    q->hide();
    followWidget->scene()->addItem(overlay);
    q->setParentItem(overlay);

    // Set z value for arrow higher than default since
    // it needs to float on top of buttons.
    arrow->setZValue(1.0f);

    // The policy notifies the widgets of their relative position inside the layout,
    // this causes the buttons to be rendered with different backgrounds for each position
    buttonLayoutPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);

    // Don't add extra margins or spacings for buttons.
    buttonLayoutPolicy->setContentsMargins(0.0f, 0.0f, 0.0f, 0.0f);
    buttonLayoutPolicy->setSpacing(0.0f);

    QObject::connect(q->sceneManager(), SIGNAL(orientationChanged(M::Orientation)),
                     q, SLOT(updateGeometry()));

    autohideTimer.setSingleShot(true);

    hideAnimation.setStartValue(1.0);
    hideAnimation.setEndValue(0.0);
    hideAnimation.setEasingCurve(QEasingCurve::InOutQuad);
    QObject::connect(&hideAnimation, SIGNAL(finished()), q, SLOT(_q_onAnimationFinished()));

    eatMButtonGestureFilter = new EatMButtonGestureFilter(q);

    // Setup toolbar to hide during scene orientation change.
    // Don't need to hide during 180 degree angle changes.
    QObject::connect(followWidget->sceneManager(),
                     SIGNAL(orientationAboutToChange(M::Orientation)),
                     q, SLOT(_q_disappearForOrientationChange()));
    // Reappearance must be connected to orientationChangeFinished rather than
    // orientationChanged because this doesn't share the orientation animation
    // with followWidget.
    QObject::connect(followWidget->sceneManager(),
                     SIGNAL(orientationChangeFinished(M::Orientation)),
                     q, SLOT(_q_reappearAfterOrientationChange()));

    disappearedForOrientationChange = false;
}

void MEditorToolbarPrivate::setPosition(const QPointF &pos,
                                        MEditorToolbarArrow::ArrowDirection arrowDirection)
{
    Q_Q(MEditorToolbar);

    // Position should not be set unless overlay's orientation is the same
    // as followWidget's. Because of this, always post-pone setting the
    // position in case we are disappeared. Which we are during orientation
    // change.

    if (isAppeared()) {
        q->setPos(followWidget->mapToItem(overlay, pos));
        updateArrow(arrowDirection);
    } else {
        positionUpdatePending = true;
        pendingPosition = pos;
        pendingArrowDirection = arrowDirection;
    }
}

void MEditorToolbarPrivate::appear(TransitionMode transition)
{
    Q_Q(MEditorToolbar);

    if (overlay->isVisible() && hideAnimation.state() == QAbstractAnimation::Stopped
        && qFuzzyCompare(q->opacity(), static_cast<qreal>(1.0f))) {
        return;
    }

    overlay->show();
    updateEditorItemVisibility();

    // then cancel currently pending actions and set new ones is necessary
    // (this function is called only by controller directly)
    stopAutoHideTimer();
    hideAnimation.setDuration(q->style()->showAnimationDuration());
    hideAnimation.setDirection(QAbstractAnimation::Backward);
    if (transition == Animated) {
        if (hideAnimation.state() == QAbstractAnimation::Stopped) {
            hideAnimation.start();
        }
    } else {
        hideAnimation.stop();
        q->setOpacity(hideAnimation.startValue().toReal());
        _q_onAnimationFinished();
    }

    if (autoHideEnabled) {
        startAutoHideTimer();
    }

    if (buttonUpdateQueued) {
        // the update was blocked during animation
        _q_updateAvailableButtons();
    }

    if (positionUpdatePending) {
        positionUpdatePending = false;
        setPosition(pendingPosition, pendingArrowDirection);
    }
}

void MEditorToolbarPrivate::disappear(TransitionMode transition)
{
    Q_Q(MEditorToolbar);

    stopAutoHideTimer();
    if (transition == Animated) {
        hideAnimation.setDirection(QAbstractAnimation::Forward);
        if (hideAnimation.state() == QAbstractAnimation::Stopped) {
            hideAnimation.start();
        }
    } else {
        hideAnimation.stop();
        q->setOpacity(hideAnimation.endValue().toReal());
        _q_onAnimationFinished();
    }

    disappearedForOrientationChange = false;
}

bool MEditorToolbarPrivate::isAppeared() const
{
    const bool hiding = hideAnimation.state() == QAbstractAnimation::Running
                        && hideAnimation.direction() == QAbstractAnimation::Forward;

    return overlay->isVisible() && !hiding;
}

void MEditorToolbarPrivate::handleActionAdded(QActionEvent *actionEvent)
{
    Q_ASSERT(actionEvent);
    Q_ASSERT(!actionEvent->before()); // we support appending only
    QAction *action(qobject_cast<QAction *>(actionEvent->action()));
    Q_ASSERT(action);

    MButton *newButton = new MButton(action->text());
    newButton->grabGesture(Qt::TapGesture);
    newButton->grabGesture(Qt::TapAndHoldGesture);
    newButton->installEventFilter(eatMButtonGestureFilter);
    newButton->setStyleName(action->objectName());
    QObject::connect(newButton, SIGNAL(clicked(bool)), action, SLOT(trigger()));

    buttons << newButton;

    if (action->isVisible()) {
        visibilityUpdated();
    }
}

void MEditorToolbarPrivate::handleActionRemoved(QActionEvent *actionEvent)
{
    Q_Q(MEditorToolbar);
    const int actionIndex = q->actions().indexOf(actionEvent->action());
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
}

void MEditorToolbarPrivate::handleActionChanged(QActionEvent *actionEvent)
{
    Q_Q(MEditorToolbar);
    // Name of action might have been changed.
    const int actionIndex = q->actions().indexOf(actionEvent->action());
    Q_ASSERT(actionIndex >= 0 && actionIndex < buttons.count());
    MButton *button(buttons.at(actionIndex));
    if (button->text() != actionEvent->action()->text()) {
        button->setText(actionEvent->action()->text());
    }

    // Update visibility of buttons to match visibility of actions.
    visibilityUpdated();
}

QSizeF MEditorToolbarPrivate::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_Q(const MEditorToolbar);
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
            width += q->style()->marginLeft() + q->style()->marginRight();
            height += q->style()->marginTop() + q->style()->marginBottom();
            hint.setWidth(width);
            hint.setHeight(height);
        }
        break;
    case Qt::MaximumSize:
        hint = QSizeF(q->sceneManager() ? q->sceneManager()->visibleSceneSize().width() : QWIDGETSIZE_MAX,
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

void MEditorToolbarPrivate::updateArrow(MEditorToolbarArrow::ArrowDirection direction)
{
    Q_Q(MEditorToolbar);

    // Clear local transforms.
    q->setTransform(QTransform());

    // Style mode is different with regarding to top and bottom margins.
    if (direction == MEditorToolbarArrow::ArrowUp) {
        q->style().setModeEditorUnderCursor();
    } else {
        q->style().setModeDefault();
    }
    q->applyStyle();

    const QRectF contentsRectangle = q->contentsRect();

    // Update horizontally, make sure widget is inside screen.
    qreal center = contentsRectangle.center().x();
    QRectF mappedSceneRect = q->mapRectFromScene(QRectF(QPointF(),
                                                        q->sceneManager()->visibleSceneSize(M::Landscape)));
    mappedSceneRect.translate(center, 0.0f);

    qreal offscreenLeft = qMax<qreal>(0.0f, mappedSceneRect.left());
    qreal offscreenRight = qMax<qreal>(0.0f, (q->effectiveSizeHint(Qt::PreferredSize).width()
                                              - mappedSceneRect.right()));
    // Screen rectangle in overlay coordinate system, just like we are
    const QRectF screenRectInOverlay(
        overlay->mapRectFromScene(QRectF(QPointF(), q->sceneManager()->visibleSceneSize(M::Landscape))));
    qreal x;

    if (q->size().width() < screenRectInOverlay.width()) {
        // The widget won't be off the screen from both ends at the same time.
        // Width is restricted to screen width.
        x = center - arrow->size().width() / 2.0f
            - offscreenLeft + offscreenRight;
        x = qBound<qreal>(contentsRectangle.left(),
                          x,
                          contentsRectangle.right() - arrow->size().width());
    } else {
        x = q->geometry().center().x() - screenRectInOverlay.center().x() - arrow->size().width() / 2.0f;
    }

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

void MEditorToolbarPrivate::updateWidgetOrigin()
{
    Q_Q(MEditorToolbar);

    // We include margin to arrow tip position.
    QPointF arrowTip(arrow->size().width() / 2.0f, 0);
    arrowTip = q->mapFromItem(arrow, arrowTip);

    qreal translateX = arrowTip.x();

    const QRectF screenRectInOverlay(
        overlay->mapRectFromScene(QRectF(QPointF(), q->sceneManager()->visibleSceneSize(M::Landscape))));

    const QSizeF size(q->size());
    const QPointF pos(q->pos());

    // Avoid editor toolbar clipping when possible
    if (size.width() < screenRectInOverlay.width()) {
        if (pos.x() < (screenRectInOverlay.width() - size.width())) {
            // Don't allow editor toolbar to go over the left edge of the screen
            translateX = qMin(translateX, pos.x());
        } else {
            // Don't allow editor toolbar to go over the right edge of the screen
            translateX = qMax(translateX, size.width() + pos.x() - screenRectInOverlay.width());
        }
    }

    // We need to round to an integer coordinate to avoid graphics glitches; if
    // widgetOrigin.x() is for example 75.5, in portrait mode with German language with
    // Cut, Copy & Paste buttons visible the one pixel thick button separator lines cannot
    // be seen.

    QPoint widgetOrigin(QPointF(translateX,
                                arrow->direction() == MEditorToolbarArrow::ArrowUp
                                ? 0.0f : size.height()).toPoint());
    const QRect candidateGeometry = QRectF(pos - widgetOrigin, size).toRect();

    if (forbiddenRegion.intersects(candidateGeometry)) {

        // Map input panel rectangle to overlay coordinates and remove it
        // from visible scene rectangle. It is assumed that input panel
        // is always at the bottom of the sreen.
        QRectF visibleSceneRect(screenRectInOverlay);
        const QRect sipRect = MInputMethodState::instance()->inputMethodArea();
        if (!sipRect.isNull()) {
            const QRect mappedPanelRect(overlay->mapRectFromScene(sipRect).toRect());

            visibleSceneRect.setBottom(qMin<qreal>(mappedPanelRect.top(),
                                                   visibleSceneRect.bottom()));
        }

        const QRectF forbiddenRect = forbiddenRegion.boundingRect();

        if (arrow->direction() == MEditorToolbarArrow::ArrowUp
            && (forbiddenRect.bottom() + size.height()) < visibleSceneRect.bottom()) {
            widgetOrigin.setY(pos.y() - forbiddenRect.bottom());
        } else if (arrow->direction() == MEditorToolbarArrow::ArrowDown
                   && (forbiddenRect.top() - size.height()) > visibleSceneRect.top()) {
            widgetOrigin.setY(pos.y() - (forbiddenRect.top() - size.height()));
        }
    }

    q->setTransform(QTransform::fromTranslate(-widgetOrigin.x(),
                                              -widgetOrigin.y()));
}

void MEditorToolbarPrivate::_q_startAutomaticHide()
{
    Q_Q(MEditorToolbar);
    hideAnimation.setDuration(q->style()->hideAnimationDuration());
    hideAnimation.setDirection(QAbstractAnimation::Forward);
    if (hideAnimation.state() == QAbstractAnimation::Stopped) {
        hideAnimation.start();
    }
}

void MEditorToolbarPrivate::_q_updateAvailableButtons()
{
    Q_Q(MEditorToolbar);

    if (isHiding()) {
        // the toolbar is hiding, and button update would make it flicker uselessly
        // buttons will be updated in appear()
        return;
    }

    buttonUpdateQueued = false;

    while (buttonLayoutPolicy->count() > 0) {
        buttonLayoutPolicy->removeAt(buttonLayoutPolicy->count() - 1);
    }

    QList<QAction *> actionList(q->actions());
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
    q->resize(q->preferredSize());

    // Hide if there is no buttons.
    updateEditorItemVisibility();
}

void MEditorToolbarPrivate::_q_disappearForOrientationChange()
{
    if (isAppeared()) {
        disappear(Immediate);
        disappearedForOrientationChange = true;
    }
}

void MEditorToolbarPrivate::_q_reappearAfterOrientationChange()
{
    if (!isAppeared() && disappearedForOrientationChange) {
        disappearedForOrientationChange = false;
        appear(Immediate);
    }
}

void MEditorToolbarPrivate::_q_onAnimationFinished()
{
    Q_Q(MEditorToolbar);
    if (qFuzzyIsNull(q->opacity())) {
        hideEditorItem();
        overlay->hide();
    }
}

void MEditorToolbarPrivate::visibilityUpdated()
{
    Q_Q(MEditorToolbar);
    if (!buttonUpdateQueued) {
        buttonUpdateQueued = true;
        QMetaObject::invokeMethod(q, "_q_updateAvailableButtons", Qt::QueuedConnection);
    }
}

void MEditorToolbarPrivate::updateEditorItemVisibility()
{
    // Visibility of editor item is determined solely by existence of buttons.
    if (buttonLayoutPolicy->count() > 0) {
        showEditorItem();
    } else {
        hideEditorItem();
    }
}

void MEditorToolbarPrivate::showEditorItem()
{
    Q_Q(MEditorToolbar);
    // Set focus proxy so that input widget doesn't lose focus.
    q->setFocusPolicy(Qt::ClickFocus);
    q->setFocusProxy(followWidget);
    q->show();
}

void MEditorToolbarPrivate::hideEditorItem()
{
    Q_Q(MEditorToolbar);
    q->setFocusProxy(0);
    q->setFocusPolicy(Qt::NoFocus);
    q->hide();
}

void MEditorToolbarPrivate::startAutoHideTimer()
{
    Q_Q(MEditorToolbar);
    int interval = q->style()->hideTimeout();
    if (interval > 0) {
        QObject::connect(&autohideTimer, SIGNAL(timeout()), q, SLOT(_q_startAutomaticHide()));
        autohideTimer.setInterval(interval);
        autohideTimer.start();
    } else if (interval == 0) {
        disappear(Immediate);
    }
}

void MEditorToolbarPrivate::stopAutoHideTimer()
{
    Q_Q(MEditorToolbar);
    autohideTimer.disconnect(q);
    autohideTimer.stop();
}

bool MEditorToolbarPrivate::isHiding()
{
    return hideAnimation.state() != QAbstractAnimation::Stopped
            && hideAnimation.direction() == QAbstractAnimation::Forward;
}

EatMButtonGestureFilter::EatMButtonGestureFilter(QObject *parent)
    :QObject(parent)
{
}

bool EatMButtonGestureFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Gesture || event->type() == QEvent::GestureOverride) {
        event->accept();
        return true;
    } else {
        return QObject::eventFilter(watched, event);
    }
}

#include "moc_meditortoolbar.cpp"
