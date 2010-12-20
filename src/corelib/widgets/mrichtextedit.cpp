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
    //! Toolbar file
    const QString ToolbarFile = QString("RichTextEditorToolbar1.xml");

    MTextEditModel *createRichTextModel(MTextEditModel::LineMode type, const QString &text)
    {
        MTextEditModel *model = new MTextEditModel;
        model->setDocument(new QTextDocument(text, model));
        model->setLine(type);
        return model;
    }
}


MRichTextEditPrivate::MRichTextEditPrivate()
    : MTextEditPrivate()
{
}


MRichTextEditPrivate::~MRichTextEditPrivate()
{
}


bool MRichTextEditPrivate::insertFromMimeData(const QMimeData *source)
{
    Q_Q(MRichTextEdit);

    QTextDocumentFragment fragment;
    bool updated = false;

    if (source->hasHtml()) {
        fragment = QTextDocumentFragment::fromHtml(source->html());
        updated = true;
    } else {
        QString text = source->text();
        if (!text.isNull()) {
            fragment = QTextDocumentFragment::fromPlainText(text);
            updated = true;
        }
    }

    if (updated) {
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


bool MRichTextEditPrivate::copy()
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
    Q_Q(MRichTextEdit);

    QTextCursor cursor = q->textCursor();
    QTextCharFormat format;
    if (isPreediting()) {
        format = currentPreeditCharFormat();
    } else {
        format = cursor.charFormat();
    }

    MInputMethodState::instance()->setToolbarItemAttribute(q->attachedToolbarId(),
                                                           "Bold",
                                                           "pressed",
                                                           QVariant((format.fontWeight() > QFont::Normal) ? "true" : "false"));
    MInputMethodState::instance()->setToolbarItemAttribute(q->attachedToolbarId(),
                                                           "Underline",
                                                           "pressed",
                                                           QVariant((format.fontUnderline()) ? "true" : "false"));
    MInputMethodState::instance()->setToolbarItemAttribute(q->attachedToolbarId(),
                                                           "Italic",
                                                           "pressed",
                                                           QVariant((format.fontItalic()) ? "true" : "false"));
}


void MRichTextEditPrivate::showTextStylingOptions()
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

    int startPos = -1;
    int endPos  = -1;
    const bool hasSelectedText = q->hasSelectedText();
    // removing the focus from the text entry widget is removing the text selection, so
    // retain the selection
    if (hasSelectedText) {
        QTextCursor textcursor = q->textCursor();
        startPos = textcursor.selectionStart();
        endPos  = textcursor.selectionEnd();
    }

    // Get rid of software input panel (SIP), which might obstruct our dialog otherwise.
    q->clearFocus();

    if (hasSelectedText) {
        q->setSelection(startPos, endPos - startPos);
    }

    dialogsManager->showTextStylingDialog(fontFamily, fontPointSize, fontColor);
    q->setFocus();

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

    q->setTextCursor(textcursor);

    if (hasSelectedText) {
        q->setSelection(startPos, endPos - startPos);
    }
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
    connect(this,
            SIGNAL(cursorPositionChanged()),
            SLOT(_q_updateStyle()));

    attachToolbar(QString(M_IM_TOOLBARS_DIR) + "/" + ToolbarFile);
}


MRichTextEdit::MRichTextEdit(MTextEditModel *model, QGraphicsItem *parent)
    : MTextEdit(new MRichTextEditPrivate, model, parent)
{
    connect(this,
            SIGNAL(cursorPositionChanged()),
            SLOT(_q_updateStyle()));

    attachToolbar(QString(M_IM_TOOLBARS_DIR) + "/" + ToolbarFile);
}


MRichTextEdit::~MRichTextEdit()
{
}


void MRichTextEdit::copy()
{
    Q_D(MRichTextEdit);

    d->copy();
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
            QFont curFont = currentFont();
            bool boldStyle = !curFont.bold();
            // set current bold style option
            setFontBold(boldStyle);
        } else if (event->matches(QKeySequence::Italic)) {
            QFont curFont = currentFont();
            bool italicStyle = !curFont.italic();
            // set current italic style option
            setFontItalic(italicStyle);
        } else if (event->matches(QKeySequence::Underline)) {
            QFont curFont = currentFont();
            bool underlineStyle = !curFont.underline();
            // set current underline style option
            setFontUnderline(underlineStyle);
        } else if ((event->key() == Qt::Key_F) && (event->modifiers() & Qt::ControlModifier)) {
            // show text styling options
            d->showTextStylingOptions();
        } else {
            // Pass the remaining events to MTextEdit will handle
            MTextEdit::keyPressEvent(event);
        }
    }
}

#include "moc_mrichtextedit.cpp"
