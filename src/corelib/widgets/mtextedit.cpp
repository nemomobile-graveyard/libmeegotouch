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

#include "mtextedit.h"
#include "mtextedit_p.h"

#include <MDebug>
#include <QString>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QInputContext>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QTextBlock>
#include <QTextLayout>
#include <QTextDocumentFragment>
#include <QRegExp>
#include <QValidator>
#include <QRegExpValidator>
#include <QClipboard>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPointer>
#include <QTime>

#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mapplicationpage.h"
#include "mtheme.h"
#include "mbreakiterator.h"
#include "mnamespace.h"
#include "mpreeditinjectionevent.h"
#include "mwidgetview.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mcompleter.h"
#include "mscenemanager.h"
#include "minputmethodstate.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MTextEdit)

//! Helper class for arrow key navigation for focusable items.
class MArrowKeyNavigator
{
public:
    MArrowKeyNavigator()
        : skipNext(false)
        , haveSeenAnyKeyEvent(false)
    {
        time.start();
    }

    void skipNextRequest()
    {
        // Not valid to skip the *very first* request => cant be autorepeat,
        // by definition:
        if (haveSeenAnyKeyEvent) {
            skipNext = true;
        }
    }

    MSceneWindow *parentSceneWindow(QGraphicsWidget *widget) const
    {
        QGraphicsWidget *parent = widget;

        while (parent) {
            parent = parent->parentWidget();
            MSceneWindow *sm = qobject_cast<MSceneWindow *>(parent);

            if (sm) {
                return sm;
            }
        }

        return 0;
    }

    bool isChildOfSceneWindow(QGraphicsWidget *child,
                              const MSceneWindow *sceneWindow) const
    {
        return (parentSceneWindow(child) == sceneWindow);
    }

    bool operator()(QGraphicsItem *focusItem, QKeyEvent *ev, QTextCursor *cursor = 0)
    {
        // FIXME:
        // Instead of using QKeyEvent::isAutoRepeat(), we remember the elapsed time to measure auto-repeat.
        // It is not possible to use key release + storing last key for this (limitation in MInputContext).
        // Elapsed time for auto-repeat detection needs to be between 10 and 150 ms - the former is a
        // safeguard against programmtically sending key events (but still allows for some IM-related latency),
        // the latter is a guess.
        //
        // See NB#181350 - autoRepeat in QKeyEvents not set correctly on the application side
        haveSeenAnyKeyEvent = true;
        const bool isAutoRepeat = (ev->isAutoRepeat() || (10 < time.elapsed() && time.elapsed() < 150));
        time.restart();
        bool result = false;

        if ((ev->modifiers().testFlag(Qt::NoModifier) ||
            ev->modifiers().testFlag(Qt::GroupSwitchModifier)) &&
            !isAutoRepeat &&
            !skipNext) {
            QGraphicsItem *nextItem = findNextFocusItem(focusItem, ev->key(), cursor);

            if (nextItem) {
                nextItem->setFocus(Qt::OtherFocusReason);
                result = true;
            }
        }

        skipNext = false;
        return result;
    }

private:
    QTime time;
    bool skipNext;
    bool haveSeenAnyKeyEvent;

    // This one is expensive, as it iterates over all scene items!
    QGraphicsItem *findNextFocusItem(QGraphicsItem *focusItem, int key, QTextCursor *cursor) const
    {
        // Initialize cursorAt* to true if no cursor was specified.
        // Keeps conditions small and clean.
        const bool cursorAtStart = cursor ? cursor->atStart() : true;
        const bool cursorAtEnd = cursor ? cursor->atEnd() : true;

        if (!focusItem ||
            !focusItem->hasFocus() ||
            !focusItem->scene() ||
            !isArrowKeyNavigationRequest(key, cursorAtStart, cursorAtEnd)) {
            return 0;
        }

        const MSceneWindow *const sm = parentSceneWindow(dynamic_cast<QGraphicsWidget *>(focusItem));
        QGraphicsItem *nextItem = 0;
        int distance = INT_MAX;

        const QPoint center = focusItem->mapToScene(focusItem->pos() +
                                                    focusItem->boundingRect().center())
                              .toPoint();

        foreach(QGraphicsItem *item, focusItem->scene()->items()) {

            // TODO: Make it work for other focusable items, not just those who accept IM:
            if (item != focusItem &&
                item->flags().testFlag(QGraphicsItem::ItemAcceptsInputMethod) &&
                isChildOfSceneWindow(dynamic_cast<QGraphicsWidget *>(item), sm)) {
                bool found = false;
                const QPoint targetCenter = item->mapToScene(item->pos() +
                                                             item->boundingRect().center())
                                            .toPoint();

                switch(key) {
                case Qt::Key_Left:
                    found = isTargetLeftOf(targetCenter, center);
                    break;

                case Qt::Key_Up:
                    found = isTargetTopOf(targetCenter, center);
                    break;

                case Qt::Key_Right:
                    found = isTargetRightOf(targetCenter, center);
                    break;

                case Qt::Key_Down:
                    found = isTargetBottomOf(targetCenter, center);
                    break;

                default:
                    break;
                }

                if (found) {
                    const int currentDistance = qMin(distance,
                                                     (targetCenter - center).manhattanLength());

                    if (currentDistance < distance) {
                        distance = currentDistance;
                        nextItem = item;
                    }
                }
            }
        }

        return nextItem;
    }

    bool isArrowKeyNavigationRequest(int key, bool cursorAtStart, bool cursorAtEnd) const
    {
        return (((key == Qt::Key_Left || key == Qt::Key_Up) && cursorAtStart) ||
               ((key == Qt::Key_Right || key == Qt::Key_Down) && cursorAtEnd));

    }

    int fallingDiagonalFor(int x, const QPoint &p) const
    {
        return (x + p.y() - p.x());
    }

    int raisingDiagonalFor(int x, const QPoint &p) const
    {
        return (-x + p.y() + p.x());
    }

    bool isTargetLeftOf(const QPoint &target, const QPoint &origin) const
    {
        if (target.y() > fallingDiagonalFor(target.x(), origin) &&
            target.y() < raisingDiagonalFor(target.x(), origin)) {
            return true;
        }

        return false;
    }

    bool isTargetRightOf(const QPoint &target, const QPoint &origin) const
    {
        if (target.y() < fallingDiagonalFor(target.x(), origin) &&
            target.y() > raisingDiagonalFor(target.x(), origin)) {
            return true;
        }

        return false;
    }

    bool isTargetTopOf(const QPoint &target, const QPoint &origin) const
    {
        if (target.y() < fallingDiagonalFor(target.x(), origin) &&
            target.y() < raisingDiagonalFor(target.x(), origin)) {
            return true;
        }

        return false;
    }

    bool isTargetBottomOf(const QPoint &target, const QPoint &origin) const
    {
        if (target.y() > fallingDiagonalFor(target.x(), origin) &&
            target.y() > raisingDiagonalFor(target.x(), origin)) {
            return true;
        }

        return false;
    }
};

namespace
{
    //! character set for Number field type
    const QString NumberCharacterSet = QString("[\\+\\-]{0,1}([0-9%1-%2]+(\\.[0-9%1-%2]*){0,1}){1,1}")
        .arg(QChar(0x0660)) // Arabic numbers begin
        .arg(QChar(0x0669)); // Arabic numbers end

    //! character set for Phone Number field type
    const QString PhoneNumberCharacterSet = QString("[\\+0-9%1-%2#\\*\\-\\+pw.\\/() ]+")
        .arg(QChar(0x0660)) // Arabic numbers begin
        .arg(QChar(0x0669)); // Arabic numbers end

    //! character set for Email field type
    const QString EmailCharacterSet       = "[a-zA-Z0-9.!#$%&'*+-\\/=?\\^_\\`{|}~@]+";

    const QChar PlusSign('+');
    const QChar MinusSign('-');

    //! Character set for line break.
    const QString LineBreakSet = QString("\n%1%2")
        .arg(QChar(0x2028))
        .arg(QChar(0x2029));

    MArrowKeyNavigator gArrowKeyNav;
}



/*!
 * \brief Constructor
 * \param type widget type (single line or multiline)
 */
MTextEditPrivate::MTextEditPrivate()
    : pendingSoftwareInputPanelRequest(false),
      focusEventState(NoFocusEventReceivedYet),
      validator(0),
      ownValidator(false),
      completer(0),
      registeredToolbarId(-1),
      editActive(false),
      omitInputMethodEvents(false),
      updateMicroFocusDisabled(0),
      pendingMicroFocusUpdate(false)
{
}


/*!
 * \brief Destructor
 */
MTextEditPrivate::~MTextEditPrivate()
{
    if (ownValidator == true) {
        delete validator;
    }
}

void MTextEditPrivate::init()
{
    Q_Q(MTextEdit);
    Q_ASSERT_X((q->model() != 0), "MTextEditPrivate::init()", "No model found!");

    QTextOption option = q->document()->defaultTextOption();
    option.setTextDirection(q->layoutDirection());

    if (q->model()->line() == MTextEditModel::SingleLine) {
        option.setWrapMode(QTextOption::NoWrap);
        q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    } else {
        //Set to expand vertically only in multiline mode
        QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        policy.setHeightForWidth(true);
        q->setSizePolicy(policy);
    }

    q->document()->setDefaultTextOption(option);
    q->model()->cursor()->movePosition(QTextCursor::End);
    q->setFocusPolicy(Qt::ClickFocus);
    q->setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);
}

