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

#include "mtexteditview.h"
#include "mtexteditview_p.h"
#include "mtextmagnifier.h"
#include "meditortoolbar.h"
#include "mcompleter.h"

#include <MSceneManager>
#include <MApplicationPage>
#include <MPannableViewport>
#include <MDeviceProfile>

#include <QApplication>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextLayout>
#include <QAbstractTextDocumentLayout>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QPanGesture>
#include <QInputContext>

#include "mtextedit.h"
#include "mtextedit_p.h"
#include "mbanner.h"
#include "mtimestamp.h"
#include "minputmethodstate.h"
#include "mdebug.h"
#include "mtextselectionoverlay.h"

namespace
{
    const int LongPressTime = 600; //! how long to wait in tap&hold to initiate long press action
    const int ScrollMargin = 20; // distance from the edge to start scrolling
    const int ScrollingTimeDuration = 100;
    const int ScrollStep = 10; // pixels to scroll at once

    const int HoldFeedbackTime = 250; // time to not play feedback after scroll while selecting or moving magnifier in ms
    const int WhitespaceCheckLimit = 10; // selection amount where existance of whitespace is checked

    //! how long before character becomes masked
    const int MaskedTimeInterval = 1000;

    const int InternalMargin = 0; // disable the margin altogether, QTextDocument default is 4

    const QChar DefaultMaskCharacter('*');

    const QChar TextVariantSeparator(0x9c, 0);

    const char * const CursorWidthProperty = "cursorWidth";

    // Editor toolbar action property that affects toolbar closing
    const char * const NoCloseToolbarOnTriggerProperty("noCloseToolbarOnTrigger");

    inline bool isGreater(const QSizeF &s1, const QSizeF &s2) {
        return s1.width() < s2.width() || s1.height() < s2.height();
    }
}


/*!
 * \brief Constructor
 * \param control MTextEdit widget which this view shows
 */
MTextEditViewPrivate::MTextEditViewPrivate(MTextEdit *control, MTextEditView *q)
    : q_ptr(q),
      controller(control),
      focused(false),
      maskedTextDocument(0),
      unmaskPosition(-1),
      unmaskLength(0),
      selectionThreshold(15),
      maskCharacter(DefaultMaskCharacter),
      hscroll(0.0),
      hscrollSnapRight(false),
      vscroll(0.0),
      scrollSpeedVertical(0),
      longPressTimer(new QTimer(this)),
      scrollTimer(new QTimer(this)),
      maskTimer(new QTimer(this)),
      selecting(false),
      ignoreSelection(false),
      startCursorPos(-1),
      documentHeight(0.0),
      mouseTarget(0, 0),
      inAutoSelectionClick(false),
      infoBanner(0),
      editActive(false),
      currentPromptOpacity(0.2),
      promptFocusAnimation(this, "promptOpacity"),
      promptShowHideAnimation(this, "promptOpacity"),
      isPromptVisible(false),
      focusAnimationDelay(new QTimer(this)),
      scrollSelectTimer(new QTimer(this)),
      focusingTap(true),
      selectionHandleIsPressed(false),
      timeOnFeedback(QElapsedTimer()),
      timeOnMove(QElapsedTimer()),
      previousHorizontalScroll(0.0),
      viewScrolled(false),
      previousSelectionCursorPosition(0),
      panningStarted(false)
{
    selectionFormat.setForeground(Qt::white);
    selectionFormat.setBackground(Qt::black);

    longPressTimer->setSingleShot(true);
    longPressTimer->setInterval(LongPressTime);

    maskTimer->setSingleShot(true);
    maskTimer->setInterval(MaskedTimeInterval);

    focusAnimationDelay->setSingleShot(true);

    scrollSelectTimer->setSingleShot(false);
    scrollSelectTimer->setInterval(200);

    handleTime.invalidate();
    timeOnFeedback.invalidate();
    timeOnMove.invalidate();

    controller->grabGesture(Qt::PanGesture);

    QObject::connect(longPressTimer, SIGNAL(timeout()), q, SLOT(handleLongPress()));
    QObject::connect(scrollTimer, SIGNAL(timeout()), this, SLOT(scrolling()));
    QObject::connect(maskTimer, SIGNAL(timeout()), this, SLOT(hideUnmaskedText()));
    QObject::connect(control, SIGNAL(cursorPositionChanged()),
                     this, SLOT(updateEditorToolbarPosition()));
    QObject::connect(control, SIGNAL(selectionChanged()),
                     this, SLOT(updateEditorToolbarPosition()));
    QObject::connect(focusAnimationDelay, SIGNAL(timeout()),
                     this, SLOT(startFocusAnimation()));
    QObject::connect(scrollSelectTimer, SIGNAL(timeout()),
                     this, SLOT(scrollSelectSlot()));

    delaySelection.setSingleShot(true);
    QObject::connect(&delaySelection, SIGNAL(timeout()),
                     this, SLOT(setSelection()));

    showMovedToolbar.setSingleShot(true);
    QObject::connect(&showMovedToolbar, SIGNAL(timeout()),
                     this, SLOT(restoreEditorToolbar()), Qt::UniqueConnection);
}


/*!
 * Destructor
 */
MTextEditViewPrivate::~MTextEditViewPrivate()
{
    delete maskedTextDocument;
}

QTextDocument *MTextEditViewPrivate::document() const
{
    return controller->document();
}

/*!
 * \brief Returns cursor position from a mouse position
 * \param event mouse event information (like position)
 * \return cursor position as characters from start of document
 */
int MTextEditViewPrivate::cursorPosition(QGraphicsSceneMouseEvent *event)
{
    QPointF hitPoint = event->pos();
    return cursorPosition(hitPoint);
}


/*!
 * \brief Returns cursor position from a mouse position
 * \param hitPoint mouse position
 * \return cursor position as characters from start of document
 */
int MTextEditViewPrivate::cursorPosition(const QPointF &point, Qt::HitTestAccuracy accuracy)
{
    Q_Q(MTextEditView);

    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(q->style().operator ->());

    // adjust widget position to textdocument position
    QPointF hitPoint = point;
    hitPoint.rx() += hscroll;
    hitPoint.ry() += vscroll;
    hitPoint.rx() -= isLayoutLeftToRight() ? s->paddingLeft() : s->paddingRight();
    hitPoint.ry() -= s->paddingTop();

    // limit the area inside the text document.
    // otherwise up from it equals start index, and below end index.
    if (hitPoint.x() < 0) {
        hitPoint.setX(0);

    } else if (hitPoint.x() > activeDocumentSize().width()) {
        hitPoint.setX(activeDocumentSize().width());
    }

    // on a few lowest pixels, the hitTest returns position on the end.
    // a limitor before the real lower edge is used to avoid this.
    const int MaxYLimitor = 4;
    int maxY = activeDocumentSize().height()
               - MaxYLimitor - 2 * activeDocument()->documentMargin();

    if (hitPoint.y() < activeDocument()->documentMargin()) {
        hitPoint.setY(activeDocument()->documentMargin());

    } else if (hitPoint.y() > maxY) {
        hitPoint.setY(maxY);
    }

    return activeDocument()->documentLayout()->hitTest(hitPoint, accuracy);
}

/*!
 * \brief Scrolling slot, do the auto scrolling when cursor moves to the scroll area
 */
void MTextEditViewPrivate::scrolling()
{
    Q_Q(MTextEditView);

    if (q->model()->textInteractionFlags() == Qt::NoTextInteraction) {
        scrollTimer->stop();
        return;
    }

    const int maxScroll(activeDocumentSize().width() - activeDocumentTextWidth());

    hscroll += scrollSpeedVertical;

    // test if we reach scrolling limits and stop if it happens
    if (hscroll < 0) {
        hscroll = 0;
        scrollTimer->stop();

    } else if (hscroll > maxScroll) {
        hscroll = maxScroll;
        scrollTimer->stop();
    }

    // update cursor to the edge of visible content
    // NOTE: doesn't limit to text document
    int cursorIndex = cursorPosition(mouseTarget);

    if (q->model()->edit() == MTextEditModel::EditModeBasic) {
        controller->setCursorPosition(cursorIndex);

    } else if (q->model()->edit() == MTextEditModel::EditModeSelect
               && !selectionHandleIsPressed) {
        controller->setSelection(startCursorPos, cursorIndex - startCursorPos, true);
    }

    q->doUpdate(); // need to redraw, widget supposedly changed.
    emit q->contentScrolled();
}

//! \brief Overloaded method introduced for convenience.
void MTextEditViewPrivate::scrollingTestAndStart(QGraphicsSceneMouseEvent *event)
{
    scrollingTestAndStart(event->pos());
}

/*!
 * \brief checks if event is on text entry edge and sets scrolling speed accordingly.
 * actual scrolling happens in scrolling()
 */
void MTextEditViewPrivate::scrollingTestAndStart(const QPointF &pos)
{
    Q_Q(MTextEditView);
    QRectF rect = q->geometry();
    int paddingLeft, paddingRight;

    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(q->style().operator ->());

    if (isLayoutLeftToRight()) {
        paddingLeft = s->paddingLeft();
        paddingRight = s->paddingRight();
    } else {
        paddingLeft = s->paddingRight();
        paddingRight = s->paddingLeft();
    }

    // event inside scrolling margin creates constant speed scrolling.
    // this could be changed to determine some scrolling speed depending on the position.
    if (q->model()->textInteractionFlags() == Qt::NoTextInteraction) {
        scrollSpeedVertical = 0;
    } else if (pos.x() < (ScrollMargin + paddingLeft) && hscroll > 0) {
        scrollSpeedVertical = -ScrollStep;
    } else if (pos.x() > (rect.width() - (ScrollMargin + paddingRight))
               && hscroll < (activeDocumentSize().width() - activeDocumentTextWidth())) {
        scrollSpeedVertical = ScrollStep;
    } else {
        scrollSpeedVertical = 0;
    }

    if (scrollSpeedVertical != 0) {
        // will do actual scrolling on timer interval
        if (scrollTimer->isActive() == false) {
            scrollTimer->start(ScrollingTimeDuration);
        }
    } else {
        scrollTimer->stop();
    }
}




