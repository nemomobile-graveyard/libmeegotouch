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

#include "duitextedit.h"
#include "duitextedit_p.h"

#include <DuiDebug>
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

#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiapplicationpage.h"
#include "duitheme.h"
#include "duibreakiterator.h"
#include "duinamespace.h"
#include "duipreeditinjectionevent.h"
#include "duiwidgetview.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duicompleter.h"
#include "duiscenemanager.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiTextEdit)

namespace
{
    //! character set for Number field type
    const QString NumberCharacterSet = "[\\+\\-]{0,1}([0-9]+(\\.[0-9]*){0,1}){1,1}";

    //! character set for Phone Number field type
    const QString PhoneNumberCharacterSet = "[\\+0-9#\\*\\-\\+pw.\\/() ]+";

    //! character set for Email field type
    const QString EmailCharacterSet       = "[a-zA-Z0-9.!#$%&'*+-\\/=?\\^_\\`{|}~@]+";

    const QChar PlusSign('+');
    const QChar MinusSign('-');
}


/*!
 * \brief Constructor
 * \param type widget type (single line or multiline)
 */
DuiTextEditPrivate::DuiTextEditPrivate()
    : validator(0),
      ownValidator(false),
      completer(0),
      editActive(false)
{
}


/*!
 * \brief Destructor
 */
DuiTextEditPrivate::~DuiTextEditPrivate()
{
    if (ownValidator == true) {
        delete validator;
    }
}


QTextCursor *DuiTextEditPrivate::cursor() const
{
    Q_Q(const DuiTextEdit);
    return q->model()->cursor();
}


/*!
  * \brief Moves cursor, parameters as in QTextCursor::movePosition()
  */
void DuiTextEditPrivate::moveCursor(QTextCursor::MoveOperation moveOp,
                                    QTextCursor::MoveMode moveMode, int n)
{
    Q_Q(DuiTextEdit);

    // Cursor movement will enter basic mode
    commitPreedit();
    q->deselect();

    if (cursor()->movePosition(moveOp, moveMode, n)) {
        q->model()->updateCursor();
        q->updateMicroFocus();
        emit q->cursorPositionChanged();
    }
}


/*!
  * \brief Backspace functionality, doesn't change mode
  */