QTextCursor *MTextEditPrivate::cursor() const
{
    Q_Q(const MTextEdit);
    return q->model()->cursor();
}

/*!
  * \brief Moves cursor, parameters as in QTextCursor::movePosition().
  * \return True if operation compeled successfully.
  */
bool MTextEditPrivate::moveCursor(QTextCursor::MoveOperation moveOp,
                                  QTextCursor::MoveMode moveMode, int n)
{
    Q_Q(MTextEdit);
    bool result;

    // Cursor movement will enter basic mode
    commitPreedit();
    if (moveMode == QTextCursor::MoveAnchor) {
        q->deselect();
    }

    result = cursor()->movePosition(moveOp, moveMode, n);
    if (result) {
        q->model()->updateCursor();
        updateMicroFocus();
        emit q->cursorPositionChanged();
    }

    return result;
}


/*!
 * \brief Move cursor to begin/end if movement up/down failed.
 * \return True if operation compeled successfully.
 */
bool MTextEditPrivate::smartMoveCursor(QTextCursor::MoveOperation moveOp,
                                       QTextCursor::MoveMode moveMode, int n)
{
    bool result = moveCursor(moveOp, moveMode, n);

    if (!result) {
        switch (moveOp) {
        case QTextCursor::Up:
            result = moveCursor(QTextCursor::StartOfLine, moveMode, 1);
            break;
        case QTextCursor::Down:
            result = moveCursor(QTextCursor::EndOfLine, moveMode, 1);
            break;
        default:
            break;
        }
    }

    return result;
}


/*!
  * \brief Backspace functionality, doesn't change mode
  */
bool MTextEditPrivate::doBackspace()
{
    Q_Q(MTextEdit);

    if (q->isReadOnly()) {
        return false;
    }

    QTextCursor currentPositionCursor = q->textCursor();
    int position = currentPositionCursor.position();

    if (position == 0) {
        return false;
    }

    currentPositionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    QTextDocumentFragment currentFragment = currentPositionCursor.selection();

    QTextCharFormat format;
    // setPosition() is required to get the style that would be applied when a text is inserted at the cursor position
    currentPositionCursor.setPosition(position);
    format = currentPositionCursor.charFormat();

    cursor()->deletePreviousChar();

    if (validateCurrentBlock() == true) {
        cursor()->setCharFormat(format);
        return true;

    } else {
        // document doesn't validate after delete -> put the character back
        cursor()->insertFragment(currentFragment);
        return false;
    }
}


/*!
  * \brief Deletes a character at cursor position
  */
bool MTextEditPrivate::doDelete()
{
    Q_Q(MTextEdit);

    if (q->isReadOnly()) {
        return false;
    }

    QTextCursor currentPositionCursor(q->textCursor());
    currentPositionCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    QTextDocumentFragment currentFragment = currentPositionCursor.selection();
    cursor()->deleteChar();

    if (!validateCurrentBlock()) {
        // document doesn't validate after delete -> put the character back
        cursor()->insertFragment(currentFragment);
        return false;
    }

    return true;
}


/*!
  * \brief Insert tab to the char stream & move cursor in front of it
  */
bool MTextEditPrivate::doTab()
{
    // TODO: missing Qt::tab action - this is on purpose or not?
    return doTextInsert("\t");
}


/*!
  * \brief Inserts a line break if editor is multiline,
  * otherwise emits signal returnPressed() if it is allowed by validator.
  */
bool MTextEditPrivate::onReturnPressed(QKeyEvent *event)
{
    Q_Q(MTextEdit);
    bool result = false;

    if (q->model()->line() == MTextEditModel::SingleLine) {
        if (q->hasAcceptableInput()) {
            emit q->returnPressed();
        }
        event->accept();
    } else {
        result = doTextInsert("\n");
    }

    return result;
}

/*!
 * \brief Inserts sign before number or cycle between + and -
 * \return True if text was changed
 */
bool MTextEditPrivate::doSignCycle()
{
    Q_Q(MTextEdit);

    if (q->contentType() != M::NumberContentType) {
        return false;
    }

    QTextCursor editCursor(q->textCursor());
    QChar firstChar(q->document()->characterAt(0));
    bool validChange = false;

    editCursor.setPosition(0);
    if (firstChar != PlusSign && firstChar != MinusSign) {
        firstChar = QChar();
    } else {
        // we are going to replace existing sign
        editCursor.setPosition(1, QTextCursor::KeepAnchor);
    }

    editCursor.insertText(firstChar == MinusSign ? PlusSign : MinusSign);

    validChange = validateCurrentBlock();
    if (!validChange) {
        // undo our change
        editCursor.deletePreviousChar();
        if (!firstChar.isNull()) {
            editCursor.insertText(firstChar);
        }
    }

    return validChange;
}


/*!
  * \brief Inserts text to the cursor position
  * \param text text to be inserted
  * \return true if some text was successfully inserted
  */
bool MTextEditPrivate::doTextInsert(const QString &text, bool usePreeditStyling)
{
    Q_Q(MTextEdit);

    if (q->isReadOnly() == true) {
        return false;
    }

    bool changed = false;
    QString filteredText = text;

    // Bug in QTextDocument::characterCount?
    int characterCount = q->document()->characterCount() - 1;
    Q_ASSERT(characterCount >= 0);

    // Total characterCount mustn't exceed maxLength.
    if (characterCount + filteredText.length() > q->maxLength()) {
        filteredText.truncate(q->maxLength() - characterCount);
        changed = true;
    }

    // on single line newline are changed into spaces
    if (q->lineMode() == MTextEditModel::SingleLine) {
        filteredText = replaceLineBreaks(filteredText, QChar(' '));
    }

    int textPosition = 0;
    int filteredTextLength = filteredText.length();
    int snippetLength = -1;
    int listIndex = -1;
    int count = 0;

    do {
        if (textPosition >= filteredTextLength) {
            break;
        }

        if (filteredText.at(textPosition) == QChar('\n')) {
            // newline is appended separately. they are only available here on multiline
            // mode, so they are permitted
            cursor()->insertText(QChar('\n'));
            textPosition++;
            changed = true;
            continue;
        }

        // text is inserted up to one line at a time because otherwise new blocks could be created
        int nextNewline = filteredText.indexOf(QChar('\n'), textPosition, Qt::CaseInsensitive);
        snippetLength = nextNewline;

        if (snippetLength != -1) {
            snippetLength = snippetLength - textPosition;
        }

        QString textSnippet = filteredText.mid(textPosition, snippetLength);

        int cursorPosBefore = cursor()->position();

        if (usePreeditStyling == true) {
            insertTextWithPreeditStyling(textSnippet, listIndex, count);
        } else {
            cursor()->insertText(textSnippet);
        }

        if (validateCurrentBlock() == true) {
            changed = true;

        } else {
            // validation failed, need to restore the previous state
            cursor()->setPosition(cursorPosBefore, QTextCursor::KeepAnchor);
            cursor()->removeSelectedText();
        }

        textPosition += snippetLength;
    } while (snippetLength != -1);

    return changed;
}


bool MTextEditPrivate::validate()
{
    Q_Q(MTextEdit);

    if (validator == 0) {
        return true;
    } else {
        QString textCopy = q->model()->document()->toPlainText();
        int cursorCopy = q->model()->cursor()->position();
        QValidator::State result = validator->validate(textCopy, cursorCopy);
        return (result != QValidator::Invalid);
    }
}


bool MTextEditPrivate::validateCurrentBlock()
{
    if (validator == 0) {
        return true;
    }

    QTextBlock currentBlock = cursor()->block();
    QString blockText = currentBlock.text();
    int blockPosition = cursor()->position() - currentBlock.position();

    QString blockTextCopy = blockText;
    int blockPositionCopy = blockPosition;

    QValidator::State validationResult = validator->validate(blockTextCopy, blockPositionCopy);

    if (validationResult != QValidator::Invalid) {
        if (blockTextCopy != blockText) {
            // mDebug("MTextEditPrivate") << __PRETTY_FUNCTION__ << "replacing" << blockText
            //          << "with" << blockTextCopy;
            // the validator changed the string somehow, need to set block content again
            QTextCursor blockCursor(*cursor());

            // first clear the content of the block
            int start = currentBlock.position();
            QString currentContent = currentBlock.text();
            int end = start + currentContent.length();

            blockCursor.setPosition(start);
            blockCursor.setPosition(end, QTextCursor::KeepAnchor);
            // mDebug("MTextEditPrivate") << "Start of selection:" << start;
            // mDebug("MTextEditPrivate") << "End of selection:" << blockCursor.position();
            blockCursor.removeSelectedText();

            // set to new values
            blockCursor.insertText(blockTextCopy);
            cursor()->setPosition(blockPositionCopy + currentBlock.position());
        }

        return true;

    } else {
        return false;
    }
}


