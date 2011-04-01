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

#include "mtexteditview.h"
#include "mtexteditview_p.h"
#include "mtextmagnifier.h"
#include "meditortoolbar.h"
#include "mcompleter.h"

#include <MSceneManager>

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

#include "mtextedit.h"
#include "mtextedit_p.h"
#include "minfobanner.h"
#include "mtimestamp.h"
#include "minputmethodstate.h"

namespace
{
    const int LongPressTime = 600; //! how long to wait in tap&hold to initiate long press action
    const int ScrollMargin = 20; // distance from the edge to start scrolling
    const int ScrollingTimeDuration = 100;
    const int ScrollStep = 10; // pixels to scroll at once

    //! how long before character becomes masked
    const int MaskedTimeInterval = 1000;

    const int InternalMargin = 0; // disable the margin altogether, QTextDocument default is 4

    const QChar DefaultMaskCharacter('*');

    // Default icon name for informational banner about failed paste
    const char *const DefaultPasteBannerIcon = "icon-m-framework-close";

    // Upper limit for how long notification will stay visible, may be closed earlier automatically
    const int NotificationDuration = 3000;
    const int BinaryTextVariantSeparator = 0x9c;

    const char * const CursorWidthProperty = "cursorWidth";

    // Editor toolbar action property that affects toolbar closing
    const char * const NoCloseToolbarOnTriggerProperty("noCloseToolbarOnTrigger");
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
      promptTextDocument(new QTextDocument(this)),
      unmaskPosition(-1),
      unmaskLength(0),
      selectionThreshold(15),
      maskCharacter(DefaultMaskCharacter),
      hscroll(0.0),
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
      hideInfoBannerTimer(new QTimer(this)),
      focusReleaseExpected(false)
{
    // copy text options from actual document to prompt
    QTextOption option = document()->defaultTextOption();
    promptTextDocument->setDefaultTextOption(option);

    selectionFormat.setForeground(Qt::white);
    selectionFormat.setBackground(Qt::black);

    longPressTimer->setSingleShot(true);
    longPressTimer->setInterval(LongPressTime);

    maskTimer->setSingleShot(true);
    maskTimer->setInterval(MaskedTimeInterval);

    hideInfoBannerTimer->setSingleShot(true);
    QObject::connect(hideInfoBannerTimer, SIGNAL(timeout()), q, SLOT(hideInfoBanner()));

    QObject::connect(longPressTimer, SIGNAL(timeout()), q, SLOT(handleLongPress()));
    QObject::connect(scrollTimer, SIGNAL(timeout()), this, SLOT(scrolling()));
    QObject::connect(maskTimer, SIGNAL(timeout()), this, SLOT(hideUnmaskedText()));
    QObject::connect(control, SIGNAL(cursorPositionChanged()),
                     this, SLOT(updateEditorToolbarPosition()));
    QObject::connect(control, SIGNAL(selectionChanged()),
                     this, SLOT(updateEditorToolbarPosition()));
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

QTextDocument *MTextEditViewPrivate::promptDocument() const
{
    return promptTextDocument;
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

    } else if (hitPoint.x() > activeDocument()->size().width()) {
        hitPoint.setX(activeDocument()->size().width());
    }

    // on a few lowest pixels, the hitTest returns position on the end.
    // a limitor before the real lower edge is used to avoid this.
    const int MaxYLimitor = 4;
    int maxY = activeDocument()->size().height()
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

    int maxScroll = activeDocument()->size().width() + 2 * activeDocument()->documentMargin()
                    - q->geometry().width();

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

    } else if (q->model()->edit() == MTextEditModel::EditModeSelect) {
        controller->setSelection(startCursorPos, cursorIndex - startCursorPos, true);
    }

    q->doUpdate(); // need to redraw, widget supposedly changed.
}


/*!
 * \brief checks if event is on text entry edge and sets scrolling speed accordingly.
 * actual scrolling happens in scrolling()
 */