bool DuiTextEditPrivate::doBackspace()
{
    Q_Q(DuiTextEdit);

    if (q->isReadOnly()) {
        return false;
    }

    QTextCursor currentPositionCursor = q->textCursor();
    currentPositionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    QTextDocumentFragment currentFragment = currentPositionCursor.selection();
    cursor()->deletePreviousChar();

    if (validateCurrentBlock() == true) {
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
bool DuiTextEditPrivate::doDelete()
{
    Q_Q(DuiTextEdit);

    if (q->isReadOnly()) {
        return false;
    }

    QTextCursor currentPositionCursor(q->textCursor());
    currentPositionCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    QTextDocumentFragment currentFragment = currentPositionCursor.selection();
    cursor()->deleteChar();

    if (validateCurrentBlock() == true) {
        emit q->cursorPositionChanged();
        return true;

    } else {
        // document doesn't validate after delete -> put the character back
        cursor()->insertFragment(currentFragment);
        return false;
    }
}


/*!
  * \brief Insert tab to the char stream & move cursor in front of it
  */
bool DuiTextEditPrivate::doTab()
{
    // TODO: missing Qt::tab action - this is on purpose or not?
    return doTextInsert("\t");
}


/*!
  * \brief Inserts a line break if editor is multiline,
  * otherwise emits signal returnPressed() if it is allowed by validator.
  */
bool DuiTextEditPrivate::onReturnPressed(QKeyEvent *event)
{
    Q_Q(DuiTextEdit);
    bool result = false;

    if (q->model()->line() == DuiTextEditModel::SingleLine) {
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
bool DuiTextEditPrivate::doSignCycle()
{
    Q_Q(DuiTextEdit);

    if (q->contentType() != Dui::NumberContentType) {
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
bool DuiTextEditPrivate::doTextInsert(const QString &text)
{
    Q_Q(DuiTextEdit);

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
    if (q->lineMode() == DuiTextEditModel::SingleLine) {
        filteredText.replace(QChar('\n'), QChar(' '));
    }

    int textPosition = 0;
    int filteredTextLength = filteredText.length();
    int snippetLength = -1;

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
        cursor()->insertText(textSnippet);

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


bool DuiTextEditPrivate::validateCurrentBlock()
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
            // duiDebug("DuiTextEditPrivate") << __PRETTY_FUNCTION__ << "replacing" << blockText
            //          << "with" << blockTextCopy;
            // the validator changed the string somehow, need to set block content again
            QTextCursor blockCursor(*cursor());

            // first clear the content of the block
            int start = currentBlock.position();
            QString currentContent = currentBlock.text();
            int end = start + currentContent.length();

            blockCursor.setPosition(start);
            blockCursor.setPosition(end, QTextCursor::KeepAnchor);
            // duiDebug("DuiTextEditPrivate") << "Start of selection:" << start;
            // duiDebug("DuiTextEditPrivate") << "End of selection:" << blockCursor.position();
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
void DuiTextEditPrivate::setPreeditText(const QString &text,
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

    textCursor->insertText(text);

    // mark preedit as selection
    int position = textCursor->position();
    textCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor,
                             preeditTextLength);
    textCursor->setPosition(position, QTextCursor::KeepAnchor);
}


/*!
  * \brief Commit current pre-edit so it becomes normal text
  */
void DuiTextEditPrivate::commitPreedit()
{
    Q_Q(DuiTextEdit);

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

    setMode(DuiTextEditModel::EditModeBasic);
    q->updateMicroFocus();

    if (q->hasFocus()) {
        // make sure the committed preedit doesn't get left on the input context
        QInputContext *ic = qApp->inputContext();
        if (ic) {
            ic->reset();
        }
    }

    emit q->textChanged();
}


/*!
 * \brief Removes pre-edit text, doesn't affect mode state
 */
void DuiTextEditPrivate::removePreedit()
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
void DuiTextEditPrivate::setMode(DuiTextEditModel::EditMode mode)
{
    Q_Q(DuiTextEdit);

    q->model()->setEdit(mode);
}


/*!
  * \brief Set cursor position.
  * \return false if \a index is an invalid cursor postion.
  */
bool DuiTextEditPrivate::setCursorPosition(int index)
{
    Q_Q(DuiTextEdit);

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
bool DuiTextEditPrivate::isPositionOnPreedit(int cursorPosition) const
{
    if (isPreediting() == false) {
        return false;
    }

    QTextCursor *textCursor = cursor();
    int start = textCursor->anchor();
    int end = textCursor->position();
    return (cursorPosition > start && cursorPosition < end);
}


bool DuiTextEditPrivate::isPreediting() const
{
    Q_Q(const DuiTextEdit);

    return (q->mode() == DuiTextEditModel::EditModeActive);
}


/*!
 * \brief Sends mouse events to input context mouse handling method
 * \param position cursor position
 * \param event event information
 */
void DuiTextEditPrivate::notifyInputContextMouseHandler(int position,
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
void DuiTextEditPrivate::sendCopyAvailable(bool yes)
{
    Q_Q(DuiTextEdit);
    if (q->echoMode() != DuiTextEditModel::Normal) {
        return;
    }

    emit q->copyAvailable(yes);
}


/*!
 * \brief Translates qgraphics mouse event type to plain mouse type (static)
 * \param input event type
 */
QEvent::Type DuiTextEditPrivate::translateGraphicsSceneMouseTypeToQMouse(QEvent::Type input)
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


void DuiTextEditPrivate::_q_confirmCompletion(const QString &completion)
{
    Q_Q(DuiTextEdit);
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
    q->updateMicroFocus();
}



///////////////////////////////////////////////
// Actual class implementation


DuiTextEdit::DuiTextEdit(DuiTextEditModel::LineMode type, const QString &text,
                         QGraphicsItem *parent)
    : DuiWidgetController(new DuiTextEditPrivate, new DuiTextEditModel, parent)
{
    setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);

    model()->setDocument(new QTextDocument(text, this->model()));
    QTextCursor *cursor = new QTextCursor(document());
    model()->setCursor(cursor);
    model()->setLine(type);

    QTextOption option = document()->defaultTextOption();
    option.setTextDirection(layoutDirection());

    if (type == DuiTextEditModel::SingleLine) {
        option.setWrapMode(QTextOption::NoWrap);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    } else {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);  //Set to expand vertically only in multiline mode
    }

    document()->setDefaultTextOption(option);

    cursor->movePosition(QTextCursor::End);

    setFocusPolicy(Qt::ClickFocus);
}


DuiTextEdit::~DuiTextEdit()
{
    // kludge so view doesn't receive memberModified() signals for the deleted pointers
    model()->beginTransaction();

    //should delete QTextDocument and QTextCursor
    delete model()->document();
    delete model()->cursor();

    model()->setDocument(NULL);
    model()->setCursor(NULL);
}


void DuiTextEdit::setSelection(int start, int length, bool useBoundaries)
{
    Q_D(DuiTextEdit);

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
        DuiBreakIterator breakIterator(text);
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
    updateMicroFocus();

    // make an actual selection
    currentCursor->setPosition(begin);
    currentCursor->setPosition(end, QTextCursor::KeepAnchor);

    // update mode to selection or basic if needed
    if (begin != end) {
        if (mode() != DuiTextEditModel::EditModeSelect) {
            d->setMode(DuiTextEditModel::EditModeSelect);
            d->sendCopyAvailable(true);
        }
    } else if (mode() != DuiTextEditModel::EditModeBasic) {
        bool wasSelecting = hasSelectedText();
        d->setMode(DuiTextEditModel::EditModeBasic);

        if (wasSelecting) {
            d->sendCopyAvailable(false);
        }
    }

    model()->updateCursor();

    emit selectionChanged();

    return;
}


QString DuiTextEdit::selectedText() const
{
    if (mode() != DuiTextEditModel::EditModeSelect) {
        return QString();
    } else {
        return textCursor().selectedText();
    }
}


bool DuiTextEdit::hasSelectedText() const
{
    return mode() == DuiTextEditModel::EditModeSelect;
}

int DuiTextEdit::selectionStart() const
{
    if (mode() == DuiTextEditModel::EditModeSelect) {
        return textCursor().selectionStart();
    } else {
        return -1;
    }
}


bool DuiTextEdit::isSelectionEnabled() const
{
    return (textInteractionFlags() &
            (Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard)) != 0;
}


void DuiTextEdit::keyPressEvent(QKeyEvent *event)
{
    Q_D(DuiTextEdit);

    // assuming that just moving the cursor or text copying requires some interaction flag
    if (textInteractionFlags() == Qt::NoTextInteraction) {
        return;
    }

    QTextCursor::MoveOperation moveDirection = QTextCursor::NoMove;
    int key = event->key();

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
        if (mode() != DuiTextEditModel::EditModeBasic) {
            d->cursor()->clearSelection();
            model()->setEdit(DuiTextEditModel::EditModeBasic);
            emit selectionChanged();
        }

        d->moveCursor(moveDirection, QTextCursor::MoveAnchor, 1);
        event->accept();
        return;
    }

    if (event->matches(QKeySequence::Copy)) {
        copy();
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
    }

    // we continue by assuming some input is to be made

    // first input made with PasswordEchoOnEdit clears the previous contents
    if (echoMode() == DuiTextEditModel::PasswordEchoOnEdit && d->editActive == false) {
        clear();
    }

    d->editActive = true;

    bool wasSelecting = hasSelectedText();

    QTextDocumentFragment selectedFragment;
    int selectionStart = -1;

    if (wasSelecting == true) {
        selectionStart = d->cursor()->selectionStart();
        selectedFragment = d->cursor()->selection();
        d->cursor()->removeSelectedText();

    } else {
        d->commitPreedit();
    }

    bool modified = false;

    switch (key) {
    case Qt::Key_Backspace:
        // backspace and delete in selection mode are special cases, only selection is removed
        if (wasSelecting == false) {
            modified = d->doBackspace();
        } else {
            modified = true;
        }
        break;

    case Qt::Key_Delete:
        if (wasSelecting == false) {
            modified = d->doDelete();
        } else {
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
            d->setMode(DuiTextEditModel::EditModeBasic);
            emit selectionChanged();
            d->sendCopyAvailable(false);
        }

        model()->updateCursor();
        updateMicroFocus();
        emit textChanged();
        emit cursorPositionChanged();

    } else {
        if (wasSelecting == true) {
            // need to put the selection back
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
            d->cursor()->removeSelectedText();
            d->cursor()->insertFragment(selectedFragment);
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
        }
    }
}

void DuiTextEdit::focusInEvent(QFocusEvent *event)
{
    Q_D(DuiTextEdit);

    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    d->editActive = false;

    if (sceneManager())
        sceneManager()->requestSoftwareInputPanel(this);

    if (model()->autoSelectionEnabled() == true) {
        selectAll();
    }

    if (completer()) {
        //The completer can be shared by several widgets, so call its setWidget,
        //and connect related slots again when getting focus.
        completer()->setWidget(this);
        connect(completer(), SIGNAL(confirmed(QString)),
                this, SLOT(_q_confirmCompletion(QString)));
        connect(this, SIGNAL(textChanged()),
                completer(), SLOT(complete()));
    }

    emit gainedFocus(event->reason());
    updateMicroFocus();
}


void DuiTextEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    Q_D(DuiTextEdit);

    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    d->commitPreedit();
    deselect();

    if (d->completer) {
        //hide completer when focus out
        d->completer->hideCompleter();
        //disconnect related slots when the widget losing focus
        disconnect(completer(), 0, this, 0);
    }

    emit lostFocus(event->reason());
    if (sceneManager())
        sceneManager()->closeSoftwareInputPanel();
}


bool DuiTextEdit::insert(const QString &text)
{
    Q_D(DuiTextEdit);

    bool wasSelecting = hasSelectedText();

    // back to basic mode
    if ((mode() == DuiTextEditModel::EditModeSelect) && d->cursor()->hasSelection()) {
        d->cursor()->removeSelectedText();
        emit selectionChanged();
    } else if (mode() == DuiTextEditModel::EditModeActive) {
        d->removePreedit();
    }

    d->setMode(DuiTextEditModel::EditModeBasic);

    if (wasSelecting) {
        d->sendCopyAvailable(false);
    }

    bool insertionSuccess = d->doTextInsert(text);

    // either something was inserted or something was deleted, or both
    if (insertionSuccess || wasSelecting) {
        emit textChanged();
        updateMicroFocus();
    }

    return insertionSuccess;
}


bool DuiTextEdit::setText(const QString &text)
{
    Q_D(DuiTextEdit);

    int cursorPosBefore = d->cursor()->position();
    bool wasSelecting = hasSelectedText();

    // clear the state
    d->removePreedit();
    d->cursor()->clearSelection();
    document()->clear();
    d->setMode(DuiTextEditModel::EditModeBasic);

    //set cursor to the start again
    d->cursor()->movePosition(QTextCursor::Start);

    QString filteredText = text;

    // Limit text length.
    filteredText.truncate(maxLength());

    if (lineMode() == DuiTextEditModel::SingleLine) {
        filteredText.replace(QChar('\n'), QChar(' '));
    }

    d->cursor()->insertText(filteredText);

    bool accepted = hasAcceptableInput();

    if (accepted == true) {
        updateMicroFocus();
        emit textChanged();

    } else {
        document()->clear();
    }

    if (d->cursor()->position() != cursorPosBefore) {
        emit cursorPositionChanged();
    }

    if (wasSelecting) {
        d->sendCopyAvailable(false);
    }

    return accepted;
}


QString DuiTextEdit::text() const
{
    return document()->toPlainText();
}


int DuiTextEdit::cursorPosition() const
{
    Q_D(const DuiTextEdit);
    return d->cursor()->position();
}


bool DuiTextEdit::setCursorPosition(int index)
{
    Q_D(DuiTextEdit);

    d->commitPreedit();
    deselect();
    bool val = d->setCursorPosition(index);

    return val;
}


void DuiTextEdit::handleMousePress(int cursorPosition, QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiTextEdit);

    d->notifyInputContextMouseHandler(cursorPosition, event);
}


void DuiTextEdit::handleMouseRelease(int eventCursorPosition, QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiTextEdit);

    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    int cursorPositionBefore = cursorPosition();

    deselect();

    if (d->isPositionOnPreedit(eventCursorPosition) == false) {
        // input context takes care of releases happening on top of preedit, the rest
        // is handled here
        d->commitPreedit();

        QString text = document()->toPlainText();
        DuiBreakIterator breakIterator(text);
        QInputContext *ic = qApp->inputContext();

        // clicks on word boundaries move the cursor
        if (breakIterator.isBoundary(eventCursorPosition) == true) {
            d->setCursorPosition(eventCursorPosition);

        } else {
            if (inputMethodCorrectionEnabled()) {
                // clicks on words remove them from the normal contents and makes them preedit.
                int start = breakIterator.previousInclusive(eventCursorPosition);
                int end = breakIterator.next(eventCursorPosition);
                QString preedit = text.mid(start, end - start);

                d->cursor()->setPosition(start);
                d->cursor()->setPosition(end, QTextCursor::KeepAnchor);
                QTextDocumentFragment preeditFragment = d->cursor()->selection();
                d->cursor()->removeSelectedText();

                // offer the word to input context as preedit. if the input context accepts it and
                // plays nicely, it should offer the preedit right back, changing the mode to
                // active.
                bool injectionAccepted = false;

                if (ic) {
                    DuiPreeditInjectionEvent event(preedit);
                    QCoreApplication::sendEvent(ic, &event);

                    injectionAccepted = event.isAccepted();
                }

                // if injection wasn't supported, put the text back and fall back to cursor changing
                if (injectionAccepted == false) {
                    d->cursor()->insertFragment(preeditFragment);
                    d->setCursorPosition(eventCursorPosition);
                }

            } else {
                d->setCursorPosition(eventCursorPosition);
            }
        }

    } else {
        d->notifyInputContextMouseHandler(eventCursorPosition, event);
    }

    if (cursorPosition() != cursorPositionBefore) {
        updateMicroFocus();
    }
}


void DuiTextEdit::handleMouseMove(int cursorPosition, QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiTextEdit);
    d->notifyInputContextMouseHandler(cursorPosition, event);
}


void DuiTextEdit::selectAll()
{
    Q_D(DuiTextEdit);
    d->commitPreedit();

    d->cursor()->setPosition(0);
    d->cursor()->movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    if (d->cursor()->position() > 0) {
        // change mode only if there was something to select
        if (mode() != DuiTextEditModel::EditModeSelect) {
            d->setMode(DuiTextEditModel::EditModeSelect);
        }

        model()->updateCursor();
        emit selectionChanged();
        d->sendCopyAvailable(true);
    }
}


void DuiTextEdit::clear()
{
    setText("");
}


void DuiTextEdit::copy()
{
    QClipboard *clipboard = QApplication::clipboard();

    if (!hasSelectedText()
            || echoMode() != DuiTextEditModel::Normal // only allow copy from normal echo mode entry
            || !clipboard) {
        return;
    }

    clipboard->setText(textCursor().selectedText());
}


void DuiTextEdit::paste()
{
    Q_D(DuiTextEdit);
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
    } else if (mode() == DuiTextEditModel::EditModeActive) {
        d->commitPreedit();
    }

    d->setMode(DuiTextEditModel::EditModeBasic);
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
        updateMicroFocus();
    } else {
        duiDebug("DuiTextEdit") << __PRETTY_FUNCTION__ << "paste failed";
        emit pasteFailed();
    }
}


