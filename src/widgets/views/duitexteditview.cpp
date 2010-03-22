/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duitexteditview.h"
#include "duitexteditview_p.h"
#include "duicompleter.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextLayout>
#include <QAbstractTextDocumentLayout>
#include <QTimer>

#include "duifeedback.h"
#include "duitextedit.h"
#include "duitheme.h"
#include "duitexteditviewzoom.h"
#include "duiviewcreator.h"
#include "duiinfobanner.h"
#include "duilocale.h"
#include "duiscenemanager.h"
#include "duitimestamp.h"

namespace
{
    const int ZoomTimeInterval = 600; //! how long to wait in tap&hold to start zooming
    const int ScrollMargin = 20; // distance from the edge to start scrolling
    const int ScrollingTimeDuration = 100;
    const int ScrollStep = 10; // pixels to scroll at once

    //! how long before character becomes masked
    const int MaskedTimeInterval = 1000;

    const int InternalMargin = 4;

    const QChar DefaultMaskCharacter('*');

    // Default icon name for informational banner about failed paste
    const char *const DefaultPasteBannerIcon = "Icon-close";

    // How long notification will stay visible
    const int NotificationDuration = 3000;
}


/*!
 * \brief Constructor
 * \param control DuiTextEdit widget which this view shows
 */
DuiTextEditViewPrivate::DuiTextEditViewPrivate(DuiTextEdit *control, DuiTextEditView *q)
    : q_ptr(q),
      controller(control),
      focused(false),
      textDocument(control->document()),
      maskedTextDocument(0),
      promptTextDocument(new QTextDocument(this)),
      unmaskPosition(-1),
      unmaskLength(0),
      selectionThreshold(15),
      maskCharacter(DefaultMaskCharacter),
      hscroll(0.0),
      vscroll(0.0),
      scrollSpeedVertical(0),
      zoomTimer(new QTimer(this)),
      scrollTimer(new QTimer(this)),
      maskTimer(new QTimer(this)),
      selecting(false),
      ignoreSelection(false),
      startCursorPos(-1),
      documentHeight(0.0),
      mouseTarget(0, 0),
      zoomable(true),
      inAutoSelectionClick(false),
      infoBanner(0),
      editActive(false)
{
    // copy text options from actual document to prompt
    QTextOption option = textDocument->defaultTextOption();
    promptTextDocument->setDefaultTextOption(option);

    selectionFormat.setForeground(Qt::white);
    selectionFormat.setBackground(Qt::black);

    zoomTimer->setSingleShot(true);
    maskTimer->setSingleShot(true);
    maskTimer->setInterval(MaskedTimeInterval);

    QObject::connect(zoomTimer, SIGNAL(timeout()), this, SLOT(createZoomView()));
    QObject::connect(scrollTimer, SIGNAL(timeout()), this, SLOT(scrolling()));
    QObject::connect(maskTimer, SIGNAL(timeout()), this, SLOT(hideUnmaskedText()));
}


/*!
 * Destructor
 */
DuiTextEditViewPrivate::~DuiTextEditViewPrivate()
{
    delete maskedTextDocument;
}


/*!
 * \brief Returns cursor position from a mouse position
 * \param event mouse event information (like position)
 * \return cursor position as characters from start of document
 */
int DuiTextEditViewPrivate::cursorPosition(QGraphicsSceneMouseEvent *event)
{
    QPointF hitPoint = event->pos();
    return cursorPosition(hitPoint);
}


/*!
 * \brief Returns cursor position from a mouse position
 * \param hitPoint mouse position
 * \return cursor position as characters from start of document
 */
int DuiTextEditViewPrivate::cursorPosition(QPointF hitPoint)
{
    Q_Q(DuiTextEditView);

    // adjust widget position to textdocument position
    hitPoint.rx() += hscroll;
    hitPoint.ry() += vscroll;
    hitPoint.rx() -= q->style()->paddingLeft();
    hitPoint.ry() -= q->style()->paddingTop();

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

    return activeDocument()->documentLayout()->hitTest(hitPoint, Qt::FuzzyHit);
}


