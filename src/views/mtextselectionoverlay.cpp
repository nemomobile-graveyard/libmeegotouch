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


#include "mtextselectionoverlay.h"
#include "mwidgetcontroller.h"
#include "mwidgetview.h"
#include "mdebug.h"

#include <QGraphicsSceneMouseEvent>

namespace {
    const int DefaultMinimumCharacterWidth = 3;

    const QString ObjectNameOverlay("TextSelectionOverlay");
    const QString ObjectNameHandleA("TextSelectionHandleA");
    const QString ObjectNameHandleB("TextSelectionHandleB");
}

MTextSelectionOverlay::MTextSelectionOverlay(MWidgetController *newController,
                                             MWidgetView *newView)
    : MTopLevelOverlay(newController->sceneManager()),
      anchorHandle(&handleA),
      cursorHandle(&handleB),
      controller(newController),
      view(newView),
      minimumCharacterWidth(DefaultMinimumCharacterWidth),
      anchor(0),
      anchorVisible(false),
      cursor(0),
      cursorVisible(false),
      mouseIsPressed(false)
{
    setFocusProxy(0);
    setFocusPolicy(Qt::NoFocus);

    hide();
    controller->scene()->addItem(this);
    handleA.setParentItem(this);
    handleB.setParentItem(this);

    setObjectName(ObjectNameOverlay);
    handleA.setObjectName(ObjectNameHandleA);
    handleB.setObjectName(ObjectNameHandleB);

    connect(view, SIGNAL(selectionChanged(int, QRectF, bool, int, QRectF, bool)),
            this, SLOT(onSelectionChange(int, QRectF, bool, int, QRectF, bool)));
    connect(view, SIGNAL(contentScrolled()),
            this, SLOT(onTextScrolled()));

    connect(&handleA, SIGNAL(disappeared()),
            this, SLOT(onHandleDisappeared()));
    connect(&handleB, SIGNAL(disappeared()),
            this, SLOT(onHandleDisappeared()));
    connect(&handleA, SIGNAL(pressed(const MTextSelectionHandle *)),
            this, SLOT(onSelectionHandlePressed(const MTextSelectionHandle *)));
    connect(&handleB, SIGNAL(pressed(const MTextSelectionHandle *)),
            this, SLOT(onSelectionHandlePressed(const MTextSelectionHandle *)));
    connect(&handleA, SIGNAL(released(const MTextSelectionHandle *)),
            this, SLOT(onHandleReleased(const MTextSelectionHandle *)));
    connect(&handleB, SIGNAL(released(const MTextSelectionHandle *)),
            this, SLOT(onHandleReleased(const MTextSelectionHandle *)));

    QVariant fontVariant = view->property("inputMethodFont");
    if (fontVariant.isValid()) {
        // use the width of smallest character 'i'
        QFont viewFont = fontVariant.value<QFont>();
        minimumCharacterWidth = QFontMetrics(viewFont).width("i");
    }
}

void MTextSelectionOverlay::disappear()
{
    onSelectionChange(0, QRectF(), false,
                      0, QRectF(), false);
}

void MTextSelectionOverlay::skipTransitions()
{
    handleA.skipTransitions();
    handleB.skipTransitions();
}

QRegion MTextSelectionOverlay::region() const
{
    QRegion result;

    if (handleA.isVisible()) {
        result = mapRectToItem(controller, handleA.region()).toRect();
    }

    if (handleB.isVisible()) {
        result |= mapRectToItem(controller, handleB.region()).toRect();
    }

    return result;
}

void MTextSelectionOverlay::onSelectionChange(int newAnchor,
                                              const QRectF &newAnchorRect,
                                              bool newAnchorVisible,
                                              int newCursor,
                                              const QRectF &newCursorRect,
                                              bool newCursorVisible)
{
    anchor = newAnchor;
    anchorRect = newAnchorRect;
    anchorVisible = newAnchorVisible;
    cursor = newCursor;
    cursorRect = newCursorRect;
    cursorVisible = newCursorVisible;

    handleSelectionUpdate();
}

void MTextSelectionOverlay::onHandleDisappeared()
{
    if (mouseIsPressed || anchor != cursor) {
        return;
    }

    if (!handleA.isVisible() && !handleB.isVisible()) {
        setFocusProxy(0);
        setFocusPolicy(Qt::NoFocus);
        hide();
    }
}

void MTextSelectionOverlay::handleSelectionUpdate()
{
    if (anchor != cursor) {
        setupSelectionHandles();
    } else if (!mouseIsPressed) {
        handleA.setPositionVisibility(false);
        handleB.setPositionVisibility(false);
        onHandleDisappeared();
    }
}