DuiTextEditModel::EditMode DuiTextEdit::mode() const
{
    return model()->edit();
}


DuiTextEditModel::LineMode DuiTextEdit::lineMode() const
{
    return model()->line();
}


Dui::TextContentType DuiTextEdit::contentType() const
{
    return model()->type();
}


void DuiTextEdit::inputMethodEvent(QInputMethodEvent *event)
{
    // FIXME: replacement info not honored.
    Q_D(DuiTextEdit);

    QString preedit = event->preeditString();
    QString commitString = event->commitString();
    bool emitReturnPressed = false;

    if (lineMode() == DuiTextEditModel::SingleLine) {
        emitReturnPressed = commitString.contains('\n');
        preedit.remove('\n');
        commitString.remove('\n');
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
            echoMode() == DuiTextEditModel::PasswordEchoOnEdit &&
            (commitString.isEmpty() == false || preedit.isEmpty() == false)) {
        // in this mode, clear the contents before starting editing and doing new input
        clear();
    }

    d->editActive = true;

    bool insertionSuccess = false;

    // append possible commit string
    if (commitString.isEmpty() == false) {
        insertionSuccess = d->doTextInsert(commitString);

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
        d->setMode(DuiTextEditModel::EditModeBasic);
        changed = true;
    }

    if (!preedit.isEmpty()) {
        QList<QInputMethodEvent::Attribute> attributes = event->attributes();
        d->setPreeditText(preedit, attributes);
        d->setMode(DuiTextEditModel::EditModeActive);
        changed = true;
    }

    if (changed) {
        updateMicroFocus();
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


void DuiTextEdit::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LayoutDirectionChange) {
        QTextOption option = document()->defaultTextOption();
        option.setTextDirection(layoutDirection());
        document()->setDefaultTextOption(option);
    }

    DuiWidgetController::changeEvent(event);
}


