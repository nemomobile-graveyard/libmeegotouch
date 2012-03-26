/***************************************************************************
**
** Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QElapsedTimer>

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

#ifdef HAVE_MALIIT
#include <maliit/preeditinjectionevent.h>
#endif
M_REGISTER_WIDGET(MTextEdit)


namespace {
    const char * const UrlContentToolbarFile("UrlContentToolbar.xml");
    const char * const EmailContentToolbarFile("EmailContentToolbar.xml");
    const int IgnorePreeditChangeAfterDoubleClickInterval(500); // in ms
}


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
    QElapsedTimer time;
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

        // If parent scene window exists, map coordinates to its rotated coordinate system.
        // If it does not exist, use scene coordinates.

        const QPoint center = sm
                              ? focusItem->mapToItem(sm, focusItem->boundingRect().center()).toPoint()
                              : focusItem->mapToScene(focusItem->boundingRect().center()).toPoint();

        foreach (QGraphicsItem *item, focusItem->scene()->items()) {

            // TODO: Make it work for other focusable items, not just those who accept IM:
            if (item != focusItem &&
                item->flags().testFlag(QGraphicsItem::ItemAcceptsInputMethod) &&
                isChildOfSceneWindow(dynamic_cast<QGraphicsWidget *>(item), sm)) {
                bool found = false;
                const QPoint targetCenter = sm
                                            ? item->mapToItem(sm, item->boundingRect().center()).toPoint()
                                            : item->mapToScene(item->boundingRect().center()).toPoint();

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
    const QString NumberCharacterSet = QString("[\\+\\-]{0,1}([0-9%1-%2%3-%4]+([\\.,%5][0-9%1-%2%3-%4]*){0,1}){1,1}")
        .arg(QChar(0x0660)) // Arabic numbers begin
        .arg(QChar(0x0669)) // Arabic numbers end
        .arg(QChar(0x06F0)) // Extended Arabic numbers begin
        .arg(QChar(0x06F9)) // Extended Arabic numbers end
        .arg(QChar(0x066B)); // Arabic decimal separator

    //! character set for Phone Number field type
    const QString PhoneNumberCharacterSet = QString("[\\+0-9%1-%2%3-%4#\\*\\-\\+pw() ]+")
        .arg(QChar(0x0660)) // Arabic numbers begin
        .arg(QChar(0x0669)) // Arabic numbers end
        .arg(QChar(0x06F0)) // Extended Arabic numbers begin
        .arg(QChar(0x06F9)); // Extended Arabic numbers end

    //! character set for Email field type
    const QString EmailCharacterSet = "[\\w.!#$%&'*+-\\/=?\\^_\\`{|}~@\\[\\]]+";

    const QChar PlusSign('+');
    const QChar MinusSign('-');

    //! Character set for line break.
    const QString LineBreakSet = QString("\r\n%1%2")
        .arg(QChar(0x2028))     // line separator
        .arg(QChar(0x2029));    // paragraph separator

    MArrowKeyNavigator gArrowKeyNav;
}


MTextEditSignalEmitter::MTextEditSignalEmitter(MTextEditPrivate &p)
    : editPtr(p),
      scenePositionChangedConnections(0),
      oldItemSendsScenePositionChanges(false)
{
}

MTextEditSignalEmitter::~MTextEditSignalEmitter()
{
}

void MTextEditSignalEmitter::connectNotify(const char *signal)
{
    if (QLatin1String(signal) == SIGNAL(scenePositionChanged())) {
        // initialize the oldItemSendsScenePositionChanges flag
        if (scenePositionChangedConnections == 0) {
            oldItemSendsScenePositionChanges = editPtr.q_func()->flags()
                                               & QGraphicsItem::ItemSendsScenePositionChanges;
        }
        scenePositionChangedConnections++;
        editPtr.q_func()->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    }
    QObject::connectNotify(signal);
}

void MTextEditSignalEmitter::disconnectNotify(const char *signal)
{
    if (QLatin1String(signal) == SIGNAL(scenePositionChanged())) {
        scenePositionChangedConnections--;
        if (scenePositionChangedConnections <= 0 && !oldItemSendsScenePositionChanges) {
            editPtr.q_func()->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, false);
        }
    }
    QObject::disconnectNotify(signal);
}


/*!
 * \brief Constructor
 * \param type widget type (single line or multiline)
 */
MTextEditPrivate::MTextEditPrivate()
    : focusEventState(NoFocusEventReceivedYet),
      validator(0),
      ownValidator(false),
      completer(0),
      registeredToolbarId(-1),
      editActive(false),
      omitInputMethodEvents(false),
      updateMicroFocusDisabled(0),
      pendingMicroFocusUpdate(false),
      doubleClick(false),
      previousReleaseWordStart(0),
      previousReleaseWordEnd(0),
      signalEmitter(*this),
      preeditInjectionInProgress(false),
      cutAction(0),
      copyAction(0),
      pasteAction(0),
      programmaticalDocumentChange(false),
      positionChangeTimeout(0),
      hasPositionChangesToHandle(false),
      pasteFailed(false),
      layoutDirection(Qt::LayoutDirectionAuto)
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
    // Do not use qApp->->layoutDirection() here, the
    // default text direction should be determined from the
    // contents of the string, not from the layout direction
    // of the application. This is achieved by using
    // Qt::LayoutDirectionAuto.
    option.setTextDirection(Qt::LayoutDirectionAuto);

    if (q->model()->line() == MTextEditModel::SingleLine)
        option.setWrapMode(QTextOption::NoWrap);

    q->document()->setDefaultTextOption(option);

    if (q->model()->line() == MTextEditModel::SingleLine) {
        q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    } else {
        //Set to expand vertically only in multiline mode
        QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        policy.setHeightForWidth(true);
        q->setSizePolicy(policy);
    }

    if (!q->model()->cursor()) {
        q->model()->setCursor(new QTextCursor(q->document()));
    }
    q->model()->cursor()->movePosition(QTextCursor::End);

    q->setFocusPolicy(Qt::ClickFocus);
    q->setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);

    // Set up default editor toolbar actions.  It's possible for MTextEdit
    // children to replace them and it's recommended that they do that right
    // after MTextEdit construction and don't change them later.  The actions of
    // MTextEdit are used as MEditorToolbar actions by MTextEditView when the
    // toolbar is shown.  A dynamic property "noCloseToolbarOnTrigger" of
    // boolean type may be set for an action and if true, it disables the
    // default behaviour where triggering an action closes the toolbar.
    // For now we don't advertise actions as a public MTextEdit API.
    QObject::connect(q, SIGNAL(copyAvailable(bool)), &cutAction, SLOT(setVisible(bool)));
    QObject::connect(q, SIGNAL(copyAvailable(bool)), &copyAction, SLOT(setVisible(bool)));
    // paste visibility is controlled by _q_updatePasteActionState,
    // (dis)connected in focusInEvent and focusOutEvent
    cutAction.setVisible(false);
    copyAction.setVisible(false);
    pasteAction.setVisible(!QApplication::clipboard()->text().isEmpty());

    QObject::connect(&cutAction, SIGNAL(triggered(bool)), q, SLOT(cut()));
    QObject::connect(&copyAction, SIGNAL(triggered(bool)), q, SLOT(_q_copyAndDeselect()));
    QObject::connect(&pasteAction, SIGNAL(triggered(bool)), q, SLOT(_q_pasteAndClear()));

    cutAction.setObjectName("cutAction");
    copyAction.setObjectName("copyAction");
    pasteAction.setObjectName("pasteAction");

    q->addAction(&cutAction);
    q->addAction(&copyAction);
    q->addAction(&pasteAction);

    QObject::connect(q, SIGNAL(geometryChanged()),
                     q, SLOT(_q_updateTextDirection()));

    q->retranslateUi();

    // to avoid too frequent calls of QInputContext::updateMicroFocus(),
    // at most 1 change in a period (see call of setInterval() for value) is issued
    // it works like this:
    // * 1st update triggers updateMicroFocus(), then timer waits for period.
    // * if there were updates, call updateMicroFocus(), repeat, otherwise stop a timer.
    positionChangeTimeout = new QTimer(q);
    positionChangeTimeout->setInterval(50);
    positionChangeTimeout->setSingleShot(false);
    QObject::connect(positionChangeTimeout, SIGNAL(timeout()), q, SLOT(_q_checkPositionChanges()));

    QObject::connect(q, SIGNAL(pasteFailed()), q, SLOT(_q_onPasteFailed()));

    QObject::connect(q->model(), SIGNAL(modified(const QList<const char *>)),
                     q, SLOT(_q_updateModelData(const QList<const char *>)));
}