/*!
 * \brief Scrolling slot, do the auto scrolling when cursor moves to the scroll area
 */
void DuiTextEditViewPrivate::scrolling()
{
    Q_Q(DuiTextEditView);

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

    if (q->model()->edit() == DuiTextEditModel::EditModeBasic) {
        controller->setCursorPosition(cursorIndex);

    } else if (q->model()->edit() == DuiTextEditModel::EditModeSelect) {
        controller->setSelection(startCursorPos, cursorIndex - startCursorPos, true);
    }

    doUpdate(); // need to redraw, widget supposedly changed.
}


/*!
 * \brief checks if event is on text entry edge and sets scrolling speed accordingly.
 * actual scrolling happens in scrolling()
 */
void DuiTextEditViewPrivate::scrollingTestAndStart(QGraphicsSceneMouseEvent *event)
{
    Q_Q(DuiTextEditView);

    QRectF rect = q->geometry();

    // event inside scrolling margin creates constant speed scrolling.
    // this could be changed to determine some scrolling speed depending on the position.
    if (event->pos().x() < (ScrollMargin + q->style()->paddingLeft()) && hscroll > 0) {
        scrollSpeedVertical = -ScrollStep;

    } else if (event->pos().x() > (rect.width() - (ScrollMargin + q->style()->paddingRight()))
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


/*!
 * \brief handle mouse move in zooming. called from DuiTextViewZoom.
 * \param event the move event
 */
void DuiTextEditViewPrivate::mouseMoveInZooming(QGraphicsSceneMouseEvent *event)
{
    // only move cursor if left mouse button is pressed
    if ((event->buttons() & Qt::LeftButton) == 0) {
        return;
    }

    setMouseTarget(event->pos());
    int cursor = cursorPosition(event);

    if (cursor >= 0) {
        controller->setCursorPosition(cursor);
    }

    // also scroll in zoom.
    // known issue: scrolling zoom view with mouse on a spot
    // requires one another mouse move to start text scrolling
    scrollingTestAndStart(event);
}


// stops text scrolling. Called from DuiTextEditViewZoom when mouse is released
void DuiTextEditViewPrivate::stopScrolling()
{
    scrollTimer->stop();
}


//! validates hscroll and vscroll values so cursor stays visible
void DuiTextEditViewPrivate::checkScroll()
{
    Q_Q(DuiTextEditView);

    // Selection doesn't currently change scrolling.
    // rationale: user initiated selection can only happen on visible part
    // and change in scrolling here would conflict with scrolling on the edges
    if (q->model()->edit() == DuiTextEditModel::EditModeSelect) {
        return;
    }

    // TODO: vscroll possibly at some point

    qreal currentX = cursorX();
    bool scrolled = false;

    // check that cursor isn't before the widget
    if (hscroll > currentX) {
        hscroll = currentX;
        scrolled = true;

    } else if (activeDocument()->textWidth() != -1) {
        // checking scrolling with respect to size only if the size is set

        if (currentX > (activeDocument()->textWidth() - 2 * activeDocument()->documentMargin()
                        + hscroll)) {
            // check cursor being after the widget (if the widget size is set)
            // FIXME: margins seem to be a bit funny. this avoids having cursor outside
            // visible area.
            hscroll = currentX - activeDocument()->textWidth()
                      + 2 * activeDocument()->documentMargin();
            scrolled = true;
        }
    }

    if (scrolled) {
        doUpdate();
    }
}


//! sets the mouse target point making sure it's inside the widget
void DuiTextEditViewPrivate::setMouseTarget(const QPointF &point)
{
    Q_Q(DuiTextEditView);

    mouseTarget.setX(qBound<qreal>(0.0, q->geometry().width(), point.x()));
    mouseTarget.setY(qBound<qreal>(0.0, q->geometry().height(), point.y()));
}


QAbstractTextDocumentLayout::PaintContext DuiTextEditViewPrivate::paintContext() const
{
    Q_Q(const DuiTextEditView);

    QAbstractTextDocumentLayout::PaintContext paintContext;
    paintContext.palette.setColor(QPalette::Text, q->style()->textColor());
    QTextCursor cursor = controller->textCursor();
    DuiTextEditModel::EchoMode echoMode = q->model()->echo();

    if (echoMode == DuiTextEditModel::NoEcho) {
        if (focused == true) {
            paintContext.cursorPosition = 0;
        }

    } else if (q->model()->edit() == DuiTextEditModel::EditModeSelect) {
        // add selection formatting
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = cursor;
        selection.format = selectionFormat;
        paintContext.selections.append(selection);

    } else if ((focused == true) && (q->model()->edit() == DuiTextEditModel::EditModeBasic)) {
        paintContext.cursorPosition = cursor.position();
    }

    return paintContext;
}


// returns cursor's x position on textdocument
qreal DuiTextEditViewPrivate::cursorX() const
{
    Q_Q(const DuiTextEditView);

    qreal currentX = 0.0;

    if (q->model()->echo() == DuiTextEditModel::NoEcho) {
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


void DuiTextEditViewPrivate::initMaskedDocument()
{
    Q_Q(DuiTextEditView);

    maskTimer->stop();

    int textLength = 0;

    if (q->model()->echo() != DuiTextEditModel::NoEcho) {
        QString text = textDocument->toPlainText();
        textLength = text.length();
    }

    QString maskedText(textLength, maskCharacter);

    if (maskedTextDocument == 0) {
        maskedTextDocument = new QTextDocument(maskedText, this);
    } else {
        maskedTextDocument->setPlainText(maskedText);
    }

    // copy the settings
    maskedTextDocument->setDocumentMargin(textDocument->documentMargin());
    maskedTextDocument->setDefaultFont(textDocument->defaultFont());
    maskedTextDocument->setTextWidth(textDocument->textWidth());

    // no word wrapping in masked mode
    QTextOption option = document()->defaultTextOption();
    option.setWrapMode(QTextOption::NoWrap);
    maskedTextDocument->setDefaultTextOption(option);
}


// returns the textdocument that is currently used for displaying text
QTextDocument *DuiTextEditViewPrivate::activeDocument() const
{
    Q_Q(const DuiTextEditView);

    DuiTextEditModel::EchoMode echoMode = q->model()->echo();

    if (echoMode == DuiTextEditModel::Normal ||
            (echoMode == DuiTextEditModel::PasswordEchoOnEdit && editActive == true) ||
            maskedTextDocument == 0) {
        return textDocument;

    } else {
        return maskedTextDocument;
    }
}


// notifies this and the possible zoom view about update
void DuiTextEditViewPrivate::doUpdate()
{
    Q_Q(DuiTextEditView);
    q->update();

    if (zoomView) {
        zoomView->update();
    }
}


void DuiTextEditViewPrivate::hideUnmaskedText()
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

        doUpdate();
    }
}


/*! Updates cached size and calls updateGeometry() for the view
 */
void DuiTextEditViewPrivate::checkSize()
{
    Q_Q(DuiTextEditView);
    // note: as of Qt documentation, the size of newly created empty document is
    // configuration dependent. in practice it seems to match the selected font size.
    // possibly as a side effect of setDefaultFont()
    qreal docHeight = activeDocument()->size().height();

    if (documentHeight != docHeight) {
        documentHeight = docHeight;
        q->updateGeometry();
    }
}


void DuiTextEditViewPrivate::createZoomView()
{
    Q_Q(DuiTextEditView);

    delete zoomView;
    zoomView = new DuiTextEditViewZoom(q, mouseTarget);
    // assuming the zoom view receives the rest of mouse events
    inAutoSelectionClick = false;
    q->updateMicroFocus(); // visualization priority gained
    // hide completer if there is active one
    if (controller->completer() && controller->completer()->isActive()) {
        controller->completer()->hideCompleter();
    }

    // destroying the zoom removes visualization priority
    connect(zoomView, SIGNAL(destroyed()), q, SLOT(updateMicroFocus()));
}


// handles text document updates
void DuiTextEditViewPrivate::handleDocumentUpdate(int position, int charsRemoved, int charsAdded)
{
    Q_Q(DuiTextEditView);

    if (focused) {
        editActive = true; // PasswordEchoOnEdit mode only becomes visible after doing editing
    }

    if (q->model()->echo() == DuiTextEditModel::NoEcho) {
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

        if (q->model()->echo() == DuiTextEditModel::Password) {
            maskCursor.insertText(newText);
            unmaskPosition = position;
            unmaskLength = charsAdded;
            maskTimer->start();

        } else { // PasswordEchoOnEdit, do immediate masking
            QString maskedText(newText.length(), maskCharacter);
            maskCursor.insertText(maskedText);
        }
    }

    doUpdate();
}


// handles size change signals from QAbstractTextDocumentLayout
void DuiTextEditViewPrivate::handleDocumentSizeChange(const QSizeF &newSize)
{
    Q_UNUSED(newSize);
    // do the checks separately and ignore new size, we might be using a different active document.
    // having the size change signal from main document should be enough anyway.
    checkSize();
    checkScroll();
}


/*!
 * \brief Method to start text selection
 * \param event mouse event information (like position)
 */
void DuiTextEditViewPrivate::startSelection(QGraphicsSceneMouseEvent *event)
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

    // selection won zooming timer in mouse press contest, we should stop it
    zoomTimer->stop();
}


/*!
 * \brief Method to update text selection when mouse is moved
 * \param pos mouse position
 */
void DuiTextEditViewPrivate::updateSelection(const QPointF &pos)
{
    if (ignoreSelection)
        return;

    controller->setSelection(startCursorPos, cursorPosition(pos) - startCursorPos, true);
}


/*!
 * \brief This is the overloaded function created for the convenience
 * \param event mouse event information (like position)
 */
void DuiTextEditViewPrivate::updateSelection(QGraphicsSceneMouseEvent *event)
{
    updateSelection(event->pos());
}


/*!
 * \brief Method to handle mouse movement
 * \param event mouse event information (like position)
 */
void DuiTextEditViewPrivate::checkStartOfSelection(QGraphicsSceneMouseEvent *event)
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
    if (0.7 *(dx + dy) < selectionThreshold)
        return;

    // if movement is more vertical than horizontal we handle this movement as scrolling
    if (dx < dy) {
        ignoreSelection = true;
        return;
    }

    startSelection(event);

    return;
}