QTextDocument *DuiTextEdit::document() const
{
    return model()->document();
}


QTextCursor DuiTextEdit::textCursor() const
{
    return QTextCursor(*model()->cursor());
}


void DuiTextEdit::setTextCursor(const QTextCursor &cursor)
{
    Q_D(DuiTextEdit);

    DuiTextEditModel::EditMode newMode;

    if (cursor.hasSelection()) {
        newMode = DuiTextEditModel::EditModeSelect;
    } else {
        newMode = DuiTextEditModel::EditModeBasic;
    }

    DuiTextEditModel::EditMode oldMode = model()->edit();
    bool changed = (*model()->cursor() != cursor);

    d->commitPreedit();
    *(d->cursor()) = cursor;
    model()->setEdit(newMode);

    if ((newMode == DuiTextEditModel::EditModeSelect && changed) ||
            (newMode == DuiTextEditModel::EditModeBasic &&
             oldMode == DuiTextEditModel::EditModeSelect)) {
        emit selectionChanged();
    }

    if (changed) {
        emit cursorPositionChanged();
    }

    if (echoMode() == DuiTextEditModel::Normal) {
        if ((newMode == DuiTextEditModel::EditModeSelect) && (newMode != oldMode)) {
            emit copyAvailable(true);
        } else if ((newMode == DuiTextEditModel::EditModeBasic) &&
                   oldMode == DuiTextEditModel::EditModeSelect) {
            emit copyAvailable(false);
        }
    }
}