void MTextEditPrivate::_q_updatePasteActionState()
{
    Q_Q(const MTextEdit);

    if (q->isReadOnly()) {
        return;
    }

    const bool hasText = !QApplication::clipboard()->text().isEmpty();
    pasteAction.setVisible(hasText);
}

void MTextEditPrivate::_q_handlePositionChanged()
{
    if (positionChangeTimeout->isActive()) {
        hasPositionChangesToHandle = true;
    } else {
        // timer is stopper ==> this update is first after some long period
        hasPositionChangesToHandle = false;
        positionChangeTimeout->start();
        updateMicroFocus();
    }
}

void MTextEditPrivate::_q_checkPositionChanges()
{
    if (hasPositionChangesToHandle) {
        hasPositionChangesToHandle = false;
        updateMicroFocus();
    } else {
        positionChangeTimeout->stop();
    }
}

void MTextEditPrivate::_q_updateTextDirection()
{
    Q_Q(MTextEdit);

    Qt::LayoutDirection dir;

    if (q->layoutDirection() == Qt::LayoutDirectionAuto) {
	QString content = q->document()->toPlainText();
	dir = strongDirectionForText(content);

	// Set direction based on input method language if...
	if (dir == Qt::LayoutDirectionAuto // ...text has no strong directionality
	    || q->model()->echo() == MTextEditModel::NoEcho // ...no text because of NoEcho
	    // ...or if entering password; we cannot see the content anyhow.
	    || q->model()->echo() == MTextEditModel::Password) {
	    dir = inputMethodBasedTextDirection();
	}
    } else {
	dir = q->layoutDirection();
    }

    // Otherwise use content based text direction.
    // The value Qt::LayoutDirectionAuto also uses text content
    // to determine layout direction but this leads to different
    // blocks having different layout directions.

    QTextOption option = q->document()->defaultTextOption();
    if (option.textDirection() != dir) {
        option.setTextDirection(dir);
        q->document()->setDefaultTextOption(option);
        q->update();
    }
}

Qt::LayoutDirection MTextEditPrivate::strongDirectionForText(const QString &text) const
{
    QString::const_iterator textEnd = text.constEnd();

    for (QString::const_iterator it = text.constBegin(); it != textEnd; ++it) {
        switch(QChar::direction(it->unicode()))
        {
        case QChar::DirL:
        case QChar::DirLRE:
        case QChar::DirLRO:
            return Qt::LeftToRight;
        case QChar::DirAL:
        case QChar::DirR:
        case QChar::DirRLE:
        case QChar::DirRLO:
            return Qt::RightToLeft;
        default:
            break;
        }
    }
    // Return Qt::LayoutDirectionAuto to indicate that the text
    // contains no strong directional characters.
    return Qt::LayoutDirectionAuto;
}

void MTextEditPrivate::_q_updateModelData(const QList<const char *> &modifications)
{
    Q_Q(const MTextEdit);

    foreach (const char *member, modifications) {
        if (member == MTextEditModel::InputContextUpdateEnabled) {
            if (q->model()->isInputContextUpdateEnabled()) {
                enableUpdateMicroFocus(true);
            } else {
                disableUpdateMicroFocus();
            }
            break;
        }
    }
}

