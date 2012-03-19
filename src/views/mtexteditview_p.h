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

#ifndef MTEXTEDITVIEW_P_H
#define MTEXTEDITVIEW_P_H

#include <QObject>
#include <QTextCharFormat>
#include <QAbstractTextDocumentLayout>
#include <QPointer>
#include <QPropertyAnimation>
#include <QTimer>
#include <QElapsedTimer>

#include "mtexteditview.h"
#include "melidedtext.h"
#include "mtextselectionoverlay.h"

class MEditorToolbar;
class MTextEdit;
class MTextMagnifier;
class QGraphicsSceneMouseEvent;
class QTextDocument;
class MBanner;

//! \internal
class MTextEditViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MTextEditView)

    Q_PROPERTY(qreal promptOpacity READ promptOpacity WRITE setPromptOpacity)

public:
    MTextEditViewPrivate(MTextEdit *controller, MTextEditView *q);
    virtual ~MTextEditViewPrivate();

    int cursorPosition(QGraphicsSceneMouseEvent *event);
    int cursorPosition(const QPointF &point, Qt::HitTestAccuracy accuracy = Qt::FuzzyHit);

    QTextDocument *document() const;
    QTextDocument *promptDocument() const;
    void checkScroll();
    void setMouseTarget(const QPointF &point);
    bool isLayoutLeftToRight() const;

    // returns a paint context for drawing. Includes selection formatting
    QAbstractTextDocumentLayout::PaintContext paintContext() const;

    qreal cursorX() const;

    void initMaskedDocument();

    QTextDocument *activeDocument() const;

    qreal heightForWidth(qreal width) const;

    void showMagnifier();
    void hideMagnifier();

    void showSelectionMagnifier();
    void hideSelectionMagnifier();

    void showEditorToolbar();
    void hideEditorToolbar();

    qreal promptOpacity() const;
    void setPromptOpacity(qreal opacity);

    QRectF clippingRect() const;

    void showSelectionOverlay();

    //! The following methods are to be used instead of accessing
    //! (active)document's width and textWidth properties. These enclose
    //! a workaround for bug NB#289538. The idea is to use width + cursorWidth
    //! instead of width because cursor is otherwise drawn outside of clipping
    //! rectangle, in certain situations.
    //! Once the bug is fixed, these should be removed and real width used instead.
    QSizeF activeDocumentSize() const;
    QSizeF documentSize() const;
    qreal activeDocumentTextWidth() const;
    qreal documentTextWidth() const;

    //! \brief calculate real document text width based on view's width
    //! \param viewWidth Width of view. If -1, current width will be used.
    qreal realDocumentTextWidth(qreal viewWidth = -1) const;

    void drawPrompt(QPainter *painter) const;

protected slots:
    void scrolling();
    void hideUnmaskedText();
    void checkSize();
    void handleDocumentUpdate(int position, int charsRemoved, int charsAdded);
    void handleDocumentUpdated();
    void handleDocumentSizeChange(const QSizeF &newSize);
    void playSelectionAndMagnifierFeedback();
    void updateMagnifierPosition();
    void makeMagnifierDisappear();
    void makeMagnifierAppear();
    void hideEditorToolbarTemporarily();
    void restoreEditorToolbar();
    void updateEditorToolbarPosition();
    void startFocusAnimation();
    void scrollSelectSlot();
    void onScenePositionChanged();

    void onEditorMoved();

    void mapSelectionChange();
    void onSelectionHandleMoved(const QPointF &position);
    void onSelectionHandlePressed(const QPointF &position, MTextSelectionOverlay::HandleType handleType);
    void onSelectionHandleReleased(const QPointF &position);
    void onSelectionHandlesAppeared();
    void onSelectionOverlayVisibleChanged();
    void hideSelectionOverlayTemporarily();
    void restoreSelectionOverlay();
    void setSelection();
    void handleSelectionChanged();

    void updateGestureGrab();

