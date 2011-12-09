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

#include <QTextDocument>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QApplication>
#include <QClipboard>
#include <MInputMethodState>
#include <MDeviceProfile>
#include "mrichtextedit.h"
#include "mrichtextedit_p.h"
#include "mrichtexteditdialogsmanager_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MRichTextEdit)

namespace
{
    MTextEditModel *createRichTextModel(MTextEditModel::LineMode type, const QString &text)
    {
        MTextEditModel *model = new MTextEditModel;
        model->setDocument(new QTextDocument(text, model));
        model->setLine(type);
        return model;
    }

    const char * const NoCloseToolbarOnTriggerProperty("noCloseToolbarOnTrigger");
}

MActionGroup::MActionGroup(QObject *parent): group(parent),
                                             _visibleItemCount(0),
                                             internalUpdate(false)
{
}

MActionGroup::~MActionGroup()
{
}

QAction *MActionGroup::addAction(QAction *a)
{
    if (!actions().contains(a)) {
        connect(a, SIGNAL(changed()),SLOT(onItemChanged()));
        group.addAction(a);
        setVisibleItemCount(readVisibleItemCount());
    }
    return a;
}

void MActionGroup::removeAction(QAction *a)
{
    if (actions().contains(a)) {
        group.removeAction(a);
        disconnect(a, SIGNAL(changed()));
        setVisibleItemCount(readVisibleItemCount());
    }
}

QList<QAction*> MActionGroup::actions() const
{
    return group.actions();
}

void MActionGroup::setVisible(bool visible)
{
    internalUpdate = true;
    group.setVisible(visible);
    internalUpdate = false;
}

bool MActionGroup::isVisible() const
{
    return group.isVisible();
}

int MActionGroup::visibleItemCount()
{
    return _visibleItemCount;
}

void MActionGroup::setVisibleItemCount(int value)
{
    if (_visibleItemCount < value) {
        _visibleItemCount = value;
        if (!internalUpdate) {
            emit itemShown();
        }
    } else if (_visibleItemCount > value) {
        _visibleItemCount = value;
        if (!internalUpdate) {
            emit itemHidden();
        }
    }
}

int MActionGroup::readVisibleItemCount()
{
    int count = 0;
    foreach (QAction* value, actions()) {
        if (value->isVisible()) {
            count++;
        }
    }
    return count;
}

void MActionGroup::onItemChanged()
{
    setVisibleItemCount(readVisibleItemCount());
}

MRichTextEditPrivate::MRichTextEditPrivate()
    : MTextEditPrivate(),
      textEditButtons(0),
      richTextEditButtons(0),
      boldAction(0),
      italicAction(0),
      underlineAction(0),
      formatAction(0),
      modeAction(0)
{
}


MRichTextEditPrivate::~MRichTextEditPrivate()
{
}