void MTextEditViewPrivate::scrollingTestAndStart(QGraphicsSceneMouseEvent *event)
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
    } else if (event->pos().x() < (ScrollMargin + paddingLeft) && hscroll > 0) {
        scrollSpeedVertical = -ScrollStep;
    } else if (event->pos().x() > (rect.width() - (ScrollMargin + paddingRight))
               && hscroll < (activeDocument()->size().width() - rect.width())) {
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

    // check that cursor isn't before the widget
    if (hscroll > currentX) {
        hscroll = currentX;
        scrolled = true;

    } else if (activeDocument()->textWidth() != -1) {
        // checking scrolling with respect to size only if the size is set

        qreal rightBorder = activeDocument()->textWidth() - 2 * activeDocument()->documentMargin()
                            + hscroll;

        if (currentX > (rightBorder - cWidth)) {
            // check cursor being after the widget (if the widget size is set)
            // FIXME: margins seem to be a bit funny. this avoids having cursor outside
            // visible area.
            hscroll = currentX - activeDocument()->textWidth()
                      + 2 * activeDocument()->documentMargin()
                      + cWidth;
            scrolled = true;
        } else if (hscroll > 0) {
            // scroll text to the right if
            // 1) there is text before widget and
            // 2) there is free space between end of text and right widget's border
            qreal endX = activeDocument()->idealWidth() + cWidth;

            if (endX < rightBorder) {
                hscroll -= rightBorder - endX;

                if (hscroll < 0) {
                    hscroll = 0;
                }
                scrolled = true;
            }
        }
    }

    if (scrolled) {
        q->doUpdate();
    }
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
        if ( q->model()->edit() == MTextEditModel::EditModeBasic) {
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

    if (activeCursor.atStart() == false && activeCursor.atBlockStart() == false) {

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
    }

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
    maskedTextDocument->setTextWidth(document()->textWidth());
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
    if (!editorToolbar) {
        editorToolbar.reset(new MEditorToolbar(*controller));
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
        QObject::connect(controller->sceneManager(),
                         SIGNAL(orientationChanged(const M::Orientation &)),
                         editorToolbar.data(), SLOT(disappearTemporarily()));
        QObject::connect(controller->sceneManager(),
                         SIGNAL(orientationChangeFinished(const M::Orientation &)),
                         this, SLOT(restoreEditorToolbar()));

        MTextEditPrivate &controllerD(*static_cast<MTextEditPrivate *>(controller->d_func()));
        connect(&controllerD.signalEmitter, SIGNAL(scenePositionChanged()),
                this, SLOT(updateEditorToolbarPosition()), Qt::UniqueConnection);
    }

    editorToolbar->appear();
    updateEditorToolbarPosition();
}


void MTextEditViewPrivate::hideEditorToolbar()
{
    if (!editorToolbar) {
        return;
    }
    editorToolbar->disappear();
    MTextEditPrivate &controllerD(*static_cast<MTextEditPrivate *>(controller->d_func()));
    disconnect(&controllerD.signalEmitter, SIGNAL(scenePositionChanged()),
               this, SLOT(updateEditorToolbarPosition()));
    disconnect(controller->sceneManager(),
               SIGNAL(orientationChangeFinished(const M::Orientation &)),
               this, SLOT(restoreEditorToolbar()));
    editorToolbar.reset();
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

    updateMagnifierPosition();
    magnifier->appear();
    MTextEditPrivate *textWidgetPtr = static_cast<MTextEditPrivate *>(controller->d_func());
    if (textWidgetPtr) {
        connect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()),
                this, SLOT(updateMagnifierPosition()), Qt::UniqueConnection);
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
            disconnect(&textWidgetPtr->signalEmitter, SIGNAL(scenePositionChanged()), this, SLOT(updateMagnifierPosition()));
        }

        magnifier->disappear();
        magnifier.reset();
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
    qreal docHeight = activeDocument()->size().height();

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
    qreal textWidth = activeDocument()->textWidth();

    // Check the need to scroll when current text width is so wide that cursor
    // would be partially visible or hidden but not wide enough to trigger
    // QAbstractTextDocumentLayout size change signal. This is a special
    // case since QAbstractTextDocumentLayout size change signal doesn't
    // take in to account the width of the cursor.
    if (currentWidth > (textWidth - cursorWidth()) && currentWidth <= textWidth) {
        checkScroll();
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

void MTextEditViewPrivate::playTextFieldSelectionFeedback()
{
    Q_Q(const MTextEditView);

    q->style()->changeSelectionFeedback().play();
}

/*!
 * \brief Method to start text selection
 * \param event mouse event information (like position)
 */
void MTextEditViewPrivate::startSelection(QGraphicsSceneMouseEvent *event)
{
    if (activeDocument() != maskedTextDocument) {
        selecting = true;

        int currentPos = cursorPosition(event);
        startCursorPos = cursorPosition(event->buttonDownPos(Qt::LeftButton));
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

    const QTextLayout *layout = currentBlock.layout();
    const QPointF layoutPos = document()->documentLayout()->blockBoundingRect(currentBlock).topLeft();
    int relativePos = position - currentBlock.position();
    QTextLine currentLine = layout->lineForTextPosition(relativePos);
    if (!currentLine.isValid())
        currentLine = layout->lineAt(0);

    if (!currentLine.isValid())
        return rect;

    int cursorWidth, cursorHeight;
    bool ok = false;
    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(q->style().operator ->());

    cursorWidth = document()->documentLayout()->property(CursorWidthProperty).toInt(&ok);
    if (!ok)
        cursorWidth = 1;
    //cursor occupies one space
    cursorWidth += QFontMetrics(currentBlock.layout()->font()).width(QLatin1Char(' '));

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

        QFontMetrics fm(q->style()->font());
        QRectF documentGeometry(documentLayoutPos,
                                document()->documentLayout()->documentSize());
        QPointF magnifierPos(qBound<qreal>(documentGeometry.left(),
                                           mouseTarget.x(),
                                           documentGeometry.right()),
                             qBound<qreal>(documentGeometry.top() + fm.height() / 2.0f,
                                           mouseTarget.y(),
                                           documentGeometry.bottom() - fm.height() / 2.0f));

        magnifier->setMagnifiedPosition(magnifierPos);
    }
}

void MTextEditViewPrivate::makeMagnifierDisappear()
{
    if (magnifier) {
        magnifier->disappear();
    }
}

void MTextEditViewPrivate::makeMagnifierAppear()
{
    if (magnifier) {
        magnifier->appear();
    }
}

void MTextEditViewPrivate::restoreEditorToolbar()
{
    // restore editor toolbar if it was temporarily hidden
    if (editorToolbar) {
        editorToolbar->removeTemporaryDisappearance();
        updateEditorToolbarPosition();
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
    MEditorToolbarArrow::ArrowDirection arrowDirection(MEditorToolbarArrow::ArrowDown);

    const QRect visibleSceneRect(
        controller->mapRectFromScene(
            0, 0, controller->sceneManager()->visibleSceneSize(M::Landscape).width(),
            controller->sceneManager()->visibleSceneSize(M::Landscape).height()).toRect());

    if (targetY - editorToolbar->size().height() < visibleSceneRect.top()) {
        targetY = secondRect.bottom();

        int bottomLimit;
        if (MInputMethodState::instance()->inputMethodArea().isEmpty()) {
            bottomLimit = visibleSceneRect.bottom();
        } else {
            bottomLimit = qMin<int>(
                controller->mapRectFromScene(
                    MInputMethodState::instance()->inputMethodArea()).toRect().top(),
                visibleSceneRect.bottom());
        }

        if (targetY + editorToolbar->size().height() <= bottomLimit) {
            targetRect = secondRect;
        } else {
            targetY = firstRect.bottom();
        }
        arrowDirection = MEditorToolbarArrow::ArrowUp;
    }

    editorToolbar->setPosition(QPointF(targetRect.center().x(), targetY),
                               arrowDirection);
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
    painter->save();

    // set clipping rectangle to draw text inside the border
    QRectF clipping(boundingRect().adjusted(paddingLeft,
                                            s->paddingTop(),
                                            -paddingRight,
                                            -s->paddingBottom()));
    clipping = clipping.intersected(option->exposedRect);
    painter->setClipRect(clipping, Qt::IntersectClip);
    // If text does not fit inside widget, it may have to be scrolled
    const qreal dx = -d->hscroll + paddingLeft;
    const qreal dy = -d->vscroll + s->paddingTop();
    painter->translate(dx, dy);
    // draw actual text to the screen

    if (d->activeDocument()->isEmpty() == true
        && d->promptDocument()->isEmpty() == false) {
        // with no content we show the prompt text if there is prompt text
        QAbstractTextDocumentLayout::PaintContext paintContext;
        if (d->focused == true) {
            paintContext.cursorPosition = 0;
        }
        QColor promptColor = s->promptColor();
        paintContext.palette.setColor(QPalette::Text, promptColor);

        d->promptDocument()->documentLayout()->draw(painter, paintContext);

    } else {
        // normal painting
        QAbstractTextDocumentLayout::PaintContext paintContext = d->paintContext();
        paintContext.clip = option->exposedRect;
        paintContext.clip.translate(-dx, -dy);
        d->activeDocument()->documentLayout()->draw(painter, paintContext);
    }

    painter->restore();
    // mTimestamp("MTextEditView", QString("end text=%1").arg(d->document()->toPlainText()));
}


void MTextEditView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MTextEditView);
    int textWidth = event->newSize().width() - style()->paddingLeft() - style()->paddingRight();

    d->document()->setTextWidth(textWidth);
    d->promptTextDocument->setTextWidth(textWidth);

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
}