private:
    void scrollingTestAndStart(QGraphicsSceneMouseEvent *event);
    /*!
     * Test and start scroll to ensue the cursor at \a pos to be visible.
     * If useTimer equal true then the scroll is repeated with a fixed speed,
     * otherwise scroll direction to the \a pos
     */
    void scrollingTestAndStart(const QPointF &pos, bool useTimer = true);
    void checkStartOfSelection(QGraphicsSceneMouseEvent *event);
    void startSelection(QGraphicsSceneMouseEvent *event);
    void stopSelection();
    void updateSelection(const QPointF &pos);
    void updateSelection(QGraphicsSceneMouseEvent *event);

    //! Returns a rectangle covering preedit text in item space
    QRect preeditRectangle() const;

    //! Returns width of the cursor
    int cursorWidth() const;

    QRect cursorRect(int position, int cursorWidth) const;

    //! Returns a rectangle of the cursor in local view coordinates
    QRect cursorRect() const;

    //! \return area covered by text starting from \a startPosition and ending
    //! to \a endPosition.  Both positions must be on the same line.
    QRect textRectangle(int startPosition, int endPosition);
    //! \return area covered by selection on its \a first or last line
    QRect selectionLineRectangle(bool first);
    //! \return rectangle in local view coordinates, which is not obscured/clipped
    //!         by pannable viewports or navigation controls
    QRect visibleArea() const;

    void playFocusAnimation(QAbstractAnimation::Direction direction,
                            qreal endValue);

    void icUpdate();

    //! \brief map item coordinates to view coordinates
    QPointF fromItem(const QPointF &itemPoint) const;
    QRectF fromItem(const QRectF &itemRect) const;

    //! \brief map local view coordinates to item coordinates
    QRectF toItem(const QRectF &viewRect) const;

    //! \brief Top left corner of view rectangle in controller coordinates.
    QPointF viewOffset() const;

    //! \brief Return text option for prompt drawing.
    QTextOption promptOption() const;

    //! \brief Elide prompt if it does not fit into current geometry.
    QString elidedPrompt(const QTextOption &textOption) const;

protected:
    MTextEditView *q_ptr;

    MTextEdit *controller;

    bool focused;

    QTextDocument *maskedTextDocument;

    int unmaskPosition; // position where unmask in masked mode starts
    int unmaskLength; // lenght of unmasked text

    // styling
    qreal selectionThreshold;
    QChar maskCharacter; // what characters are replaced with in masked mode
    QTextCharFormat selectionFormat;

    qreal hscroll; // horizontal offset of text
    bool hscrollSnapRight;
    qreal vscroll; // vertical offset

    int scrollSpeedVertical;
    int scrollSpeedHorizontal;

    QTimer *longPressTimer;
    QTimer *scrollTimer;
    QTimer *maskTimer;

    bool selecting; // separate from controller mode because the selection there doesn't tell
    // this view initiated the selection by mouse press
    bool ignoreSelection;
    int startCursorPos;

    qreal documentHeight;
    QPointF mouseTarget; // known mouse position inside widget

    bool inAutoSelectionClick;

    QPointer<MBanner> infoBanner;

    bool editActive; // true if editing started and having focus

    QWeakPointer<MTextMagnifier> magnifier;
    QWeakPointer<MEditorToolbar> editorToolbar;

    qreal currentPromptOpacity;
    QPropertyAnimation promptFocusAnimation;
    QPropertyAnimation promptShowHideAnimation;
    bool isPromptVisible;
    QTimer * const focusAnimationDelay;
    QTimer * const scrollSelectTimer;
    QPointF scrollSelectScenePosition;

    /*! When receiving mouse events, this variable tells whether
     *  ongoing tap is the one that gives this widget focus.
     *
     *  Note that this is different from whether the widget actually
     *  has focus. It depends whether scene is using focus-on-press
     *  or focus-on-release.
     */
    bool focusingTap;

    QWeakPointer<MTextSelectionOverlay> selectionOverlay;
    bool selectionHandleIsPressed;
    QPointF lastHandlePos;
    QElapsedTimer handleTime;
    QTimer delaySelection;
    int anchorPos;
    int cursorPos;

    QElapsedTimer timeOnFeedback;
    QElapsedTimer timeOnMove;
    QPointF currentPosition;
    QPointF previousPosition;
    qreal previousHorizontalScroll;
    bool viewScrolled;
    int previousSelectionCursorPosition;

    QTimer showMovedToolbar;
    bool panningStarted;
    QPointF lastScenePos; // last controller position in scene coordinates
    QElapsedTimer movementTime;

    MElidedText shortPrompt;

#ifdef UNIT_TEST
    friend class Ut_MTextEditView;
    friend class Ut_MTextEdit;
#endif
};
//! \internal_end

#endif