/*!
  * \brief sets preedit to given parameters with given formatting attributes
  */
void MTextEditPrivate::setPreeditText(const QString &text,
                                        const QList<QInputMethodEvent::Attribute> &attributes)
{
    QTextCursor *textCursor = cursor();

    // Remove old pre-edit text
    removePreedit();

    int preeditTextLength = text.count();

    QTextBlock block = textCursor->block();
    QTextLayout *layout = block.layout();

    QList<QTextLayout::FormatRange> preeditStyles;

    // parse attributes
    const int size = attributes.size();
    for (int i = 0; i < size; ++i) {
        const QInputMethodEvent::Attribute &attribute = attributes.at(i);
        if (attribute.type == QInputMethodEvent::TextFormat) {
            const QTextCharFormat format = attribute.value.value<QTextFormat>().toCharFormat();

            if (format.isValid()) {
                QTextLayout::FormatRange style;
                style.start = attribute.start + textCursor->position() - block.position();
                style.length = attribute.length;
                style.format = format;
                preeditStyles.append(style);
            }
        }
        // TODO: should honor Cursor attribute too
    }

    // set the preedit styling as additional format of the current qtextlayout.
    // preedit is implemented as selected normal text with additional formatting on current
    // QTextLayout. Using the additional formats here seems a bit dangerous. In principle Qt
    // says the layout from a block shouldn't be changed except from
    // QAbstractTextDocumentLayout::documentChanged(), but in reality e.g. QTextEdit
    // uses it like this, and even sets the preedit to the layout.
    // If this becomes problematic, we should move this formatting to paintContext of the view.
    layout->setAdditionalFormats(preeditStyles);

    int listIndex = -1;
    int count = 0;
    insertTextWithPreeditStyling(text, listIndex, count);
    clearUnusedPreeditStyling(listIndex, count);

    // mark preedit as selection
    int position = textCursor->position();
    textCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor,
                             preeditTextLength);
    textCursor->setPosition(position, QTextCursor::KeepAnchor);
}


/*!
  * \brief Commit current pre-edit so it becomes normal text
  */
void MTextEditPrivate::commitPreedit()
{
    Q_Q(MTextEdit);

    preeditStyling.clear();

    // Nothing to commit if not pre-editing
    if (isPreediting() == false) {
        return;
    }

    QTextCursor *textCursor = cursor();

    int characterCount = q->document()->characterCount() - 1;
    if (characterCount > q->maxLength()) {
        // Set markers for current selection as follows:
        // <start> Preserved preedit text <startErase> Erased preedit text <end>
        // "Erased preedit text" is the part that causes overall length to exceed maxLength().
        int exceedingCharCount = characterCount - q->maxLength();
        int start = textCursor->selectionStart();
        int end = textCursor->selectionEnd();
        int startErase = end - exceedingCharCount;

        if (startErase < start) {
            // Character count exceeded maxLength even before this preedit.
            // Only erase the whole preedit text.
            startErase = start;
        }

        // Erase and then set new modified selection.
        textCursor->setPosition(startErase, QTextCursor::MoveAnchor);
        textCursor->setPosition(end, QTextCursor::KeepAnchor);
        textCursor->removeSelectedText();
        textCursor->setPosition(start, QTextCursor::MoveAnchor);
        textCursor->setPosition(startErase, QTextCursor::KeepAnchor);
    }

    // clear styling
    QTextBlock block = textCursor->block();
    QTextLayout *layout = block.layout();
    layout->clearAdditionalFormats();

    if (validateCurrentBlock() == true) {
        // make preedit selection part of the normal text
        textCursor->clearSelection();

    } else {
        // block content with the preedit doesn't validate, need to remove the commit string
        textCursor->removeSelectedText();
    }

    setMode(MTextEditModel::EditModeBasic);
    updateMicroFocus();

    if (q->hasFocus()) {
        // make sure the committed preedit doesn't get left on the input context
        QInputContext *ic = qApp->inputContext();
        if (ic) {
            omitInputMethodEvents = true;
            ic->reset();
            omitInputMethodEvents = false;
        }
    }

    emit q->textChanged();
}


/*!
 * \brief Removes pre-edit text, doesn't affect mode state
 */
void MTextEditPrivate::removePreedit()
{
    if (isPreediting() == false) {
        return;
    }

    QTextBlock block = cursor()->block();
    QTextLayout *layout = block.layout();
    layout->clearAdditionalFormats();

    cursor()->removeSelectedText();
}



/*!
 * Set text edit mode
 * NOTE: every function calling setMode is potential candidate
 * to emit signal copyAvailable(bool)
 * \param mode text edit mode
 */
void MTextEditPrivate::setMode(MTextEditModel::EditMode mode)
{
    Q_Q(MTextEdit);

    q->model()->setEdit(mode);
}


/*!
  * \brief Set cursor position.
  * \return false if \a index is an invalid cursor postion.
  */
bool MTextEditPrivate::setCursorPosition(int index)
{
    Q_Q(MTextEdit);

    bool val = false;
    QTextBlock block = q->document()->findBlock(index);

    if (block.isValid()) {
        val = true;
        commitPreedit(); // note: also calls updateMicroFocus()
        if (index != cursor()->position()) {
            cursor()->setPosition(index);
            emit q->cursorPositionChanged();
            q->model()->updateCursor();
        }
    }

    return val;
}

/*!
 * \brief Returns true if given position is on pre-edit string.
 * Note: preedit starting position is not considered to be on top
 */
bool MTextEditPrivate::isPositionOnPreedit(int cursorPosition) const
{
    if (isPreediting() == false) {
        return false;
    }

    QTextCursor *textCursor = cursor();
    int start = textCursor->anchor();
    int end = textCursor->position();
    return (cursorPosition > start && cursorPosition < end);
}


bool MTextEditPrivate::isPreediting() const
{
    Q_Q(const MTextEdit);

    return (q->mode() == MTextEditModel::EditModeActive);
}

void MTextEditPrivate::requestSip()
{
    QInputContext *inputContext = qApp->inputContext();
    QGraphicsView *focusedGraphicsView = dynamic_cast<QGraphicsView *>(QApplication::focusWidget());

    if (!inputContext || !focusedGraphicsView) {
        return;
    }

    // FIXME: this is a temporary workaround because of the
    // QGraphicsView unable to correctly update the attribute.
    // We're waiting for fixing this on Qt side.
    focusedGraphicsView->setAttribute(Qt::WA_InputMethodEnabled, true);

    // Enforce update if focus is moved from one MTextEdit to other.
    // If WA_InputMethodEnabled is not set then Qt will call
    // setFocusWidget automatically
    inputContext->setFocusWidget(focusedGraphicsView);

    MInputMethodState::requestSoftwareInputPanel();
    pendingSoftwareInputPanelRequest = false;

    Q_Q(MTextEdit);

    if (q->sceneManager()) {
        q->sceneManager()->ensureCursorVisible();
    }
}

void MTextEditPrivate::requestAutoSip(Qt::FocusReason fr)
{
    Q_Q(MTextEdit);

    if (!q->isAutoSipEnabled()) {
        return;
    }

    if (fr == Qt::MouseFocusReason) {
        // Wait for the mouse release event instead so that the window relocation that might
        // happen does not change the mouse position *before* the button is released.
        pendingSoftwareInputPanelRequest = true;
    } else {
        requestSip();
    }
}

void MTextEditPrivate::closeSip()
{
    MInputMethodState::closeSoftwareInputPanel();
    pendingSoftwareInputPanelRequest = false;
}

void MTextEditPrivate::closeAutoSip()
{
    Q_Q(MTextEdit);

    if (!q->isAutoSipEnabled()) {
        return;
    }

    closeSip();
}

/*!
 * \brief Sends mouse events to input context mouse handling method
 * \param position cursor position
 * \param event event information
 */
void MTextEditPrivate::notifyInputContextMouseHandler(int position,
        QGraphicsSceneMouseEvent *event)
{
    if (isPositionOnPreedit(position) == false) {
        // only send events on top of preedit
        return;
    }

    QInputContext *ic = qApp->inputContext();

    if (ic == 0) {
        return;
    }

    // translate to normal mouse event and send to input context
    QEvent::Type mouseEventType = translateGraphicsSceneMouseTypeToQMouse(event->type());
    QPoint mousePoint = event->screenPos(); // right?
    QMouseEvent mouseEvent(mouseEventType, mousePoint, event->button(),
                           event->buttons(), event->modifiers());
    int preeditOffset = position - cursor()->anchor();
    ic->mouseHandler(preeditOffset, &mouseEvent);
}


/*!
 * \brief Emits signal copyAvailable if content echo mode is normal
 * \param yes bool Signal parameter
 */
void MTextEditPrivate::sendCopyAvailable(bool yes)
{
    Q_Q(MTextEdit);
    if (q->echoMode() != MTextEditModel::Normal) {
        return;
    }

    emit q->copyAvailable(yes);
}


/*!
 * \brief Translates qgraphics mouse event type to plain mouse type (static)
 * \param input event type
 */