void MRichTextEditPrivate::init()
{
    Q_Q(MRichTextEdit);

    // Remember all parent buttons
    foreach (QAction* action, q->actions()) {
        textEditButtons.addAction(action);
    }
    textEditButtons.setVisible(false);
    q->connect(&textEditButtons, SIGNAL(itemShown()), SLOT(_q_updateToolbarButtons()));
    q->connect(&textEditButtons, SIGNAL(itemHidden()), SLOT(_q_updateToolbarButtons()));

    // Add rich text edit buttons B,I,U,F
    boldAction.setObjectName("Bold");
    italicAction.setObjectName("Italic");
    underlineAction.setObjectName("Underline");
    formatAction.setObjectName("Format");

    boldAction.setCheckable(true);
    italicAction.setCheckable(true);
    underlineAction.setCheckable(true);

    boldAction.setProperty(NoCloseToolbarOnTriggerProperty, true);
    italicAction.setProperty(NoCloseToolbarOnTriggerProperty, true);
    underlineAction.setProperty(NoCloseToolbarOnTriggerProperty, true);
    formatAction.setProperty(NoCloseToolbarOnTriggerProperty, true);

    q->connect(&boldAction, SIGNAL(triggered(bool)), SLOT(_q_toggleFontBold()));
    q->connect(&italicAction, SIGNAL(triggered(bool)), SLOT(_q_toggleFontItalic()));
    q->connect(&underlineAction, SIGNAL(triggered(bool)), SLOT(_q_toggleFontUnderline()));
    q->connect(&formatAction, SIGNAL(triggered(bool)), SLOT(_q_showTextStylingOptions()));

    q->addAction(&underlineAction);
    q->addAction(&boldAction);
    q->addAction(&italicAction);
    q->addAction(&formatAction);

    richTextEditButtons.addAction(&boldAction);
    richTextEditButtons.addAction(&italicAction);
    richTextEditButtons.addAction(&underlineAction);
    richTextEditButtons.addAction(&formatAction);
    richTextEditButtons.setExclusive(false);
    richTextEditButtons.setVisible(textEditButtons.visibleItemCount() == 0);

    // Add mode toggle button T
    modeAction.setObjectName("Mode");
    modeAction.setVisible(textEditButtons.visibleItemCount() != 0);
    modeAction.setCheckable(true);
    modeAction.setChecked(false);
    modeAction.setProperty(NoCloseToolbarOnTriggerProperty, true);
    q->connect(&modeAction, SIGNAL(triggered(bool)), SLOT(_q_showRichTextToolbar(bool)));
    q->addAction(&modeAction);
}

void MRichTextEditPrivate::_q_updateToolbarButtons()
{
    Q_Q(MRichTextEdit);
    if (!q->isReadOnly()) {
        // Make group visible to check buttons state in it
        // otherwise all buttons in this group will be invisible
        textEditButtons.setVisible(true);
        if (!textEditButtons.visibleItemCount()) {
            modeAction.setVisible(false);
            _q_showRichTextToolbar(true);
        } else {
            modeAction.setVisible(true);
            modeAction.setChecked(false);
            _q_showRichTextToolbar(false);
        }
    } else {
        textEditButtons.setVisible(true);
        richTextEditButtons.setVisible(false);
        modeAction.setChecked(false);
        modeAction.setVisible(false);
    }
}

void MRichTextEditPrivate::_q_showRichTextToolbar(bool show)
{
    Q_Q(MRichTextEdit);
    if (show && !q->isReadOnly()) {
        textEditButtons.setVisible(false);
        richTextEditButtons.setVisible(true);
    } else {
        textEditButtons.setVisible(true);
        richTextEditButtons.setVisible(false);
    }
}

void MRichTextEditPrivate::_q_toggleFontBold()
{
    Q_Q(MRichTextEdit);

    QTextCharFormat currentFormat = currentCharFormat();
    bool boldStyle = !currentFormat.font().bold();
    // set current bold style option
    q->setFontBold(boldStyle);
}

void MRichTextEditPrivate::_q_toggleFontItalic()
{
    Q_Q(MRichTextEdit);

    QTextCharFormat currentFormat = currentCharFormat();
    bool italicStyle = !currentFormat.font().italic();
    // set current italic style option
    q->setFontItalic(italicStyle);
}

void MRichTextEditPrivate::_q_toggleFontUnderline()
{
    Q_Q(MRichTextEdit);

    // As a workaround to NB#223092 we don't use currentFont() and QFont::underline().
    const QTextCharFormat format = currentCharFormat();
    const bool underlineStyle = format.underlineStyle() == QTextCharFormat::NoUnderline;
    // set current underline style option
    q->setFontUnderline(underlineStyle);
}

QTextCharFormat MRichTextEditPrivate::currentCharFormat() const
{
    Q_Q(const MRichTextEdit);

    QTextCharFormat format;
    if (isPreediting() == true) {
        format = currentPreeditCharFormat();
    } else {
        QTextCursor textcursor = q->textCursor();
        // QTextCursor::charFormat() returns the format of the character
        // immediately before the cursor position regardless if there is
        // a selection or not. In case of selection we want to know the
        // format of the first character of the selected text.
        if (textcursor.hasSelection() == true) {
            textcursor.setPosition(qMin(textcursor.position(), textcursor.anchor()) + 1);
        }
        format = textcursor.charFormat();
    }

    return format;
}