void DuiTextEdit::setContentType(Dui::TextContentType type)
{
    Q_D(DuiTextEdit);
    Qt::InputMethodHints newHint;

    model()->setType(type);

    // FIXME: doesn't work if model has content type already from somewhere
    if (d->ownValidator == true) {
        delete d->validator;
    }

    d->validator = 0;

    QRegExp rx;

    switch (type) {
    case Dui::NumberContentType:
        rx.setPattern(NumberCharacterSet);
        d->validator = new QRegExpValidator(rx, 0);
        setInputMethodCorrectionEnabled(false);
        newHint = Qt::ImhFormattedNumbersOnly;
        break;

    case Dui::PhoneNumberContentType:
        rx.setPattern(PhoneNumberCharacterSet);
        d->validator = new QRegExpValidator(rx, 0);
        setInputMethodCorrectionEnabled(false);
        newHint = Qt::ImhDialableCharactersOnly;
        break;

    case Dui::EmailContentType:
        rx.setPattern(EmailCharacterSet);
        d->validator = new QRegExpValidator(rx, 0);
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);
        newHint = Qt::ImhEmailCharactersOnly;
        break;

    case Dui::UrlContentType:
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);
        //TODO: No check rule for URL yet
        newHint = Qt::ImhUrlCharactersOnly;
        break;

    default:
        setInputMethodAutoCapitalizationEnabled(true);
        break;
    }

    // if a validator was created, we own it
    if (d->validator != 0) {
        d->ownValidator = true;
    }

    setInputMethodHints((inputMethodHints() & ~Qt::ImhExclusiveInputMask) | newHint);
}