//! validates hscroll and vscroll values so cursor stays visible
void MTextEditViewPrivate::checkScroll()
{
    Q_Q(MTextEditView);

    // Selection doesn't currently change scrolling.
    // rationale: user initiated selection can only happen on visible part
    // and change in scrolling here would conflict with scrolling on the edges
    if (q->model()->edit() == MTextEditModel::EditModeSelect) {
        return;
    }

    // TODO: vscroll possibly at some point

    qreal currentX = cursorX();
    int cWidth = cursorWidth();
    bool scrolled = false;
    // 1. size().width is in fact not less than availableWidth, even when actual text is smaller
    // I do not know why, but this allows to align RTL text
    // 2. if cursor is in the very end of the text or at start of RTL text, it takes more
    //    space to be drawn than size().width(). We work around it by using extended width
    //    for document and document's textWidth, see activeDocumentSize() and activeDocumentTextWidth().
    //    Now we are sure that whole cursor always is inside of (0, contentsWidth)
    int contentsWidth = activeDocumentSize().width();
    int availableWidth = activeDocumentTextWidth();

    if (controller->lineMode() != MTextEditModel::MultiLine // multiline text is not scrolled as it always wraps
        && availableWidth > 0 // just safety, inherited from older code
        && contentsWidth > availableWidth // text is longer than the edit field, so it may need scrolling
        ) {
        // the first 2 possible violations are mutually exclusive
        if (currentX < hscroll) {
            // cursor went too far to left
            hscroll = currentX;
            hscrollSnapRight = false;
            scrolled = true;
        } else if (currentX + cWidth > availableWidth + hscroll) {
            // cursor went too far to right (considering its width)
            hscroll = currentX + cWidth - availableWidth;
            hscrollSnapRight = true;
            scrolled = true;
        }
        // now cursor is visible
        if (contentsWidth < availableWidth + hscroll) {
            // there is a gap on right
            // nothing of contents is hidden by this move, including the cursor
            hscroll = contentsWidth - availableWidth;
            hscrollSnapRight = true;
            scrolled = true;
        }
        if (!scrolled
            && hscrollSnapRight
            && contentsWidth > availableWidth + hscroll // right end of text became hidden
            && currentX >= contentsWidth - availableWidth) { // the cursor is close enough to the right end not to be hidden
            // scrolling is snapped to right and it is possible to keep it locked
            hscroll = contentsWidth - availableWidth;
            scrolled = true;
        }
    } else if (hscroll != 0) {
        hscroll = 0;
        hscrollSnapRight = false;
        scrolled = true;
    }

    if (scrolled) {
        q->doUpdate();
    }
}

QSizeF MTextEditViewPrivate::activeDocumentSize() const
{
    return activeDocument()->size() + QSizeF(cursorWidth(), 0);
}

QSizeF MTextEditViewPrivate::documentSize() const
{
    // Report larger width by the amount of cursorWidth().
    // Reserve this extra space as a workaround so that QTextDocumentLayout
    // does not draw cursor outside its area when RTL + cursor at position/index 0.
    return document()->size() + QSizeF(cursorWidth(), 0);
}

qreal MTextEditViewPrivate::activeDocumentTextWidth() const
{
    return activeDocument()->textWidth() + cursorWidth();
}

qreal MTextEditViewPrivate::documentTextWidth() const
{
    // Report larger width by the amount of cursorWidth().
    return document()->textWidth() + cursorWidth();
}

qreal MTextEditViewPrivate::realDocumentTextWidth(qreal width) const
{
    Q_Q(const MTextEditView);
    if (width < 0.0) {
        width = q->size().width();
    }
    // Real document textWidth is one cursorWidth less than the available space for text.
    return width - q->style()->paddingLeft() - q->style()->paddingRight() - cursorWidth();
}

//! sets the mouse target point making sure it's inside the widget
void MTextEditViewPrivate::setMouseTarget(const QPointF &point)
{
    Q_Q(MTextEditView);

    mouseTarget.setX(qBound<qreal>(0.0, point.x(), q->geometry().width()));
    mouseTarget.setY(qBound<qreal>(0.0, point.y(), q->geometry().height()));
}


bool MTextEditViewPrivate::isLayoutLeftToRight() const
{
    if (controller->layoutDirection() == Qt::LayoutDirectionAuto) {
        return qApp->isLeftToRight();
    }

    return controller->layoutDirection() == Qt::LeftToRight;
}


QAbstractTextDocumentLayout::PaintContext MTextEditViewPrivate::paintContext() const
{
    Q_Q(const MTextEditView);

    QAbstractTextDocumentLayout::PaintContext paintContext;

    foreach (MTextEditFormatRange formatRange, q->model()->additionalFormats()) {
        QTextCursor cursor(activeDocument());
        cursor.setPosition(formatRange.start, QTextCursor::MoveAnchor);
        cursor.setPosition(formatRange.start + formatRange.length, QTextCursor::KeepAnchor);
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = cursor;
        selection.format = formatRange.format;
        paintContext.selections.append(selection);
    }

    paintContext.palette.setColor(QPalette::Text, q->style()->textColor());
    QTextCursor cursor = controller->textCursor();
    MTextEditModel::EchoMode echoMode = q->model()->echo();

    if (echoMode == MTextEditModel::NoEcho) {
        if (focused == true) {
            paintContext.cursorPosition = 0;
        }

    } else if (q->model()->edit() == MTextEditModel::EditModeSelect) {
        // add selection formatting
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = cursor;
        selection.format = selectionFormat;
        paintContext.selections.append(selection);

    } else if (focused == true) {
        if (controller->isReadOnly()) {
            // dont't draw cursor if it is readOnly
            paintContext.cursorPosition = -1;
        } else if ( q->model()->edit() == MTextEditModel::EditModeBasic) {
            paintContext.cursorPosition = cursor.position();
        } else if (q->model()->edit() == MTextEditModel::EditModeActive
                   && q->model()->preeditCursor() >= 0) {
            // the absolute position for preedit cursor is the relative
            // preeditCursor + preedit start (selection start).
            paintContext.cursorPosition = cursor.selectionStart() + q->model()->preeditCursor();

        }
    }

    return paintContext;
}


// returns cursor's x position on textdocument
qreal MTextEditViewPrivate::cursorX() const
{
    Q_Q(const MTextEditView);

    qreal currentX = 0.0;

    if (q->model()->echo() == MTextEditModel::NoEcho) {
        return currentX;
    }

    QTextCursor activeCursor(activeDocument());
    activeCursor.setPosition(controller->textCursor().position());

    QTextBlock currentBlock = activeCursor.block();
    if (!currentBlock.isValid()) {
        return currentX;
    }

    QTextLayout *layout = currentBlock.layout();
    int relativePos = activeCursor.position() - currentBlock.position();
    QTextLine line = layout->lineForTextPosition(relativePos);

    if (!line.isValid())
        line = layout->lineAt(0);

    currentX = line.cursorToX(relativePos);

    return currentX;
}


void MTextEditViewPrivate::initMaskedDocument()
{
    Q_Q(MTextEditView);

    maskTimer->stop();

    int textLength = 0;

    if (q->model()->echo() != MTextEditModel::NoEcho) {
        QString text = document()->toPlainText();
        textLength = text.length();
    }

    QString maskedText(textLength, maskCharacter);

    if (maskedTextDocument == 0) {
        maskedTextDocument = new QTextDocument(maskedText, this);
    } else {
        maskedTextDocument->setPlainText(maskedText);
    }

    // copy the settings
    maskedTextDocument->setDocumentMargin(document()->documentMargin());
    maskedTextDocument->setDefaultFont(document()->defaultFont());
    maskedTextDocument->setTextWidth(documentTextWidth());
    QVariant cursorWidth = document()->documentLayout()->property(CursorWidthProperty);
    maskedTextDocument->documentLayout()->setProperty(CursorWidthProperty, cursorWidth);

    // no word wrapping in masked mode
    QTextOption option = document()->defaultTextOption();
    option.setWrapMode(QTextOption::NoWrap);
    maskedTextDocument->setDefaultTextOption(option);
}


// returns the textdocument that is currently used for displaying text
QTextDocument *MTextEditViewPrivate::activeDocument() const
{
    Q_Q(const MTextEditView);

    MTextEditModel::EchoMode echoMode = q->model()->echo();

    if (echoMode == MTextEditModel::Normal ||
            (echoMode == MTextEditModel::PasswordEchoOnEdit && editActive == true) ||
            maskedTextDocument == 0) {
        return document();
    } else {
        return maskedTextDocument;
    }
}

void MTextEditViewPrivate::showEditorToolbar()
{
    Q_Q(const MTextEditView);
    if (!editorToolbar) {
        editorToolbar.reset(new MEditorToolbar(controller));
        const MTextEditStyle *textEditStyle = static_cast<const MTextEditStyle *>(q->style().operator ->());
        if (textEditStyle) {
            editorToolbar->setStyleName(textEditStyle->toolbarStyleName());
        }
        foreach (QAction *action, controller->actions()) {
            if (!action->property(NoCloseToolbarOnTriggerProperty).toBool()) {
                connect(action, SIGNAL(triggered()), editorToolbar.data(), SLOT(disappear()));
            }
            editorToolbar->addAction(action);
        }

        QObject::connect(controller, SIGNAL(textChanged()),
                         editorToolbar.data(), SLOT(disappear()));
        QObject::connect(editorToolbar.data(), SIGNAL(sizeChanged()),
                         this, SLOT(updateEditorToolbarPosition()));

        MTextEditPrivate &controllerD(*static_cast<MTextEditPrivate *>(controller->d_func()));
        connect(&controllerD.signalEmitter, SIGNAL(scenePositionChanged()),
                this, SLOT(onEditorMoved()), Qt::UniqueConnection);
    }

    editorToolbar->setAutoHideEnabled(!controller->hasSelectedText());
    editorToolbar->appear();
    updateEditorToolbarPosition();
    lastScenePos = controller->scenePos();
    movementTime.start();
}


void MTextEditViewPrivate::hideEditorToolbar()
{
    MTextEditPrivate &controllerD(*static_cast<MTextEditPrivate *>(controller->d_func()));
    disconnect(&controllerD.signalEmitter, SIGNAL(scenePositionChanged()),
               this, SLOT(onEditorMoved()));
    showMovedToolbar.stop();

    if (!editorToolbar) {
        return;
    }

    editorToolbar->disappear();
    editorToolbar.reset();
}

void MTextEditViewPrivate::hideEditorToolbarTemporarily()
{
    if (editorToolbar && editorToolbar->isAppeared()) {
        editorToolbar->disappear();
    }
}

void MTextEditViewPrivate::restoreEditorToolbar()
{
    // If editorToolbar exists and is disappeared then assume it was hidden
    // only temporarily. Restore it.
    if (editorToolbar && !editorToolbar->isAppeared()) {
        editorToolbar->setAutoHideEnabled(!controller->hasSelectedText());
        editorToolbar->appear();
        updateEditorToolbarPosition();
    }
}

qreal MTextEditViewPrivate::promptOpacity() const
{
    return currentPromptOpacity;
}

void MTextEditViewPrivate::setPromptOpacity(qreal opacity)
{
    Q_Q(MTextEditView);

    currentPromptOpacity = opacity;
    q->doUpdate();
}

