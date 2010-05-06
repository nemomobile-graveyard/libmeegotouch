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
#include "mcompleter.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextLayout>
#include <QAbstractTextDocumentLayout>
#include <QTimer>

#include "mfeedback.h"
#include "mtextedit.h"
#include "mtheme.h"
#include "mviewcreator.h"
#include "minfobanner.h"
#include "mlocale.h"
#include "mscenemanager.h"
#include "mtimestamp.h"

namespace
{
    const int LongPressTime = 600; //! how long to wait in tap&hold to initiate long press action
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
 * \param control MTextEdit widget which this view shows
 */
MTextEditViewPrivate::MTextEditViewPrivate(MTextEdit *control, MTextEditView *q)
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
      hideInfoBannerTimer(new QTimer(this))
{
    // copy text options from actual document to prompt
    QTextOption option = textDocument->defaultTextOption();
    promptTextDocument->setDefaultTextOption(option);

    selectionFormat.setForeground(Qt::white);
    selectionFormat.setBackground(Qt::black);

    longPressTimer->setSingleShot(true);
    longPressTimer->setInterval(LongPressTime);

    maskTimer->setSingleShot(true);
    maskTimer->setInterval(MaskedTimeInterval);

    hideInfoBannerTimer->setSingleShot(true);

    QObject::connect(longPressTimer, SIGNAL(timeout()), q, SLOT(handleLongPress()));
    QObject::connect(scrollTimer, SIGNAL(timeout()), this, SLOT(scrolling()));
    QObject::connect(maskTimer, SIGNAL(timeout()), this, SLOT(hideUnmaskedText()));
}


/*!
 * Destructor
 */
MTextEditViewPrivate::~MTextEditViewPrivate()
{
    delete maskedTextDocument;
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
int MTextEditViewPrivate::cursorPosition(QPointF hitPoint)
{
    Q_Q(MTextEditView);

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
void MTextEditViewPrivate::scrolling()
{
    Q_Q(MTextEditView);

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
        q->doUpdate();
    }
}


//! sets the mouse target point making sure it's inside the widget
void MTextEditViewPrivate::setMouseTarget(const QPointF &point)
{
    Q_Q(MTextEditView);

    mouseTarget.setX(qBound<qreal>(0.0, q->geometry().width(), point.x()));
    mouseTarget.setY(qBound<qreal>(0.0, q->geometry().height(), point.y()));
}


QAbstractTextDocumentLayout::PaintContext MTextEditViewPrivate::paintContext() const
{
    Q_Q(const MTextEditView);

    QAbstractTextDocumentLayout::PaintContext paintContext;
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

    } else if ((focused == true) && (q->model()->edit() == MTextEditModel::EditModeBasic)) {
        paintContext.cursorPosition = cursor.position();
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
QTextDocument *MTextEditViewPrivate::activeDocument() const
{
    Q_Q(const MTextEditView);

    MTextEditModel::EchoMode echoMode = q->model()->echo();

    if (echoMode == MTextEditModel::Normal ||
            (echoMode == MTextEditModel::PasswordEchoOnEdit && editActive == true) ||
            maskedTextDocument == 0) {
        return textDocument;

    } else {
        return maskedTextDocument;
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

        if (q->model()->echo() == MTextEditModel::Password) {
            maskCursor.insertText(newText);
            unmaskPosition = position;
            unmaskLength = charsAdded;
            maskTimer->start();

        } else { // PasswordEchoOnEdit, do immediate masking
            QString maskedText(newText.length(), maskCharacter);
            maskCursor.insertText(maskedText);
        }
    }

    q->doUpdate();
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
    if (0.7 *(dx + dy) < selectionThreshold)
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


QRect MTextEditViewPrivate::cursorRect() const
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
    Q_UNUSED(option);
    Q_D(const MTextEditView);

    mTimestamp("MTextEditView", QString("start text=%1").arg(d->document()->toPlainText()));
    painter->save();

    // set clipping rectangle to draw text inside the border
    QRectF clipping(boundingRect().left() + style()->paddingLeft(),
                    boundingRect().top() + style()->paddingTop(),
                    boundingRect().width() - style()->paddingLeft() - style()->paddingRight(),
                    boundingRect().height() - style()->paddingTop() - style()->paddingBottom());
    painter->setClipRect(clipping, Qt::IntersectClip);

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
    mTimestamp("MTextEditView", QString("end text=%1").arg(d->document()->toPlainText()));
}


void MTextEditView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MTextEditView);
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

    int cursor = d->cursorPosition(event);
    d->setMouseTarget(event->pos());

    // start timer to check if still holding mouse after a while
    d->longPressTimer->start();

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


void MTextEditView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEditView);

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
    d->longPressTimer->stop();
    d->scrollTimer->stop();
}