QRectF MTextEditView::boundingRect() const
{
    return QRectF(QPointF(0, 0), geometry().size());
}


void MTextEditView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEditView);

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
        // Honor MWidgetView's style and play press feedback
        style()->pressFeedback().play();

        if (location == MTextEdit::Word) {
            style()->pressWordFeedback().play();
        } else {
            style()->pressBoundaryFeedback().play();
        }
    }

    // Connect when pressing and disconnect when releasing
    // to make sure that feedback is only given when user is
    // making changes to the selection by touching.
    QObject::connect(d->controller, SIGNAL(selectionChanged()),
                     d, SLOT(playTextFieldSelectionFeedback()),
                     Qt::UniqueConnection);
}


void MTextEditView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEditView);

    MTextEdit::TextFieldLocationType location;

    const bool magnifierHidden(d->magnifier && d->magnifier->isAppeared());
    d->hideMagnifier();

    // Make sure cursor is visible. This is done here on release so
    // we don't do unnecessary scrolling if panning is started.
    if (d->controller->sceneManager()) {
        d->controller->sceneManager()->ensureCursorVisible();
    }

    if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
        // Honor MWidgetView's style and play release feedback
        style()->releaseFeedback().play();
    }

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
        d->controller->handleMouseRelease(eventCursorPos, event, &location);

        if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
            if (location == MTextEdit::Word) {
                style()->releaseWordFeedback().play();
            } else {
                style()->releaseBoundaryFeedback().play();
            }
        }
    }

    if (magnifierHidden || (!d->focusReleaseExpected && !insideCurrentPreedit)
        || d->controller->hasSelectedText()) {
        d->showEditorToolbar();
    }
    d->focusReleaseExpected = false;

    d->selecting = false;
    d->inAutoSelectionClick = false;
    d->longPressTimer->stop();
    d->scrollTimer->stop();

    QObject::disconnect(d->controller, SIGNAL(selectionChanged()),
                        d, SLOT(playTextFieldSelectionFeedback()));
}