QTextCharFormat MRichTextEditPrivate::filterFormat(const QTextCharFormat &originalFormat)
{
    QTextCharFormat newFormat;
    static const QList<int> copyProperties =
	QList<int>()
	    << QTextFormat::FontFamily
	    << QTextFormat::FontPointSize
	    << QTextFormat::FontItalic
	    << QTextFormat::FontUnderline
	    << QTextFormat::ForegroundBrush;

    const QMap<int, QVariant> originalProperties = originalFormat.properties();
    foreach (int property, copyProperties) {
	QMap<int, QVariant>::const_iterator propIterator = originalProperties.find(property);
	if (propIterator != originalProperties.end()) {
	    newFormat.setProperty(property, propIterator.value());
	}
    }

    QMap<int, QVariant>::const_iterator weightIterator = originalProperties.find(QTextFormat::FontWeight);
    if (weightIterator != originalProperties.end()) {
	const int weight = weightIterator.value().toInt();
	newFormat.setProperty(QTextFormat::FontWeight,
			      QVariant(int((weight > QFont::Normal) ? QFont::Bold : QFont::Normal)));
    }

    return newFormat;
}

QTextDocumentFragment MRichTextEditPrivate::fragmentFromHtml(const QString &html) const
{
    Q_Q(const MRichTextEdit);

    QTextDocument newDocument;
    newDocument.setHtml(html);
    QTextCursor newCursor(&newDocument);

    // replace all newlines by spaces, if control is singleline
    if (q->lineMode() == MTextEditModel::SingleLine) {
	newCursor.movePosition(QTextCursor::Start);
	while (newCursor.movePosition(QTextCursor::NextBlock)) {
	    newCursor.deletePreviousChar();
	    newCursor.insertText(QChar(' '));
	}
    }

    // drop formatting options which are not editable in MRichTextEdit UI
    newCursor.movePosition(QTextCursor::Start);
    do {
	// filter block-wide styles
	newCursor.setBlockFormat(QTextBlockFormat());
	newCursor.setBlockCharFormat(filterFormat(newCursor.blockCharFormat()));

	// now filter styles of individual characters
	// grouping them by sequences with same format
	newCursor.movePosition(QTextCursor::StartOfBlock);
	while (!newCursor.atBlockEnd()) {
	    // this sets anchor to position, further moves will extend selection from here
	    newCursor.clearSelection();

	    // initialize format
	    newCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	    QTextCharFormat format = newCursor.charFormat();

	    // find the end of same-formatted characters sequence
	    while (true) {
		newCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
		if (newCursor.atBlockEnd()) {
		    break;
		} else if (newCursor.charFormat() != format) {
		    // charFormat() returns formatting of the previous character
		    // so at this point position is already one character too far
		    newCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
		    break;
		}
	    }

	    // modify format of selection
	    newCursor.setCharFormat(filterFormat(format));
	}
    } while (newCursor.movePosition(QTextCursor::NextBlock));

    return QTextDocumentFragment(&newDocument);
}

bool MRichTextEditPrivate::insertFromMimeData(const QMimeData *source)
{
    Q_Q(MRichTextEdit);

    QTextDocumentFragment fragment;
    bool updated = false;

    if (source->hasHtml()) {
	fragment = fragmentFromHtml(source->html());
        updated = true;
    } else {
        QString text = source->text();
        if (!text.isNull()) {
            // on single line newline are changed into spaces
            if (q->lineMode() == MTextEditModel::SingleLine) {
                fragment = QTextDocumentFragment::fromPlainText(MTextEditPrivate::replaceLineBreaks(text, QChar(' ')));
            } else {
                fragment = QTextDocumentFragment::fromPlainText(text);
            }
            updated = true;
        }
    }

    if (updated) {
        commitPreedit(true); // Reset preedit before paste
        QTextCursor textcursor = q->textCursor();
        textcursor.insertFragment(fragment);
        q->setTextCursor(textcursor);
    }

    return updated;
}