QEvent::Type MTextEditPrivate::translateGraphicsSceneMouseTypeToQMouse(QEvent::Type input)
{
    QEvent::Type result;

    switch (input) {
    case QEvent::GraphicsSceneMouseDoubleClick:
        result = QEvent::MouseButtonDblClick;
        break;

    case QEvent::GraphicsSceneMousePress:
        result = QEvent::MouseButtonPress;
        break;

    case QEvent::GraphicsSceneMouseRelease:
        result = QEvent::MouseButtonRelease;
        break;

    case QEvent::GraphicsSceneMouseMove:
        result = QEvent::MouseMove;
        break;

    default:
        result = QEvent::None;
    }

    return result;
}

/*!
 * \brief stores the style information of text in pre-edit mode
 * \param start position from where to start reading the style for storing
 * \param end position where to end reading the style
 */
void MTextEditPrivate::storePreeditTextStyling(int start, int end)
{
    QTextCursor *textCursor = cursor();
    int cursorPosition = textCursor->position();

    for (int i = start + 1; i <= end; ++i) {
        textCursor->setPosition(i, QTextCursor::KeepAnchor);
        QTextCharFormat charFormat = textCursor->charFormat();

        if (preeditStyling.isEmpty() == false) {
            styleData &currentStyle = preeditStyling.last();

            if (currentStyle.charFormat != charFormat) {
                styleData newStyle;

                newStyle.charFormat = charFormat;
                newStyle.count = 1;
                preeditStyling.push_back(newStyle);
            } else {
                currentStyle.count++;
            }
        } else {
            styleData newStyle;

            newStyle.charFormat = charFormat;
            newStyle.count = 1;
            preeditStyling.push_back(newStyle);
        }
    }

    textCursor->setPosition(cursorPosition, QTextCursor::KeepAnchor);
}


/*!
 * \brief inserts text by applying the stored preedit styling information
 * \param currentListIndex styling list index to start reading from the stored styling
 * \param currentCount character index within the current styling list index
 */
void MTextEditPrivate::insertTextWithPreeditStyling(const QString &text, int &currentListIndex, int &currentCount)
{
    QTextCursor *textCursor = cursor();
    const int textLength = text.length();
    int listIndex = currentListIndex;
    int count = currentCount;
    int preeditStyleSize = preeditStyling.size();
    int currentIndex = textLength;

    QTextCharFormat format = textCursor->charFormat();

    styleData newStyle;
    newStyle.charFormat = format;
    newStyle.count = 1;

    if (preeditStyleSize == 0) {
        preeditStyling.push_back(newStyle);
        preeditStyleSize++;
    }

    for (int i = 0; i < textLength; ++i) {
        if (preeditStyling.isEmpty() != true) {
            if (count == 0) {
                listIndex++;
                if (listIndex == preeditStyleSize) {
                    QTextCharFormat charFormat = preeditStyling.at(preeditStyleSize - 1).charFormat;
                    textCursor->setCharFormat(charFormat);
                    currentIndex = i;
                    listIndex = preeditStyleSize - 1;
                    break;
                }

                count = preeditStyling.at(listIndex).count;
            }

            if (count == 0) {
                styleData &lastStyle = preeditStyling[listIndex];
                lastStyle.count++;
                count = 1;
            }

            QTextCharFormat charFormat = preeditStyling.at(listIndex).charFormat;
            textCursor->setCharFormat(charFormat);
            count--;
        }

        textCursor->insertText(text.at(i));
    }

    for (int i = currentIndex; i < textLength; ++i) {
        if (preeditStyling.isEmpty() != true) {
            styleData &newStyle = preeditStyling[listIndex];
            newStyle.count++;
        }
        textCursor->insertText(text.at(i));
    }

    currentListIndex = listIndex;
    currentCount = count;
}


/*!
 * \brief adds a style to the stored preedit styling
 * \param StyleType current style type
 */
void MTextEditPrivate::addStyleToPreeditStyling(StyleType currentStyleType, bool setValue)
{
    Q_Q(MTextEdit);
    QTextCharFormat format;
    int preeditStyleSize = preeditStyling.size();

    if (preeditStyleSize > 0) {
        format = preeditStyling[preeditStyleSize - 1].charFormat;
    }

    if (currentStyleType == Underline) {
        format.setFontUnderline(setValue);
    } else if (currentStyleType == Italic) {
        format.setFontItalic(setValue);
    } else { // bold
        QFont font = q->document()->defaultFont();
        QFont::Weight defaultWeight = static_cast<QFont::Weight>(font.weight());
        QFont::Weight wt = (setValue ? QFont::Bold : defaultWeight);

        format.setFontWeight(wt);
    }

    if ((preeditStyleSize > 0) && (preeditStyling[preeditStyleSize - 1].count == 0)) {
        preeditStyling[preeditStyleSize - 1].charFormat = format;
    } else {
        styleData currentStyle;
        currentStyle.charFormat = format;
        currentStyle.count = 0;
        preeditStyling.push_back(currentStyle);
    }
}


/*!
 * \brief clears the unused styling from stored preedit styling information
 * \param currentListIndex styling list index to start erasing from the stored styling
 * \param currentCount character index within the current styling list index
 */
void MTextEditPrivate::clearUnusedPreeditStyling(int currentListIndex, int currentCount)
{
    int preeditTextStyleSize = preeditStyling.size();

    if ((currentListIndex < 0) || (currentListIndex >= preeditTextStyleSize)) {
        return;
    }

    styleData &style = preeditStyling[currentListIndex];
    style.count -= currentCount;

    for (int i = currentListIndex + 1; i < preeditTextStyleSize; i++) {
        preeditStyling.removeAt(i);
    }

    if (style.count == 0) {
        preeditStyling.removeAt(currentListIndex);
    }
}

void MTextEditPrivate::_q_confirmCompletion(const QString &completion)
{
    Q_Q(MTextEdit);
    if (!completer)
        return;
    //select prefix
    QString prefix = completer->completionPrefix();
    QTextBlock block = cursor()->block();
    int cursorPos = cursor()->position() - block.position();
    QString text = block.text();
    int index = -1;
    do {
        index++;
        index = text.indexOf(prefix, index);
    } while ((index >= 0) && (index < cursorPos - prefix.length()));

    //select prefix, then insert confirmed completion, this way is faster than backspacing
    commitPreedit();
    cursor()->setPosition(index + block.position());
    cursor()->setPosition(index + block.position() + prefix.length(), QTextCursor::KeepAnchor);
    cursor()->removeSelectedText();
    doTextInsert(completion);
    QObject::disconnect(q, SIGNAL(textChanged()), completer, SLOT(complete()));
    emit q->textChanged();
    QObject::connect(q, SIGNAL(textChanged()), completer, SLOT(complete()));
    updateMicroFocus();
}

bool MTextEditPrivate::copy()
{
    Q_Q(MTextEdit);
    QClipboard *clipboard = QApplication::clipboard();

    if (!q->hasSelectedText()
            || q->echoMode() != MTextEditModel::Normal // only allow copy from normal echo mode entry
            || !clipboard) {
        return false;
    }

    clipboard->setText(cursor()->selectedText());
    return true;
}

QString MTextEditPrivate::replaceLineBreaks(QString text, QChar replacement)
{
    // FIXME: this implementation works quite slow if text or LineBreakSet is long
    foreach (QChar lineBreak, LineBreakSet) {
        text.replace(lineBreak, replacement);
    }

    return text;
}

void MTextEditPrivate::disableUpdateMicroFocus()
{
    ++updateMicroFocusDisabled;
}

void MTextEditPrivate::enableUpdateMicroFocus(bool flush)
{
    Q_Q(MTextEdit);
    Q_ASSERT(updateMicroFocusDisabled);
    if ((--updateMicroFocusDisabled == 0) && pendingMicroFocusUpdate && flush) {
        q->updateMicroFocus();
    }
    pendingMicroFocusUpdate = false;
}

void MTextEditPrivate::updateMicroFocus()
{
    Q_Q(MTextEdit);

    if (updateMicroFocusDisabled <= 0) {
        q->updateMicroFocus();
        pendingMicroFocusUpdate = false;
    } else {
        pendingMicroFocusUpdate = true;
    }
}


///////////////////////////////////////////////
// Actual class implementation


MTextEdit::MTextEdit(MTextEditModel::LineMode type, const QString &text,
                         QGraphicsItem *parent)
    : MWidgetController(new MTextEditPrivate, new MTextEditModel, parent)
{
    Q_D(MTextEdit);

    model()->setDocument(new QTextDocument(text, this->model()));
    model()->setLine(type);
    d->init();
}

MTextEdit::MTextEdit(MTextEditModel *model, QGraphicsItem *parent)
    : MWidgetController(new MTextEditPrivate, model, parent)
{
    Q_D(MTextEdit);

    d->init();
}

MTextEdit::MTextEdit(MTextEditPrivate *dd, MTextEditModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MTextEdit);

    d->init();
}