QRectF MTextEditViewPrivate::clippingRect()
{
    Q_Q(MTextEditView);

    qreal paddingLeft, paddingRight, marginLeft, marginRight;

    if (isLayoutLeftToRight()) {
        paddingLeft = q->style()->paddingLeft();
        marginLeft = q->marginLeft();
        paddingRight = q->style()->paddingRight();
        marginRight = q->marginRight();
    } else {
        paddingLeft = q->style()->paddingRight();
        marginLeft = q->marginRight();
        paddingRight = q->style()->paddingLeft();
        marginRight = q->marginLeft();
    }
    // as sanity check use style clipping only if it's smaller than real padding
    const int leftClipping = qMin<int>(q->style()->textClippingLeft(), paddingLeft)
                             + marginLeft;
    const int rightClipping = qMin<int>(q->style()->textClippingRight(), paddingRight)
                              + marginRight;
    const int topClipping = qMin<int>(q->style()->textClippingTop(), q->style()->paddingTop())
                            + q->marginTop();
    const int bottomClipping = qMin<int>(q->style()->textClippingBottom(), q->style()->paddingBottom())
                               + q->marginBottom();

    // rectangle which could be used for text drawing
    const QRectF clipping(controller->boundingRect().adjusted(leftClipping,
                                                              topClipping,
                                                              -rightClipping,
                                                              -bottomClipping));

    return clipping;
}

void MTextEditViewPrivate::showMagnifier()
{
    Q_Q(MTextEditView);

    // Never show editor at the same time with magnifier.
    // There is separate logic for showing the toolbar again
    // once magnifier is gone on mouse release.
    hideEditorToolbar();

    if (controller->sceneManager()) {
        // Prevent relocations when moving magnifier.
        QObject::disconnect(static_cast<const QObject *>(controller), SIGNAL(cursorPositionChanged()),
                            static_cast<const QObject *>(controller->sceneManager()), SLOT(ensureCursorVisible()));

        QObject::connect(controller->sceneManager(), SIGNAL(orientationChanged(const M::Orientation  &)),
                         this, SLOT(makeMagnifierDisappear()), Qt::UniqueConnection);
        QObject::connect(controller->sceneManager(), SIGNAL(orientationChangeFinished(const M::Orientation  &)),
                         this, SLOT(makeMagnifierAppear()), Qt::UniqueConnection);
    }

    // Make fake mouse event to update cursor position.
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
    mouseEvent.setPos(mouseTarget);
    q->updateCursorPosition(&mouseEvent, false);

    if (!magnifier) {
        magnifier.reset(new MTextMagnifier(*controller,
                                           cursorRect().size()));
    }

    QObject::connect(controller, SIGNAL(cursorPositionChanged()),
                     this, SLOT(playSelectionAndMagnifierFeedback()),
                     Qt::UniqueConnection);

    updateMagnifierPosition();
    magnifier->appear();
    MTextEditPrivate *textWidgetPtr = static_cast<MTextEditPrivate *>(controller->d_func());
    if (textWidgetPtr) {
        connect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                this, SLOT(updateMagnifierPosition()), Qt::UniqueConnection);
        connect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                this, SLOT(onScenePositionChanged()), Qt::UniqueConnection);
    }
}

void MTextEditViewPrivate::hideMagnifier()
{
    if (controller->sceneManager()) {
        QObject::connect(controller, SIGNAL(cursorPositionChanged()),
                         controller->sceneManager(), SLOT(ensureCursorVisible()),
                         Qt::UniqueConnection);
        QObject::disconnect(controller->sceneManager(),
                            SIGNAL(orientationChanged(const M::Orientation  &)),
                            this, SIGNAL(makeMagnifierDisappear()));
        QObject::disconnect(controller->sceneManager(),
                            SIGNAL(orientationChangeFinished(const M::Orientation  &)),
                            this, SIGNAL(makeMagnifierAppear()));
    }

    if (magnifier) {
        MTextEditPrivate *textWidgetPtr = static_cast<MTextEditPrivate *>(controller->d_func());
        if (textWidgetPtr) {
            disconnect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                       this, SLOT(updateMagnifierPosition()));
            disconnect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                       this, SLOT(onScenePositionChanged()));
        }

        viewScrolled = false;

        magnifier->disappear(MTextMagnifier::DestroyWhenDone);
        (void)magnifier.take(); // Set pointer to null without destroying.
    }

    QObject::disconnect(controller, SIGNAL(cursorPositionChanged()),
                        this, SLOT(playSelectionAndMagnifierFeedback()));
}

void MTextEditViewPrivate::showSelectionMagnifier()
{
    if (!magnifier) {
        magnifier.reset(new MTextMagnifier(*controller,
                                           cursorRect().size()));
    }

    updateMagnifierPosition();

    QObject::connect(controller, SIGNAL(selectionChanged()),
                     this, SLOT(playSelectionAndMagnifierFeedback()),
                     Qt::UniqueConnection);

    MTextEditPrivate *textWidgetPtr = static_cast<MTextEditPrivate *>(controller->d_func());
    if (textWidgetPtr) {
        connect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                this, SLOT(onScenePositionChanged()), Qt::UniqueConnection);
    }

    magnifier->appear();
}

void MTextEditViewPrivate::hideSelectionMagnifier()
{
    if (magnifier) {
        magnifier->disappear(MTextMagnifier::DestroyWhenDone);
        (void)magnifier.take(); // Set pointer to null without destroying.
    }

    MTextEditPrivate *textWidgetPtr
        = static_cast<MTextEditPrivate *>(controller->d_func());
    if (textWidgetPtr) {
        QObject::disconnect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                            this, SLOT(onScenePositionChanged()));
    }

    QObject::disconnect(controller, SIGNAL(selectionChanged()),
                        this, SLOT(playSelectionAndMagnifierFeedback()));

    viewScrolled = false;
}

void MTextEditViewPrivate::showSelectionOverlay()
{
    if (activeDocument() == maskedTextDocument)
        return;

    Q_Q(MTextEditView);
    if (selectionOverlay.isNull() && controller->sceneManager()) {
        selectionOverlay.reset(new MTextSelectionOverlay(controller, q));


        QObject::connect(selectionOverlay.data(), SIGNAL(visibleChanged()),
                         this, SLOT(onSelectionOverlayVisibleChanged()));
        QObject::connect(selectionOverlay.data(), SIGNAL(selectionHandleMoved(QPointF)),
                         this, SLOT(onSelectionHandleMoved(QPointF)));

        QObject::connect(selectionOverlay.data(), SIGNAL(selectionHandlePressed(const QPointF)),
                         this, SLOT(onSelectionHandlePressed(const QPointF)));
        QObject::connect(selectionOverlay.data(), SIGNAL(selectionHandleReleased()),
                         this, SLOT(onSelectionHandleReleased()));

        QObject::connect(controller->sceneManager(),
                         SIGNAL(orientationChanged(const M::Orientation &)),
                         this, SLOT(hideSelectionOverlayTemporarily()));
        QObject::connect(controller->sceneManager(),
                         SIGNAL(orientationChangeFinished(const M::Orientation &)),
                         this, SLOT(restoreSelectionOverlay()));
    }

    if (!selectionOverlay.isNull()) {
        MTextEditPrivate *textWidgetPtr
            = static_cast<MTextEditPrivate *>(controller->d_func());
        if (textWidgetPtr) {
            QObject::connect(&textWidgetPtr->signalEmitter,
                             SIGNAL(scenePositionChanged()),
                             this,
                             SLOT(mapSelectionChange()),
                             Qt::UniqueConnection);
        }

        QObject::connect(controller, SIGNAL(selectionChanged()),
                         this, SLOT(mapSelectionChange()),
                         Qt::UniqueConnection);
        QObject::connect(MInputMethodState::instance(),
                         SIGNAL(inputMethodAreaChanged(const QRect &)),
                         this, SLOT(mapSelectionChange()),
                         Qt::UniqueConnection);

        mapSelectionChange();
        q->model()->setIsSelecting(true);
    }
}

// notifies this and the possible derived classes about an update
void MTextEditView::doUpdate()
{
    update();
}

const QPointF &MTextEditView::mouseTarget() const
{
    Q_D(const MTextEditView);
    return d->mouseTarget;
}

int MTextEditView::cursorPosition(const QPointF &hitPoint)
{
    Q_D(MTextEditView);
    return d->cursorPosition(hitPoint, Qt::FuzzyHit);
}

int MTextEditView::cursorPosition(const QPointF &hitPoint, Qt::HitTestAccuracy accuracy)
{
    Q_D(MTextEditView);
    return d->cursorPosition(hitPoint, accuracy);
}

void MTextEditViewPrivate::hideUnmaskedText()
{
    if (unmaskPosition >= 0 && maskedTextDocument) {
        // remove unmasked characters
        QTextCursor maskCursor(maskedTextDocument);
        maskCursor.setPosition(unmaskPosition);
        maskCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, unmaskLength);
        maskCursor.removeSelectedText();

        // insert mask characters
        QString maskedText(unmaskLength, maskCharacter);
        maskCursor.insertText(maskedText);

        unmaskPosition = -1;
        unmaskLength = 0;

        Q_Q(MTextEditView);
        q->doUpdate();
    }
}


/*! Updates cached size and calls updateGeometry() for the view
 */
void MTextEditViewPrivate::checkSize()
{
    Q_Q(MTextEditView);
    // note: as of Qt documentation, the size of newly created empty document is
    // configuration dependent. in practice it seems to match the selected font size.
    // possibly as a side effect of setDefaultFont()
    qreal docHeight = activeDocumentSize().height();

    if (documentHeight != docHeight) {
        documentHeight = docHeight;
        q->updateGeometry();
    }
}