QMimeData *MRichTextEditPrivate::createMimeDataFromSelection()
{
    Q_Q(MRichTextEdit);

    const QTextDocumentFragment fragment(q->textCursor());
    QMimeData *mimeData = new QMimeData();

    mimeData->setData(QLatin1String("text/html"), fragment.toHtml("utf-8").toUtf8());
    mimeData->setText(fragment.toPlainText());

    return mimeData;
}


bool MRichTextEditPrivate::checkingCopy()
{
    Q_Q(MRichTextEdit);

    QClipboard *clipboard = QApplication::clipboard();

    if (!q->hasSelectedText()
            || q->echoMode() != MTextEditModel::Normal // only allow copy from normal echo mode entry
            || !clipboard) {
        return false;
    }

    clipboard->setMimeData(createMimeDataFromSelection());
    return true;
}


void MRichTextEditPrivate::_q_updateStyle()
{
    QTextCharFormat format = currentCharFormat();

    boldAction.setChecked(format.fontWeight() > QFont::Normal);
    underlineAction.setChecked(format.fontUnderline());
    italicAction.setChecked(format.fontItalic());
}


void MRichTextEditPrivate::_q_showTextStylingOptions()
{
    Q_Q(MRichTextEdit);

    MRichTextEditDialogsManager *dialogsManager = MRichTextEditDialogsManager::instance();

    q->connect(dialogsManager, SIGNAL(fontFamilySelected(QString)),
               SLOT(_q_setFontFamily(QString)));
    q->connect(dialogsManager, SIGNAL(fontSizeSelected(int)),
               SLOT(_q_setFontSize(int)));
    q->connect(dialogsManager, SIGNAL(fontColorSelected(QColor)),
               SLOT(_q_setFontColor(QColor)));

    QString fontFamily;
    int fontPointSize = -1;
    QColor fontColor;

    textStyleValues(&fontFamily, &fontPointSize, &fontColor);

    // The appearing styling dialog should grab focus with Qt::PopupFocusReason. But
    // because MRichTextEdit can't access the styling dialog, so below is a workaround
    // way by calling MTextEdit::FocusOutEvent() with Qt::PopupFocusReason and then
    // calling clearFocus to get rid of software input panel (SIP) by clear focus,
    // which might obstruct our dialog otherwise.
    QFocusEvent focusOut(QEvent::FocusOut, Qt::PopupFocusReason);
    q->focusOutEvent(&focusOut);
    q->clearFocus();

    dialogsManager->showTextStylingDialog(fontFamily, fontPointSize, fontColor);
    q->setFocus(Qt::PopupFocusReason);

    q->disconnect(dialogsManager, 0, q, 0);
}


void MRichTextEditPrivate::textStyleValues(QString *fontfamily, int *fontPointSize, QColor *fontColor)
{
    Q_Q(MRichTextEdit);

    QTextCursor textcursor = q->textCursor();
    *fontfamily = textcursor.charFormat().font().family();
    *fontPointSize = MDeviceProfile::instance()->pixelsToPt(textcursor.charFormat().font().pixelSize());
    *fontColor = textcursor.charFormat().foreground().color();

    if (!q->hasSelectedText()) {
        return;
    }

    // If the text selection has different styles don't set the current text style value
    int startPos = textcursor.selectionStart();
    int endPos  = textcursor.selectionEnd();
    bool familyDiffers = false;
    bool sizeDiffers = false;
    bool colorDiffers = false;

    int fontPixelSize = textcursor.charFormat().font().pixelSize();
    // Starting from startPos + 1 to get the style that would be used when
    // text is inserted in the positions
    for (int i = startPos + 1; i <= endPos; i++) {
        textcursor.setPosition(i);

        if (!familyDiffers && (fontfamily != textcursor.charFormat().font().family())) {
            familyDiffers = true;
            fontfamily->clear();
        }

        if (!sizeDiffers && (fontPixelSize != textcursor.charFormat().font().pixelSize())) {
            sizeDiffers = true;
            *fontPointSize = -1;
        }

        if (!colorDiffers && (*fontColor != textcursor.charFormat().foreground().color())) {
            colorDiffers = true;
            *fontColor = QColor();
        }

        if (familyDiffers && sizeDiffers && colorDiffers) {
            break;
        }
    }
}