MTextEdit::~MTextEdit()
{
    Q_D(MTextEdit);

    if (d->focusEventState == MTextEditPrivate::FocusInEventReceived) {
        d->closeAutoSip();
    }

    detachToolbar();

    // TODO: This cannot be right - MTextEdit does not own the model, so we cannot just delete stuff from the model (another text edit could be using them).
    // Revise after MR#434 has been merged!

    // kludge so view doesn't receive memberModified() signals for the deleted pointers
    model()->beginTransaction();

    //should delete QTextDocument and QTextCursor
    delete model()->document();
    delete model()->cursor();

    model()->setDocument(NULL);
    model()->setCursor(NULL);
}


void MTextEdit::setSelection(int start, int length, bool useBoundaries)
{
    Q_D(MTextEdit);

    if (!isSelectionEnabled()) {
        return;
    }

    QString text = document()->toPlainText();

    // boundary check for positions
    start = qBound(0, start, text.length());
    length = qBound(-start, length, text.length() - start);

    d->commitPreedit();

    // determine beginning of selection and the end
    int begin;
    int end;

    if (length > 0) {
        begin = start;
        end = start + length;
    } else {
        begin = qMax(start + length, 0);
        end = begin - length;
    }

    if (useBoundaries) {
        MBreakIterator breakIterator(text);
        begin = breakIterator.previousInclusive(begin);

        if (breakIterator.isBoundary(end) == false) {
            end = breakIterator.next(end);
        }
    }

    // check if change actually happens
    QTextCursor *currentCursor = d->cursor();
    if (begin == currentCursor->anchor() && end == currentCursor->position()) {
        return;
    }

    // make an actual selection
    currentCursor->setPosition(begin);
    currentCursor->setPosition(end, QTextCursor::KeepAnchor);

    // update mode to selection or basic if needed
    if (begin != end) {
        if (mode() != MTextEditModel::EditModeSelect) {
            d->setMode(MTextEditModel::EditModeSelect);
            d->sendCopyAvailable(true);
        }
    } else if (mode() != MTextEditModel::EditModeBasic) {
        bool wasSelecting = hasSelectedText();
        d->setMode(MTextEditModel::EditModeBasic);

        if (wasSelecting) {
            d->sendCopyAvailable(false);
        }
    }

    model()->updateCursor();

    d->updateMicroFocus();

    emit selectionChanged();

    return;
}


QString MTextEdit::selectedText() const
{
    if (mode() != MTextEditModel::EditModeSelect) {
        return QString();
    } else {
        return textCursor().selectedText();
    }
}


bool MTextEdit::hasSelectedText() const
{
    return mode() == MTextEditModel::EditModeSelect;
}

int MTextEdit::selectionStart() const
{
    if (mode() == MTextEditModel::EditModeSelect) {
        return textCursor().selectionStart();
    } else {
        return -1;
    }
}


bool MTextEdit::isSelectionEnabled() const
{
    return (textInteractionFlags() &
            (Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard)) != 0;
}

void MTextEdit::keyPressEvent(QKeyEvent *event)
{
    Q_D(MTextEdit);

    // assuming that just moving the cursor or text copying requires some interaction flag
    if (textInteractionFlags() == Qt::NoTextInteraction) {
        return;
    }

    if (gArrowKeyNav(this, event, model()->cursor())) {
        return;
    }

    QTextCursor::MoveOperation moveDirection = QTextCursor::NoMove;
    int key = event->key();
    bool wasSelecting = hasSelectedText();

    // first check if this is cursor movement
    switch (key) {
    case Qt::Key_Left:
        moveDirection = QTextCursor::Left;
        break;

    case Qt::Key_Right:
        moveDirection = QTextCursor::Right;
        break;

    case Qt::Key_Down:
        moveDirection = QTextCursor::Down;
        break;

    case Qt::Key_Up:
        moveDirection = QTextCursor::Up;
        break;

    }

    // if input was just movement, we're done
    if (moveDirection != QTextCursor::NoMove) {
        if (event->modifiers() == Qt::NoModifier) {
            d->smartMoveCursor(moveDirection, QTextCursor::MoveAnchor, 1);
        } else if (event->modifiers() == Qt::ShiftModifier
                   &&  (textInteractionFlags() & Qt::TextSelectableByKeyboard)) {
            d->disableUpdateMicroFocus();
            const bool moved = d->smartMoveCursor(moveDirection, QTextCursor::KeepAnchor, 1);
            if (!wasSelecting && d->cursor()->hasSelection()) { // first character was selected
                d->setMode(MTextEditModel::EditModeSelect);
                d->sendCopyAvailable(true);
            } else if (wasSelecting && !d->cursor()->hasSelection()) { // selection have just vanished
                d->setMode(MTextEditModel::EditModeBasic);
                d->sendCopyAvailable(false);
            }
            if (moved) {
                emit selectionChanged();
                d->updateMicroFocus();
            }
            d->enableUpdateMicroFocus(true);
        }
        event->accept();
        return;
    }

    if (event->matches(QKeySequence::Copy)) {
        copy();
        event->accept();
        return;
    } else if (event->matches(QKeySequence::SelectAll)) {
        selectAll();
        event->accept();
        return;
    }

    if ((textInteractionFlags() & Qt::TextEditable) == 0) {
        return;
    }

    if (event->matches(QKeySequence::Paste)) {
        paste();
        event->accept();
        return;
    } else if (event->matches(QKeySequence::Cut)) {
        cut();
        event->accept();
        return;
    }

    // we continue by assuming some input is to be made

    // first input made with PasswordEchoOnEdit clears the previous contents
    if (echoMode() == MTextEditModel::PasswordEchoOnEdit && d->editActive == false) {
        clear();
    }

    d->editActive = true;

    QTextDocumentFragment selectedFragment;
    int selectionStart = -1;
    QTextCharFormat format;
    bool needRemoveFirst = true;

    if (wasSelecting == true) {
        QString tmpText = event->text();
        if (key != Qt::Key_Backspace && key != Qt::Key_Delete &&
            key != Qt::Key_Tab && key != Qt::Key_Space &&
            key != Qt::Key_Return && key != Qt::Key_plusminus &&
            (tmpText.isEmpty() || !tmpText.at(0).isPrint())) {
            needRemoveFirst = false;
        }
        if (key == Qt::Key_Return && model()->line() == MTextEditModel::SingleLine) {
            needRemoveFirst = false;
        }

        if (needRemoveFirst) {
            QTextCursor positionCursor = textCursor();
            int position = positionCursor.selectionStart();
            // setPosition() is required to get the style that would be applied when a text is
            // inserted at the position + 1
            positionCursor.setPosition(position + 1);
            format = positionCursor.charFormat();
            selectionStart = d->cursor()->selectionStart();
            selectedFragment = d->cursor()->selection();
            d->cursor()->removeSelectedText();
        }
    } else {
        d->commitPreedit();
    }

    // Whether text was modified, excluding the removal of 'needRemoveFirst' text above.
    bool modified = false;

    switch (key) {
    case Qt::Key_Backspace:
        // backspace and delete in selection mode are special cases, only selection is removed
        // These also need explicit validation here.
        if (wasSelecting == false) {
            modified = d->doBackspace();
        } else if (d->validateCurrentBlock()) {
            d->cursor()->setCharFormat(format);
            modified = true;
        }
        break;

    case Qt::Key_Delete:
        if (wasSelecting == false) {
            modified = d->doDelete();
        } else if (d->validateCurrentBlock()) {
            modified = true;
        }
        break;

    case Qt::Key_Tab:
        modified = d->doTab();
        break;

    case Qt::Key_Space:
        modified = d->doTextInsert(" ");
        break;

    case Qt::Key_Return:
        modified = d->onReturnPressed(event);
        break;

    case Qt::Key_plusminus:
        modified = d->doSignCycle();
        break;

    default: {
        QString text = event->text();
        if (!text.isEmpty() && text.at(0).isPrint()) {
            modified = d->doTextInsert(event->text());
        }
    }
    }

    if (modified == true) {
        event->accept();

        if (wasSelecting == true) {
            d->setMode(MTextEditModel::EditModeBasic);
            emit selectionChanged();
            d->sendCopyAvailable(false);
        }

        model()->updateCursor();
        d->updateMicroFocus();
        emit textChanged();
        emit cursorPositionChanged();

    } else {
        if (wasSelecting == true && needRemoveFirst == true) {
            // need to put the selection back
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
            d->cursor()->removeSelectedText();
            d->cursor()->insertFragment(selectedFragment);
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
        }
    }
}


void MTextEdit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEdit);
    MWidgetController::mouseReleaseEvent(event);

    // Widget was focused-in on corresponding mouse press event:
    if (d->pendingSoftwareInputPanelRequest) {
        d->requestSip();
    }
}


void MTextEdit::focusInEvent(QFocusEvent *event)
{
    Q_D(MTextEdit);

    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    d->focusEventState = MTextEditPrivate::FocusInEventReceived;

    d->editActive = false;

    if (sceneManager()) {
        connect(this, SIGNAL(cursorPositionChanged()),
                sceneManager(), SLOT(ensureCursorVisible()),
                Qt::UniqueConnection);

        d->requestAutoSip(event->reason());
    }

    if (model()->autoSelectionEnabled() == true) {
        selectAll();
    }

    if (completer()) {
        //The completer can be shared by several widgets, so call its setWidget,
        //and connect related slots again when getting focus.
        completer()->setWidget(this);
        connect(completer(), SIGNAL(confirmed(QString, QModelIndex)),
                this, SLOT(_q_confirmCompletion(QString)));
        connect(this, SIGNAL(textChanged()),
                completer(), SLOT(complete()));
    }

    emit gainedFocus(event->reason());
}


void MTextEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    Q_D(MTextEdit);

    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    d->focusEventState = MTextEditPrivate::FocusOutEventReceived;

    // Need to tell the MArrowKeyNavigator that the next key event it sees
    // would come from a different focus item. Otherwise, it could think
    // of the key event as an auto-repeat
    // TODO: Replace with better auto-repeat detection, and remove this hack.
    gArrowKeyNav.skipNextRequest();

    d->commitPreedit();
    deselect();

    if (d->completer) {
        //hide completer when focus out
        d->completer->hideCompleter();
        //disconnect related slots when the widget losing focus
        disconnect(completer(), 0, this, 0);
    }

    emit lostFocus(event->reason());
    if (sceneManager()) {
        disconnect(this, SIGNAL(cursorPositionChanged()),
                   sceneManager(), SLOT(ensureCursorVisible()));
    }

    MInputMethodState::closeSoftwareInputPanel();
    d->pendingSoftwareInputPanelRequest = false;
}

bool MTextEdit::insert(const QString &text)
{
    Q_D(MTextEdit);

    bool wasSelecting = hasSelectedText();

    // back to basic mode
    if ((mode() == MTextEditModel::EditModeSelect) && d->cursor()->hasSelection()) {
        d->cursor()->removeSelectedText();
        emit selectionChanged();
    } else if (mode() == MTextEditModel::EditModeActive) {
        d->removePreedit();
        d->preeditStyling.clear();
    }

    d->setMode(MTextEditModel::EditModeBasic);

    if (wasSelecting) {
        d->sendCopyAvailable(false);
    }

    bool insertionSuccess = d->doTextInsert(text);

    // either something was inserted or something was deleted, or both
    if (insertionSuccess || wasSelecting) {
        emit textChanged();
        d->updateMicroFocus();
    }

    return insertionSuccess;
}


bool MTextEdit::setText(const QString &text)
{
    Q_D(MTextEdit);

    int cursorPosBefore = d->cursor()->position();
    bool wasSelecting = hasSelectedText();
    bool wasEmpty = (document()->characterCount() == 0);

    // clear the state
    d->removePreedit();
    d->preeditStyling.clear();
    d->cursor()->clearSelection();
    document()->clear();
    d->setMode(MTextEditModel::EditModeBasic);

    //set cursor to the start again
    d->cursor()->movePosition(QTextCursor::Start);

    QString filteredText = text;

    // Limit text length.
    filteredText.truncate(maxLength());

    if (lineMode() == MTextEditModel::SingleLine) {
        filteredText.replace(QChar('\n'), QChar(' '));
    }

    d->cursor()->insertText(filteredText);

    bool accepted = d->validate();

    if (!accepted) {
        document()->clear();
    }

    // only avoid signaling if empty before and after
    if (!((document()->characterCount() == 0) && wasEmpty)) {
        d->updateMicroFocus();
        emit textChanged();
    }

    if (d->cursor()->position() != cursorPosBefore) {
        emit cursorPositionChanged();
    }

    if (wasSelecting) {
        d->sendCopyAvailable(false);
    }

    return accepted;
}


QString MTextEdit::text() const
{
    return document()->toPlainText();
}


int MTextEdit::cursorPosition() const
{
    Q_D(const MTextEdit);
    return d->cursor()->position();
}


bool MTextEdit::setCursorPosition(int index)
{
    Q_D(MTextEdit);

    d->commitPreedit();
    deselect();
    bool val = d->setCursorPosition(index);

    return val;
}


void MTextEdit::handleMousePress(int cursorPosition, QGraphicsSceneMouseEvent *event)
{
    handleMousePress(cursorPosition, event, NULL);
}


void MTextEdit::handleMousePress(int cursorPosition, QGraphicsSceneMouseEvent *event,
                                 TextFieldLocationType *location)
{
    Q_D(MTextEdit);

    if (textInteractionFlags() != Qt::NoTextInteraction && location) {
        QString text = document()->toPlainText();
        MBreakIterator breakIterator(text);

        if (breakIterator.isBoundary(cursorPosition) == true) {
            *location = MTextEdit::WordBoundary;
        } else {
            *location = MTextEdit::Word;
        }
    }

    d->notifyInputContextMouseHandler(cursorPosition, event);
}


void MTextEdit::handleMouseRelease(int eventCursorPosition, QGraphicsSceneMouseEvent *event)
{
    handleMouseRelease(eventCursorPosition, event, NULL);
}


void MTextEdit::handleMouseRelease(int eventCursorPosition, QGraphicsSceneMouseEvent *event,
                                   TextFieldLocationType *location)
{
    Q_D(MTextEdit);
    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    int cursorPositionBefore = cursorPosition();

    deselect();

    if (d->isPositionOnPreedit(eventCursorPosition) == false) {
        // input context takes care of releases happening on top of preedit, the rest
        // is handled here
        d->commitPreedit();

        QString text = document()->toPlainText();
        MBreakIterator breakIterator(text);
        QInputContext *ic = qApp->inputContext();

        // clicks on word boundaries move the cursor
        if (breakIterator.isBoundary(eventCursorPosition) == true) {
            if (location) {
                *location = MTextEdit::WordBoundary;
            }
            d->setCursorPosition(eventCursorPosition);

        } else {
            if (location) {
                *location = MTextEdit::Word;
            }
            if (inputMethodCorrectionEnabled()) {
                // clicks on words remove them from the normal contents and makes them preedit.
                int start = breakIterator.previousInclusive(eventCursorPosition);
                int end = breakIterator.next(eventCursorPosition);
                QString preedit = text.mid(start, end - start);

                d->storePreeditTextStyling(start, end);
                d->cursor()->setPosition(start);
                d->cursor()->setPosition(end, QTextCursor::KeepAnchor);
                QTextDocumentFragment preeditFragment = d->cursor()->selection();
                d->cursor()->removeSelectedText();

                // offer the word to input context as preedit. if the input context accepts it and
                // plays nicely, it should offer the preedit right back, changing the mode to
                // active.
                bool injectionAccepted = false;

                if (ic) {
                    MPreeditInjectionEvent event(preedit);
                    QCoreApplication::sendEvent(ic, &event);

                    injectionAccepted = event.isAccepted();
                }

                // if injection wasn't supported, put the text back and fall back to cursor changing
                if (injectionAccepted == false) {
                    d->cursor()->insertFragment(preeditFragment);
                    d->setCursorPosition(eventCursorPosition);
                    d->preeditStyling.clear();
                }

            } else {
                d->setCursorPosition(eventCursorPosition);
            }
        }

    } else {
        if (location) {
            *location = MTextEdit::Word;
        }
        d->notifyInputContextMouseHandler(eventCursorPosition, event);
    }

    if (cursorPosition() != cursorPositionBefore) {
        d->updateMicroFocus();
    }
}

void MTextEdit::handleMouseMove(int cursorPosition, QGraphicsSceneMouseEvent *event)
{
    Q_D(MTextEdit);
    d->notifyInputContextMouseHandler(cursorPosition, event);
}


void MTextEdit::selectAll()
{
    Q_D(MTextEdit);
    d->disableUpdateMicroFocus();
    d->commitPreedit();

    d->cursor()->setPosition(0);
    d->cursor()->movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    if (d->cursor()->position() > 0) {
        // change mode only if there was something to select
        if (mode() != MTextEditModel::EditModeSelect) {
            d->setMode(MTextEditModel::EditModeSelect);
        }

        model()->updateCursor();
        emit selectionChanged();
        d->sendCopyAvailable(true);
        d->updateMicroFocus();
    }
    d->enableUpdateMicroFocus(true);
}


void MTextEdit::clear()
{
    setText("");
}


void MTextEdit::copy()
{
    Q_D(MTextEdit);

    d->copy();
}


void MTextEdit::paste()
{
    Q_D(MTextEdit);
    QClipboard *clipboard = QApplication::clipboard();
    bool wasSelecting = hasSelectedText();
    bool changed = false;


    if (isReadOnly() || !clipboard) {
        return;
    }

    QString text = clipboard->text();

    if (text.isEmpty()) {
        return;
    }

    // back to basic mode
    if (wasSelecting) {
        d->cursor()->removeSelectedText();
        emit selectionChanged();
    } else if (mode() == MTextEditModel::EditModeActive) {
        d->commitPreedit();
    }

    d->setMode(MTextEditModel::EditModeBasic);
    if (wasSelecting) {
        d->sendCopyAvailable(false);
    }

    if (d->validator) {
        //use slow algorithm if validator was assigned
        foreach(const QChar & c, text) {
            changed = d->doTextInsert(c) || changed;
        }
    } else {
        changed = d->doTextInsert(text);
    }

    if (changed) {
        emit textChanged();
        emit cursorPositionChanged();
        d->updateMicroFocus();
    } else {
        mDebug("MTextEdit") << __PRETTY_FUNCTION__ << "paste failed";
        emit pasteFailed();
    }
}