// handles text document updates
void MTextEditViewPrivate::handleDocumentUpdate(int position, int charsRemoved, int charsAdded)
{
    Q_Q(MTextEditView);

    if (focused) {
        editActive = true; // PasswordEchoOnEdit mode only becomes visible after doing editing
    }

    const bool hasText = !document()->isEmpty();
    const bool hasPrompt = !q->model()->prompt().isEmpty();

    if (isPromptVisible && hasText && hasPrompt) {
        isPromptVisible = false;
        promptShowHideAnimation.setStartValue(currentPromptOpacity);
        promptShowHideAnimation.setDirection(QAbstractAnimation::Forward);
        promptShowHideAnimation.start();
        promptFocusAnimation.stop();
        focusAnimationDelay->stop();
    } else if (!isPromptVisible && !hasText && hasPrompt) {
        isPromptVisible = true;
        promptShowHideAnimation.setStartValue(q->style()->focusedPromptOpacity());
        promptShowHideAnimation.setDirection(QAbstractAnimation::Backward);
        promptShowHideAnimation.start();
        promptFocusAnimation.stop();
        focusAnimationDelay->stop();
    }

    if (q->model()->echo() == MTextEditModel::NoEcho) {
        // early bail out, nothing is really done on no echo mode
        return;
    }

    // for echo modes using masking, we should update masked content
    if (maskedTextDocument != 0) {
        // first hide already visible text
        hideUnmaskedText();

        QTextCursor maskCursor = QTextCursor(maskedTextDocument);
        maskCursor.setPosition(position);
        // remove the same amount of chars from masked document
        maskCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, charsRemoved);
        maskCursor.removeSelectedText();

        // append new chars as unmasked or masked
        QTextCursor newTextCursor(controller->textCursor());
        newTextCursor.setPosition(position);
        newTextCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, charsAdded);
        QString newText = newTextCursor.selectedText();

        // PasswordEchoOnEdit does immediate masking
        bool delayedMask = (q->model()->echo() == MTextEditModel::Password);

        // ugly hack to avoid masking when app developer uses setText()
        MTextEditPrivate *textWidgetPtr = static_cast<MTextEditPrivate *>(controller->d_func());
        if (textWidgetPtr && textWidgetPtr->programmaticalDocumentChange) {
            delayedMask = false;
        }

        if (delayedMask) {
            maskCursor.insertText(newText);
            unmaskPosition = position;
            unmaskLength = newText.length();
            maskTimer->start();

        } else {
            QString maskedText(newText.length(), maskCharacter);
            maskCursor.insertText(maskedText);
        }
    }

    q->doUpdate();
}

void MTextEditViewPrivate::handleDocumentUpdated()
{
    qreal currentWidth = activeDocument()->idealWidth();
    qreal textWidth = activeDocumentTextWidth();

    // Check the need to scroll when current text width is so wide that cursor
    // would be partially visible or hidden but not wide enough to trigger
    // QAbstractTextDocumentLayout size change signal. This is a special
    // case since QAbstractTextDocumentLayout size change signal doesn't
    // take in to account the width of the cursor.
    if (currentWidth > (textWidth - cursorWidth()) && currentWidth <= textWidth) {
        checkScroll();
    }

    // Update masked document's text direction.
    if (maskedTextDocument) {
        Qt::LayoutDirection dir = document()->defaultTextOption().textDirection();
        QTextOption option = maskedTextDocument->defaultTextOption();
        option.setTextDirection(dir);
        maskedTextDocument->setDefaultTextOption(option);
    }

    // New selection has to be notified because both changed text and changed formatting may
    // change starting and ending position of selection.
    if (!selectionOverlay.isNull()) {
        mapSelectionChange();
    }
}

// handles size change signals from QAbstractTextDocumentLayout
void MTextEditViewPrivate::handleDocumentSizeChange(const QSizeF &newSize)
{
    Q_UNUSED(newSize);
    // do the checks separately and ignore new size, we might be using a different active document.
    // having the size change signal from main document should be enough anyway.
    checkSize();
    checkScroll();
}

void MTextEditViewPrivate::playSelectionAndMagnifierFeedback()
{
    Q_Q(const MTextEditView);

    static bool holdFeedback = false;

    const int moveLimit = MDeviceProfile::instance()->mmToPixels(q->style()->feedbackSpeedLimit())
                          * timeOnMove.restart();
    // approximates sqrt(dx^2 + dy^2) * 1000
    const int moveAmount = (qAbs(previousPosition.x() - currentPosition.x())
                            + qAbs(previousPosition.y() - currentPosition.y())) * 700;

    // Play feedback if:
    // - We are not scrolling (vertically or horizontally) and
    // - Enough time has passed since scrolling and
    // - Finger is moving slow enough and
    // - Enough time has elapsed since last feedback
    if (viewScrolled || previousHorizontalScroll != hscroll) {
        timeOnFeedback.start();
        viewScrolled = false;
        previousHorizontalScroll = hscroll;
        holdFeedback = true;
    } else if (moveAmount < moveLimit && timeOnFeedback.isValid()
        && timeOnFeedback.elapsed() > q->style()->minimumFeedbackInterval()
        && (!holdFeedback || (timeOnFeedback.elapsed() > HoldFeedbackTime))) {
        if (magnifier) {
            // Cursor moved while text magnifier is up
            q->style()->magnifierCursorMoveFeedback().play();
            timeOnFeedback.start();
            holdFeedback = false;
        } else {
            // Selection changed
            const int selectedAmount = qAbs(previousSelectionCursorPosition
                                            - controller->cursorPosition());
            bool playFeedback = false;

            // Don't play feedback for whitespace. If selected amount is greater
            // than WhitespaceCheckLimit, assume that selection contains something
            // other than whitespace.
            if (selectedAmount > WhitespaceCheckLimit) {
                playFeedback = true;
            } else {
                const int start = qMin(previousSelectionCursorPosition,
                                       controller->cursorPosition());
                for (int i = 0; i < selectedAmount; ++i) {
                    if (!activeDocument()->characterAt(start + i).isSpace()) {
                        playFeedback = true;
                        break;
                    }
                }
            }

            if (playFeedback) {
                q->style()->changeSelectionFeedback().play();
                timeOnFeedback.start();
                holdFeedback = false;
            }
        }
    }

    previousSelectionCursorPosition = controller->cursorPosition();
    previousPosition = currentPosition;
}

/*!
 * \brief Method to start text selection
 * \param event mouse event information (like position)
 */
void MTextEditViewPrivate::startSelection(QGraphicsSceneMouseEvent *event)
{
    if (activeDocument() != maskedTextDocument) {
        selecting = true;
        if (editorToolbar && editorToolbar->isAppeared()) {
            hideEditorToolbar();
        }

        QObject::connect(controller, SIGNAL(selectionChanged()),
                         this, SLOT(playSelectionAndMagnifierFeedback()),
                         Qt::UniqueConnection);

        MTextEditPrivate *textWidgetPtr = static_cast<MTextEditPrivate *>(controller->d_func());
        if (textWidgetPtr) {
            connect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                    this, SLOT(onScenePositionChanged()), Qt::UniqueConnection);
        }

        int currentPos = cursorPosition(event);
        startCursorPos = cursorPosition(event->buttonDownPos(Qt::LeftButton));
        previousSelectionCursorPosition = startCursorPos;
        controller->setSelection(startCursorPos, currentPos - startCursorPos, true);
    } else {
        // with masked input we just select all
        selecting = false;
        inAutoSelectionClick = true;
        controller->selectAll();
    }

    // selection won long press timer in mouse press contest, we should stop it
    longPressTimer->stop();
}

void MTextEditViewPrivate::stopSelection()
{
    scrollSelectTimer->stop();

    if (selecting) {
        MTextEditPrivate *textWidgetPtr = static_cast<MTextEditPrivate *>(controller->d_func());
        if (textWidgetPtr) {
            disconnect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                       this, SLOT(onScenePositionChanged()));
        }

        QObject::disconnect(controller, SIGNAL(selectionChanged()),
                            this, SLOT(playSelectionAndMagnifierFeedback()));

        viewScrolled = false;
        selecting = false;
    }
}

/*!
 * \brief Method to update text selection when mouse is moved
 * \param pos mouse position
 */
void MTextEditViewPrivate::updateSelection(const QPointF &pos)
{
    if (ignoreSelection)
        return;

    controller->setSelection(startCursorPos, cursorPosition(pos) - startCursorPos, true);
}


/*!
 * \brief This is the overloaded function created for the convenience
 * \param event mouse event information (like position)
 */
void MTextEditViewPrivate::updateSelection(QGraphicsSceneMouseEvent *event)
{
    updateSelection(event->pos());
}

/*!
 * \brief Checks if widget is scrolled while selection
 * If user start selection and then cause scrolling by coming too close to border
 * selection must watch scrolling. This makes flickering less and can prevent
 * a stuck if user does not move the finger.
 * Since currently there is no way to notify a widget that is is being scrolled,
 * the only way to know it is watch how mapping of last mouse position behaves.
 * if it resolved ot another position in text - widget is being scrolled.
 */
void MTextEditViewPrivate::scrollSelectSlot()
{
    QPointF pos = controller->mapFromScene(scrollSelectScenePosition);
    if (cursorPosition(pos) != controller->cursorPosition()) {
        updateSelection(pos);
        if (controller->sceneManager()) {
            controller->sceneManager()->ensureCursorVisible();
        }
    }
}

void MTextEditViewPrivate::onScenePositionChanged()
{
    viewScrolled = true;
}

void MTextEditViewPrivate::onEditorMoved()
{
    Q_Q(MTextEditView);
    panningStarted = true;
    if (editorToolbar) {
        const qreal elapsed = movementTime.restart();
        const qreal distance = (lastScenePos - controller->scenePos()).manhattanLength();
        const qreal speed = elapsed ? distance / elapsed : 0;
        if (speed > q->style()->toolbarSpeedLimit()) {
            showMovedToolbar.start();
            hideEditorToolbarTemporarily();
            lastScenePos = controller->scenePos();
        } else {
            updateEditorToolbarPosition();
        }
    }
}

void MTextEditViewPrivate::mapSelectionChange()
{
    Q_Q(MTextEditView);

    if (!controller->hasSelectedText() || !focused) {
        emit q->selectionChanged(0, QRect(), false, 0, QRect(), false);
        return;
    }

    QTextCursor cursor = controller->textCursor();
    const int anchor = cursor.anchor();
    const QRectF anchorRect = cursorRect(anchor, 1);
    const int position = cursor.position();
    const QRectF rect = cursorRect(position, 1);
    const QRectF clipping = clippingRect().intersected(visibleArea());
    const bool anchorVisible = clipping.contains(anchorRect);
    const bool cursorVisible = clipping.contains(rect);
    emit q->selectionChanged(anchor, anchorRect, anchorVisible, position, rect, cursorVisible);
}

void MTextEditViewPrivate::onSelectionHandleMoved(const QPointF &position)
{
    Q_Q(MTextEditView);

    QTextCursor cursor = controller->textCursor();

    // The position comes in in item coordinates. Translate it to view coordinates.
    const QPointF viewPos = fromItem(position);
    anchorPos = cursor.anchor();
    cursorPos = cursorPosition(viewPos);
    currentPosition = viewPos;

    qreal speed = 0;
    const qreal elapsed = handleTime.restart();

    if (!lastHandlePos.isNull()) {
        speed = (position - lastHandlePos).manhattanLength() / elapsed;
    }
    lastHandlePos = viewPos;
    if (speed < q->style()->selectionSpeedThreshold()) {
        setSelection();
        delaySelection.stop();
    } else {
        delaySelection.start();
    }

    setMouseTarget(viewPos);
    updateMagnifierPosition();
}