void MRichTextEditPrivate::_q_setFontFamily(const QString &fontFamily)
{
    Q_Q(MRichTextEdit);

    QTextCursor textcursor = q->textCursor();
    QTextCharFormat format;

    format.setFontFamily(fontFamily);
    textcursor.mergeCharFormat(format);
    q->setTextCursor(textcursor);
}


void MRichTextEditPrivate::_q_setFontSize(int fontPointSize)
{
    Q_Q(MRichTextEdit);

    QTextCursor textcursor = q->textCursor();
    const bool hasSelectedText = q->hasSelectedText();
    int pixelSize = MDeviceProfile::instance()->ptToPixels(fontPointSize);

    // Since the QTextCharFormat doesn't have method for setting the font pixel size, updating the
    // font pixel size using QFont::setPixelSize
    if (!hasSelectedText) {
        QTextCharFormat curFormat = textcursor.charFormat();
        QFont font = curFormat.font();
        font.setPixelSize(pixelSize);

        // QTextCharFormat::setFontPointSize() is required to retain font size during copy/paste
        curFormat.setFontPointSize(fontPointSize);
        curFormat.setFont(font);

        textcursor.setCharFormat(curFormat);
        q->setTextCursor(textcursor);
        return;
    }

    int savePosition = textcursor.position();
    int saveAnchor = textcursor.anchor();
    // To avoid losing style, processing each character in the selection individually
    int startPos = textcursor.selectionStart();
    int endPos  = textcursor.selectionEnd();
    for (int i = startPos; i < endPos; i++) {
        textcursor.setPosition(i);
        // Setting the char format atBlockEnd applies it to the whole block, it is causing problem when the
        // block has different styles, just skipping to set there.
        if (textcursor.atBlockEnd()) {
            continue;
        }
        // Selecting the current character. QTextCharFormat::setCharFormat doesn't applies the
        // char format to an existing character if it is not selected.
        textcursor.setPosition(i + 1, QTextCursor::KeepAnchor);
        QTextCharFormat curFormat = textcursor.charFormat();
        QFont font = curFormat.font();
        font.setPixelSize(pixelSize);

        curFormat.setFontPointSize(fontPointSize);
        curFormat.setFont(font);
        textcursor.setCharFormat(curFormat);
    }

    textcursor.setPosition(saveAnchor);
    textcursor.setPosition(savePosition, QTextCursor::KeepAnchor);
    q->setTextCursor(textcursor);
}


void MRichTextEditPrivate::_q_setFontColor(const QColor &fontColor)
{
    Q_Q(MRichTextEdit);

    QTextCursor textcursor = q->textCursor();
    QTextCharFormat format;

    format.setForeground(fontColor);
    textcursor.mergeCharFormat(format);
    q->setTextCursor(textcursor);
}

///////////////////////////////////////////////
// Actual class implementation


MRichTextEdit::MRichTextEdit(MTextEditModel::LineMode type, const QString &text, QGraphicsItem *parent)
    : MTextEdit(new MRichTextEditPrivate, createRichTextModel(type, text), parent)
{
    Q_D(MRichTextEdit);

    connect(this,
            SIGNAL(cursorPositionChanged()),
            SLOT(_q_updateStyle()));
    connect(this,
            SIGNAL(selectionChanged()),
            SLOT(_q_updateStyle()));

    connect(this,
            SIGNAL(cursorPositionChanged()),
            SLOT(_q_updateToolbarButtons()));
    connect(this,
            SIGNAL(selectionChanged()),
            SLOT(_q_updateToolbarButtons()));

    d->init();
    d->_q_updateToolbarButtons();
}