QRect DuiTextEditViewPrivate::preeditRectangle() const
{
    QRect rect;

    if (controller->mode() == DuiTextEditModel::EditModeActive) {

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


QRect DuiTextEditViewPrivate::cursorRect() const
{
    QRect rect;
    int position = controller->cursorPosition();
    const QTextBlock currentBlock = textDocument->findBlock(position);
    if (!currentBlock.isValid())
        return rect;

    const QTextLayout *layout = currentBlock.layout();
    const QPointF layoutPos = textDocument->documentLayout()->blockBoundingRect(currentBlock).topLeft();
    int relativePos = position - currentBlock.position();
    QTextLine currentLine = layout->lineForTextPosition(relativePos);
    if (!currentLine.isValid())
        currentLine = layout->lineAt(0);

    if (!currentLine.isValid())
        return rect;

    int cursorWidth, cursorHeight;
    bool ok = false;

    cursorWidth = textDocument->documentLayout()->property("cursorWidth").toInt(&ok);
    if (!ok)
        cursorWidth = 1;
    //cursor occupies one space
    cursorWidth += QFontMetrics(currentBlock.layout()->font()).width(QLatin1Char(' '));

    cursorHeight = currentLine.height();
    qreal x = currentLine.cursorToX(relativePos);

    rect = QRect((layoutPos.x() + x - hscroll), (layoutPos.y() + currentLine.y() - vscroll),
                 cursorWidth, cursorHeight);

    return rect;
}


//////////////////////
//// Actual class ////
//////////////////////


DuiTextEditView::DuiTextEditView(DuiTextEdit *controller)
    : DuiWidgetView(controller),
      d_ptr(new DuiTextEditViewPrivate(controller, this))
{
    QObject::connect(controller, SIGNAL(gainedFocus(Qt::FocusReason)),
                     SLOT(setFocused(Qt::FocusReason)));
    QObject::connect(controller, SIGNAL(lostFocus(Qt::FocusReason)),
                     SLOT(removeFocus(Qt::FocusReason)));
    QObject::connect(controller, SIGNAL(pasteFailed()),
                     SLOT(informPasteFailed()));
}


DuiTextEditView::~DuiTextEditView()
{
    delete d_ptr;
}


void DuiTextEditView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const DuiTextEditView);

    duiTimestamp("DuiTextEditView", QString("start text=%1").arg(d->document()->toPlainText()));
    painter->save();

    // set clipping rectangle to draw text inside the border
    QRectF clipping(boundingRect().left() + style()->paddingLeft(),
                    boundingRect().top() + style()->paddingTop(),
                    boundingRect().width() - style()->paddingLeft() - style()->paddingRight(),
                    boundingRect().height() - style()->paddingTop() - style()->paddingBottom());
    painter->setClipRect(clipping);

    // If text does not fit inside widget, it may have to be scrolled
    painter->translate(-d->hscroll + style()->paddingLeft(), -d->vscroll + style()->paddingTop());

    // draw actual text to the screen
    if (d->focused == false && d->activeDocument()->isEmpty() == true) {
        // with no focus and content we show the prompt text
        QAbstractTextDocumentLayout::PaintContext paintContext;
        QColor promptColor = style()->promptColor();
        paintContext.palette.setColor(QPalette::Text, promptColor);

        d->promptDocument()->documentLayout()->draw(painter, paintContext);

    } else {
        // normal painting
        QAbstractTextDocumentLayout::PaintContext paintContext = d->paintContext();
        d->activeDocument()->documentLayout()->draw(painter, paintContext);
    }

    painter->restore();
    duiTimestamp("DuiTextEditView", QString("end text=%1").arg(d->document()->toPlainText()));
}