void MTextEditViewPrivate::onSelectionHandlePressed(const QPointF &position)
{
    Q_Q(MTextEditView);
    selectionHandleIsPressed = true;
    hideEditorToolbarTemporarily();
    q->model()->setInputContextUpdateEnabled(false);
    controller->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    const QPointF viewPos = fromItem(position);
    previousPosition = viewPos;
    currentPosition = viewPos;

    setMouseTarget(viewPos);
    showSelectionMagnifier();

    lastHandlePos = QPointF();
    handleTime.start();

    QTextCursor cursor = controller->textCursor();

    int pressedHandlePos = cursorPosition(viewPos);
    anchorPos = cursor.anchor();
    cursorPos = cursor.position();

    // make the active handle is always combined with cursor
    if (pressedHandlePos == anchorPos) {
        controller->setSelection(cursorPos, anchorPos - cursorPos, false);
        mapSelectionChange();
    }
}

void MTextEditViewPrivate::onSelectionHandleReleased()
{
    Q_Q(MTextEditView);
    selectionHandleIsPressed = false;
    scrollTimer->stop();
    restoreEditorToolbar();
    controller->setCacheMode(QGraphicsItem::NoCache);
    q->model()->setInputContextUpdateEnabled(true);
    hideSelectionMagnifier();
    mapSelectionChange();
}

void MTextEditViewPrivate::onSelectionOverlayVisibleChanged()
{
    Q_Q(MTextEditView);
    if (!selectionOverlay.isNull() && !selectionOverlay.data()->isVisible()) {
        QObject::disconnect(controller, SIGNAL(selectionChanged()),
                            this, SLOT(mapSelectionChange()));
        QObject::disconnect(MInputMethodState::instance(),
                         SIGNAL(inputMethodAreaChanged(const QRect &)),
                         this, SLOT(mapSelectionChange()));

        MTextEditPrivate *textWidgetPtr
            = static_cast<MTextEditPrivate *>(controller->d_func());
        if (textWidgetPtr) {
            QObject::disconnect(&textWidgetPtr->signalEmitter,
                                SIGNAL(scenePositionChanged()),
                                this,
                                SLOT(mapSelectionChange()));
        }
        q->model()->setIsSelecting(false);
    }
}

QPointF MTextEditViewPrivate::fromItem(const QPointF &point) const
{
    Q_Q(const MTextEditView);
    // Translate item point to view coordinates.
    const qreal leftMargin = isLayoutLeftToRight() ? q->marginLeft() : q->marginRight();
    return (point - QPointF(leftMargin, q->marginTop()));
}

/*!
 * \brief Method to handle mouse movement
 * \param event mouse event information (like position)
 */
void MTextEditViewPrivate::checkStartOfSelection(QGraphicsSceneMouseEvent *event)
{
    // sanity check, should only happen when left button is held down
    if ((event->buttons() & Qt::LeftButton) == 0) {
        return;
    }

    if (ignoreSelection)
        return;

    qreal dx = qAbs(event->buttonDownPos(Qt::LeftButton).x() - event->pos().x());
    qreal dy = qAbs(event->buttonDownPos(Qt::LeftButton).y() - event->pos().y());

    // approximates sqrt(dx^2 + dy^2)
    if (0.7f *(dx + dy) < selectionThreshold)
        return;

    // if movement is more vertical than horizontal we handle this movement as scrolling
    if (dx < dy) {
        ignoreSelection = true;
        return;
    }

    startSelection(event);
}


QRect MTextEditViewPrivate::preeditRectangle() const
{
    QRect rect;

    if (controller->mode() == MTextEditModel::EditModeActive) {

        // Find QTextLine for the current selection.
        QTextCursor textCursor = controller->textCursor();
        QTextBlock block = textCursor.block();
        QTextLayout *layout = block.layout();

        int start = textCursor.selectionStart() - block.position();
        int end = textCursor.selectionEnd() - block.position();
        QTextLine line = layout->lineForTextPosition(start);

        if (!line.isValid()) {
            line = layout->lineAt(0);
        }

        // Get local x coordinate for beginning and end of the selection.
        int x1 = line.cursorToX(start);
        int x2 = line.cursorToX(end);

        // Calculate offset from widget origin, having the current hscroll
        // and possibly positive line.y() value for multiline text edits.
        QPointF offset = layout->position() + QPointF(x1 - hscroll, line.y());

        rect.setRect(0, 0, x2 - x1, line.height());
        rect.translate(offset.toPoint());
    }

    return rect;
}


int MTextEditViewPrivate::cursorWidth() const
{
    int cursorWidth = 0;
    bool ok;

    cursorWidth = activeDocument()->documentLayout()->property(CursorWidthProperty).toInt(&ok);
    if (!ok)
        cursorWidth = 1;

    return cursorWidth;
}


QRect MTextEditViewPrivate::cursorRect(int position, int cursorWidth) const
{
    Q_Q(const MTextEditView);
    QRect rect;

    const QTextBlock currentBlock = document()->findBlock(position);
    if (!currentBlock.isValid())
        return rect;

    const QTextLayout *layout = currentBlock.layout();
    const QPointF layoutPos = document()->documentLayout()->blockBoundingRect(currentBlock).topLeft();
    int relativePos = position - currentBlock.position();
    QTextLine currentLine = layout->lineForTextPosition(relativePos);
    if (!currentLine.isValid())
        currentLine = layout->lineAt(0);

    if (!currentLine.isValid())
        return rect;

    int cursorHeight;
    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(q->style().operator ->());

    cursorHeight = currentLine.height();
    qreal x = currentLine.cursorToX(relativePos);

    const qreal horizontalPaddingAndMargin(
        isLayoutLeftToRight() ? (s->paddingLeft() + s->marginLeft())
        : (s->paddingRight() + s->marginRight()));
    rect = QRect(horizontalPaddingAndMargin + layoutPos.x() + x - hscroll,
                 s->paddingTop() + layoutPos.y() + currentLine.y() - vscroll,
                 cursorWidth, cursorHeight);

    return rect;
}

QRect MTextEditViewPrivate::cursorRect() const
{
    Q_Q(const MTextEditView);
    QRect rect;

    int position = controller->cursorPosition();
    if (q->model()->edit() == MTextEditModel::EditModeActive
        && q->model()->preeditCursor() >= 0) {
        position = controller->textCursor().selectionStart() + q->model()->preeditCursor();
    }

    const QTextBlock currentBlock = document()->findBlock(position);
    if (!currentBlock.isValid())
        return rect;

    int cursorWidth;
    bool ok = false;

    cursorWidth = document()->documentLayout()->property(CursorWidthProperty).toInt(&ok);
    if (!ok)
        cursorWidth = 1;
    //cursor occupies one space
    cursorWidth += QFontMetrics(currentBlock.layout()->font()).width(QLatin1Char(' '));

    return cursorRect(position, cursorWidth);
}

void MTextEditViewPrivate::updateMagnifierPosition()
{
    Q_Q(MTextEditView);

    if (magnifier) {
        // Magnified position will be the current mouse position but with
        // extra contraints applied:
        // - magnified position is inside document layout
        // - top and bottom margins are used so magnifier never shows content
        //   that is not interesting to user. The margin is half of current font's height.
        //   Font can change inside text edit but in most cases this is sufficient.
        // Magnifier itself makes sure it doesn't go out of screen.

        QPointF documentLayoutPos(0, q->style()->paddingTop());
        if (isLayoutLeftToRight()) {
            documentLayoutPos.setX(q->style()->paddingLeft());
        } else {
            documentLayoutPos.setX(q->style()->paddingRight());
        }

        const QFontMetrics fm(q->style()->font());
        const QRectF documentGeometry(documentLayoutPos,
                                      document()->documentLayout()->documentSize());
        const QPointF magnifierPos(qBound<qreal>(documentGeometry.left(),
                                                 mouseTarget.x(),
                                                 documentGeometry.right()),
                                   qBound<qreal>(documentGeometry.top() + fm.height() / 2.0f,
                                                 mouseTarget.y(),
                                                 documentGeometry.bottom() - fm.height() / 2.0f));

        magnifier->setMagnifiedPosition(magnifierPos);


        bool toRelocate = false;
        if (magnifier->isAppeared()) {
            // extend cursor rectangle to trigger automatic scrolling earlier
            const QRectF rect = cursorRect().adjusted(0,
                                                       -q->style()->scrollCursorMarginTop(),
                                                       0,
                                                       q->style()->scrollCursorMarginBottom());

            if (!visibleArea().contains(rect.toRect())) {
                toRelocate = true;
            }
        }
        if (toRelocate) {
            controller->sceneManager()->ensureCursorVisible();
        }
    }
}

void MTextEditViewPrivate::makeMagnifierDisappear()
{
    if (magnifier) {
        magnifier->disappear(MTextMagnifier::KeepWhenDone);
    }
}

void MTextEditViewPrivate::makeMagnifierAppear()
{
    if (magnifier) {
        magnifier->appear();
    }
}

QRect MTextEditViewPrivate::textRectangle(const int startPosition, const int endPosition)
{
    Q_Q(const MTextEditView);

    const QTextBlock startBlock(document()->findBlock(startPosition));
    const int blockRelativeStartPosition(startPosition - startBlock.position());
    const QTextLine firstLine(startBlock.layout()->lineForTextPosition(
                                  blockRelativeStartPosition));
    Q_ASSERT(firstLine.isValid());

    const qreal horizontalPaddingAndMargin(
        isLayoutLeftToRight() ? (q->style()->paddingLeft() + q->style()->marginLeft())
        : (q->style()->paddingRight() + q->style()->marginRight()));
    const QPointF commonOffset(document()->documentLayout()->blockBoundingRect(startBlock).topLeft()
                               + QPointF(horizontalPaddingAndMargin - hscroll,
                                         q->style()->paddingTop() - vscroll));
    const QPointF topLeft(QPointF(firstLine.cursorToX(blockRelativeStartPosition),
                                  firstLine.naturalTextRect().top())
                          + commonOffset);
    const QPointF bottomRight(QPointF(firstLine.cursorToX(endPosition
                                                          - startBlock.position()),
                                      firstLine.naturalTextRect().bottom())
                              + commonOffset);
    return QRectF(topLeft, bottomRight).toRect();
}