void MTextEdit::cut()
{
    Q_D(MTextEdit);
    QValidator::State result = QValidator::Acceptable;

    if (isReadOnly() || !d->copy()) {
        return;
    }

    QTextCursor *cursor = model()->cursor();

    if (d->validator) {
        QString textCopy = model()->document()->toPlainText();
        int start = cursor->selectionStart();
        int length = cursor->selectionEnd() - start;

        textCopy.remove(start, length);
        result = d->validator->validate(textCopy, start);
    }
    if (result != QValidator::Invalid) {
        cursor->removeSelectedText();
        d->setMode(MTextEditModel::EditModeBasic);
        d->sendCopyAvailable(false);
        emit selectionChanged();
        emit textChanged();
        emit cursorPositionChanged();
        d->updateMicroFocus();
    }
}

MTextEditModel::EditMode MTextEdit::mode() const
{
    return model()->edit();
}


MTextEditModel::LineMode MTextEdit::lineMode() const
{
    return model()->line();
}


M::TextContentType MTextEdit::contentType() const
{
    return model()->type();
}


void MTextEdit::inputMethodEvent(QInputMethodEvent *event)
{
    // FIXME: replacement info not honored.
    Q_D(MTextEdit);

    if (d->omitInputMethodEvents) {
        return;
    }

    QString preedit = event->preeditString();
    QString commitString = event->commitString();
    bool emitReturnPressed = false;

    if (lineMode() == MTextEditModel::SingleLine) {
        emitReturnPressed = commitString.contains('\n');
        preedit.remove('\n');
        commitString.remove('\n');

        preedit = d->replaceLineBreaks(preedit, QChar(' '));
        commitString = d->replaceLineBreaks(commitString, QChar(' '));
    }

    // get and remove the current selection
    const bool wasSelecting = hasSelectedText();
    QTextDocumentFragment selectedFragment;
    int selectionStart = -1;

    if (wasSelecting == true) {
        selectionStart = d->cursor()->selectionStart();
        selectedFragment = d->cursor()->selection();
        d->cursor()->removeSelectedText();
    }

    const bool wasPreediting = d->isPreediting();

    d->removePreedit();

    if (d->editActive == false &&
            echoMode() == MTextEditModel::PasswordEchoOnEdit &&
            (commitString.isEmpty() == false || preedit.isEmpty() == false)) {
        // in this mode, clear the contents before starting editing and doing new input
        clear();
    }

    d->editActive = true;

    bool insertionSuccess = false;

    // append possible commit string
    if (commitString.isEmpty() == false) {
        insertionSuccess = d->doTextInsert(commitString, true);
        d->preeditStyling.clear();

        if (insertionSuccess == false && wasSelecting == true) {
            // validation failed, put the old selection back
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
            d->cursor()->removeSelectedText();
            d->cursor()->insertFragment(selectedFragment);
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
        }
    }

    bool changed = false;

    if (insertionSuccess || (preedit.isEmpty() && wasPreediting)) {
        // return to basic mode on insertion or removed preedit
        d->setMode(MTextEditModel::EditModeBasic);
        changed = true;
    }

    if (!preedit.isEmpty()) {
        QList<QInputMethodEvent::Attribute> attributes = event->attributes();
        d->setPreeditText(preedit, attributes);
        d->setMode(MTextEditModel::EditModeActive);
        changed = true;
    }

    if (changed) {
        d->updateMicroFocus();
        emit textChanged();
        emit cursorPositionChanged();
    }

    if (emitReturnPressed && hasAcceptableInput()) {
        emit returnPressed();
    }

    if (wasSelecting != hasSelectedText()) {
        d->sendCopyAvailable(hasSelectedText());
    }
}


void MTextEdit::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LayoutDirectionChange) {
        QTextOption option = document()->defaultTextOption();
        option.setTextDirection(layoutDirection());
        document()->setDefaultTextOption(option);
    }

    MWidgetController::changeEvent(event);
}


QTextDocument *MTextEdit::document() const
{
    if (!model()->document()) {
        MTextEditModel *m = const_cast<MTextEditModel *>(model());
        m->setDocument(new QTextDocument(m));
    }

    return model()->document();
}


QTextCursor MTextEdit::textCursor() const
{
    if (!model()->cursor()) {
        MTextEditModel *m = const_cast<MTextEditModel *>(model());
        m->setCursor(new QTextCursor(document()));
    }

    return QTextCursor(*model()->cursor());
}


void MTextEdit::setTextCursor(const QTextCursor &cursor)
{
    Q_D(MTextEdit);

    MTextEditModel::EditMode newMode;

    if (cursor.hasSelection()) {
        newMode = MTextEditModel::EditModeSelect;
    } else {
        newMode = MTextEditModel::EditModeBasic;
    }

    MTextEditModel::EditMode oldMode = model()->edit();
    bool changed = (*model()->cursor() != cursor);

    d->commitPreedit();
    *(d->cursor()) = cursor;
    model()->setEdit(newMode);

    if ((newMode == MTextEditModel::EditModeSelect && changed) ||
            (newMode == MTextEditModel::EditModeBasic &&
             oldMode == MTextEditModel::EditModeSelect)) {
        emit selectionChanged();
    }

    if (changed) {
        emit cursorPositionChanged();
    }

    if (echoMode() == MTextEditModel::Normal) {
        if ((newMode == MTextEditModel::EditModeSelect) && (newMode != oldMode)) {
            emit copyAvailable(true);
        } else if ((newMode == MTextEditModel::EditModeBasic) &&
                   oldMode == MTextEditModel::EditModeSelect) {
            emit copyAvailable(false);
        }
    }
}


void MTextEdit::setContentType(M::TextContentType type)
{
    Q_D(MTextEdit);

    model()->setType(type);

    // update validator if it's currently our, otherwise leave it intact
    // FIXME: doesn't work if model has content type already from somewhere
    if (d->ownValidator == true) {
        delete d->validator;
        d->validator = 0;
        d->ownValidator = false;
    }

    if (d->validator == 0) {
        QRegExp rx;

        switch (type) {
        case M::NumberContentType:
            rx.setPattern(NumberCharacterSet);
            d->validator = new QRegExpValidator(rx, 0);
            break;

        case M::PhoneNumberContentType:
            rx.setPattern(PhoneNumberCharacterSet);
            d->validator = new QRegExpValidator(rx, 0);
            break;

        case M::EmailContentType:
            rx.setPattern(EmailCharacterSet);
            d->validator = new QRegExpValidator(rx, 0);
            break;

        case M::UrlContentType:
            //TODO: No check rule for URL yet
            break;

        default:
            break;
        }

        // if a validator was created, we own it
        if (d->validator != 0) {
            d->ownValidator = true;
        }
    }

    // update other state
    Qt::InputMethodHints newHint;

    switch (type) {
    case M::NumberContentType:
        setInputMethodCorrectionEnabled(false);
        newHint = Qt::ImhFormattedNumbersOnly;
        break;

    case M::PhoneNumberContentType:
        setInputMethodCorrectionEnabled(false);
        newHint = Qt::ImhDialableCharactersOnly;
        break;

    case M::EmailContentType:
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);
        newHint = Qt::ImhEmailCharactersOnly;
        break;

    case M::UrlContentType:
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);
        newHint = Qt::ImhUrlCharactersOnly;
        break;

    default:
        setInputMethodAutoCapitalizationEnabled(true);
        break;
    }

    setInputMethodHints((inputMethodHints() & ~Qt::ImhExclusiveInputMask) | newHint);
}


QVariant MTextEdit::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_D(const MTextEdit);
    QTextBlock block = d->cursor()->block();

    switch ((int)query) {
    case Qt::ImCursorPosition:
        return QVariant(cursorPosition() - block.position());

    case Qt::ImSurroundingText:
        return QVariant(block.text());

    case Qt::ImCurrentSelection:
        return QVariant(selectedText());

    case M::ImCorrectionEnabledQuery:
        return QVariant(inputMethodCorrectionEnabled());

    case M::InputMethodToolbarIdQuery:
        return QVariant(attachedToolbarId());

    case M::InputMethodToolbarQuery:
        return QVariant(attachedToolbar());

    case Qt::ImMicroFocus:
        // Only check whether this text edit *has* a completer, not whether the completer is
        // visible. Otherwise, it would cause further window relocations once the completer hides
        // (shows) again.
        {
            QRect rect = MWidgetController::inputMethodQuery(query).toRect();
            if (d->completer) {
                // sum up the cursor rectangle and the completer rectangle.
                QRect completerRect = d->completer->boundingRect().toRect();
                completerRect.translate(rect.left(), rect.bottom());
                QRegion region(rect);
                region += completerRect;
                rect = region.boundingRect();
            }
            return rect;
        }

        // No break - intended fall-through to default case:

    default:
        return MWidgetController::inputMethodQuery(query);
    }
}