void MTextEditView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    updateCursorPosition(event, true);
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
        d->controller->setCursorPosition(cursor);
    }

    d->scrollingTestAndStart(event);
}


QSizeF MTextEditView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MTextEditView);

    QSizeF hint = MWidgetView::sizeHint(which, constraint);
    qreal minHeight = d->documentHeight + style()->paddingTop() + style()->paddingBottom();

    if (hint.height() < minHeight || model()->line() == MTextEditModel::SingleLine) {
        hint.setHeight(minHeight);
    }

    // FIXME: apply constraint?
    return hint;
}


void MTextEditView::changeEvent(QEvent *event)
{
    Q_D(MTextEditView);

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


void MTextEditView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);

    // restore state before as before mouse press
    Q_D(MTextEditView);
    d->selecting = false;
    d->inAutoSelectionClick = false;
    d->longPressTimer->stop();
    d->scrollTimer->stop();

    // hide completer if there is active one
    if (d->controller->completer() && d->controller->completer()->isActive()) {
        d->controller->completer()->hideCompleter();
    }
}


QVariant MTextEditView::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_D(const MTextEditView);

    if (query == Qt::ImFont) {
        return QVariant(); // FIXME: return currently used font

    } else if (query == Qt::ImMicroFocus) {
        return QVariant(d->cursorRect());
    } else if (static_cast<int>(query) == M::VisualizationPriorityQuery) {
        return QVariant(false);
    } else if (static_cast<int>(query) == M::PreeditRectangleQuery) {
        return QVariant(d->preeditRectangle());
    } else {
        return QVariant();
    }
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
            }

        } else if (member == MTextEditModel::Prompt) {
            d->promptTextDocument->setPlainText(model()->prompt());

            if (d->textDocument->isEmpty() == true) {
                viewChanged = true;
            }
        } else if (member == MTextEditModel::Document) {
            // this shouldn't really be happening
            qWarning("MTextEditView doesn't support changing the model's document member");
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

    d->hideInfoBannerTimer->start();

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
        //~ uispec-document DirectUI_Virtual_Keyboard_UI_Specification_0.30.doc
        //: Information banner to indicate that no
        //: characters could be pasted from the
        //: clipboard to this text field.
        //: Is shown when user tries to paste
        //: non-numeric text to a numeric or phone
        //: number text field.
        //% "Cannot paste text here"
        qtTrId("qtn_vkb_cantpaste"));

    d->controller->sceneManager()->appearSceneWindow(d->infoBanner, MSceneWindow::DestroyWhenDone);
    d->hideInfoBannerTimer->setInterval(duration);
    connect(d->hideInfoBannerTimer, SIGNAL(timeout()), this, SLOT(hideInfoBanner()));
}

void MTextEditView::handleLongPress()
{
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

    style().setModeSelected();
    if (reason == Qt::MouseFocusReason &&
            textEdit != 0 && textEdit->isAutoSelectionEnabled() == true) {
        // assuming the selection got made since the autoselection is enabled and focus was
        // gained. This click gets handled a bit differently because selection is already made
        // when the mouse press arrives.
        d->inAutoSelectionClick = true;
    }

    d->focused = true;
    doUpdate();
}


void MTextEditView::removeFocus(Qt::FocusReason reason)
{
    Q_D(MTextEditView);
    Q_UNUSED(reason);

    style().setModeDefault();
    d->focused = false;
    d->editActive = false;
    doUpdate();
}


void MTextEditView::applyStyle()
{
    Q_D(MTextEditView);

    d->maskTimer->setInterval(style()->maskingDelay());

    d->selectionFormat.setForeground(style()->selectionTextColor());
    d->selectionFormat.setBackground(style()->selectionBackgroundColor());

    // movement threshold for selection start
    qreal threshold = style()->selectionThreshold();
    if (threshold > 0) {
        d->selectionThreshold = threshold;
    }

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

    MWidgetView::applyStyle();
}


M_REGISTER_VIEW_NEW(MTextEditView, MTextEdit)