void MTextSelectionOverlay::onSelectionHandlePressed(const MTextSelectionHandle *handle)
{
    if (!handle) {
        return;
    }

    const QPointF position = mapToItem(controller, handle->hotSpot());

    emit selectionHandlePressed(position);
}

void MTextSelectionOverlay::onHandleMoved(MTextSelectionHandle *handle)
{
    if (!handle) {
        return;
    }

    const QPointF position = mapToItem(controller, handle->hotSpot());
    emit selectionHandleMoved(position);
}

void MTextSelectionOverlay::onHandleReleased(const MTextSelectionHandle *handle)
{
    // we do not use multitouch, so both handles are
    // released now
    handleSelectionUpdate();

    if (!handle) {
        return;
    }
    const QPointF position = mapToItem(controller, handle->hotSpot());
    emit selectionHandleReleased(position);
}

void MTextSelectionOverlay::onTextScrolled()
{
    MTextSelectionHandle *pressedHandle = 0;

    if (handleA.isPressed()) {
        pressedHandle = &handleA;
    } else if (handleB.isPressed()) {
        pressedHandle = &handleB;
    }

    onHandleMoved(pressedHandle);
}

void MTextSelectionOverlay::setupSelectionHandles()
{
    if (!isVisible()) {
        show();
    }
    if (handleA.isPressed()){
        cursorHandle = &handleA;
        anchorHandle = &handleB;
    } else if (handleB.isPressed()) {
        cursorHandle = &handleB;
        anchorHandle = &handleA;
    }

    if (!anchorHandle->isPressed()) {
        anchorHandle->setPositionVisibility(anchorVisible);
        if (!anchorVisible) {
            anchorHandle->skipTransitions();
        }
        const QRectF anchorRectLocal = mapFromItem(controller, anchorRect).boundingRect();
        anchorHandle->updatePosition(anchorRectLocal.bottomLeft());
    }

    if (!cursorHandle->isPressed()) {
        cursorHandle->setPositionVisibility(cursorVisible);
        if (!cursorVisible) {
            cursorHandle->skipTransitions();
        }
        const QRectF cursorRectLocal = mapFromItem(controller, cursorRect).boundingRect();
        cursorHandle->updatePosition(cursorRectLocal.bottomLeft());
    }
}

void MTextSelectionOverlay::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!handleA.isVisible() && !handleB.isVisible()) {
        event->ignore();
        return;
    }


    MTextSelectionHandle *selectionHandle = 0;
    MTextSelectionHandle *idleHandle = 0;

    if (handleA.isReady()
        && handleA.sceneBoundingRect().contains(event->scenePos())) {
        selectionHandle = &handleA;
        idleHandle = &handleB;
    } else if (handleB.isReady()
               && handleB.sceneBoundingRect().contains(event->scenePos())) {
        selectionHandle = &handleB;
        idleHandle = &handleA;
    }

    if (selectionHandle) {
        event->accept();
        mouseIsPressed = true;
        grabMouse();
        grabGesture(Qt::PanGesture);
        selectionHandle->setPressed(true);
        offset = event->pos() - selectionHandle->pos();
        lastUpdatePos = selectionHandle->pos();

        setFocusPolicy(Qt::ClickFocus);
        setFocusProxy(controller);

        selectionHandle->hideTemporarily();
        idleHandle->disable();
    } else {
        event->ignore();
    }
}

void MTextSelectionOverlay::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    MTextSelectionHandle &selectionHandle = handleA.isPressed()
                                           ? handleA
                                           : handleB;
    QPointF newPos = event->pos() - offset;

    if ((newPos - selectionHandle.pos()).manhattanLength()
        > selectionHandle.movementSensitivity()) {
        selectionHandle.setPos(newPos);
    }

    if ((newPos - lastUpdatePos).manhattanLength() > minimumCharacterWidth) {
        lastUpdatePos = newPos;
        onHandleMoved(&selectionHandle);
    }
}

void MTextSelectionOverlay::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    cancelEvent(0);
}

void MTextSelectionOverlay::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    mouseIsPressed = false;
    ungrabMouse();
    ungrabGesture(Qt::PanGesture);
    handleA.enable();
    handleB.enable();
    handleA.restore();
    handleB.restore();
    handleA.setPressed(false);
    handleB.setPressed(false);

    setFocusProxy(0);
    setFocusPolicy(Qt::NoFocus);
}