QVariant MTextEdit::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(MTextEdit);

    switch (change) {

    case QGraphicsItem::ItemVisibleHasChanged:
        // Workaround for NB#186087 -  QGraphicsItem never gets focusOutEvent on hide:
        if (!value.toBool() &&
            d->focusEventState == MTextEditPrivate::FocusInEventReceived) {

            // Hidden (but focused) MTextEdit (please don't ask what it means
            // to hide a controller) does not need a VKB:
            d->closeAutoSip();

            // Need to notify the view, too:
            emit lostFocus(Qt::OtherFocusReason);
        }
        break;

    default:
        break;
    }

    return MWidgetController::itemChange(change, value);
}


void MTextEdit::setTextInteractionFlags(Qt::TextInteractionFlags flags)
{
    Q_D(MTextEdit);

    // don't set same flags twice
    if (model()->textInteractionFlags() == flags)
        return;

    model()->setTextInteractionFlags(flags);

    // if not editable, remove focus and hide cursor. Commit preedit
    if (!(model()->textInteractionFlags() & Qt::TextEditable)) {
        d->commitPreedit();
        setFocusPolicy(Qt::NoFocus);

    } else {
        setFocusPolicy(Qt::ClickFocus);
    }

    // TODO: notify input context if editability changed

    // TODO: Check other flags (Qt::TextSelectableByKeyboard etc) if necessary
}


Qt::TextInteractionFlags MTextEdit::textInteractionFlags() const
{
    return model()->textInteractionFlags();
}


bool MTextEdit::isReadOnly() const
{
    Qt::TextInteractionFlags flags = textInteractionFlags();
    return !(flags & Qt::TextEditable);
}


void MTextEdit::setReadOnly(bool readOnly)
{
    Qt::TextInteractionFlags flags = textInteractionFlags();

    if (readOnly == true) {
        flags = flags & ~Qt::TextEditable;
    } else {
        flags = flags | Qt::TextEditable;
    }

    setTextInteractionFlags(flags);
}


MTextEditModel::EchoMode MTextEdit::echoMode() const
{
    return model()->echo();
}


void MTextEdit::setEchoMode(MTextEditModel::EchoMode echoMode)
{
    model()->setEcho(echoMode);
    if (echoMode != MTextEditModel::Normal) {
        setInputMethodHints(inputMethodHints() | Qt::ImhHiddenText);
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);

    } else {
        setInputMethodHints(inputMethodHints() & ~Qt::ImhHiddenText);
    }

    if (mode() == MTextEditModel::EditModeSelect) {
        emit copyAvailable(echoMode == MTextEditModel::Normal);
    }
}


void MTextEdit::deselect()
{
    Q_D(MTextEdit);

    if (mode() == MTextEditModel::EditModeSelect) {
        d->cursor()->clearSelection();
        d->setMode(MTextEditModel::EditModeBasic);
        model()->updateCursor();
        d->sendCopyAvailable(false);
        d->updateMicroFocus();
        emit selectionChanged();
    }
}


bool MTextEdit::isAutoSelectionEnabled() const
{
    return isSelectionEnabled() && model()->autoSelectionEnabled();
}


void MTextEdit::setAutoSelectionEnabled(bool enable)
{
    model()->setAutoSelectionEnabled(enable);
}


void MTextEdit::setInputMethodCorrectionEnabled(bool enabled)
{
    // only allow to turn on correction for the text edit which is normal echo mode
    // content type is free text or custom content type.
    if ((enabled == false)
        || (echoMode() == MTextEditModel::Normal
            && (contentType() == M::FreeTextContentType || contentType() == M::CustomContentType))) {
        model()->setInputMethodCorrectionEnabled(enabled);
    }
}


bool MTextEdit::inputMethodCorrectionEnabled() const
{
    return model()->inputMethodCorrectionEnabled();
}


void MTextEdit::setInputMethodPredictionEnabled(bool enabled)
{
    model()->setInputMethodPredictionEnabled(enabled);
    if (enabled) {
        setInputMethodHints(inputMethodHints() & ~Qt::ImhNoPredictiveText);
    } else {
        setInputMethodHints(inputMethodHints() | Qt::ImhNoPredictiveText);
    }
}

bool MTextEdit::inputMethodPredictionEnabled() const
{
    return !(inputMethodHints() & Qt::ImhNoPredictiveText);
}

int MTextEdit::maxLength() const
{
    return model()->maxLength();
}

void MTextEdit::setMaxLength(int numChars)
{
    if (numChars < 0) {
        numChars = 0;
    }
    model()->setMaxLength(numChars);

    // Bug in QTextDocument::characterCount?
    int characterCount = document()->characterCount() - 1;
    Q_ASSERT(characterCount >= 0);

    if (characterCount > maxLength()) {
        Q_D(MTextEdit);

        // If we have preedit commit it first
        d->commitPreedit();

        // Make sure we're not in selection mode.
        d->setMode(MTextEditModel::EditModeBasic);

        // Check again
        characterCount = document()->characterCount() - 1;
        if (characterCount > maxLength()) {
            // Select everything after position maxLength and remove the selection.
            d->setCursorPosition(maxLength());
            d->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
            d->cursor()->removeSelectedText();
        }
    }
}

void MTextEdit::setInputMethodAutoCapitalizationEnabled(bool enabled)
{
    // only allow to turn on auto capitalization for the text edit which is normal echo mode
    // content type is free text or custom content type.
    if ((enabled == false)
        || (echoMode() == MTextEditModel::Normal
            && (contentType() == M::FreeTextContentType || contentType() == M::CustomContentType))) {
        model()->setInputMethodAutoCapitalizationEnabled(enabled);
    }

    if (model()->inputMethodAutoCapitalizationEnabled()) {
        setInputMethodHints(inputMethodHints() & ~Qt::ImhNoAutoUppercase);
    } else {
        setInputMethodHints(inputMethodHints() | Qt::ImhNoAutoUppercase);
    }
}


bool MTextEdit::inputMethodAutoCapitalizationEnabled() const
{
    return !(inputMethodHints() & Qt::ImhNoAutoUppercase);
}


void MTextEdit::setPrompt(const QString &prompt)
{
    // TODO: should replace newlines with spaces in single line?
    model()->setPrompt(prompt);
}


QString MTextEdit::prompt() const
{
    return model()->prompt();
}


void MTextEdit::setValidator(const QValidator *validator)
{
    Q_D(MTextEdit);

    if (d->ownValidator == true) {
        delete d->validator;
    }

    d->validator = validator;
    d->ownValidator = false;
}


const QValidator *MTextEdit::validator() const
{
    Q_D(const MTextEdit);
    return d->validator;
}


bool MTextEdit::hasAcceptableInput() const
{
    Q_D(const MTextEdit);

    if (d->validator == 0) {
        return true;
    } else {
        QString textCopy = model()->document()->toPlainText();
        int cursorCopy = model()->cursor()->position();
        QValidator::State result = d->validator->validate(textCopy, cursorCopy);
        return (result == QValidator::Acceptable);
    }
}

void MTextEdit::setCompleter(MCompleter *completer)
{
    Q_D(MTextEdit);
    if (completer == d->completer)
        return;
    if (d->completer) {
        disconnect(d->completer, 0, this, 0);
        d->completer->setWidget(0);
    }

    d->completer = completer;
    if (d->completer) {
        d->completer->setWidget(this);
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);

        if (hasFocus()) {
            d->updateMicroFocus();
            connect(d->completer, SIGNAL(confirmed(QString, QModelIndex)),
                    this, SLOT(_q_confirmCompletion(QString)));
        }
    }
}

MCompleter *MTextEdit::completer()
{
    Q_D(MTextEdit);
    return d->completer;
}

void MTextEdit::setAutoSipEnabled(bool enabled)
{
    model()->setAutoSipEnabled(enabled);
}

bool MTextEdit::isAutoSipEnabled() const
{
    return model()->isAutoSipEnabled();
}

void MTextEdit::attachToolbar(const QString &name)
{
    Q_D(MTextEdit);
    if (attachedToolbar() == name)
        return;
    detachToolbar();
    // record registered toolbar identifier which will be unregistered when detachToolbar().
    d->registeredToolbarId = MInputMethodState::instance()->registerToolbar(name);
    model()->setToolbar(name);
    model()->setToolbarId(d->registeredToolbarId);
}

void MTextEdit::attachToolbar(int id)
{
    if (attachedToolbarId() == id)
        return;
    detachToolbar();
    const QString toolbar = MInputMethodState::instance()->toolbar(id);
    if (!toolbar.isEmpty()) {
        // record attached toolbar identifier and file name.
        // The toolbar identifier is an unique id in the space of the application.
        // Two text entries may attach to the same toolbar file, but have two different
        // toolbar instances (two ids).
        model()->setToolbar(toolbar);
        model()->setToolbarId(id);
    }
}

QString MTextEdit::attachedToolbar() const
{
    return model()->toolbar();
}

int MTextEdit::attachedToolbarId() const
{
    return model()->toolbarId();
}

void MTextEdit::detachToolbar()
{
    Q_D(MTextEdit);
    // only unregister toolbar it has registered.
    if (d->registeredToolbarId >= 0) {
        MInputMethodState::instance()->unregisterToolbar(d->registeredToolbarId);
        d->registeredToolbarId = -1;
    }
    model()->setToolbar(QString());
    model()->setToolbarId(-1);
}

#include "moc_mtextedit.cpp"