QRect MTextEditViewPrivate::selectionLineRectangle(bool first)
{
    const int selectionStart(controller->textCursor().selectionStart());
    const int selectionEnd(controller->textCursor().selectionEnd());
    QTextCursor cursor(controller->textCursor());
    if (first) {
        cursor.setPosition(selectionStart);
        cursor.movePosition(QTextCursor::EndOfLine);
        const bool multiLine(selectionEnd > cursor.position());
        if (!multiLine) {
            cursor.setPosition(selectionEnd);
        }
        return textRectangle(selectionStart, cursor.position());
    } else {
        cursor.setPosition(selectionEnd);
        cursor.movePosition(QTextCursor::StartOfLine);
        const bool multiLine(selectionStart < cursor.position());
        if (!multiLine) {
            cursor.setPosition(selectionStart);
        }
        return textRectangle(cursor.position(), selectionEnd);
    }
}

void MTextEditViewPrivate::playFocusAnimation(QAbstractAnimation::Direction direction,
                                              qreal endValue)
{
    Q_Q(MTextEditView);
    if (q->model()->prompt().isEmpty() || !document()->isEmpty()) {
        promptFocusAnimation.stop();
        return;
    }

    promptFocusAnimation.setDirection(direction);
    promptFocusAnimation.setEndValue(endValue);
    switch (promptFocusAnimation.state()) {
    case QAbstractAnimation::Stopped:
        focusAnimationDelay->start();
        break;
    case QAbstractAnimation::Paused:
        promptFocusAnimation.resume();
        break;
    case QAbstractAnimation::Running:
        break;
    }
}

void MTextEditViewPrivate::icUpdate()
{
    QInputContext *ic = qApp->inputContext();

    if (!ic || !controller->hasFocus()) {
        return;
    }

    ic->update();
}

void MTextEditViewPrivate::updateEditorToolbarPosition()
{
    if (!editorToolbar || !editorToolbar->isAppeared()) {
        return;
    }

    QRect firstRect;
    QRect secondRect;

    if (controller->hasSelectedText()) {
        firstRect = selectionLineRectangle(true);
        secondRect = selectionLineRectangle(false);
    } else {
        firstRect = cursorRect();
        secondRect = firstRect;
    }

    QRect targetRect(firstRect);
    int targetY(firstRect.top());
    MEditorToolbar::ToolbarPlacement placement(MEditorToolbar::AbovePointOfInterest);

    const QRect visibleRect = visibleArea();

    const QRect visibleSceneRect = controller->mapRectFromScene(
        QRectF(QPointF(), controller->sceneManager()->visibleSceneSize(M::Landscape))).toRect();

    const int topLimit = qMax<int>(visibleRect.top() - editorToolbar->size().height(), visibleSceneRect.top());

    if (targetY - editorToolbar->size().height() < topLimit) {
        if (secondRect.bottom() + editorToolbar->size().height() <= visibleRect.bottom()) {
            targetY = secondRect.bottom();
            targetRect = secondRect;
            placement = MEditorToolbar::BelowPointOfInterest;
        } else {
            targetY = topLimit + editorToolbar->size().height();
        }
    }

    targetRect.moveTop(visibleRect.top());
    const int targetX(visibleRect.intersected(targetRect).center().x());

    if (selectionOverlay) {
        editorToolbar->setForbiddenRegion(selectionOverlay->region());
    } else {
        editorToolbar->setForbiddenRegion(QRegion());
    }

    editorToolbar->setPosition(QPointF(targetX, targetY), placement);
}

void MTextEditViewPrivate::startFocusAnimation()
{
    Q_Q(MTextEditView);
    qreal targetOpacity;

    if (promptFocusAnimation.direction() == QAbstractAnimation::Forward) {
        targetOpacity = q->style()->focusedPromptOpacity();
    } else {
        targetOpacity = q->style()->unfocusedPromptOpacity();
    }

    if (currentPromptOpacity != targetOpacity) {
        promptFocusAnimation.start();
    }
}

QRect MTextEditViewPrivate::visibleArea() const
{
    QRect visibleRect = controller->mapRectFromScene(
        QRectF(QPointF(), controller->sceneManager()->visibleSceneSize(M::Landscape))).toRect();

    // Map input panel rectangle to local coordinates and remove it
    // from visible scene rectangle. It is assumed that input panel
    // is always at the bottom of the sreen.
    const QRect sipRect = MInputMethodState::instance()->inputMethodArea();
    if (!sipRect.isNull()) {
        const QRect mappedPanelRect(controller->mapRectFromScene(sipRect).toRect());

        visibleRect.setBottom(qMin<int>(mappedPanelRect.top(),
                                        visibleRect.bottom()));
    }

    // Map further restricting rectangles to local coordinates and clip
    // them with visible scene rectangle.
    QGraphicsWidget *widget = controller->parentWidget();
    while (widget) {
        QRect widgetClipRect;
        if (MSceneWindow *sceneWindow = qobject_cast<MSceneWindow *>(widget)) {
            if (sceneWindow->windowType() == MSceneWindow::ApplicationPage) {
                widgetClipRect = static_cast<MApplicationPage *>(sceneWindow)->exposedContentRect().toRect();
            }
        } else if (MPannableViewport *viewport = qobject_cast<MPannableViewport *>(widget)) {
            if (viewport->hasClipping()) {
                widgetClipRect = viewport->rect().toRect();
            }
        }

        if (!widgetClipRect.isNull()) {
            visibleRect &= controller->mapRectFromItem(widget, widgetClipRect).toRect();
        }

        widget = widget->parentWidget();
    }

    return visibleRect;
}

void MTextEditViewPrivate::hideSelectionOverlayTemporarily()
{
    // hide text selection overlay temporarily
    if (controller->hasSelectedText() && !selectionOverlay.isNull()) {
        QObject::disconnect(selectionOverlay.data(), SIGNAL(visibleChanged()),
                            this, SLOT(onSelectionOverlayVisibleChanged()));

        selectionOverlay->disappear();
    }
}

void MTextEditViewPrivate::restoreSelectionOverlay()
{
    // restore text selection overlay if it was temporarily hidden
    if (controller->hasSelectedText() && !selectionOverlay.isNull()) {
        showSelectionOverlay();
        QObject::connect(selectionOverlay.data(), SIGNAL(visibleChanged()),
                         this, SLOT(onSelectionOverlayVisibleChanged()));
        selectionOverlay->skipTransitions();
    }
}

void MTextEditViewPrivate::setSelection()
{
    controller->setSelection(anchorPos, cursorPos - anchorPos, false);
    scrollingTestAndStart(lastHandlePos);
}

//////////////////////
//// Actual class ////
//////////////////////


MTextEditView::MTextEditView(MTextEdit *controller)
    : MWidgetView(controller),
      d_ptr(new MTextEditViewPrivate(controller, this))
{
    QObject::connect(controller, SIGNAL(gainedFocus(Qt::FocusReason)),
                     SLOT(setFocused(Qt::FocusReason)));
    QObject::connect(controller, SIGNAL(lostFocus(Qt::FocusReason)),
                     SLOT(removeFocus(Qt::FocusReason)));
    QObject::connect(controller, SIGNAL(pasteFailed()),
                     SLOT(informPasteFailed()));
}

MTextEditView::~MTextEditView()
{
    delete d_ptr;
}

void MTextEditView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MTextEditView);
    int paddingLeft, paddingRight;

    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(style().operator ->());

    if (d->isLayoutLeftToRight()) {
        paddingLeft = s->paddingLeft();
        paddingRight = s->paddingRight();
    } else {
        paddingLeft = s->paddingRight();
        paddingRight = s->paddingLeft();
    }

    // mTimestamp("MTextEditView", QString("start text=%1").arg(d->document()->toPlainText()));

    // as sanity check use style clipping only if it's smaller than real padding
    int leftClipping = qMin<int>(s->textClippingLeft(), paddingLeft);
    int rightClipping = qMin<int>(s->textClippingRight(), paddingRight);
    int topClipping = qMin<int>(s->textClippingTop(), s->paddingTop());
    int bottomClipping = qMin<int>(s->textClippingBottom(), s->paddingBottom());

    // set clipping rectangle to draw text inside the border
    QRectF clipping(boundingRect().adjusted(leftClipping,
                                            topClipping,
                                            -rightClipping,
                                            -bottomClipping));
    clipping = clipping.intersected(option->exposedRect);

    // If text does not fit inside widget, it may have to be scrolled
    const qreal dx = -d->hscroll + paddingLeft;
    const qreal dy = -d->vscroll + s->paddingTop();

    bool clip = false;

    // with no content we show the prompt text
    bool showPrompt = d->isPromptVisible || d->promptShowHideAnimation.state() == QAbstractAnimation::Running;
    // prompt box is now directly got from document, so no need to check its clipping explicitly
    clip = isGreater(clipping.size(), d->activeDocumentSize());


    if (clip) {
        painter->save();
        painter->setClipRect(clipping, Qt::IntersectClip);
    }

    painter->translate(dx, dy);

    // draw actual text to the screen
    if (showPrompt
        && d->activeDocument()->blockCount() > 0 // this is in fact always so, even if text is empty
                                                 // checking just to prevent segfault by activeDocument()->begin()
        ) {
        QTextOption promptOptions = d->document()->defaultTextOption();
        // direction of document is used for internal purposes after fix of
        // NB#282904 - Arabic text starts from the left side of the text field when using only special characters or numerals
        // take the intended direction from the controller's property
        promptOptions.setTextDirection(d->controller->layoutDirection());
        if (promptOptions.textDirection() == Qt::LayoutDirectionAuto) {
            // QPainter::drawText() does not consider LayoutDirectionAuto properly
            // so we take the direction from contents ourself
            foreach (QChar c, model()->prompt()) {
                QChar::Direction d = c.direction();
                if (d == QChar::DirL) {
                    promptOptions.setTextDirection(Qt::LeftToRight);
                    break;
                } else if (d == QChar::DirR || d == QChar::DirAL) {
                    promptOptions.setTextDirection(Qt::RightToLeft);
                    break;
                }
            }
        }
        
        // The document layout is the most proper box for drawing the prompt
        QRectF promptClipping = d->activeDocument()->documentLayout()->blockBoundingRect(d->activeDocument()->begin());
        // there is a issue in Qt, most probably a bug, that is does not consider indentation of the first block
        // for counting the boundingRect position
        // the following is a workaround for this. It must produce correct result always
        QTextLayout *layout = d->activeDocument()->begin().layout();
        promptClipping.translate(layout->boundingRect().topLeft() - layout->position());

        qreal opacity = painter->opacity();
        painter->setOpacity(d->currentPromptOpacity);
        QPen savePen = painter->pen();
        painter->setPen(s->promptColor());
        QFont saveFont = painter->font();
        painter->setFont(s->promptFont());
        painter->drawText(promptClipping, model()->prompt(), promptOptions);
        painter->setPen(savePen);
        painter->setFont(saveFont);
        painter->setOpacity(opacity);
    }

    // normal painting, also draw cursor when prompt is visible
    QAbstractTextDocumentLayout::PaintContext paintContext = d->paintContext();
    paintContext.clip = option->exposedRect;
    paintContext.clip.translate(-dx, -dy);
    d->activeDocument()->documentLayout()->draw(painter, paintContext);

    if (clip)
        painter->restore();
    // mTimestamp("MTextEditView", QString("end text=%1").arg(d->document()->toPlainText()));
}