void DuiTextEditView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(DuiTextEditView);
    int textWidth = event->newSize().width() - style()->paddingLeft() - style()->paddingRight();

    d->textDocument->setTextWidth(textWidth);
    d->promptTextDocument->setTextWidth(textWidth);

    if (d->maskedTextDocument != 0) {
        d->maskedTextDocument->setTextWidth(textWidth);
    }

    if (event->oldSize().isValid()) {
        qreal horizontalChange = event->newSize().width() - event->oldSize().width();

        if (horizontalChange > 0) {
            d->hscroll = qMax<qreal>(d->hscroll - horizontalChange, 0.0);
            d->doUpdate();
        }
    }

    d->checkScroll();
}


QRectF DuiTextEditView::boundingRect() const
{
    return QRectF(QPointF(0, 0), geometry().size());
}


void DuiTextEditView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiTextEditView);

    int cursor = d->cursorPosition(event);
    d->setMouseTarget(event->pos());

    if (d->zoomable) {
        // start timer to check if still holding mouse after a while
        d->zoomTimer->start(ZoomTimeInterval);
    }

    event->accept();

    // whether mouse movement should initiate selection at all
    if (d->inAutoSelectionClick == true) {
        d->ignoreSelection = true;
    } else {
        d->ignoreSelection = !d->controller->isSelectionEnabled();
    }

    if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
        style()->pressFeedback().play();
    }

    // let the controller react on click on a cursor index
    d->controller->handleMousePress(cursor, event);
}