MRichTextEdit::MRichTextEdit(MTextEditModel *model, QGraphicsItem *parent)
    : MTextEdit(new MRichTextEditPrivate, model, parent)
{
    Q_D(MRichTextEdit);

    connect(this,
            SIGNAL(cursorPositionChanged()),
            SLOT(_q_updateStyle()));
    connect(this,
            SIGNAL(selectionChanged()),
            SLOT(_q_updateStyle()));

    connect(this,
            SIGNAL(cursorPositionChanged()),
            SLOT(_q_updateToolbarButtons()));
    connect(this,
            SIGNAL(selectionChanged()),
            SLOT(_q_updateToolbarButtons()));

    d->init();
}


MRichTextEdit::~MRichTextEdit()
{
}


void MRichTextEdit::copy()
{
    Q_D(MRichTextEdit);

    (void)d->checkingCopy();
}


void MRichTextEdit::paste()
{
    Q_D(MRichTextEdit);

    QClipboard *clipboard = QApplication::clipboard();

    if (isReadOnly() || !clipboard) {
        return;
    }

    const QMimeData *mimedata = clipboard->mimeData();

    bool updated = false;

    if (mimedata)
        updated = d->insertFromMimeData(mimedata);

    if (updated) {
        emit textChanged();
        updateMicroFocus();
    }
}


void MRichTextEdit::cut()
{
    MTextEdit::cut();
}


bool MRichTextEdit::setHtml(const QString &text)
{
    Q_D(MRichTextEdit);

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

    d->cursor()->insertHtml(text);

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


QString MRichTextEdit::toHtml() const
{
    return document()->toHtml();
}


void MRichTextEdit::setFontUnderline(bool underline)
{
    Q_D(MRichTextEdit);

    if (d->isPreediting() == true) {
        d->addStyleToPreeditStyling(MRichTextEditPrivate::Underline, underline);
    } else {
        QTextCursor textcursor = textCursor();
        QTextCharFormat format;
        format.setFontUnderline(underline);
        textcursor.mergeCharFormat(format);
        setTextCursor(textcursor);
    }
}


void MRichTextEdit::setFontItalic(bool italic)
{
    Q_D(MRichTextEdit);

    if (d->isPreediting() == true) {
        d->addStyleToPreeditStyling(MRichTextEditPrivate::Italic, italic);
    } else {
        QTextCursor textcursor = textCursor();
        QTextCharFormat format;
        format.setFontItalic(italic);
        textcursor.mergeCharFormat(format);
        setTextCursor(textcursor);
    }
}


void MRichTextEdit::setFontBold(bool bold)
{
    Q_D(MRichTextEdit);

    if (d->isPreediting() == true) {
        d->addStyleToPreeditStyling(MRichTextEditPrivate::Bold, bold);
    } else {
        QTextCursor textcursor = textCursor();

        QFont font = document()->defaultFont();
        QFont::Weight defaultWeight = static_cast<QFont::Weight>(font.weight());
        QFont::Weight wt = (bold ? QFont::Bold : defaultWeight);

        QTextCharFormat format;
        format.setFontWeight(wt);
        textcursor.mergeCharFormat(format);
        setTextCursor(textcursor);
    }
}


QFont MRichTextEdit::currentFont()
{
    Q_D(MRichTextEdit);

    QFont font;
    if (d->isPreediting() == true) {
        font = d->currentPreeditCharFormat().font();
    } else {
        QTextCursor textcursor = textCursor();
        font = textcursor.charFormat().font();
    }
    return font;
}


void MRichTextEdit::keyPressEvent(QKeyEvent *event)
{
    Q_D(MRichTextEdit);

    if (QEvent::KeyPress == event->type()) {
        if (event->matches(QKeySequence::Bold)) {
            d->_q_toggleFontBold();
        } else if (event->matches(QKeySequence::Italic)) {
            d->_q_toggleFontItalic();
        } else if (event->matches(QKeySequence::Underline)) {
            d->_q_toggleFontUnderline();
        } else if ((event->key() == Qt::Key_F) && (event->modifiers() & Qt::ControlModifier)) {
            // show text styling options
            d->_q_showTextStylingOptions();
        } else {
            // Pass the remaining events to MTextEdit will handle
            MTextEdit::keyPressEvent(event);
        }
    }
}

#include "moc_mrichtextedit.cpp"
