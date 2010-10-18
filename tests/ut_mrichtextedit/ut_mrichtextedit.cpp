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

#include <QTest>
#include <QTextCursor>
#include <QInputMethodEvent>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QClipboard>
#include "ut_mrichtextedit.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MRichTextEdit>

/*!
 * Called once before the first testcase is run.
 */
void Ut_MRichTextEdit::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_mrichtextedit" };

    // prevent loading of minputcontext because we don't need it
    MApplication::setLoadMInputContext(false);

    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;
}

/*!
 * Called once after the last testcase has finished.
 */
void Ut_MRichTextEdit::cleanupTestCase()
{
    delete m_appWindow;
    m_appWindow = 0;
    delete m_app;
    m_app = 0;
}

/*!
 * Called before each testcase.
 */
void Ut_MRichTextEdit::init()
{
    m_subject = new MRichTextEdit();
}

/*!
 * Called after each testcase.
 */
void Ut_MRichTextEdit::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

/*!
 * Test SetFontBold.
 */
void Ut_MRichTextEdit::testSetFontBold()
{
    QFont curFont = m_subject->currentFont();
    bool curBoldStyle = curFont.bold();
    QCOMPARE(curBoldStyle, false);

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_B, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);
    curFont = m_subject->currentFont();
    curBoldStyle = curFont.bold();
    QCOMPARE(curBoldStyle, true);

    delete event;
    event = 0;
}

/*!
 * Test SetFontItalic.
 */
void Ut_MRichTextEdit::testSetFontItalic()
{
    QFont curFont = m_subject->currentFont();
    bool curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, false);

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_I, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);
    curFont = m_subject->currentFont();
    curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, true);

    delete event;
    event = 0;
}

/*!
 * Test SetFontUnderline.
 */
void Ut_MRichTextEdit::testSetFontUnderline()
{
    QFont curFont = m_subject->currentFont();
    bool curUnderlineStyle = curFont.underline();
    QCOMPARE(curUnderlineStyle, false);

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_U, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);
    curFont = m_subject->currentFont();
    curUnderlineStyle = curFont.underline();
    QCOMPARE(curUnderlineStyle, true);

    delete event;
    event = 0;
}

/*!
 * Test Copy, Paste Rich Text.
 */
void Ut_MRichTextEdit::testCopyPaste()
{
    const char *text = "abcde";
    m_subject->setText(text);
    m_subject->setSelection(0, 3, false);

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    bool hasHtml = mimeData->hasHtml();
    QCOMPARE(hasHtml, true);

    QString htmlText = "<b>bold</b>";
    m_subject->document()->setHtml(htmlText);
    m_subject->setSelection(0, 4, false);

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    htmlText = "text";
    m_subject->document()->setHtml(htmlText);

    QFont curFont = m_subject->currentFont();
    bool curBoldStyle = curFont.bold();
    QCOMPARE(curBoldStyle, false);
    delete event;

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    curFont = m_subject->currentFont();
    curBoldStyle = curFont.bold();
    QCOMPARE(curBoldStyle, true);
    delete event;

    htmlText = "<i>italic</i>";
    m_subject->document()->setHtml(htmlText);
    m_subject->setSelection(0, 4, false);

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    htmlText = "text";
    m_subject->document()->setHtml(htmlText);

    curFont = m_subject->currentFont();
    bool curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, false);
    delete event;

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    curFont = m_subject->currentFont();
    curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, true);
    delete event;
    event = 0;
}

/*!
 * Test Cut Rich Text.
 */
void Ut_MRichTextEdit::testCut()
{
    QString htmlText = "<b>bold</b>";
    m_subject->document()->setHtml(htmlText);
    m_subject->setSelection(0, 4, false);

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    htmlText = "text";
    m_subject->document()->setHtml(htmlText);

    QFont curFont = m_subject->currentFont();
    bool curBoldStyle = curFont.bold();
    QCOMPARE(curBoldStyle, false);
    delete event;

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    curFont = m_subject->currentFont();
    curBoldStyle = curFont.bold();
    QCOMPARE(curBoldStyle, true);
    delete event;

    htmlText = "<i>italic</i>";
    m_subject->document()->setHtml(htmlText);
    m_subject->setSelection(0, 4, false);

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    htmlText = "text";
    m_subject->document()->setHtml(htmlText);

    curFont = m_subject->currentFont();
    bool curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, false);
    delete event;

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier, QString(""));
    m_subject->keyPressEvent(event);

    curFont = m_subject->currentFont();
    curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, true);
    delete event;
    event = 0;
}

/*!
 * Test style in preedit mode.
 */
void Ut_MRichTextEdit::testStyleOnPreedit()
{
    QString htmlText = "<b>bold </b><i>italic </i>";
    m_subject->document()->setHtml(htmlText);

    QString text("preedit mode text");
    QInputMethodEvent event1(text, QList<QInputMethodEvent::Attribute>());

    m_subject->inputMethodEvent(&event1);

    QFont curFont = m_subject->currentFont();
    bool curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, true);

    event1.setCommitString("Test preedit style");
    m_subject->inputMethodEvent(&event1);

    curFont = m_subject->currentFont();
    curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, true);

    m_subject->setText("text");
    text = QString("preedit mode");
    QInputMethodEvent event2(text, QList<QInputMethodEvent::Attribute>());

    int eventCursorPosition = 1;
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent();
    m_subject->handleMouseRelease(eventCursorPosition, mouseEvent);
    m_subject->setFontUnderline(true);
    m_subject->inputMethodEvent(&event2);

    text = QString("a");
    QInputMethodEvent event3(text, QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&event3);

    curFont = m_subject->currentFont();
    bool curUnderlineStyle = curFont.underline();
    QCOMPARE(curUnderlineStyle, true);

    m_subject->setText("text");
    text = QString("preedit mode");
    QInputMethodEvent event4(text, QList<QInputMethodEvent::Attribute>());

    m_subject->handleMouseRelease(eventCursorPosition, mouseEvent);
    m_subject->setFontUnderline(true);
    m_subject->inputMethodEvent(&event4);
    event4.setCommitString(" ");
    m_subject->inputMethodEvent(&event4);

    QTextCursor cursor = m_subject->textCursor();
    cursor.setPosition(0);
    curFont = m_subject->currentFont();
    curUnderlineStyle = curFont.underline();
    QCOMPARE(curUnderlineStyle, true);

    delete mouseEvent;
    mouseEvent = 0;
}

/*!
 * Test setting of Rich Text contents.
 */
void Ut_MRichTextEdit::testSetHtml()
{
    QString htmlText = "<b>bold </b><i>italic </i>";
    QString plainText = "bold italic ";

    m_subject->setHtml(htmlText);

    QFont curFont = m_subject->currentFont();
    bool curItalicStyle = curFont.italic();
    QCOMPARE(curItalicStyle, true);

    QString currentHtmlText = m_subject->toHtml();
    m_subject->setHtml(currentHtmlText);

    QTextCursor textcursor = m_subject->textCursor();
    textcursor.setPosition(0);
    const QTextCharFormat charFormat = textcursor.charFormat();
    bool curBoldStyle = charFormat.fontWeight() > QFont::Normal;
    QCOMPARE(curBoldStyle, true);

    QString curPlainText = m_subject->text();
    QCOMPARE(curPlainText, plainText);
}

QTEST_APPLESS_MAIN(Ut_MRichTextEdit);