void DuiTextEditView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiTextEditView);

    event->accept();

    // controller shouldn't do anything for selection ending mouse release
    if (d->selecting == false) {
        // don't send either focus gaining mouse click with autoselect
        if (d->inAutoSelectionClick == false) {
            int cursor = d->cursorPosition(event);

            if (model()->textInteractionFlags() != Qt::NoTextInteraction) {
                style()->releaseFeedback().play();
            }

            d->controller->handleMouseRelease(cursor, event);
        }
    }

    d->selecting = false;
    d->inAutoSelectionClick = false;
    d->zoomTimer->stop();
    d->scrollTimer->stop();
}


void DuiTextEditView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiTextEditView);

    int cursor = d->cursorPosition(event);

    d->setMouseTarget(event->pos());
    event->accept();

    if (d->selecting) {
        d->updateSelection(event);

    } else {
        d->checkStartOfSelection(event);
        d->controller->handleMouseMove(cursor, event);
    }

    d->scrollingTestAndStart(event);
}


QSizeF DuiTextEditView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiTextEditView);

    QSizeF hint = DuiWidgetView::sizeHint(which, constraint);
    qreal minHeight = d->documentHeight + style()->paddingTop() + style()->paddingBottom();

    if (hint.height() < minHeight || model()->line() == DuiTextEditModel::SingleLine) {
        hint.setHeight(minHeight);
    }

    // FIXME: apply constraint?
    return hint;
}