QVariant DuiTextEdit::inputMethodQuery(Qt::InputMethodQuery query) const
{
    Q_D(const DuiTextEdit);
    QTextBlock block = d->cursor()->block();

    switch ((int)query) {
    case Qt::ImCursorPosition:
        return QVariant(cursorPosition() - block.position());

    case Qt::ImSurroundingText:
        return QVariant(block.text());

    case Qt::ImCurrentSelection:
        return QVariant(selectedText());

    case Dui::InputEnabledQuery:
        return QVariant(!isReadOnly());

    case Dui::ImCorrectionEnabledQuery:
        return QVariant(inputMethodCorrectionEnabled());

    case Dui::InputMethodToolbarQuery:
        return QVariant(attachedToolbar());

    default:
        return DuiWidgetController::inputMethodQuery(query);
    }
}


void DuiTextEdit::setTextInteractionFlags(Qt::TextInteractionFlags flags)
{
    Q_D(DuiTextEdit);

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


Qt::TextInteractionFlags DuiTextEdit::textInteractionFlags() const
{
    return model()->textInteractionFlags();
}


bool DuiTextEdit::isReadOnly() const
{
    Qt::TextInteractionFlags flags = textInteractionFlags();
    return !(flags & Qt::TextEditable);
}


void DuiTextEdit::setReadOnly(bool readOnly)
{
    Qt::TextInteractionFlags flags = textInteractionFlags();

    if (readOnly == true) {
        flags = flags & ~Qt::TextEditable;
    } else {
        flags = flags | Qt::TextEditable;
    }

    setTextInteractionFlags(flags);
}


DuiTextEditModel::EchoMode DuiTextEdit::echoMode() const
{
    return model()->echo();
}


void DuiTextEdit::setEchoMode(DuiTextEditModel::EchoMode echoMode)
{
    model()->setEcho(echoMode);
    if (echoMode != DuiTextEditModel::Normal) {
        setInputMethodHints(inputMethodHints() | Qt::ImhHiddenText);
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);

    } else {
        setInputMethodHints(inputMethodHints() & ~Qt::ImhHiddenText);
    }

    if (mode() == DuiTextEditModel::EditModeSelect) {
        emit copyAvailable(echoMode == DuiTextEditModel::Normal);
    }
}