void MTextEditView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MTextEditView);
    qreal textWidth = d->realDocumentTextWidth();
    d->document()->setTextWidth(textWidth);

    if (d->maskedTextDocument != 0) {
        d->maskedTextDocument->setTextWidth(textWidth);
    }

    if (event->oldSize().isValid()) {
        qreal horizontalChange = event->newSize().width() - event->oldSize().width();

        if (horizontalChange > 0) {
            d->hscroll = qMax<qreal>(d->hscroll - horizontalChange, 0.0);
            doUpdate();
        }
    }

    d->checkScroll();

    if (d->controller->hasSelectedText()
        && !d->selectionOverlay.isNull()
        && d->selectionOverlay.data()->isVisible()) {
        d->mapSelectionChange();
    }
}


QRectF MTextEditView::boundingRect() const
{
    // This virtual method should be removed since base class implementation is sufficient.
    return MWidgetView::boundingRect();
}


void MTextEditView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEditView);

    if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
        // Honor MWidgetView's style and play press feedback
        style()->pressFeedback().play();
    }

    // Only proceed if this is not the focusing tap.
    if (d->focusingTap) {
        return;
    }

    d->timeOnMove.start();
    d->timeOnFeedback.start();
    d->previousHorizontalScroll = d->hscroll;
    d->previousPosition = event->pos();
    d->currentPosition = event->pos();

    MTextEdit::TextFieldLocationType location;
    int cursor = d->cursorPosition(event);
    d->setMouseTarget(event->pos());

    // start timer to check if still holding mouse after a while
    d->longPressTimer->start();

    // whether mouse movement should initiate selection at all
    if (d->inAutoSelectionClick == true) {
        d->ignoreSelection = true;
    } else {
        d->ignoreSelection = !d->controller->isSelectionEnabled();
    }

    // let the controller react on click on a cursor index
    d->controller->handleMousePress(cursor, event, &location);

    if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
        if (location == MTextEdit::Word) {
            style()->pressWordFeedback().play();
        } else {
            style()->pressBoundaryFeedback().play();
        }
    }
}


void MTextEditView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEditView);
    d->scrollSelectTimer->stop();

    // Make sure cursor is visible. This is done here on release so
    // we don't do unnecessary scrolling if panning is started.
    if (d->controller->sceneManager()) {
        d->controller->sceneManager()->ensureCursorVisible();
    }

    if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
        // Honor MWidgetView's style and play release feedback
        style()->releaseFeedback().play();
    }

    const bool magnifierHidden(d->magnifier && d->magnifier->isAppeared());
    d->hideMagnifier();

    const int eventCursorPos = d->cursorPosition(event);
    const QTextCursor cursor(d->controller->textCursor());
    const bool insideCurrentPreedit((model()->edit() == MTextEditModel::EditModeActive)
                                    && (eventCursorPos > cursor.selectionStart())
                                    && (eventCursorPos < cursor.selectionEnd()));

    // We don't want pre-edit activation when magnifier is hidden, because it serves no
    // useful purpose and also because we want the cursor to be left where it was
    // positioned with magnifier and activation might change that if the input method so
    // decides.
    if (!magnifierHidden
        // controller shouldn't do anything for selection ending mouse release
        && !d->selecting
        // don't send either focus gaining mouse click with autoselect
        && !d->inAutoSelectionClick) {
        MTextEdit::TextFieldLocationType location;
        d->controller->handleMouseRelease(eventCursorPos, event, &location);

        if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
            if (location == MTextEdit::Word) {
                style()->releaseWordFeedback().play();
            } else {
                style()->releaseBoundaryFeedback().play();
            }
        }
    }

    // Only proceed if this is not the focusing tap.
    if (d->focusingTap) {
        if (d->focused) {
            // Got focus. The next tap will not be focusing tap.
            d->focusingTap = false;
        }
        return;
    }

    // In case magnifier was hidden the codepath above didn't lead to controller's handleMouseRelease()
    // which means input context was not updated even though cursor position changed. Therefore, we
    // call QInputContext::update() directly to notify about changed properties such as surrounding
    // text and cursor rectangle.
    // In case selection was made or updated this has already been called.
    // TODO: Logic here is too complex. The above handleMouseRelease should probably be called
    // anyways and pre-edit activation should be prevented by other means.
    if (magnifierHidden) {
        d->icUpdate();
    }

    if ((magnifierHidden
         || !insideCurrentPreedit
         || d->controller->hasSelectedText())
        && !style()->disableToolbar()) {
        d->showEditorToolbar();
    }

    d->stopSelection();

    d->inAutoSelectionClick = false;
    d->longPressTimer->stop();
    d->scrollTimer->stop();

    if (d->controller->hasSelectedText()) {
        d->showSelectionOverlay();
    }
}


void MTextEditView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEditView);

    if (d->focusingTap) {
        return;
    }

    d->currentPosition = event->pos();

    // Only update selection if magnifier is not in use.
    const bool updateSelection = !(d->magnifier && d->magnifier->isAppeared());
    updateCursorPosition(event, updateSelection);
    if (d->selecting) {
        // start watching scrolling
        // will stop in case of mouse release of if event is cancelled
        d->scrollSelectScenePosition = event->scenePos();
        d->scrollSelectTimer->start();

        if (d->controller->sceneManager()) {
            d->controller->sceneManager()->ensureCursorVisible();
        }
    }

    d->updateMagnifierPosition();
}

void MTextEditView::updateCursorPosition(QGraphicsSceneMouseEvent *event, const bool updateSelection)
{
    Q_D(MTextEditView);

    const int cursor = d->cursorPosition(event);
    d->setMouseTarget(event->pos());

    if (updateSelection) {
        if (d->selecting) {
            d->updateSelection(event);
        } else {
            d->checkStartOfSelection(event);
            d->controller->handleMouseMove(cursor, event);
        }
    } else if (cursor >= 0) {
        if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
            d->controller->setCursorPosition(cursor);
        }
    }

    d->scrollingTestAndStart(event);
}


QSizeF MTextEditView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MTextEditView);
    // sizeHint follows the rules:
    //  Qt::MinimumSize:
    //      Returns a size which is the width of one character and the height of one line
    //  Qt::PreferredSize:
    //      SingleLine:
    //          Returns the width of the document and the minimum height of one line.
    //      MultiLine:
    //          If there is a width constraint:
    //              Resizes the current document to the given constraint's width.
    //              Stores that document height. Resizes current document back to old size.
    //              Uses the stored height (including padding).
    //          If there is no width constraint:
    //              Return the current size of the document (including padding).
    //  Qt::MaximumSize:
    //      Return QWIDGETSIZE_MAX

    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(style().operator ->());

    QSizeF hint = constraint;
    const QTextBlock block = d->document()->firstBlock();
    const QTextLayout *layout = block.layout();
    QTextLine line = layout->lineAt(0);
    qreal minLineHeight = line.height() + s->paddingTop() + s->paddingBottom()
                          + 2 * d->document()->documentMargin();

    switch (which) {
    case Qt::MinimumSize:
        if (hint.width() < 0) {
            QFontMetrics fm(style()->font());
            qreal minWidth = fm.width('x') + s->paddingLeft() + s->paddingRight()
                             + 2 * d->document()->documentMargin();
            hint.setWidth(minWidth);
        }
        if (hint.height() < 0) {
            hint.setHeight(minLineHeight);
        }
        break;
    case Qt::PreferredSize:
        {
            if (model()->line() == MTextEditModel::SingleLine) {
                if (hint.width() < 0) {
                    hint.setWidth(d->documentSize().width() +
                        s->paddingLeft() + s->paddingRight());
                }
                if (hint.height() < 0) {
                    hint.setHeight(minLineHeight);
                }
            } else {
                // multi line
                if (hint.width() > 0) {
                    hint.setHeight(d->heightForWidth(hint.width()));
                } else {
                    //Use the current document width if we are given no constraints
                    hint.setWidth(d->documentSize().width() +
                        s->paddingLeft() + s->paddingRight());
                    if (hint.height() < 0)
                        hint.setHeight(d->documentSize().height() +
                            s->paddingTop() + s->paddingBottom());
                }
            }
        }
        break;
    case Qt::MaximumSize:
        if (hint.width() < 0) {
            hint.setWidth(QWIDGETSIZE_MAX);
        }
        if (hint.height() < 0) {
            if (model()->line() == MTextEditModel::SingleLine) {
                hint.setHeight(minLineHeight);
            } else {
                hint.setHeight(QWIDGETSIZE_MAX);
            }
        }
        break;
    default:
        qWarning("MTextEditView::sizeHint() don't know how to handle the value of 'which' ");
        hint = MWidgetView::sizeHint(which, constraint);
        break;
    }

    return hint;
}

qreal MTextEditViewPrivate::heightForWidth(qreal width) const
{
    Q_Q(const MTextEditView);
    /* To find out the preferred height, we need to set the text document width
     * to the given constraint width.  But we don't want this to trigger
     * a documentsSizeChanged so we disconnect that signal and reconnect afterwards
     */
    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(q->style().operator ->());
    const qreal oldDocumentWidth = document()->textWidth();
    const qreal verticalPadding = s->paddingTop() + s->paddingBottom();

    // Disconnect size change signal
    disconnect(document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
               this, SLOT(handleDocumentSizeChange(QSizeF)));

    // Set temporary width to document and read in corresponding height for widget.
    document()->setTextWidth(realDocumentTextWidth(width));
    const qreal height = activeDocumentSize().height() + verticalPadding;

    // Restore
    document()->setTextWidth(oldDocumentWidth);
    connect(document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(handleDocumentSizeChange(QSizeF)));
    return height;
}

void MTextEditView::changeEvent(QEvent *event)
{
    Q_UNUSED(event);
}


void MTextEditView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MTextEditView);

    style()->cancelFeedback().play();

    d->stopSelection();

    // restore state before as before mouse press
    d->hideMagnifier();
    d->inAutoSelectionClick = false;
    d->longPressTimer->stop();
    d->scrollTimer->stop();

    // hide completer if there is active one
    if (d->controller->completer() && d->controller->completer()->isActive()) {
        d->controller->completer()->hideCompleter();
    }
}

void MTextEditView::notifyItemChange(QGraphicsItem::GraphicsItemChange change,
                                     const QVariant &value)
{
    MWidgetView::notifyItemChange(change, value);
}