void DuiTextEditView::changeEvent(QEvent *event)
{
    Q_D(DuiTextEditView);

    if (event->type() == QEvent::LayoutDirectionChange) {
        // controller takes care of adjusting layout of the model, we need to update
        // our own QTextDocument instances
        if (d->maskedTextDocument != 0) {
            QTextOption option = d->maskedTextDocument->defaultTextOption();
            option.setTextDirection(d->controller->layoutDirection());
            d->maskedTextDocument->setDefaultTextOption(option);
        }

        QTextOption option = d->promptTextDocument->defaultTextOption();
        option.setTextDirection(d->controller->layoutDirection());
        d->promptTextDocument->setDefaultTextOption(option);
    }
}


void DuiTextEditView::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);

    // restore state before as before mouse press
    Q_D(DuiTextEditView);
    d->selecting = false;
    d->inAutoSelectionClick = false;
    d->zoomTimer->stop();
    d->scrollTimer->stop();
}


QVariant DuiTextEditView::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_D(const DuiTextEditView);

    if (query == Qt::ImFont) {
        return QVariant(); // FIXME: return currently used font

    } else if (query == Qt::ImMicroFocus) {
        return QVariant(d->cursorRect());

    } else if (static_cast<int>(query) == Dui::VisualizationPriorityQuery) {
        return QVariant(d->zoomView.isNull() == false);

    } else if (static_cast<int>(query) == Dui::PreeditRectangleQuery) {
        return QVariant(d->preeditRectangle());
    } else {
        return QVariant();
    }
}


void DuiTextEditView::updateData(const QList<const char *> &modifications)
{
    Q_D(DuiTextEditView);
    bool viewChanged = false;

    // check if the modifications might affect the visual appearance
    foreach(const char * member, modifications) {
        if (member == DuiTextEditModel::Edit || member == DuiTextEditModel::Cursor) {
            viewChanged = true;

        } else if (member == DuiTextEditModel::Echo) {
            viewChanged = true;

            // create or delete masked document here if needed
            if (model()->echo() != DuiTextEditModel::Normal) {
                d->initMaskedDocument();
            } else {
                delete d->maskedTextDocument;
                d->maskedTextDocument = 0;
            }

        } else if (member == DuiTextEditModel::Prompt) {
            d->promptTextDocument->setPlainText(model()->prompt());

            if (d->textDocument->isEmpty() == true) {
                viewChanged = true;
            }
        } else if (member == DuiTextEditModel::Document) {
            // this shouldn't really be happening
            qWarning("DuiTextEditView doesn't support changing the model's document member");
        }
    }

    if (viewChanged) {
        d->checkSize();
        d->checkScroll();
        d->doUpdate();
    }

    DuiWidgetView::updateData(modifications);
}