void DuiTextEdit::deselect()
{
    Q_D(DuiTextEdit);

    if (mode() == DuiTextEditModel::EditModeSelect) {
        d->cursor()->clearSelection();
        d->setMode(DuiTextEditModel::EditModeBasic);
        model()->updateCursor();
        d->sendCopyAvailable(false);
        updateMicroFocus();
    }
}


bool DuiTextEdit::isAutoSelectionEnabled() const
{
    return isSelectionEnabled() && model()->autoSelectionEnabled();
}


void DuiTextEdit::setAutoSelectionEnabled(bool enable)
{
    model()->setAutoSelectionEnabled(enable);
}


void DuiTextEdit::setInputMethodCorrectionEnabled(bool enabled)
{
    // only allow to turn on correction for the text edit which is normal echo mode
    // content type is free text or custom content type.
    if ((enabled == false)
        || (echoMode() == DuiTextEditModel::Normal
            && (contentType() == Dui::FreeTextContentType || contentType() == Dui::CustomContentType))) {
        model()->setInputMethodCorrectionEnabled(enabled);
    }
}


bool DuiTextEdit::inputMethodCorrectionEnabled() const
{
    return model()->inputMethodCorrectionEnabled();
}


void DuiTextEdit::setInputMethodPredictionEnabled(bool enabled)
{
    model()->setInputMethodPredictionEnabled(enabled);
    if (enabled) {
        setInputMethodHints(inputMethodHints() & ~Qt::ImhNoPredictiveText);
    } else {
        setInputMethodHints(inputMethodHints() | Qt::ImhNoPredictiveText);
    }
}