void MTextEditView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEditView);
    // Only update selection if magnifier is not in use.
    const bool updateSelection = !(d->magnifier && d->magnifier->isAppeared());
    updateCursorPosition(event, updateSelection);

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
                    hint.setWidth(d->document()->size().width() +
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
                    hint.setWidth(d->document()->size().width() +
                        s->paddingLeft() + s->paddingRight());
                    if (hint.height() < 0)
                        hint.setHeight(d->document()->size().height() +
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
    const qreal horizontalPadding = s->paddingLeft() + s->paddingRight();
    const qreal verticalPadding = s->paddingTop() + s->paddingBottom();

    // Disconnect size change signal
    disconnect(document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
               this, SLOT(handleDocumentSizeChange(QSizeF)));

    // Set temporary width to document and read in corresponding height for widget.
    document()->setTextWidth(width - horizontalPadding);
    const qreal height = document()->size().height() + verticalPadding;

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

    // restore state before as before mouse press
    d->hideMagnifier();
    d->selecting = false;
    d->inAutoSelectionClick = false;
    d->longPressTimer->stop();
    d->scrollTimer->stop();

    // hide completer if there is active one
    if (d->controller->completer() && d->controller->completer()->isActive()) {
        d->controller->completer()->hideCompleter();
    }

    QObject::disconnect(d->controller, SIGNAL(selectionChanged()),
                        d, SLOT(playTextFieldSelectionFeedback()));
}

void MTextEditView::notifyItemChange(QGraphicsItem::GraphicsItemChange change,
                                     const QVariant &value)
{
    MWidgetView::notifyItemChange(change, value);
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

    case Qt::ImFont: // FIXME: return currently used font
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
            // Note: only using the first length variant now
            QString promptText = model()->prompt();
            promptText = promptText.left(promptText.indexOf(QChar(BinaryTextVariantSeparator)));
            d->promptTextDocument->setPlainText(promptText);

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

    if (d->infoBanner) {
        return;
    }

    QString iconName = style()->pasteFailedIcon();
    int duration = style()->pasteFailedDuration();

    if (iconName.isEmpty()) {
        iconName = DefaultPasteBannerIcon;
    }

    if (duration <= 0) {
        duration = NotificationDuration;
    }

    d->infoBanner = new MInfoBanner(MInfoBanner::Information);
    d->infoBanner->setImageID(iconName);
    d->infoBanner->setBodyText(
        //: Information banner to indicate that no characters could be pasted from the
        //: clipboard to this text field. Is shown when user tries to paste
        //: non-numeric text to a numeric or phone number text field.
        //% "Cannot paste text here"
        qtTrId("qtn_comm_cantpaste"));

    d->controller->sceneManager()->appearSceneWindow(d->infoBanner, MSceneWindow::DestroyWhenDone);
    d->hideInfoBannerTimer->setInterval(duration);
    d->hideInfoBannerTimer->start();
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

    // Bring up magnifier on long press.
    d->showMagnifier();
}

void MTextEditView::hideInfoBanner()
{
    Q_D(MTextEditView);

    if (d->infoBanner)
        d->infoBanner->dismiss();
    d->infoBanner = 0;
}

void MTextEditView::setupModel()
{
    Q_D(MTextEditView);

    delete d->maskedTextDocument;
    d->maskedTextDocument = 0;

    if (model()->echo() != MTextEditModel::Normal) {
        d->initMaskedDocument();
    }

    d->promptDocument()->setPlainText(model()->prompt());

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

    d->focusReleaseExpected = reason == Qt::MouseFocusReason;

    d->focused = true;
    doUpdate();
}


void MTextEditView::removeFocus(Qt::FocusReason reason)
{
    Q_D(MTextEditView);
    Q_UNUSED(reason);

    if (reason != Qt::ActiveWindowFocusReason && reason != Qt::PopupFocusReason)
        d->hideEditorToolbar();

    if (d->controller->sceneManager()) {
        disconnect(d->controller, SIGNAL(cursorPositionChanged()),
                   d->controller->sceneManager(), SLOT(ensureCursorVisible()));
    }

    if (!model()->errorHighlight()) {
        style().setModeDefault();
    }
    d->focused = false;
    d->editActive = false;
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
    if (maskString.isEmpty() == false) {
        d->maskCharacter = maskString.at(0); // use only the first character
    }

    // Set document font
    d->document()->setDefaultFont(s->font());
    d->promptTextDocument->setDefaultFont(s->promptFont());

    // Note: currently using fixed internal margin
    d->document()->setDocumentMargin(InternalMargin);
    d->promptTextDocument->setDocumentMargin(InternalMargin);

    // Note: using non-documented property
    d->document()->documentLayout()->setProperty(CursorWidthProperty,
                                                 s->cursorWidth());
    d->promptTextDocument->documentLayout()->setProperty(CursorWidthProperty,
                                                         s->cursorWidth());

    if (d->maskedTextDocument != 0) {
        d->maskedTextDocument->setDefaultFont(s->font());
        d->maskedTextDocument->setDocumentMargin(InternalMargin);
        d->maskedTextDocument->documentLayout()->setProperty(CursorWidthProperty,
                                                             s->cursorWidth());
    }

    // font etc might affect size
    d->checkSize();

    MWidgetView::applyStyle();
}

M_REGISTER_VIEW_NEW(MTextEditView, MTextEdit)