void MTextEditView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    Q_D(MTextEditView);

    if (gesture->state() == Qt::GestureStarted) {
        QTransform itemTransform(d->controller->sceneTransform().inverted());
        QPointF itemSpaceOffset = gesture->offset() * itemTransform - QPointF(itemTransform.dx(),itemTransform.dy());

        bool horizontalPan = qAbs(itemSpaceOffset.y()) <= qAbs(itemSpaceOffset.x());
        if (!horizontalPan) {
            event->ignore(gesture);
            return;
        }
    }

    event->accept(gesture);
}

QVariant MTextEditView::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_D(const MTextEditView);

    QVariant result;

    switch (static_cast<int>(query)) {
    case Qt::ImMicroFocus:
        result = QVariant(d->cursorRect());
        break;

    case M::VisualizationPriorityQuery:
        result = QVariant(false);
        break;

    case M::PreeditRectangleQuery:
        result = QVariant(d->preeditRectangle());
        break;

    case Qt::ImFont:
        result = QVariant(style()->font());
        break;

    default:
        break;
    }

    return result;
}


void MTextEditView::updateData(const QList<const char *> &modifications)
{
    Q_D(MTextEditView);
    bool viewChanged = false;

    // check if the modifications might affect the visual appearance
    foreach(const char * member, modifications) {
        if (member == MTextEditModel::Edit || member == MTextEditModel::Cursor) {
            viewChanged = true;

        } else if (member == MTextEditModel::Echo) {
            viewChanged = true;

            // create or delete masked document here if needed
            if (model()->echo() != MTextEditModel::Normal) {
                d->initMaskedDocument();
            } else {
                delete d->maskedTextDocument;
                d->maskedTextDocument = 0;

                d->maskTimer->stop();
                d->unmaskPosition = -1;
                d->unmaskLength = 0;
            }

        } else if (member == MTextEditModel::Prompt) {
            d->setPromptOpacity(d->focused ? style()->focusedPromptOpacity() : style()->unfocusedPromptOpacity());
            d->isPromptVisible = (d->activeDocument()->isEmpty() == true
                                  && model()->prompt().isEmpty() == false);

            if (d->document()->isEmpty() == true) {
                viewChanged = true;
            }
        } else if ((member == MTextEditModel::PreeditCursor)
                   || (member == MTextEditModel::AdditionalFormats)) {
            doUpdate(); // no need for checking sizes just for these
        } else if (member == MTextEditModel::Document) {
            // this shouldn't really be happening
            qWarning("MTextEditView doesn't support changing the model's document member");
        } else if (member == MTextEditModel::ErrorHighlight) {
            viewChanged = true;

            if (model()->errorHighlight()) {
                style().setModeError();
            } else if (d->focused) {
                style().setModeSelected();
            } else {
                style().setModeDefault();
            }

        }
    }

    if (viewChanged) {
        d->checkSize();
        d->checkScroll();
        doUpdate();
    }

    MWidgetView::updateData(modifications);
}


void MTextEditView::informPasteFailed()
{
    Q_D(MTextEditView);

    if (d->infoBanner || !d->controller->scene()) {
        return;
    }

    d->infoBanner = new MBanner();
    d->infoBanner->setStyleName("InformationBanner");
    d->infoBanner->setTitle(
        //: Information banner to indicate that no characters could be pasted from the
        //: clipboard to this text field. Is shown when user tries to paste
        //: non-numeric text to a numeric or phone number text field.
        //% "Cannot paste text here"
        qtTrId("qtn_comm_cantpaste"));

    d->infoBanner->appear(d->controller->scene(), MSceneWindow::DestroyWhenDone);
}

void MTextEditView::handleLongPress()
{
    Q_D(MTextEditView);

    if ((model()->echo() != MTextEditModel::Normal
         && model()->echo() != MTextEditModel::PasswordEchoOnEdit)
        || style()->disableMagnifier()
        || d->controller->text().isEmpty()) {
        return;
    }

    // Bring up magnifier on long press if it is not readonly.
    if (!d->controller->isReadOnly()) {
        d->showMagnifier();
    }
}

void MTextEditView::hideInfoBanner()
{
    /* Deprecated */
}

void MTextEditView::setupModel()
{
    Q_D(MTextEditView);

    delete d->maskedTextDocument;
    d->maskedTextDocument = 0;

    if (model()->echo() != MTextEditModel::Normal) {
        d->initMaskedDocument();
    }

    d->setPromptOpacity(d->focused ? style()->focusedPromptOpacity() : style()->unfocusedPromptOpacity());
    d->isPromptVisible = (d->activeDocument()->isEmpty() == true
                          && model()->prompt().isEmpty() == false);

    // directly connect text document contents changing to separate handler
    // note: qtextdocument signal is emitted before it does it layout updates
    connect(model()->document(), SIGNAL(contentsChange(int, int, int)),
            d, SLOT(handleDocumentUpdate(int, int, int)));

    connect(model()->document(), SIGNAL(contentsChanged()),
            d, SLOT(handleDocumentUpdated()));

    // handle size changes separately
    connect(model()->document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            d, SLOT(handleDocumentSizeChange(QSizeF)));

    d->checkSize();
    d->checkScroll();

    MWidgetView::setupModel();
}


void MTextEditView::setFocused(Qt::FocusReason reason)
{
    Q_D(MTextEditView);
    MTextEdit *textEdit = qobject_cast<MTextEdit *>(sender());

    if (!model()->errorHighlight()) {
        style().setModeSelected();
    }

    if (reason == Qt::MouseFocusReason &&
            textEdit != 0 && textEdit->isAutoSelectionEnabled() == true) {
        // assuming the selection got made since the autoselection is enabled and focus was
        // gained. This click gets handled a bit differently because selection is already made
        // when the mouse press arrives.
        d->inAutoSelectionClick = true;
    }

    if (d->controller->sceneManager()) {
        connect(d->controller, SIGNAL(cursorPositionChanged()),
                d->controller->sceneManager(), SLOT(ensureCursorVisible()),
                Qt::UniqueConnection);

        if (reason != Qt::MouseFocusReason) {
            // For mouse reason, this is done on mouse release.
            d->controller->sceneManager()->ensureCursorVisible();
        }
    }

    if (reason != Qt::MouseFocusReason) {
        // Next tap (or even current) will not be one that sets focus.
        d->focusingTap = false;
    }

    d->focused = true;

    d->playFocusAnimation(QAbstractAnimation::Forward, style()->focusedPromptOpacity());

    if (d->controller->hasSelectedText()) {
        d->showEditorToolbar();
        d->showSelectionOverlay();
    } else {
        // Try restoring editor toolbar if window is no longer minimized,
        // or if a popup window has disappeared.
        if (reason == Qt::ActiveWindowFocusReason
            || reason == Qt::PopupFocusReason) {
            d->restoreEditorToolbar();
            d->restoreSelectionOverlay();
        }
    }

    doUpdate();
}


void MTextEditView::removeFocus(Qt::FocusReason reason)
{
    Q_D(MTextEditView);
    Q_UNUSED(reason);
    d->focusingTap = true;

    if (d->editorToolbar && d->editorToolbar->isAppeared()) {
        if (reason != Qt::ActiveWindowFocusReason
            && reason != Qt::PopupFocusReason) {
            d->hideEditorToolbar();
        } else {
            d->hideEditorToolbarTemporarily();
        }
    }

    if (!d->selectionOverlay.isNull() && d->selectionOverlay->isVisible()) {
        emit selectionChanged(0, QRect(), false, 0, QRect(), false);
    }

    if (d->controller->sceneManager()) {
        disconnect(d->controller, SIGNAL(cursorPositionChanged()),
                   d->controller->sceneManager(), SLOT(ensureCursorVisible()));
    }

    if (!model()->errorHighlight()) {
        style().setModeDefault();
    }
    d->focused = false;
    d->editActive = false;

    d->playFocusAnimation(QAbstractAnimation::Backward, d->currentPromptOpacity);

    doUpdate();
}


void MTextEditView::applyStyle()
{
    Q_D(MTextEditView);

    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(style().operator ->());

    d->maskTimer->setInterval(s->maskingDelay());

    d->selectionFormat.setForeground(s->selectionTextColor());
    d->selectionFormat.setBackground(s->selectionBackgroundColor());

    // movement threshold for selection start
    qreal threshold = s->selectionThreshold();
    if (threshold > 0) {
        d->selectionThreshold = threshold;
    }

    QString maskString = s->maskString();

    // special case when mask-string is an unicode character in the form "\uXXXX".
    // reason: theming system doesn't support unicode strings for now.
    if (maskString.length() >= 6 &&
        (maskString.indexOf("\\u", 0) == 0 || maskString.indexOf("\\U", 0) == 0)) {
        maskString.remove(0, 2);
        ushort us = maskString.toUShort(0, 16);
        maskString = QChar(us);
    }

    if (maskString.isEmpty() == false) {
        d->maskCharacter = maskString.at(0); // use only the first character
    }

    // Set document font
    d->document()->setDefaultFont(s->font());

    // Note: currently using fixed internal margin
    d->document()->setDocumentMargin(InternalMargin);

    // Note: using non-documented property
    d->document()->documentLayout()->setProperty(CursorWidthProperty,
                                                 s->cursorWidth());

    if (d->maskedTextDocument != 0) {
        d->maskedTextDocument->setDefaultFont(s->font());
        d->maskedTextDocument->setDocumentMargin(InternalMargin);
        d->maskedTextDocument->documentLayout()->setProperty(CursorWidthProperty,
                                                             s->cursorWidth());
    }

    // font etc might affect size
    d->checkSize();

    d->promptFocusAnimation.setDuration(s->focusTransitionDuration());
    d->promptFocusAnimation.setStartValue(s->unfocusedPromptOpacity());
    d->promptFocusAnimation.setEndValue(s->focusedPromptOpacity());
    d->setPromptOpacity(d->focused ? s->focusedPromptOpacity() : s->unfocusedPromptOpacity());

    d->promptShowHideAnimation.setDuration(s->hideShowPromptDuration());
    d->promptShowHideAnimation.setStartValue(s->unfocusedPromptOpacity());
    d->promptShowHideAnimation.setEndValue(0);

    d->focusAnimationDelay->setInterval(s->promptTransitionDelay());

    if (d->editorToolbar) {
        d->editorToolbar->setStyleName(s->toolbarStyleName());
    }

    d->delaySelection.setInterval(s->selectionDelay());

    d->showMovedToolbar.setInterval(s->toolbarShowDelay());

    MWidgetView::applyStyle();
}

QVariant MTextEditView::inputMethodFont()
{
    return inputMethodQuery(Qt::ImFont);
}

M_REGISTER_VIEW_NEW(MTextEditView, MTextEdit)