bool DuiTextEdit::inputMethodPredictionEnabled() const
{
    return !(inputMethodHints() & Qt::ImhNoPredictiveText);
}

int DuiTextEdit::maxLength() const
{
    return model()->maxLength();
}

void DuiTextEdit::setMaxLength(int numChars)
{
    if (numChars < 0) {
        numChars = 0;
    }
    model()->setMaxLength(numChars);

    // Bug in QTextDocument::characterCount?
    int characterCount = document()->characterCount() - 1;
    Q_ASSERT(characterCount >= 0);

    if (characterCount > maxLength()) {
        Q_D(DuiTextEdit);

        // If we have preedit commit it first
        d->commitPreedit();

        // Make sure we're not in selection mode.
        d->setMode(DuiTextEditModel::EditModeBasic);

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

void DuiTextEdit::setInputMethodAutoCapitalizationEnabled(bool enabled)
{
    // only allow to turn on auto capitalization for the text edit which is normal echo mode
    // content type is free text or custom content type.
    if ((enabled == false)
        || (echoMode() == DuiTextEditModel::Normal
            && (contentType() == Dui::FreeTextContentType || contentType() == Dui::CustomContentType))) {
        model()->setInputMethodAutoCapitalizationEnabled(enabled);
    }

    if (model()->inputMethodAutoCapitalizationEnabled()) {
        setInputMethodHints(inputMethodHints() & ~Qt::ImhNoAutoUppercase);
    } else {
        setInputMethodHints(inputMethodHints() | Qt::ImhNoAutoUppercase);
    }
}


bool DuiTextEdit::inputMethodAutoCapitalizationEnabled() const
{
    return !(inputMethodHints() & Qt::ImhNoAutoUppercase);
}


void DuiTextEdit::setPrompt(const QString &prompt)
{
    // TODO: should replace newlines with spaces in single line?
    model()->setPrompt(prompt);
}


QString DuiTextEdit::prompt() const
{
    return model()->prompt();
}


void DuiTextEdit::setValidator(const QValidator *validator)
{
    Q_D(DuiTextEdit);

    if (d->ownValidator == true) {
        delete d->validator;
    }

    d->validator = validator;
    d->ownValidator = false;
}


const QValidator *DuiTextEdit::validator() const
{
    Q_D(const DuiTextEdit);
    return d->validator;
}


bool DuiTextEdit::hasAcceptableInput() const
{
    Q_D(const DuiTextEdit);

    if (d->validator == 0) {
        return true;
    } else {
        QString textCopy = model()->document()->toPlainText();
        int cursorCopy = model()->cursor()->position();
        QValidator::State result = d->validator->validate(textCopy, cursorCopy);
        return (result == QValidator::Acceptable);
    }
}

void DuiTextEdit::setCompleter(DuiCompleter *completer)
{
    Q_D(DuiTextEdit);
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
            updateMicroFocus();
            connect(d->completer, SIGNAL(confirmed(QString)),
                    this, SLOT(_q_confirmCompletion(QString)));
        }
    }
}

DuiCompleter *DuiTextEdit::completer()
{
    Q_D(DuiTextEdit);
    return d->completer;
}

void DuiTextEdit::attachToolbar(const QString &name)
{
    model()->setToolbar(name);
}

QString DuiTextEdit::attachedToolbar() const
{
    return model()->toolbar();
}

#include "moc_duitextedit.cpp"