void DuiTextEditView::informPasteFailed()
{
    Q_D(DuiTextEditView);

    QString iconName = style()->pasteFailedIcon();
    int duration = style()->pasteFailedDuration();

    if (iconName.isEmpty())
        iconName = DefaultPasteBannerIcon;

    if (duration <= 0)
        duration = NotificationDuration;

    d->infoBanner = new DuiInfoBanner(DuiInfoBanner::Information);
    d->infoBanner->setImageID(iconName);
    d->infoBanner->setBodyText(
        //~ uispec-document DirectUI_Virtual_Keyboard_UI_Specification_0.30.doc
        //: Information banner to indicate that no
        //: characters could be pasted from the
        //: clipboard to this text field.
        //: Is shown when user tries to paste
        //: non-numeric text to a numeric or phone
        //: number text field.
        //% "Cannot paste text here"
        qtTrId("qtn_vkb_cantpaste"));

    d->controller->sceneManager()->appearSceneWindow(d->infoBanner, DuiSceneWindow::DestroyWhenDone);
    QTimer::singleShot(duration, this, SLOT(hideInfoBanner()));
}

void DuiTextEditView::hideInfoBanner()
{
    Q_D(DuiTextEditView);

    if (d->infoBanner)
        d->infoBanner->dismiss();
    d->infoBanner = 0;
}

void DuiTextEditView::setupModel()
{
    Q_D(DuiTextEditView);

    delete d->maskedTextDocument;
    d->maskedTextDocument = 0;

    if (model()->echo() != DuiTextEditModel::Normal) {
        d->initMaskedDocument();
    }

    d->promptDocument()->setPlainText(model()->prompt());

    // directly connect text document contents changing to separate handler
    // note: qtextdocument signal is emitted before it does it layout updates
    connect(model()->document(), SIGNAL(contentsChange(int, int, int)),
            d, SLOT(handleDocumentUpdate(int, int, int)));

    // handle size changes separately
    connect(model()->document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            d, SLOT(handleDocumentSizeChange(QSizeF)));

    d->checkSize();
    d->checkScroll();

    DuiWidgetView::setupModel();
}


void DuiTextEditView::setFocused(Qt::FocusReason reason)
{
    Q_D(DuiTextEditView);
    DuiTextEdit *textEdit = qobject_cast<DuiTextEdit *>(sender());

    style().setModeSelected();
    if (reason == Qt::MouseFocusReason &&
            textEdit != 0 && textEdit->isAutoSelectionEnabled() == true) {
        // assuming the selection got made since the autoselection is enabled and focus was
        // gained. This click gets handled a bit differently because selection is already made
        // when the mouse press arrives.
        d->inAutoSelectionClick = true;
    }

    d->focused = true;
    d->doUpdate();
}


void DuiTextEditView::removeFocus(Qt::FocusReason reason)
{
    Q_D(DuiTextEditView);
    Q_UNUSED(reason);

    style().setModeDefault();
    d->focused = false;
    d->editActive = false;
    d->doUpdate();
}


void DuiTextEditView::applyStyle()
{
    Q_D(DuiTextEditView);

    d->maskTimer->setInterval(style()->maskingDelay());

    d->selectionFormat.setForeground(style()->selectionTextColor());
    d->selectionFormat.setBackground(style()->selectionBackgroundColor());

    // movement threshold for selection start
    qreal threshold = style()->selectionThreshold();
    if (threshold > 0) {
        d->selectionThreshold = threshold;
    }

    d->zoomable = style()->zoomable();

    QString maskString = style()->maskString();
    if (maskString.isEmpty() == false) {
        d->maskCharacter = maskString.at(0); // use only the first character
    }

    // Set document font
    d->textDocument->setDefaultFont(style()->font());
    d->promptTextDocument->setDefaultFont(style()->font());

    // Note: currently using fixed internal margin
    d->textDocument->setDocumentMargin(InternalMargin);
    d->promptTextDocument->setDocumentMargin(InternalMargin);

    if (d->maskedTextDocument != 0) {
        d->maskedTextDocument->setDefaultFont(style()->font());
        d->maskedTextDocument->setDocumentMargin(InternalMargin);
    }

    // font etc might affect size
    d->checkSize();

    DuiWidgetView::applyStyle();
}


DUI_REGISTER_VIEW_NEW(DuiTextEditView, DuiTextEdit)