Qt::LayoutDirection MTextEditPrivate::inputMethodBasedTextDirection() const
{
    QString language = MInputMethodState::instance()->language();

    Qt::LayoutDirection dir;
    if (!language.isEmpty()) {
        dir = QLocale(language).textDirection();
    } else {
        dir = Qt::LayoutDirectionAuto;
    }

    return dir;
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
  * \param True if start of word is deleted. False if only single character is
  * deleted.
  * \return success
  */
bool MTextEditPrivate::doBackspace(bool deleteStartOfWord)
{
    Q_Q(MTextEdit);

    if (q->isReadOnly()) {
        return false;
    }

    int position = q->textCursor().position();
    if (position == 0) {
        return false;
    }

    // Get the style that would be applied when a text is inserted at the cursor position
    QTextCharFormat format = cursor()->charFormat();

    if (deleteStartOfWord) {
        cursor()->movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
    } else {
        cursor()->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    }
    QTextDocumentFragment currentFragment = cursor()->selection();
    cursor()->removeSelectedText();

    if (validateCurrentBlock() == true) {
        cursor()->setCharFormat(format);
        return true;

    } else {
        // document doesn't validate after delete -> put the fragment back
        cursor()->insertFragment(currentFragment);
        return false;
    }
}


/*!
  * \brief Deletes a character at cursor position
  * \param True if end of word is deleted. False if only single character is
  * deleted.
  * \return success
  */
bool MTextEditPrivate::doDelete(bool deleteEndOfWord)
{
    Q_Q(MTextEdit);

    if (q->isReadOnly()) {
        return false;
    }

    // Set text cursor to model, if not yet set.
    q->textCursor();

    if (deleteEndOfWord) {
        cursor()->movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
    } else {
        cursor()->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    }
    QTextDocumentFragment currentFragment = cursor()->selection();
    cursor()->removeSelectedText();

    if (!validateCurrentBlock()) {
        // document doesn't validate after delete -> put the fragment back
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
    int characterCount = realCharacterCount();

    // Total characterCount mustn't exceed maxLength.
    if (characterCount + filteredText.length() > q->maxLength()) {
        filteredText.truncate(q->maxLength() - characterCount);

        if (filteredText.length() == 0) {
            return false;
        }
        changed = true;
    }

    // on single line newline are changed into spaces
    if (q->lineMode() == MTextEditModel::SingleLine) {
        filteredText = replaceLineBreaks(filteredText, QChar(' '));
    }

    int textPosition = 0;
    int filteredTextLength = filteredText.length();
    int snippetLength = -1;
    int listIndex = 0;
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

        if (snippetLength > 0) {
            textPosition += snippetLength;
        } else {
            textPosition = filteredTextLength;
        }
    } while (textPosition < filteredTextLength);
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
    Q_Q(MTextEdit);
    QTextCursor &textCursor(*cursor());

    const bool append(isPreediting() && !text.isEmpty() && text.startsWith(textCursor.selectedText()));

    if (!append) {
        removePreedit();
    }

    const int preeditTextLength(text.length());
    const int oldPreeditLength(append ? textCursor.selectedText().length() : 0);

    QTextCursor preeditStartCursor(textCursor);
    preeditStartCursor.setPosition(preeditStartCursor.position() - oldPreeditLength, QTextCursor::MoveAnchor);

    if (isPreediting()) {
        // destroys character format even if there is no selection, so only do this when pre-editing
        textCursor.clearSelection();
    }

    QList<MTextEditFormatRange> additionalFormats;

    // set preeditCursorPos to -1 to hide cursor for preedit by default
    int preeditCursorPos = -1;
    // parse attributes
    const int size = attributes.size();
    for (int i = 0; i < size; ++i) {
        const QInputMethodEvent::Attribute &attribute = attributes.at(i);
        if (attribute.type == QInputMethodEvent::TextFormat) {
            const QTextCharFormat format = attribute.value.value<QTextFormat>().toCharFormat();

            if (format.isValid()) {
                MTextEditFormatRange formatRange;
                formatRange.start = attribute.start + preeditStartCursor.position();
                formatRange.length = attribute.length;
                formatRange.format = format;
                additionalFormats.append(formatRange);
            }
        } else if (attribute.type == QInputMethodEvent::Cursor) {
            if (attribute.length > 0) {
                preeditCursorPos = attribute.start;
                //TODO: should honor cursor color
            }
        }
    }
    q->model()->setPreeditCursor(preeditCursorPos);

    //MTextEditView formats the pre-edit text depending on the values set here.
    //
    //An alternative implementation would be to use QTextLayout::setAdditionalFormats.
    //But this would cause problems in MTextEditView because calling QTextLayout::setAdditionalFormats
    //causes a contentChange signal covering the whole text to be emitted.
    q->model()->setAdditionalFormats(additionalFormats);

    int listIndex(append ? preeditStyling.size() - 1 : 0);
    int count(append ? preeditStyling[listIndex].count : 0);
    insertTextWithPreeditStyling(text.mid(oldPreeditLength), listIndex, count);
    clearUnusedPreeditStyling(listIndex, count);

    // mark preedit as selection
    int position = textCursor.position();
    textCursor.setPosition(position - preeditTextLength, QTextCursor::MoveAnchor);
    textCursor.setPosition(position, QTextCursor::KeepAnchor);
}


/*!
  * \brief Commit current pre-edit so it becomes normal text
  */
void MTextEditPrivate::commitPreedit(bool keepPreeditCursorPosition)
{
    Q_Q(MTextEdit);

    preeditStyling.clear();

    // Nothing to commit if not pre-editing
    if (isPreediting() == false) {
        return;
    }

    QTextCursor *textCursor = cursor();

    int characterCount = realCharacterCount();
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
    q->model()->setAdditionalFormats(QList<MTextEditFormatRange>());

    int newCursorPosition = 0;
    if (keepPreeditCursorPosition) {
        newCursorPosition = textCursor->selectionStart() + q->model()->preeditCursor();
    }

    if (validateCurrentBlock() == true) {
        // make preedit selection part of the normal text
        textCursor->clearSelection();

    } else {
        // block content with the preedit doesn't validate, need to remove the commit string
        textCursor->removeSelectedText();
    }

    setMode(MTextEditModel::EditModeBasic);

    if (keepPreeditCursorPosition) {
        setCursorPosition(newCursorPosition);
    }

    updateMicroFocus();

    safeReset();
}


/*!
 * \brief Removes pre-edit text, doesn't affect mode state
 */
void MTextEditPrivate::removePreedit()
{
    Q_Q(MTextEdit);
    
    if (isPreediting() == false) {
        return;
    }

    q->model()->setAdditionalFormats(QList<MTextEditFormatRange>());

    // Remember current format because during
    // removeSelectedText it will be cleared
    QTextCharFormat format = cursor()->charFormat();
    cursor()->removeSelectedText();
    cursor()->setCharFormat(format);
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
    Q_Q(MTextEdit);
    QInputContext *inputContext = qApp->inputContext();
    QGraphicsView *focusedGraphicsView = dynamic_cast<QGraphicsView *>(QApplication::focusWidget());

    const bool editable = q->model()->textInteractionFlags() & Qt::TextEditable;
    if (!inputContext || !focusedGraphicsView || !editable) {
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
}

void MTextEditPrivate::requestAutoSip()
{
    Q_Q(MTextEdit);

    if (q->isAutoSipEnabled()) {
        requestSip();
    }
}

void MTextEditPrivate::closeAutoSip()
{
    Q_Q(MTextEdit);

    if (q->isAutoSipEnabled()) {
        MInputMethodState::closeSoftwareInputPanel();
    }
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
    Q_Q(MTextEdit);
    QTextCursor textCursor(q->document());

    for (int i = start + 1; i <= end; ++i) {
        textCursor.setPosition(i, QTextCursor::KeepAnchor);
        const QTextCharFormat charFormat = textCursor.charFormat();

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
}


/*!
 * \brief inserts text by applying the stored preedit styling information
 * \param currentListIndex styling list index to start reading from the stored styling
 * \param currentCount character index within the current styling list index.  In other
 * words, currentCount is the number of characters that have already been used from the
 * number indicated by preeditStyling[currentListIndex].count.  Warning: this is
 * exactly the opposite of what it used to be originally.
 */
void MTextEditPrivate::insertTextWithPreeditStyling(const QString &text, int &currentListIndex,
                                                    int &currentCount)
{
    if (text.isEmpty()) {
        return;
    }

    QTextCursor &textCursor(*cursor());

    if (preeditStyling.size() == 0) {
        Q_ASSERT(currentListIndex == 0);
        Q_ASSERT(currentCount == 0);

        styleData defaultStyle;
        defaultStyle.charFormat = textCursor.charFormat();
        defaultStyle.count = text.length();
        preeditStyling.push_back(defaultStyle);
    }

    const int textLength(text.length());
    const int numStyles(preeditStyling.size());
    int textIndex(0);

    Q_ASSERT((currentListIndex < numStyles) && (currentListIndex >= 0));
    Q_ASSERT(currentCount <= preeditStyling[currentListIndex].count);

    --currentListIndex;

    do {
        ++currentListIndex;
        const int remainingTextLength(textLength - textIndex);
        if ((currentListIndex == (numStyles - 1))
            && (preeditStyling[currentListIndex].count < (currentCount + remainingTextLength))) {
            preeditStyling[currentListIndex].count = remainingTextLength + currentCount;
        }

        if (preeditStyling[currentListIndex].count == 0) {
            continue;
        }

        const int insertLength(qMin(preeditStyling[currentListIndex].count - currentCount,
                                    remainingTextLength));
        currentCount = (currentCount + insertLength) % preeditStyling[currentListIndex].count;
        textCursor.insertText(text.mid(textIndex, insertLength),
                              preeditStyling[currentListIndex].charFormat);
        textIndex += insertLength;
    } while (currentListIndex < (numStyles - 1) && (textIndex < textLength));

    if (currentCount == 0) {    // we used all of the last style?
        currentCount = preeditStyling[currentListIndex].count;
    }
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
    const QString preedit = cursor()->selectedText();
    Q_ASSERT(!preedit.isEmpty());

    const int preeditCursorPos = q->model()->preeditCursor();
    int currentStyleLastCharIndex = 0;
    int currentStyleIndex = currentPreeditStylingIndex(currentStyleLastCharIndex);
    if (currentStyleIndex >= 0) {
        format = preeditStyling.at(currentStyleIndex).charFormat;
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

    if ((preeditStyleSize > 0) && (preeditStyling.at(currentStyleIndex).count == 0)) {
        preeditStyling[currentStyleIndex].charFormat = format;
    } else {
        // insert/append new style
        styleData newStyle;
        newStyle.charFormat = format;
        newStyle.count = 0;
        if (preeditStyleSize == 0 || preeditCursorPos == preedit.length()) {
            // append new style to the end of (or empty) style list.
            preeditStyling.push_back(newStyle);
        } else {
            preeditStyling.insert(currentStyleIndex + 1, newStyle);
            if (currentStyleLastCharIndex > preeditCursorPos) {
                // new style splits current style to two parts when cursor is inside preedit
                styleData splitStyle = preeditStyling.at(currentStyleIndex);
                preeditStyling[currentStyleIndex].count -= currentStyleLastCharIndex - preeditCursorPos;
                splitStyle.count = currentStyleLastCharIndex - preeditCursorPos;
                preeditStyling.insert(currentStyleIndex + 2, splitStyle);
            }
        }
    }
}

/*!
 * \brief returns the cursor's current character format in the stored preedit styling
 */
QTextCharFormat MTextEditPrivate::currentPreeditCharFormat() const
{
    QTextCharFormat format;
    const QString preedit = cursor()->selectedText();
    int currentStyleLastCharIndex = 0;
    int currentStyleIndex = currentPreeditStylingIndex(currentStyleLastCharIndex);
    if (currentStyleIndex >= 0) {
        format = preeditStyling.at(currentStyleIndex).charFormat;
    }
    return format;

}

/*!
 * \brief clears the unused styling from stored preedit styling information
 * \param currentListIndex styling list index to start erasing from the stored styling
 * \param currentCount character index within the current styling list index
 * \sa MTextEditPrivate::insertTextWithPreeditStyling
 */
void MTextEditPrivate::clearUnusedPreeditStyling(int currentListIndex, int currentCount)
{
    if ((currentListIndex < 0) || (currentListIndex >= preeditStyling.size())) {
        return;
    }

    // Truncate the count of the last used style to what was actually used up and...
    styleData &lastUsedStyle = preeditStyling[currentListIndex];
    lastUsedStyle.count = qMin(lastUsedStyle.count, currentCount);

    // ...remove it, if nothing was used, and the rest of the style entries.
    preeditStyling.erase(preeditStyling.begin() + currentListIndex + (lastUsedStyle.count ? 1 : 0),
                         preeditStyling.end());
}

/*!
 * \brief Returns the index of the preedit styling used for current cursor position in
 *  the stored preedit styling list.
 *  \param currentStyleLastCharIndex The index of last character for the found preedit style.
 */
int MTextEditPrivate::currentPreeditStylingIndex(int &currentStyleLastCharIndex) const
{
    Q_Q(const MTextEdit);

    const QString preedit = cursor()->selectedText();
    Q_ASSERT(!preedit.isEmpty());

    int preeditStyleSize = preeditStyling.size();
    const int preeditCursorPos = q->model()->preeditCursor();
    currentStyleLastCharIndex = 0;

    if (preeditStyleSize <= 0)
        return -1;

    int currentStyleIndex = 0;

    if (preeditCursorPos > 0 && preeditCursorPos != preedit.length()) {
        // when cursor is inside preedit, search the index according to preedit
        // cursor position.
        for (int i = 0; i < preeditStyleSize; i++) {
            currentStyleLastCharIndex += preeditStyling.at(i).count;
            if (currentStyleLastCharIndex >= preeditCursorPos) {
                // If the cursor is just at the end of the found style, and the
                // next style has an empty count which means it is a new inserted
                // style, then the next style should be the current style for
                // the cursor.
                if (i < preeditStyleSize - 1
                    && currentStyleLastCharIndex == preeditCursorPos
                    && preeditStyling.at(i + 1).count == 0) {
                    currentStyleIndex = i + 1;
                } else {
                    currentStyleIndex = i;
                }
                break;
            }
        }
    } else {
        // when cursor is at the end of preedit, return the last index.
        currentStyleIndex = preeditStyleSize - 1;
    }
    return currentStyleIndex;
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
    int index = text.lastIndexOf(prefix, cursorPos - text.length() - 1);
    index = qMax(index, 0);
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

bool MTextEditPrivate::checkingCopy()
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

QString MTextEditPrivate::replaceLineBreaks(QString text, QString replacement)
{
    // FIXME: this implementation works quite slow if text or LineBreakSet is long

    // First, handle \r\n sequences to avoid double replacement characters.
    text.replace("\r\n", replacement);
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

void MTextEditPrivate::icUpdate()
{
    Q_Q(MTextEdit);
    QInputContext *ic = qApp->inputContext();

    if (!ic || !q->hasFocus()) {
        return;
    }

    ic->update();
}

void MTextEditPrivate::safeReset()
{
    Q_Q(MTextEdit);

    if (q->hasFocus()) {
        QInputContext *ic = qApp->inputContext();
        if (ic) {
            omitInputMethodEvents = true;
            ic->reset();
            omitInputMethodEvents = false;
        }
    }
}

int MTextEditPrivate::realCharacterCount() const
{
    Q_Q(const MTextEdit);

    // QTextDocument::characterCount() will return one extra character
    // as there is invisible paragraph separator (0x2029) at the end.
    // see: http://bugreports.qt.nokia.com/browse/QTBUG-4841
    return q->document()->characterCount() - 1;
}

void  MTextEditPrivate::connectCompleter()
{
    Q_Q(MTextEdit);
    if (completer) {
        // The completer can be shared by several widgets, so call its setWidget,
        // and connect related slots again when getting focus.
        completer->setWidget(q);
        QObject::connect(completer, SIGNAL(confirmed(QString, QModelIndex)),
                         q, SLOT(_q_confirmCompletion(QString)));
        QObject::connect(q, SIGNAL(textChanged()),
                         completer, SLOT(complete()));
    }
}

void  MTextEditPrivate::disconnectCompleter()
{
    Q_Q(MTextEdit);
    if (completer) {
        // hide completer when disconnecting (focus out)
        completer->hideCompleter();
        // disconnect related slots when the widget (losing focus)
        QObject::disconnect(completer, 0, q, 0);
        QObject::disconnect(q, 0, completer, 0);
    }
}

void MTextEditPrivate::_q_copyAndDeselect()
{
    Q_Q(MTextEdit);
    q->copy();
    q->deselect();
}

void MTextEditPrivate::_q_pasteAndClear()
{
    Q_Q(MTextEdit);
    pasteFailed = false;
    q->paste();
    if (!pasteFailed) {
        QApplication::clipboard()->clear(QClipboard::Clipboard);
    }
}

void MTextEditPrivate::_q_onPasteFailed()
{
    pasteFailed = true;
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
        d->disconnectCompleter();
        if (d->completer) {
            d->completer->setWidget(0);
        }
    }

    detachToolbar();
}


void MTextEdit::setSelection(int anchorPosition, int length, bool useBoundaries)
{
    setSelection(anchorPosition, length, useBoundaries, MTextEditModel::SelectionHandlesAndToolbar);
}

void MTextEdit::setSelection(int anchorPosition, int length, bool useBoundaries, MTextEditModel::SelectionControls selectionControls)
{
    Q_D(MTextEdit);
    if (!isSelectionEnabled()) {
        return;
    }

    QString text = document()->toPlainText();

    // boundary check for positions
    anchorPosition = qBound(0, anchorPosition, text.length());
    length = qBound(-anchorPosition, length, text.length() - anchorPosition);

    d->commitPreedit();

    int newCursorPosition = anchorPosition + length;

    if (useBoundaries) {
        MBreakIterator breakIterator(text);
        const bool forward = newCursorPosition >= anchorPosition;
        if (forward) {
            anchorPosition = breakIterator.previousInclusive(anchorPosition);
            if (!breakIterator.isBoundary(newCursorPosition)) {
                newCursorPosition = breakIterator.next(newCursorPosition);
            }
        } else {
            if (!breakIterator.isBoundary(anchorPosition)) {
                anchorPosition = breakIterator.next(anchorPosition);
            }
            newCursorPosition = breakIterator.previousInclusive(newCursorPosition);
        }
    }

    // check if change actually happens
    QTextCursor *currentCursor = d->cursor();
    if (anchorPosition == currentCursor->anchor() && newCursorPosition == currentCursor->position()
        && model()->enabledSelectionControls() == selectionControls) {
        emit selectionChanged(); // for updating selection handles
        return;
    }

    // make an actual selection
    currentCursor->setPosition(anchorPosition, QTextCursor::MoveAnchor);
    currentCursor->setPosition(newCursorPosition, QTextCursor::KeepAnchor);

    // update mode to selection or basic if needed
    if (anchorPosition != newCursorPosition) {
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

    model()->setEnabledSelectionControls(selectionControls);
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

    case Qt::Key_Home:
        moveDirection = QTextCursor::StartOfLine;
        break;

    case Qt::Key_End:
        moveDirection = QTextCursor::EndOfLine;
        break;

    case Qt::Key_PageDown:
        moveDirection = QTextCursor::End;
        break;

    case Qt::Key_PageUp:
        moveDirection = QTextCursor::Start;
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
                model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
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
        document()->clear();
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
            !event->matches(QKeySequence::DeleteStartOfWord) &&
            !event->matches(QKeySequence::DeleteEndOfWord) &&
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

    if ( event->matches(QKeySequence::DeleteStartOfWord) ) {
        if (wasSelecting == false) {
            modified = d->doBackspace(true);
        } else if (d->validateCurrentBlock()) {
            d->cursor()->setCharFormat(format);
            modified = true;
        }
    } else if ( event->matches(QKeySequence::DeleteEndOfWord) ) {
        if (wasSelecting == false) {
            modified = d->doDelete(true);
        } else if (d->validateCurrentBlock()) {
            modified = true;
        }
    } else {

        switch (key) {
        case Qt::Key_Backspace:
            // backspace and delete in selection mode are special cases, only selection is removed
            // These also need explicit validation here.
            if (wasSelecting == false) {
                modified = d->doBackspace(false);
            } else if (d->validateCurrentBlock()) {
                d->cursor()->setCharFormat(format);
                modified = true;
            }
            break;

        case Qt::Key_Delete:
            if (wasSelecting == false) {
                modified = d->doDelete(false);
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
        } // default
        } // switch
    }

    if (modified == true) {
        event->accept();

        if (wasSelecting == true) {
            d->setMode(MTextEditModel::EditModeBasic);
            model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
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
    MWidgetController::mouseReleaseEvent(event);
}

void MTextEdit::focusInEvent(QFocusEvent *event)
{
    Q_D(MTextEdit);

    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    d->focusEventState = MTextEditPrivate::FocusInEventReceived;

    d->editActive = false;

    // Setup text direction. This special direction handling is done on focus
    // in event since that's when cursors becomes visible.
    // 1) Empty text edit:
    //    Text direction is set based on a guess on what user will be typing.
    //    Results are visible in cursor position, left or right aligned.
    //    Implemented by using language information from MInputMethodState.
    // 2) Non-empty text edit:
    //    Text direction is set based on text content.
    //    Implemented by using the flag Qt::LayoutDirectionAuto.
    //    If QApplication::keyboardInputDirection() was supported the case 1)
    //    would be handled automatically but now direction of empty text edit
    //    has to be determined separately.

    // Text direction depends on text contents on the row cursor is currently in,
    // and input language; connect appropriate signals.

    d->_q_updateTextDirection();
    bool ok = true;
    ok &= connect(this, SIGNAL(cursorPositionChanged()),
                  this, SLOT(_q_updateTextDirection()));
    ok &= connect(MInputMethodState::instance(), SIGNAL(languageChanged(QString)),
                  this, SLOT(_q_updateTextDirection()));
    Q_ASSERT(ok);

    if (sceneManager()) {
        d->requestAutoSip();
    }

    if (model()->autoSelectionEnabled() == true) {
        selectAll();
    }

    d->connectCompleter();

    // Listen to clipboard changes and update visibility of paste action accordingly.
    d->_q_updatePasteActionState();
    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(_q_updatePasteActionState()));

    emit gainedFocus(event->reason());

    connect(&d->signalEmitter, SIGNAL(scenePositionChanged()), this, SLOT(_q_handlePositionChanged()));
}

// KLUDGE WARNING: Qt doesn't remove subfocus when losing focus by clicking outside 
// the widget. Workaround by accessing private method this way. Remove when Qt is fixed.
// http://bugreports.qt.nokia.com/browse/QTBUG-16014
class QGraphicsItemPrivate {
public:
    void clearSubFocus(QGraphicsItem *rootItem = 0, QGraphicsItem *stopItem = 0);
};

void MTextEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    Q_D(MTextEdit);

    if (d->focusEventState == MTextEditPrivate::FocusOutEventReceived) {
	// NB#275500 - Selected text does not have any rich-text-editor
	// NB#288313 - Text selection handles are shown over style settings dialog
	// this event handler has been already manually called with proper reason
	// not executing this second automatic call to avoid undesireable effect.
	// NOTE: things may go wrong if focusing in failed to send focusInEvent()
	// then subsequent focusOutEvent() would be ignored due to this;
	// but it is a minor problem compared to other results of not receiving focusInEvent(),
	// so not doing anything to avoid it
        return;
    }

    bool ok = true;
    ok &= disconnect(this, SIGNAL(cursorPositionChanged()),
                     this, SLOT(_q_updateTextDirection()));
    ok &= disconnect(MInputMethodState::instance(), SIGNAL(languageChanged(QString)),
                     this, SLOT(_q_updateTextDirection()));
    if (!ok) {
        qWarning() << "Signal disconnection failed in MTextEdit::focusOutEvent";
    }

    disconnect(&d->signalEmitter, SIGNAL(scenePositionChanged()), this, SLOT(_q_handlePositionChanged()));

    disconnect(QApplication::clipboard(), SIGNAL(dataChanged()),
               this, SLOT(_q_updatePasteActionState()));

    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    d->focusEventState = MTextEditPrivate::FocusOutEventReceived;
    d->lastMousePressTime = QTime(); // assume click somewhere else that removed focus

    // Need to tell the MArrowKeyNavigator that the next key event it sees
    // would come from a different focus item. Otherwise, it could think
    // of the key event as an auto-repeat
    // TODO: Replace with better auto-repeat detection, and remove this hack.
    gArrowKeyNav.skipNextRequest();

    d->commitPreedit();

    d->doubleClick = false;
    d->doubleClickSelectionTime = QTime();

    Qt::FocusReason reason = event->reason();
    if (reason != Qt::ActiveWindowFocusReason && reason != Qt::PopupFocusReason)
        deselect();

#if QT_VERSION >= 0x040702 // 4.7.1 was missing stopItem parameter
    // kludge warning!
    QGraphicsItem::d_ptr->clearSubFocus();
#endif

    d->disconnectCompleter();

    emit lostFocus(event->reason());

    MInputMethodState::closeSoftwareInputPanel();
}

bool MTextEdit::insert(const QString &text)
{
    Q_D(MTextEdit);

    bool wasSelecting = hasSelectedText();

    // back to basic mode
    if ((mode() == MTextEditModel::EditModeSelect) && d->cursor()->hasSelection()) {
        d->cursor()->removeSelectedText();
        model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
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

    d->safeReset();

    int cursorPosBefore = d->cursor()->position();
    bool wasSelecting = hasSelectedText();
    bool wasEmpty = document()->isEmpty();

    d->programmaticalDocumentChange = true; // hack to allow MTextEditView avoid delayed masking

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
        filteredText = d->replaceLineBreaks(filteredText, QChar(' '));
    }

    d->cursor()->insertText(filteredText);

    bool accepted = d->validate();

    if (!accepted) {
        document()->clear();
    }

    // only avoid signaling if empty before and after
    if (!(document()->isEmpty() && wasEmpty)) {
        d->updateMicroFocus();
        emit textChanged();
    }

    if (d->cursor()->position() != cursorPosBefore) {
        emit cursorPositionChanged();
    }

    if (wasSelecting) {
        d->sendCopyAvailable(false);
    }

    d->programmaticalDocumentChange = false;

    return accepted;
}


QString MTextEdit::text() const
{
    Q_D(const MTextEdit);
    QString content = document()->toPlainText();

    // preedit cannot be limited that easily, but we'll respect the limitation
    // in return value by chopping preedit
    if (d->isPreediting() && (content.length() > maxLength())) {
        int surplusCharacters = content.length() - maxLength();        
        content.remove(d->cursor()->position() - surplusCharacters, surplusCharacters);
    }

    return content;
}


int MTextEdit::cursorPosition() const
{
    Q_D(const MTextEdit);
    int position = d->cursor()->position();

    if (d->isPreediting() && model()->preeditCursor() >= 0) {
        position = d->cursor()->anchor() + model()->preeditCursor();
    }

    return position;
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

    QString content = document()->toPlainText();
    MBreakIterator breakIterator(content);
    d->doubleClick = d->lastMousePressTime.isValid()
        && (d->lastMousePressTime.elapsed() < QApplication::doubleClickInterval())
        && (breakIterator.previousInclusive(cursorPosition) == d->previousReleaseWordStart)
        && (breakIterator.next(cursorPosition) == d->previousReleaseWordEnd);

    if (d->doubleClick) {
        d->lastMousePressTime = QTime();
    } else {
        d->lastMousePressTime.start();
    }

    if (textInteractionFlags() != Qt::NoTextInteraction && location) {
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


// Beware: handleMouseRelease is not called for every mouse release event; see
// MTextEditView::mouseReleaseEvent().
void MTextEdit::handleMouseRelease(int eventCursorPosition, QGraphicsSceneMouseEvent *event,
                                   TextFieldLocationType *location)
{
    Q_D(MTextEdit);
    if (textInteractionFlags() == Qt::NoTextInteraction)
        return;

    d->disableUpdateMicroFocus();
    int cursorPositionBefore = d->cursor()->position();
    bool updatePending = false;

    deselect();

    d->previousReleaseWordStart = 0;
    d->previousReleaseWordEnd = 0;

    if (d->isPositionOnPreedit(eventCursorPosition) == false
        || (d->doubleClick && (textInteractionFlags() & Qt::TextSelectableByMouse))) {
        // input context takes care of releases happening on top of preedit, the rest
        // is handled here
        d->commitPreedit();

        QString text = document()->toPlainText();
        MBreakIterator breakIterator(text);

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
            d->previousReleaseWordStart = breakIterator.previousInclusive(eventCursorPosition);
            d->previousReleaseWordEnd = breakIterator.next(eventCursorPosition);

            if (d->doubleClick) {
                // select the word if flags allow, otherwise omit the gesture
                if (textInteractionFlags() & Qt::TextSelectableByMouse) {
                    const int start(d->previousReleaseWordStart);
                    const int end(d->previousReleaseWordEnd);

                    d->cursor()->setPosition(start);
                    d->cursor()->setPosition(end, QTextCursor::KeepAnchor);
                    d->setMode(MTextEditModel::EditModeSelect);
                    model()->updateCursor();
                    model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
                    emit selectionChanged();
                    d->sendCopyAvailable(true);
                    d->doubleClickSelectionTime = QTime::currentTime();
                    updatePending = true;
                }
            } else if (inputMethodCorrectionEnabled()
                       && !isReadOnly()) {
                // clicks on words remove them from the normal contents and makes them preedit.
                const int start(d->previousReleaseWordStart);
                const int end(d->previousReleaseWordEnd);

                d->cursor()->setPosition(start);
                d->cursor()->setPosition(end, QTextCursor::KeepAnchor);

                d->storePreeditTextStyling(start, end);
                QTextDocumentFragment preeditFragment = d->cursor()->selection();
                d->cursor()->removeSelectedText();

                // offer the word to input context as preedit. if the input context accepts it and
                // plays nicely, it should offer the preedit right back, changing the mode to
                // active.
                bool injectionAccepted = false;

                QInputContext *ic = qApp->inputContext();
                if (ic) { 
                    QString preedit = text.mid(start, end - start);
                    d->preeditInjectionInProgress = true;
#ifdef HAVE_MALIIT
                    Maliit::PreeditInjectionEvent event(preedit, eventCursorPosition - start);
#else
                    MPreeditInjectionEvent event(preedit, eventCursorPosition - start);
#endif
                    injectionAccepted = ic->filterEvent(&event);
                    d->preeditInjectionInProgress = false;
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

    if (updatePending || d->cursor()->position() != cursorPositionBefore) {
        d->updateMicroFocus();
    }
    d->enableUpdateMicroFocus(true);
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
        model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
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

    (void)d->checkingCopy();
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
        model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
        emit selectionChanged();
    } else if (mode() == MTextEditModel::EditModeActive) {
        d->commitPreedit(true);
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

    if (isReadOnly() || !d->checkingCopy()) {
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
        model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
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
    Q_D(MTextEdit);

    bool emitTextChanged = false;
    const int cursorPositionBefore(cursorPosition());
    QString preedit = event->preeditString();

    // The first click of a double click sequence causes pre-edit injection, which implies
    // that the IC sends pre-edit to the IM server and the input method plugin may send it
    // back with correct formatting.  Due to asynchronous operation the pre-edit sent by
    // the plugin may arrive after the second click of the double click sequence, at which
    // point it will remove double click selection unless we ignore it here.  The logic to
    // recognize exactly that particular pre-edit sending by time and content is not 100%
    // precise but should be pretty safe.
    if ((d->doubleClickSelectionTime.addMSecs(IgnorePreeditChangeAfterDoubleClickInterval) > QTime::currentTime())
        && !preedit.isEmpty() && preedit == selectedText()) {
        d->doubleClickSelectionTime = QTime();
        return;
    }
    if (d->omitInputMethodEvents) {
        return;
    }
    QString commitString = event->commitString();
    bool emitReturnPressed = false;

    preedit = d->replaceLineBreaks(preedit, " ");

    if ((lineMode() == MTextEditModel::SingleLine) && !commitString.isEmpty()) {
        // FIXME: remove \n-check once VKB has been changed to send \r as return
        emitReturnPressed = commitString.contains('\n') || commitString.contains('\r');

        // Remove trailing line breaks
        int i = commitString.length() - 1;
        for (; i >= 0; --i) {
            if (!LineBreakSet.contains(commitString[i])) {
                break;
            }
        }
        commitString.truncate(i + 1);

        // Replace other line breaks with space
        commitString = d->replaceLineBreaks(commitString, QChar(' '));
    } else if (lineMode() == MTextEditModel::MultiLine) {
        commitString.replace("\r", "\n");
    }

    // get and remove the current selection if there are preedit string or commit string
    const bool wasSelecting = hasSelectedText();
    const QTextDocumentFragment selectedFragment = d->cursor()->selection();
    int selectionStart = -1;

    if (wasSelecting && (!preedit.isEmpty() || !commitString.isEmpty())) {
        selectionStart = d->cursor()->selectionStart();
        d->cursor()->removeSelectedText();
        emitTextChanged = true;
    }

    const bool wasPreediting = d->isPreediting();
    int preeditCursorPos = 0;
    if (!preedit.isEmpty()) {
        foreach (const QInputMethodEvent::Attribute attribute, event->attributes()) {
            if (attribute.type == QInputMethodEvent::Cursor && attribute.length > 0) {
                preeditCursorPos = attribute.start;
                break;
            }
        }
    }

    // update stored multi-preedit-styles according to different actions (insert/backspace)
    // when cursor is inside preedit.
    // replacement is the exception which will be handled later.
    const int insertedTextCount = preedit.length() - d->cursor()->selectedText().length();
    if (wasPreediting
        && insertedTextCount != 0
        && !event->replacementLength()
        && preeditCursorPos >= 0
        && preeditCursorPos != preedit.length()
        && d->preeditStyling.count() > 1) {

        int currentStyleLastCharIndex = 0;
        int currentStyleIndex = d->currentPreeditStylingIndex(currentStyleLastCharIndex);
        if (currentStyleIndex >= 0) {
            d->preeditStyling[currentStyleIndex].count += insertedTextCount;
            if (d->preeditStyling.at(currentStyleIndex).count <= 0) {
                d->preeditStyling.removeAt(currentStyleIndex);
            }
        }
    }

    if (!d->editActive
        && echoMode() == MTextEditModel::PasswordEchoOnEdit
        && (!commitString.isEmpty() || !preedit.isEmpty())) {
        // in this mode, clear the contents before starting editing and doing new input
        document()->clear();
    }

    d->editActive = true;
    // Pre-edit position will be adjusted based on replacement and commit text
    // and possibly overridden at the end by Selection attribute.
    int newPreeditPosition(wasPreediting ? d->cursor()->selectionStart() : d->cursor()->position());

    if (event->replacementLength()) {
        emitTextChanged = true;
        d->removePreedit();
        // store styles of replaced text to preedit styles.
        const int start = qMax(0, d->cursor()->position() + event->replacementStart());
        const int end = qMin(d->realCharacterCount(), start + event->replacementLength());
        const int storedStyleEnd = (preeditCursorPos <= event->replacementLength()) ?
                                    start + preeditCursorPos : end;
        if (wasPreediting) {
            if (event->replacementStart() < 0) {
                newPreeditPosition -= qMin(end, newPreeditPosition) - start;
                QList<MTextEditPrivate::styleData> storedPreeditStyle = d->preeditStyling;
                d->preeditStyling.clear();
                d->storePreeditTextStyling(start, storedStyleEnd);
                d->preeditStyling += storedPreeditStyle;
            } else {
                d->storePreeditTextStyling(start, storedStyleEnd);
            }
        } else {
            newPreeditPosition = start;
            d->preeditStyling.clear();
            d->storePreeditTextStyling(start, storedStyleEnd);
        }

        d->cursor()->setPosition(start);
        d->cursor()->setPosition(end, QTextCursor::KeepAnchor);
        d->cursor()->removeSelectedText();
    } else if (event->replacementStart()) {
        emitTextChanged = true;
        d->removePreedit();
        d->cursor()->setPosition(qBound(0, d->cursor()->position() + event->replacementStart(),
                                        d->realCharacterCount()));
        if (!wasPreediting) {
            newPreeditPosition = d->cursor()->position();
        }
    }

    bool insertionSuccess = false;

    // append possible commit string
    if (!commitString.isEmpty()) {
        d->removePreedit();
        const int commitPosition(d->cursor()->position());
        insertionSuccess = d->doTextInsert(commitString, true);
        emitTextChanged = emitTextChanged || wasPreediting || insertionSuccess;

        if (!insertionSuccess && wasSelecting && preedit.isEmpty()) {
            // validation failed, put the old selection back
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
            d->cursor()->removeSelectedText();
            d->cursor()->insertFragment(selectedFragment);
            d->cursor()->setPosition(selectionStart, QTextCursor::KeepAnchor);
        } else if (wasPreediting && d->preeditStyling.count() > 0) {
            // if validation passed, then set the cursor charformat with the
            // last preedit styling used by the cursor.
            d->cursor()->setCharFormat(d->preeditStyling.last().charFormat);
        }
        if (insertionSuccess) {
            if (!wasPreediting) {
                newPreeditPosition = d->cursor()->position();
            } else if (commitPosition <= newPreeditPosition) {
                newPreeditPosition += d->cursor()->position() - commitPosition;
            }
        }
        d->preeditStyling.clear();
    }

    if (insertionSuccess || (preedit.isEmpty() && wasPreediting)) {
        // return to basic mode on insertion or removed preedit
        if (!insertionSuccess) {
            emitTextChanged = true;
            d->removePreedit();
        }
        d->setMode(MTextEditModel::EditModeBasic);
    }

    const QList<QInputMethodEvent::Attribute> attributes(event->attributes());

    d->disableUpdateMicroFocus();
    foreach (const QInputMethodEvent::Attribute &attribute, attributes) {
        if (attribute.type == QInputMethodEvent::Selection) {
            // attribute.start is related to block start, while setSelection() is
            // using start which is related to plain text, need to do map.
            const int absoluteStart(attribute.start + d->cursor()->block().position());
            if (preedit.isEmpty()) {
                setSelection(absoluteStart, attribute.length);
            } else {
                // We don't support pre-edit and selection at the same time; pre-edit
                // wins, just set the cursor position in that case.  We remove the
                // pre-edit because attribute.start is likely != 0.
                d->removePreedit();
                newPreeditPosition = qBound(0, absoluteStart, d->realCharacterCount());
            }
        }
    }

    // special case for omitting preedit if already at max length.
    // rationale: max length commonly used without word correction so keyboards
    // do input character at a time. commonly cannot know how much space preedit would
    // use when committed, but full entry can be known not to accept anything.
    if (!preedit.isEmpty()) {
        const bool atMaxLength((maxLength() >= 0) && (d->realCharacterCount() == maxLength()));
        if (!atMaxLength || (preedit == d->cursor()->selectedText())) {
            emitTextChanged = emitTextChanged || !wasPreediting || (d->cursor()->selectedText() != preedit);
            if (!(wasPreediting && d->cursor()->hasSelection() && (newPreeditPosition == d->cursor()->selectionStart()))) {
                d->cursor()->setPosition(newPreeditPosition);
            }
            d->setPreeditText(preedit, attributes);
            d->setMode(MTextEditModel::EditModeActive);
        } else {
            const QString textBefore = d->cursor()->selectedText();
            d->removePreedit();
            d->doTextInsert(textBefore);
            d->cursor()->setPosition(cursorPositionBefore);
            d->setMode(MTextEditModel::EditModeBasic);
            d->safeReset();
            d->updateMicroFocus();
        }
    }

    const bool emitCursorPositionChanged(cursorPosition() != cursorPositionBefore);

    if (emitCursorPositionChanged || emitTextChanged) {
        d->updateMicroFocus();
    }
    d->enableUpdateMicroFocus(true);

    if (emitTextChanged
        // no signal if committing existing preedit as is...
        && !((wasPreediting && commitString == selectedFragment.toPlainText()
              && insertionSuccess && preedit.isEmpty())
             // ...or this is pre-edit injection
             || d->preeditInjectionInProgress)) {
        emit textChanged();
    }

    if (emitCursorPositionChanged) {
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
    MWidgetController::changeEvent(event);
}

void MTextEdit::retranslateUi()
{
    Q_D(MTextEdit);

    //% "Cut"
    d->cutAction.setText(qtTrId("qtn_comm_cut"));
    //% "Copy"
    d->copyAction.setText(qtTrId("qtn_comm_copy"));
    //% "Paste"
    d->pasteAction.setText(qtTrId("qtn_comm_paste"));
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
        model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
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
        if (attachedToolbarId() == -1) {
            attachToolbar(QString(M_IM_TOOLBARS_DIR) + "/" + EmailContentToolbarFile);
        }
        break;

    case M::UrlContentType:
        setInputMethodCorrectionEnabled(false);
        setInputMethodAutoCapitalizationEnabled(false);
        newHint = Qt::ImhUrlCharactersOnly;
        if (attachedToolbarId() == -1) {
            attachToolbar(QString(M_IM_TOOLBARS_DIR) + "/" + UrlContentToolbarFile);
        }
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
    case Qt::ImCursorPosition: {
            int pos = d->cursor()->position() - block.position();
            if (d->isPreediting()) {
                pos = d->cursor()->anchor() - block.position();
            }
            return QVariant(pos);
        }

    case Qt::ImAnchorPosition: {
            int pos = d->cursor()->anchor() - block.position();
            return QVariant(pos);
        }

    case Qt::ImSurroundingText: {
            QString surroundingText = block.text();
            if (d->isPreediting()) {

                int start = d->cursor()->anchor();
                int end = d->cursor()->position();

                // Remove the block's cursor offset from start, end:
                surroundingText.remove(start - block.position(), end - start);
            }
            return QVariant(surroundingText);
        }

    case Qt::ImCurrentSelection:
        return QVariant(selectedText());

    case M::ImCorrectionEnabledQuery:
        return QVariant(inputMethodCorrectionEnabled());

    case M::InputMethodAttributeExtensionIdQuery:
        return QVariant(attachedToolbarId());

    case M::InputMethodAttributeExtensionQuery:
        return QVariant(attachedToolbar());

    case M::WesternNumericInputEnforcedQuery:
        return QVariant(isWesternNumericInputEnforced());

    case M::ImRelocatorRectangle: {
        // First ask cursor rectangle from view.
        QVariant cursorQueryResult = MWidgetController::inputMethodQuery(Qt::ImMicroFocus);
        if (cursorQueryResult.isValid()) {
            QRectF relocatorRect = cursorQueryResult.toRectF();
            // If completer exists, add its height to the rectangle. Do this regardless of
            // whether completer is visible or not to avoid jumping when completer appears and disappears.
            // Position cannot be used since it can change due to the value returned here.
            if (d->completer) {
                relocatorRect.adjust(0, 0, 0, d->completer->preferredHeight());
            }
            return relocatorRect;
        } else {
            break;
        }
    }

    default:
        break;
    }
    return MWidgetController::inputMethodQuery(query);
}

QVariant MTextEdit::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(MTextEdit);

    switch (change) {
    case QGraphicsItem::ItemScenePositionHasChanged:
        // notify widgets (completer or magnifier) that the scene position has changed.
        emit d->signalEmitter.scenePositionChanged();
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

    bool wasEditable = model()->textInteractionFlags() & Qt::TextEditable;
    bool editable = flags & Qt::TextEditable;
    model()->setTextInteractionFlags(flags);

    // if not editable. Commit preedit, and disable cut/paste
    if (!editable && wasEditable) {
        d->commitPreedit();

        QObject::disconnect(this, SIGNAL(copyAvailable(bool)),
                            &(d->cutAction), SLOT(setVisible(bool)));
        d->cutAction.setVisible(false);
        d->pasteAction.setVisible(false);
    } else if (editable && !wasEditable) {

        QObject::connect(this, SIGNAL(copyAvailable(bool)),
                         &(d->cutAction), SLOT(setVisible(bool)));
        d->cutAction.setVisible(hasSelectedText());
        d->_q_updatePasteActionState();
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
        model()->setEnabledSelectionControls(MTextEditModel::SelectionHandlesAndToolbar);
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
    Q_D(MTextEdit);

    if (numChars < 0) {
        numChars = 0;
    }
    model()->setMaxLength(numChars);

    int characterCount = d->realCharacterCount();

    if (characterCount > maxLength()) {

        // If we have preedit commit it first
        d->commitPreedit();

        // Make sure we're not in selection mode.
        d->setMode(MTextEditModel::EditModeBasic);

        // Check again
        characterCount = d->realCharacterCount();
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
        disconnect(this, 0, d->completer, 0);
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
    d->registeredToolbarId = MInputMethodState::instance()->registerAttributeExtension(name);
    model()->setToolbar(name);
    model()->setToolbarId(d->registeredToolbarId);
    d->icUpdate();
}

void MTextEdit::attachToolbar(int id)
{
    Q_D(MTextEdit);
    if (attachedToolbarId() == id)
        return;
    detachToolbar();
    const QString toolbar = MInputMethodState::instance()->attributeExtensionFile(id);
    // record attached toolbar identifier and file name.
    // The toolbar identifier is an unique id in the space of the application.
    // Two text entries may attach to the same toolbar file, but have two different
    // toolbar instances (two ids).
    model()->setToolbar(toolbar);
    model()->setToolbarId(id);
    d->icUpdate();
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
        MInputMethodState::instance()->unregisterAttributeExtension(d->registeredToolbarId);
        d->registeredToolbarId = -1;
    }
    model()->setToolbar(QString());
    model()->setToolbarId(-1);
    d->icUpdate();
}

bool MTextEdit::errorHighlight() const
{
    return model()->errorHighlight();
}

void MTextEdit::setErrorHighlight(bool showErrorHighlight)
{
    model()->setErrorHighlight(showErrorHighlight);
}

Qt::LayoutDirection MTextEdit::layoutDirection() const
{
    Q_D(const MTextEdit);
    return d->layoutDirection;
}

void MTextEdit::setLayoutDirection(Qt::LayoutDirection dir)
{
    Q_D(MTextEdit);
    d->layoutDirection = dir;
}

void MTextEdit::setWesternNumericInputEnforced(bool enforce)
{
    model()->setWesternNumericInputEnforced(enforce);
}

bool MTextEdit::isWesternNumericInputEnforced() const
{
    return model()->isWesternNumericInputEnforced();
}

#include "moc_mtextedit.cpp"
