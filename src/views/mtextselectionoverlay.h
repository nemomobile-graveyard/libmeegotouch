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
#ifndef MTEXTSELECTIONOVERLAY_H
#define MTEXTSELECTIONOVERLAY_H

#include "mtopleveloverlay.h"
#include "mtextselectionhandle.h"
#include <QPointer>
#include <QRegion>

class MWidgetController;
class MWidgetView;

//! \internal

/*! 
 * MTextSelectionOverlay is to allow text selection handler to stay on top of
 * other widgets.
 */
class MTextSelectionOverlay : public MTopLevelOverlay
{
    Q_OBJECT
public:

    enum HandleType {
        Cursor,
        Anchor
    };

    /*!
     * \brief Constructs text selection overlay.
     *
     * \param newController The widget which uses text selection overlay.
     * \param newView View assciated with given controller.
     *
     */
    explicit MTextSelectionOverlay(MWidgetController *newController,
                                   MWidgetView *newView);

    virtual ~MTextSelectionOverlay();

    /*!
     * \brief Hide selection overlay.
     */
    void disappear();

    /*!
     * \brief Skip any transitions, so overlay and selection handles
     * are changing state immediately.
     */
    void skipTransitions();

    /*!
     * \brief Return region occupied by selection handles.
     * This region is in coordinates of controller.
     */
    QRegion region() const;

Q_SIGNALS:
    //! \brief Emitted when any selection handle is pressed.
    //!
    //! \param position Handle position in coordinates of controller
    //! \param handleType Indicates whether Cursor or Anchor handle was pressed on.
    void selectionHandlePressed(const QPointF &position, MTextSelectionOverlay::HandleType handleType);

    //! \brief Emitted when pressed selection handle is moved by user.
    //!
    //! \param position New position of the handle in coordinates of controller
    void selectionHandleMoved(const QPointF &position);

    //! \brief Emitted when selection handle is released.
    void selectionHandleReleased(const QPointF &position);

    //! \brief Emitted when one or more selection handle becomes visible
    //! 
    //! \note Only emitted when initially both handles are not visible and either one
    //!       or both becomes visible.
    void selectionHandlesAppeared();

protected:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    //! \reimp_end

private Q_SLOTS:
    /*!
     * \brief Remember new selection parameters and then update state of selection handles
     *
     * \sa handleSelectionUpdate()
     */
    void onSelectionChange(int newAnchor, const QRectF &newAnchorRect, bool newAnchorVisible,
                           int newCursor, const QRectF &newCursorRect, bool newCursorVisible);

    /*!
     * \brief Verifies whether overlay should be hidden or not.
     */
    void onHandleDisappeared();

    /*!
     * \brief This slot is called when selection handle is pressed.
     * Emits selectionHandlePressed(QPointF) signal.
     */
    void onSelectionHandlePressed(const MTextSelectionHandle *handle);

    /*!
     * \brief This slot is called when selection handle is moved by user.
     * Emits selectionHandleMoved(QPointF) signal.
     */
    void onHandleMoved(MTextSelectionHandle *handle);

    /*!
     * \brief This slot is called when selection handle is released.
     *
     * Updates status of selection handles and emits selectionHandleReleased() signal.
     */
    void onHandleReleased(const MTextSelectionHandle *handle);

    /*!
     * \brief Moves selection handles when selected text is moved across the screen.
     */
    void onTextScrolled();

private:
    //! \brief Shows or hides selection handles depending on selection status.
    void handleSelectionUpdate();

    //! \brief Shows overlay and update state of selection handles depending on selection status.
    void setupSelectionHandles();


    MTextSelectionHandle handleA;
    MTextSelectionHandle handleB;
    MTextSelectionHandle *anchorHandle;
    MTextSelectionHandle *cursorHandle;

    QPointer<MWidgetController> controller;
    QPointer<MWidgetView> view;

    int minimumCharacterWidth;
    //parameters of last selection update
    int anchor;
    QRectF anchorRect;
    bool anchorVisible;
    int cursor;
    QRectF cursorRect;
    bool cursorVisible;
    QPointF offset; // offset from mouse position to pressed handle
    QPointF lastUpdatePos; // last position used for selection update
    bool mouseIsPressed;

    friend class Ut_MTextSelectionOverlay;
};

//! \internal_end

#endif

