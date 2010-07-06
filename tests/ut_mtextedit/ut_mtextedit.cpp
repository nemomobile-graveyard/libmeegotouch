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

#include "ut_mtextedit.h"

#include <QTest>
#include <MApplication>
#include <MApplicationWindow>
#include <QString>
#include <QTextCursor>
#include <QSignalSpy>
#include <QTextDocument>
#include <QInputMethodEvent>
#include <QGraphicsSceneMouseEvent>
#include <QValidator>
#include <QRegExpValidator>
#include <QRegExp>
#include <QClipboard>
#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include <QInputContext>
#include <QInputContextFactory>

#include <mtextedit.h>
#include <mtexteditview.h>
#include <mwidgetmodel.h>

#include <mscenemanager.h>
#include <mscenewindow.h>
#include <mscene.h>
#include <mnavigationbar.h>
#include <mhomebuttonpanel.h>
#include <mapplicationpage.h>

#include "mtextedit_p.h"

Q_DECLARE_METATYPE(Qt::InputMethodHints);
Q_DECLARE_METATYPE(QValidator::State);
Q_DECLARE_METATYPE(MTextEditModel::EchoMode);
Q_DECLARE_METATYPE(MTextEditModel::EditMode);

const QString Ut_MTextEdit::testString = QString("jallajalla");

Q_DECLARE_METATYPE(MTextEditModel::LineMode);

class ReplacerValidator : public QValidator
{
public:
    ReplacerValidator() : QValidator(0) {}
    virtual ~ReplacerValidator() {}

    virtual State validate(QString &input, int &pos) const {
        Q_UNUSED(pos)
        input.replace(QChar('a'), QChar('b'));
        return QValidator::Acceptable;
    }
};

class SimpleValidator : public QValidator
{
public:
    SimpleValidator() : QValidator(0), state(Acceptable) {}
    virtual ~SimpleValidator() {}

    virtual State validate(QString &input, int &pos) const {
        Q_UNUSED(input);
        Q_UNUSED(pos);

        return state;
    }

    QValidator::State state;
};

class SimpleInputContext: public QInputContext
{
public:
    SimpleInputContext(QObject *parent = 0)
        : QInputContext(parent),
          m_sipVisible(false)
    {}

    bool wouldSipBecomeVisible()
    {
        return m_sipVisible;
    }

    QString identifierName()
    {
        return QString("SimpleInputContext");
    }

    QString language()
    {
        return QString();
    }

    void reset()
    {}

    bool isComposing() const
    {
        return false;
    }

    bool filterEvent(const QEvent *ev)
    {
        switch (ev->type())
        {

        case QEvent::RequestSoftwareInputPanel:
            m_sipVisible = true;
            break;

        case QEvent::CloseSoftwareInputPanel:
            m_sipVisible = false;
            break;

        default:
            break;
        }

        return false;
    }

private:
    bool m_sipVisible;
};



/*!
 * Called once before the first testcase is run.
 */
void Ut_MTextEdit::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_mtextedit" };

    // prevent loading of minputcontext because we don't need it
    MApplication::setLoadMInputContext(false);

    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;
    m_sic = new SimpleInputContext(m_app);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    // contains valid strings which should be stored by widget as they are
    validStrings << "normal" << "normal with spaces" << "specials: !@#$%^&*()_+=-[]{}"
                 << "A string that is probably too long to fit to one line and needs to be wrapped or scrolled.";
    // TODO: Invent more test strings
}


/*!
 * Called once after the last testcase has finished.
 */
void Ut_MTextEdit::cleanupTestCase()
{
    delete m_appWindow;
    m_appWindow = 0;
    delete m_app;
    m_app = 0;
}


/*!
 * Called before each testcase.
 */
void Ut_MTextEdit::init()
{
    m_subject = new MTextEdit(MTextEditModel::MultiLine, "");
}


/*!
 * Called after each testcase.
 */
void Ut_MTextEdit::cleanup()
{
    delete m_subject;
    m_subject = 0;
}


/*!
 * Helper function for testSingleLineKeyPressEvent.
 * Makes sure that given key press event has no effect on widget.
 */
void Ut_MTextEdit::confirmKeyEventIgnored(MTextEdit *subject, int key, int expectedReturnPressed)
{
    subject->setText(testString);
    QSignalSpy textChangedSpy(subject, SIGNAL(textChanged()));
    QSignalSpy returnPressedSpy(subject, SIGNAL(returnPressed()));
    int startCursorPos = subject->cursorPosition();

    // Send event
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, key, 0, QString());
    subject->keyPressEvent(event);
    delete event;

    // Check that text was not changed and cursor has not moved
    QCOMPARE(textChangedSpy.count(), 0);
    QString text = subject->text();
    QCOMPARE(text, testString);
    QCOMPARE(startCursorPos, subject->cursorPosition());
    QCOMPARE(returnPressedSpy.count(), expectedReturnPressed);
}


/*!
 * Helper function for testConstraints.
 * Inserts input string by simulating key press for reach character and checks
 * that the result matches expectedOutput.
 */
void Ut_MTextEdit::constraintTest(MTextEdit *subject, const QString &input,
                                    const QString &expectedOutput)
{
    subject->setText(QString());
    for (int i = 0; i < input.size(); i++) {
        QString character = QString(input.at(i));
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, 0, 0, character);
        subject->keyPressEvent(event);
        delete event;
    }

    qDebug() << "In:" << input << "Out:" << subject->text() << "expected:" << expectedOutput;
    QCOMPARE(subject->text(), expectedOutput);
}


/*!
 * Test text setting.
 * Sets various strings and confirms that they are set correctly.
 */
void Ut_MTextEdit::testSetText()
{
    QTextDocument *document = m_subject->document();
    QSignalSpy mySpy(document, SIGNAL(contentsChanged()));
    QSignalSpy mySpy2(m_subject, SIGNAL(textChanged()));

    for (int i = 0; i < validStrings.size(); i++) {
        QString setText = validStrings.at(i);
        m_subject->setText(setText);
        QString getText = m_subject->text();

        // Check that the signals were sent and texts match
        QCOMPARE(mySpy.count(), (i + 1) * 2); //one to clear old second to set the new
        QCOMPARE(mySpy2.count(), (i + 1));
        QCOMPARE(getText, setText);
    }

    // test with phone number content type so validator gets tested too.
    int currentSpyCount = mySpy2.count();
    m_subject->setContentType(M::PhoneNumberContentType);
    m_subject->setText("+358-"); // partial phone number
    QCOMPARE(mySpy2.count(), currentSpyCount + 1);

    currentSpyCount = mySpy2.count();
    m_subject->setText(""); // empty string should be always ok
    QCOMPARE(mySpy2.count(), currentSpyCount + 1);
}


/*!
 * Test inserting text to widget
 * inserts strings from test list to widget and to a test string and make sure
 * that they match.
 */
void Ut_MTextEdit::testInsert()
{
    QTextDocument *document = m_subject->document();
    QSignalSpy mySpy(document, SIGNAL(contentsChanged()));
    QSignalSpy mySpy2(m_subject, SIGNAL(textChanged()));
    QSignalSpy copyAvailableSpy(m_subject, SIGNAL(copyAvailable(bool)));

    QString appendedString = QString();

    for (int i = 0; i < validStrings.size(); i++) {
        QString appendText = validStrings.at(i);
        m_subject->insert(appendText);
        appendedString.append(appendText);
        QString getText = m_subject->text();

        // Check that the signals were sent and texts match
        QCOMPARE(mySpy.count(), (i + 1)); //one to clear old second to set the new
        QCOMPARE(mySpy2.count(), (i + 1));
        QCOMPARE(getText, appendedString);
    }
    QVERIFY(copyAvailableSpy.count() == 0);

    m_subject->setText("1 2 3 4");
    m_subject->setSelection(1, 2, false);
    m_subject->insert("");
    QCOMPARE(m_subject->text(), QString("1 3 4"));
    QVERIFY(copyAvailableSpy.first().count() == 1);
    QVERIFY(copyAvailableSpy.first().first().toBool() == true);
    copyAvailableSpy.clear();
}


/*!
 * Test key press events.
 * Checks that pressed keys appear on text. Contains separate tests for
 * backspace and delete.
 */
void Ut_MTextEdit::testKeyPressEvent()
{
    // Test key press for keys 0-125 because only the characters that are visible are
    // should be returned from the QTextDocument.

    for (int round = 0; round < 125; round++) {
        QChar cRound(round);
        switch (cRound.category()) {
            // these cases cause problems
        case QChar::Other_NotAssigned:
        case QChar::Other_Control:
            break;

        default: {
            QString character = QString(cRound);
            m_subject->setText("");
            QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, round, 0, character);
            m_subject->keyPressEvent(event);
            delete event;

            QCOMPARE(m_subject->text().toUtf8(), character.toUtf8());
        }
        break;
        }
    }


    // test backspace
    m_subject->setText("abcd");

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, 0, QString());
    m_subject->keyPressEvent(event);
    delete event;

    QCOMPARE(m_subject->text(), QString("abc"));

    // test delete
    m_subject->setCursorPosition(0);
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Delete, 0, QString());
    m_subject->keyPressEvent(event);
    delete event;

    QCOMPARE(m_subject->text(), QString("bc"));

    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, 0, QString());
    m_subject->keyPressEvent(event);
    delete event;

    //cursor at beginning, backspage key will do nothing
    QCOMPARE(m_subject->text(), QString("bc"));
}


/*!
 * Test that single line edit ignores return key and up/down arrows.
 */
void Ut_MTextEdit::testSingleLineKeyPressEvent()
{
    MTextEdit singleLine(MTextEditModel::SingleLine, "");

    // Test line mode reading
    MTextEditModel::LineMode mode = singleLine.lineMode();
    QCOMPARE(mode, MTextEditModel::SingleLine);

    confirmKeyEventIgnored(&singleLine, Qt::Key_Return, 1);
}

#include <MEscapeButtonPanel>
/*!
 * Test functionality when widget gains focus.
 */
void Ut_MTextEdit::testFocusInEvent()
{
    MApplicationPage *page = new MApplicationPage();
    MTextEdit *m_subject = new MTextEdit(MTextEditModel::MultiLine, "");

    page->setCentralWidget(m_subject);
    m_appWindow->sceneManager()->appearSceneWindowNow(page);

    // Set up spies on gainedFocus and lostFocus signals
    QSignalSpy spyGainedFocus(m_subject, SIGNAL(gainedFocus(Qt::FocusReason)));
    QSignalSpy spyLostFocus(m_subject, SIGNAL(lostFocus(Qt::FocusReason)));

    // Perform test
    QFocusEvent *focusEvent = new QFocusEvent(QEvent::FocusIn);
    m_subject->focusInEvent(focusEvent);
    delete focusEvent;

    // Check that the signals were emitted and their parameters were correct
    QCOMPARE(spyGainedFocus.count(), 1);
    QCOMPARE(spyLostFocus.count(), 0);
}


/*!
 * Test functionality when widget loses focus.
 */
void Ut_MTextEdit::testFocusOutEvent()
{
    // Set up spies on gainedFocus and lostFocus signals
    QSignalSpy spyGainedFocus(m_subject, SIGNAL(gainedFocus(Qt::FocusReason)));
    QSignalSpy spyLostFocus(m_subject, SIGNAL(lostFocus(Qt::FocusReason)));

    // Perform test
    QFocusEvent *focusEvent = new QFocusEvent(QEvent::FocusOut);
    m_subject->focusOutEvent(focusEvent);
    delete focusEvent;

    // Check that the signals were emitted and their parameters were correct
    QCOMPARE(spyGainedFocus.count(), 0);
    QCOMPARE(spyLostFocus.count(), 1);
}


/*!
 * Test moving to pre-editing mode.
 */
void Ut_MTextEdit::testInputMethodEvent()
{
    QString testString2 = testString + '2';
    QSignalSpy spy(m_subject, SIGNAL(textChanged()));
    QVERIFY(spy.isValid());

    QInputMethodEvent *event = new QInputMethodEvent(testString,
            QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(event);
    delete event;
    QCOMPARE(m_subject->text(), testString);
    QCOMPARE(spy.count(), 1);
    spy.clear();

    // Confirm that widget moved to pre-editing mode
    QVERIFY(m_subject->mode() == MTextEditModel::EditModeActive);

    event = new QInputMethodEvent;
    event->setCommitString(testString2);
    m_subject->inputMethodEvent(event);
    delete event;
    QCOMPARE(m_subject->text(), testString2);
    QCOMPARE(spy.count(), 1);
    spy.clear();
}


/*!
 * Test setting cursor position.
 */
void Ut_MTextEdit::testSetCursorPosition()
{
    m_subject->setText(testString);

    for (int i = 0; i < testString.count(); i++) {
        m_subject->setCursorPosition(i);
        QCOMPARE(m_subject->cursorPosition(), i);
    }
}


// test that setTextCursor updates the state properly
void Ut_MTextEdit::testSetTextCursor()
{
    m_subject->setText(testString);

    QTextCursor cursor = m_subject->textCursor();
    cursor.setPosition(0);
    cursor.setPosition(5, QTextCursor::KeepAnchor);
    m_subject->setTextCursor(cursor);

    QCOMPARE(m_subject->mode(), MTextEditModel::EditModeSelect);

    cursor.setPosition(7);
    m_subject->setTextCursor(cursor);
    QCOMPARE(m_subject->mode(), MTextEditModel::EditModeBasic);
}


/*!
 * Test functionality when Qt::TextEditable-flag is set and removed
 */
void Ut_MTextEdit::testTextInteractionFlags()
{
    // Set editable-flag
    m_subject->setTextInteractionFlags(Qt::TextEditable);

    // Confirm that flag was set and focus policy set to ClickFocus
    QCOMPARE(m_subject->textInteractionFlags(), Qt::TextEditable);
    QCOMPARE(m_subject->focusPolicy(), Qt::ClickFocus);

    // Send event to set pre-edited text
    QInputMethodEvent *event = new QInputMethodEvent(testString,
            QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(event);
    delete event;

    // Confirm that widget is now in pre-edit mode
    QCOMPARE(m_subject->mode(), MTextEditModel::EditModeActive);

    // Remove editable-flag
    m_subject->setTextInteractionFlags(0);

    // Confirm that flags were removed and policy set to NoFocus
    QCOMPARE(m_subject->textInteractionFlags(), 0);
    QCOMPARE(m_subject->focusPolicy(), Qt::NoFocus);

    // Confirm also that pre-edit mode was exited
    QVERIFY(m_subject->mode() != MTextEditModel::EditModeActive);
}


void Ut_MTextEdit::testConstraints()
{
    MTextEdit singleLine(MTextEditModel::SingleLine, "");

    // Test line mode reading
    MTextEditModel::LineMode mode = singleLine.lineMode();
    QCOMPARE(mode, MTextEditModel::SingleLine);

    QString testInput("abcdABCD1234*/+-#[]{}()");

    // Test free text (input should remain unchanged)
    singleLine.setContentType(M::FreeTextContentType);
    QCOMPARE(singleLine.contentType(), M::FreeTextContentType);
    constraintTest(&singleLine, testInput, testInput);

    // Test numbers
    singleLine.setContentType(M::NumberContentType);
    QCOMPARE(singleLine.contentType(), M::NumberContentType);
    constraintTest(&singleLine, testInput, "1234");

    singleLine.setContentType(M::NumberContentType);
    QCOMPARE(singleLine.contentType(), M::NumberContentType);
    constraintTest(&singleLine, "+1234", "+1234");

    singleLine.setContentType(M::NumberContentType);
    QCOMPARE(singleLine.contentType(), M::NumberContentType);
    constraintTest(&singleLine, "-1234", "-1234");

    singleLine.setContentType(M::NumberContentType);
    QCOMPARE(singleLine.contentType(), M::NumberContentType);
    constraintTest(&singleLine, "+-1234", "+1234");

    // Test phone number
    singleLine.setContentType(M::PhoneNumberContentType);
    QCOMPARE(singleLine.contentType(), M::PhoneNumberContentType);
    constraintTest(&singleLine, testInput, "1234*/+-#()");

    // Test email address
    singleLine.setContentType(M::EmailContentType);
    QCOMPARE(singleLine.contentType(), M::EmailContentType);
    constraintTest(&singleLine, testInput, "abcdABCD1234*/+-#{}");

    // Test URL address
    // TODO: URL mode does not currently constrain anything
    singleLine.setContentType(M::UrlContentType);
    QCOMPARE(singleLine.contentType(), M::UrlContentType);
    constraintTest(&singleLine, testInput, testInput);
}


void Ut_MTextEdit::testReadOnly()
{
    QString content = "ReadOnly";
    QString changedContent = "Not ReadOnly";
    QString appendedString = "append";
    m_subject->setText(content);
    //test readonly
    m_subject->setReadOnly(true);
    QVERIFY(m_subject->isReadOnly() == true);

    // test backspace when readonly
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, 0, QString());
    m_subject->keyPressEvent(event);
    delete event;
    QCOMPARE(m_subject->text(), content);

    // test delete when readonly
    m_subject->setCursorPosition(0);
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Delete, 0, QString());
    m_subject->keyPressEvent(event);
    delete event;
    QCOMPARE(m_subject->text(), content);


    m_subject->setReadOnly(false);
    QVERIFY(m_subject->isReadOnly() == false);
    QVERIFY(m_subject->setText(changedContent) == true);
    QCOMPARE(m_subject->text(), changedContent);
    QVERIFY(m_subject->insert(appendedString) == true);
    QCOMPARE(m_subject->text(), changedContent + appendedString);

    m_subject->setText(content);
    // test backspace when not readonly
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, 0, QString());
    m_subject->keyPressEvent(event);
    delete event;
    QCOMPARE(m_subject->text(), QString("ReadOnl"));

    m_subject->setText(content);
    // test delete when not readonly
    m_subject->setCursorPosition(0);
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Delete, 0, QString());
    m_subject->keyPressEvent(event);
    delete event;
    QCOMPARE(m_subject->text(), QString("eadOnly"));
}

void Ut_MTextEdit::testMaxLength_data()
{
    QTest::addColumn<MTextEditModel::LineMode>("lineMode");
    QTest::addColumn<int>("maxLength");
    QTest::addColumn<QString>("text");

    QString longText(
        "This test checks that maxLength property of MTextEdit trims\n\n"
        "the text as expected.\nasdfasdf\n\nasdfasd");
    MTextEditModel::LineMode lineModes[2] = {MTextEditModel::SingleLine, MTextEditModel::MultiLine};
    for (int i = 0; i < 2; ++i) {
        QTest::newRow("too short") << lineModes[i] << -5 << longText;
        QTest::newRow("zero")      << lineModes[i] <<  0 << longText;
        QTest::newRow("short")     << lineModes[i] <<  5 << longText;
        QTest::newRow("long")      << lineModes[i] << (longText.length() - 5) << longText;
        QTest::newRow("too long")  << lineModes[i] << (longText.length() + 5) << longText;
    }
}

void Ut_MTextEdit::testMaxLength()
{
    QFETCH(MTextEditModel::LineMode, lineMode);
    QFETCH(int, maxLength);
    QFETCH(QString, text);

    const int initialMaxLength = m_subject->maxLength();

    if (m_subject->lineMode() != lineMode) {
        delete m_subject;
        m_subject = new MTextEdit(lineMode);
    }

    for (int i = 0; i < 3; ++i) {
        m_subject->clear();
        m_subject->setMaxLength(initialMaxLength);

        QString expectedText = text;

        // Test in three different ways:
        if (i == 0) {
            // Set limitation afterwards.
            m_subject->setText(text);
            m_subject->setMaxLength(maxLength);
        } else if (i == 1) {
            // Set limitation prior setting text.
            m_subject->setMaxLength(maxLength);
            m_subject->setText(text);
        } else {
            // Check maxLength with insert().
            m_subject->setMaxLength(maxLength);
            m_subject->setText(testString); // Set some initial diibadaaba text.
            // Special case where text is inserted at the end.
            m_subject->insert(text);

            expectedText.prepend(testString);
        }

        // Mimic the filtering MTextEdit does for single-line edits.
        if (lineMode == MTextEditModel::SingleLine) {
            expectedText.replace(QChar('\n'), QChar(' '));
        }

        // If maxLength is negative, treat as if zero.
        if (maxLength < 0) {
            maxLength = 0;
        }

        // Check that correct value was stored.
        QCOMPARE(m_subject->maxLength(), maxLength);

        // This is what should happen to the text.
        expectedText.truncate(maxLength);

        QCOMPARE(m_subject->text(), expectedText);

        // Length must always be equal or less than maxLength.
        QVERIFY(m_subject->text().length() <= maxLength);
    }
}

/*
void Ut_MTextEdit::testFeedback()
{
    Qt::TextInteractionFlag testGiveFeedback[] = {
        Qt::TextSelectableByMouse,
        Qt::TextSelectableByKeyboard,
        Qt::LinksAccessibleByMouse,
        Qt::LinksAccessibleByKeyboard,
        Qt::TextEditable,
        Qt::TextEditorInteraction,
        Qt::TextBrowserInteraction
    };
    Qt::TextInteractionFlag testNoFeedback[] = {
        Qt::NoTextInteraction
    };

    gMFeedbackPlayerStub->stubSetReturnValue<MFeedbackPlayerStub::FeedbackAction>(
            "feedback", MFeedbackPlayerStub::RETURN_OBJECT);
    MTextEdit edit1("", MTextEditModel::SingleLine);
    int cursorPosition = 0;
    QGraphicsSceneMouseEvent event;

    for (unsigned xx = 0; xx < sizeof(testGiveFeedback) / sizeof(testGiveFeedback[0]); ++xx)
    {
        edit1.setTextInteractionFlags(testGiveFeedback[xx]);
        gMFeedbackStub->stubReset();
        edit1.handleMousePress(cursorPosition, &event);
        QCOMPARE(gMFeedbackStub->stubCallCount("play"), 1);

        edit1.handleMouseRelease(cursorPosition, &event);
        QCOMPARE(gMFeedbackStub->stubCallCount("play"), 2);
    }

    for (unsigned xx = 0; xx < sizeof(testNoFeedback) / sizeof(testNoFeedback[0]); ++xx)
    {
        edit1.setTextInteractionFlags(testNoFeedback[xx]);
        gMFeedbackStub->stubReset();
        edit1.handleMousePress(cursorPosition, &event);
        QVERIFY(gMFeedbackStub->stubCallCount("play") == 0);

        edit1.handleMouseRelease(cursorPosition, &event);
        QVERIFY(gMFeedbackStub->stubCallCount("play") == 0);
    }
}
*/


void Ut_MTextEdit::testBadData()
{
    qDebug() << "Check of line mode";

    //check single line with multi line input
    MTextEdit *subject = new MTextEdit(MTextEditModel::SingleLine, "");
    subject->setText("I \n have \n more columns");
    QVERIFY(subject->lineMode() == MTextEditModel::SingleLine);
    qDebug() << subject->text();

    // Bug 1: Single line not handling \n properly
    // QVERIFY(subject->text() == "I");

    // check multi line with multi line input
    delete subject;
    subject = new MTextEdit(MTextEditModel::MultiLine, "");
    subject->setText("I \n have \n more columns");

    QVERIFY(subject->lineMode() == MTextEditModel::MultiLine);
    QVERIFY(subject->text() == "I \n have \n more columns");

    // check of TextInteractionFlags
    qDebug() << "Check of text interaction flags";
    delete subject;
    subject = new MTextEdit(MTextEditModel::SingleLine, "");
    QVERIFY(subject->textInteractionFlags() == Qt::TextEditorInteraction);
    subject->setTextInteractionFlags(Qt::NoTextInteraction);
    QVERIFY(subject->textInteractionFlags() == Qt::NoTextInteraction);
    QList<Qt::TextInteractionFlags> listOfFlags;
    listOfFlags << (Qt::TextEditorInteraction | Qt::TextEditable | Qt::TextSelectableByMouse
                    | Qt::TextSelectableByKeyboard);
    listOfFlags << (Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    listOfFlags << (Qt::NoTextInteraction | Qt::TextSelectableByKeyboard);

    for (int i = 0; i < listOfFlags.size(); ++i) {
        subject->setTextInteractionFlags(listOfFlags.at(i));

        if (subject->textInteractionFlags() != listOfFlags.at(i)) {
            qDebug() << "teststep: " << i << " is wrong!";
        }

        QVERIFY(subject->textInteractionFlags() == listOfFlags.at(i));
    }

    delete subject;

    // check of constraints
    qDebug() << "check of constraints";
    subject = new MTextEdit(MTextEditModel::SingleLine, "");

    // Number
    qDebug() << "   number";
    QStringList listNumber, listNumberRes;
    subject->setContentType(M::NumberContentType);
    listNumber  << "25"  << "99a99" << "88\t88" << "1234567890+-*/" << "noNumberHere" << "110";
    listNumberRes << "25" << "25" << "25" << "25" << "25" << "110";

    for (int i = 0; i < listNumber.size(); ++i) {
        subject->setText(listNumber.at(i));

        if (subject->text() != listNumberRes.at(i)) {
            qDebug() << "teststep (number): " << i << " is wrong!";
            qDebug() << "In:" << listNumber.at(i) << "Out:" << subject->text()
                     << "expected:" << listNumberRes.at(i);
        }

        // Bug #2 Constraints can't handle setting whole strings. If invalid text is set,
        // old text is deleted.
        // FIXME: DISABLED NOW - not sure if want to have content checking on setText()
        //QVERIFY(subject->text() == listNumberRes.at(i));
        //QVERIFY(subject->contentType() == M::NumberContentType);
    }

    listNumber.clear();
    listNumberRes.clear();
    subject->setText("");

    // PhoneNumber
    qDebug() << "   phone number";
    subject->setContentType(M::PhoneNumberContentType);
    listNumber << "0049618265231"   << "++49618275667" << "0180-667676-999-0#12" << "112"
               << "45 99 99 99 66 77";
    listNumberRes << "0049618265231" << "0049618265231" << "0180-667676-999-0#12" << "112"
                  << "459999996677";

    for (int i = 0; i < listNumber.size(); ++i) {
        subject->setText(listNumber.at(i));
        if (subject->text() == listNumber.at(i)) {
            qDebug() << "teststep (phone number): " << i << " is wrong!";
        }
        // Bug: MTextEdit don't check if Phone Number is valid
        // QCOMPARE(subject->text(), listNumberRes.at(i));
        // QVERIFY(subject->contentType() == MTextEditModel::PhoneNumber);
    }

    listNumber.clear();
    listNumberRes.clear();
    subject->setText("");

    //email
    //qDebug() << "   e-mail";
    QStringList listString, listStringRes;

    subject->setContentType(M::EmailContentType);
    listString    << "mail@mailme.com" << "coolMail@@nowhere.com" << "mailwithoutAdAndPoint"
                  << "myMail@mailme.com" << "testmail@abc..com" << "5999@888.com"
                  << "björnsmail@gmx.net";
    listStringRes << "mail@mailme.com" << "mail@mailme.com" << "" << "myMail@mailme.com"
                  << "testmail@abc.com" << "5999@888.com" << "björnsmail@gmx.net";

    for (int i = 0; i < listString.size(); ++i) {
        subject->setText(listString.at(i));
        if (subject->text() == listString.at(i)) {
            qDebug() << "teststep (e-mail): " << i << " is wrong!";
        }
        // Bug: MTextEdit don't check if E-Mail is valid
        // QCOMPARE(subject->text(), listStringRes.at(i));
        // QVERIFY(subject->contentType() == MTextEditModel::Email);
    }

    listString.clear();
    listStringRes.clear();
    subject->setText("");

    // url
    qDebug() << "   url";
    subject->setContentType(M::EmailContentType);
    listString    << "www.heise.de" << "UrlwithoutAdAndPoint"
                  << "http://www.google.de/search?hl=de&q=nokia&btnG=Google-Suche&meta="
                  << "httx:\\www..stern..de" << "www.888.com";
    listStringRes << "www.heise.de" << "www.heise.de"
                  << "http://www.google.de/search?hl=de&q=nokia&btnG=Google-Suche&meta="
                  << "http://www.google.de/search?hl=de&q=nokia&btnG=Google-Suche&meta="
                  << "www.888.com";

    for (int i = 0 ; i < listString.size(); ++i) {
        subject->setText(listString.at(i));

        if (subject->text() == listString.at(i)) {
            qDebug() << "teststep (url): " << i << " is wrong!";
        }
        // Bug: MTextEdit don't check if URL is valid
        // QCOMPARE(subject->text(), listStringRes.at(i));
        // QVERIFY(subject->contentType() == MTextEditModel::Email);
    }

    listString.clear();
    listStringRes.clear();
    subject->setText("");

    // check on setCursor
    qDebug() << "check setCursor";
    QList<int> numbers, numbersRes;
    numbers << 0 << -900 << 0 << 1 << -5 << 99 << 37000 << 36 << 22 << -12;
    numbersRes << 0 << 0 << 0 << 1 << 1 << 1 << 1 << 36 << 22 << 22;
    subject->setContentType(M::FreeTextContentType);
    subject->setText("A simple teststring, nothing useful.");
    // FIXME: How can I check the set cursor?}

    for (int i = 0; i < numbers.size(); ++i) {
        subject->setCursorPosition(numbers.at(i));
        // qDebug() << "In:" << numbers.at(i) << "Out:" << subject->cursorPosition()
        // << "expected:" << numbersRes.at(i);
        QCOMPARE(subject->cursorPosition(), numbersRes.at(i));
    }
}


void Ut_MTextEdit::testSelection()
{
    Qt::TextInteractionFlag testSelection[] = {
        Qt::TextEditorInteraction,
    };

    Qt::TextInteractionFlag testNoSelection[] = {
        Qt::NoTextInteraction,
        Qt::LinksAccessibleByMouse,
        Qt::LinksAccessibleByKeyboard,
    };

    QSignalSpy copyAvailableSpy(m_subject, SIGNAL(copyAvailable(bool)));
    const char *text = "a bcd e";
    m_subject->setText(text);

    m_subject->setSelection(0, 1, true);
    QVERIFY(m_subject->text() == text);
    QVERIFY(m_subject->mode() == MTextEditModel::EditModeSelect);
    //verify if setSelection emitted copyAvailable(true)
    QVERIFY(copyAvailableSpy.count() == 1);
    QVERIFY(copyAvailableSpy.first().count() == 1);
    QVERIFY(copyAvailableSpy.first().first().toBool() == true);
    copyAvailableSpy.clear();

    m_subject->deselect();
    QVERIFY(m_subject->text() == text);
    QVERIFY(m_subject->mode() == MTextEditModel::EditModeBasic);
    QVERIFY(copyAvailableSpy.count() == 1);
    QVERIFY(copyAvailableSpy.first().count() == 1);
    QVERIFY(copyAvailableSpy.first().first().toBool() == false);
    copyAvailableSpy.clear();

    for (unsigned n = 0; n < sizeof(testSelection) / sizeof(testSelection[0]); ++n) {
        qDebug() << n << testSelection[n];

        m_subject->setTextInteractionFlags(testSelection[n]);
        m_subject->setText(text);
        m_subject->setSelection(1, 1, true);

        QSignalSpy updatedSpy(m_subject, SIGNAL(selectionChanged()));
        m_subject->setSelection(2, 1, true);
        QCOMPARE(updatedSpy.count(), 1);
        QVERIFY(m_subject->mode() == MTextEditModel::EditModeSelect);
        QVERIFY(copyAvailableSpy.count() == 1);
        QVERIFY(copyAvailableSpy.first().count() == 1);
        QVERIFY(copyAvailableSpy.first().first().toBool() == true);
        copyAvailableSpy.clear();

        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, 0, QString());
        m_subject->keyPressEvent(event);
        delete event;

        QCOMPARE(m_subject->text(), QString("a  e"));
        QCOMPARE(updatedSpy.count(), 2);
        QVERIFY(m_subject->mode() == MTextEditModel::EditModeBasic);
        QVERIFY(copyAvailableSpy.count() == 1);
        QVERIFY(copyAvailableSpy.first().count() == 1);
        QVERIFY(copyAvailableSpy.first().first().toBool() == false);
        copyAvailableSpy.clear();
    }

    for (unsigned n = 0; n < sizeof(testNoSelection) / sizeof(testNoSelection[0]); ++n) {
        qDebug() << n << testNoSelection[n];
        m_subject->setTextInteractionFlags(testNoSelection[n]);
        m_subject->setText(text);
        m_subject->setSelection(2, 1, true);

        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, 0, QString());
        m_subject->keyPressEvent(event);
        delete event;
        QCOMPARE(m_subject->text(), QString(text));
        QCOMPARE(copyAvailableSpy.count(), 0);

        QVERIFY(m_subject->mode() == MTextEditModel::EditModeBasic);
    }

    bool success = m_subject->setText(text);
    QCOMPARE(success, true);
    m_subject->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_subject->setSelection(1, 1, true);

    copyAvailableSpy.clear(); // ignore all previuos signals

    m_subject->setCursorPosition(0);
    QCOMPARE(m_subject->text(), QString(text));
    QVERIFY(m_subject->mode() == MTextEditModel::EditModeBasic);
    // setCursorPosition should emit signal copyAvailable(false)
    // because text was deselected
    QVERIFY(copyAvailableSpy.count() == 1);
    QVERIFY(copyAvailableSpy.first().count() == 1);
    QVERIFY(copyAvailableSpy.first().first().toBool() == false);
    copyAvailableSpy.clear();

    m_subject->setSelection(1, 1, true);
    copyAvailableSpy.clear();
    m_subject->setText(text);
    QCOMPARE(m_subject->text(), QString(text));
    QVERIFY(m_subject->mode() == MTextEditModel::EditModeBasic);
    // setTest should emit signal copyAvailable(false)
    // because text was deselected
    QVERIFY(copyAvailableSpy.count() == 1);
    QVERIFY(copyAvailableSpy.first().count() == 1);
    QVERIFY(copyAvailableSpy.first().first().toBool() == false);
    copyAvailableSpy.clear();

    m_subject->setText(text);
    QCOMPARE(m_subject->text(), QString(text));
    QVERIFY(m_subject->mode() == MTextEditModel::EditModeBasic);
    // setText should not emit signal copyAvailable because
    // there was no selection
    QVERIFY(copyAvailableSpy.count() == 0);
}


void Ut_MTextEdit::testAutoSelection()
{
    Qt::TextInteractionFlag testSelection[] = {
        Qt::TextSelectableByMouse,
        Qt::TextSelectableByKeyboard,
        Qt::TextEditorInteraction,
        Qt::TextBrowserInteraction,
    };
    Qt::TextInteractionFlag testNoSelection[] = {
        Qt::NoTextInteraction,
        Qt::LinksAccessibleByMouse,
        Qt::LinksAccessibleByKeyboard,
    };

    for (unsigned n = 0; n < sizeof(testSelection) / sizeof(testSelection[0]); ++n) {
        qDebug() << n << testSelection[n];
        m_subject->setTextInteractionFlags(testSelection[n]);

        m_subject->setAutoSelectionEnabled(false);
        QVERIFY(m_subject->isAutoSelectionEnabled() == false);

        m_subject->setAutoSelectionEnabled(true);
        QVERIFY(m_subject->isAutoSelectionEnabled() == true);
    }

    for (unsigned n = 0; n < sizeof(testNoSelection) / sizeof(testNoSelection[0]); ++n) {
        qDebug() << n << testNoSelection[n];
        m_subject->setTextInteractionFlags(testNoSelection[n]);
        m_subject->setAutoSelectionEnabled(false);
        QVERIFY(m_subject->isAutoSelectionEnabled() == false);
        m_subject->setAutoSelectionEnabled(true);
        QVERIFY(m_subject->isAutoSelectionEnabled() == false);
    }
}


void Ut_MTextEdit::testPrompt()
{
    MWidgetModel *model = 0;

    QStringList prompts;
    QList<int> expectedSignals;

    QList<M::TextContentType> contentTypes; //all possible content types

    contentTypes << M::FreeTextContentType << M::NumberContentType
                 << M::PhoneNumberContentType << M::EmailContentType
                 << M::UrlContentType << M::CustomContentType;

    prompts
            << "Some text"
            << "Some text"
            << QString()
            << QString()
            << QString(1000, 'A') //very long prompt
            << "1234567890"
            << QString("Öylätti")
            << QString("Äänekoski")
            << QString("Åbo")
            << "Multi\nline\nprompt"
            << "Multi\nline\nprompt";

    foreach(M::TextContentType contentType, contentTypes) {
        delete m_subject;
        m_subject = new MTextEdit(MTextEditModel::MultiLine, "");
        m_subject->setContentType(contentType);

        model = m_subject->model();
        QVERIFY(model != 0);
        //Verify default prompt
        QVERIFY(m_subject->prompt() == QString());

        //qDebug() << "Multi line text entry; content type:" << contentType;
        for (int n = 0; n < prompts.count(); ++n) {
            //qDebug() << "Test step #" << n << "prompt:" << prompts.at(n);

            m_subject->setPrompt(prompts.at(n));
            QVERIFY(m_subject->prompt() == prompts.at(n));
        }
    }

    //test single line editor
    prompts.removeLast();
    prompts.removeLast();

    foreach(M::TextContentType contentType, contentTypes) {
        delete m_subject;
        m_subject = new MTextEdit(MTextEditModel::SingleLine, "");
        m_subject->setContentType(contentType);

        model = m_subject->model();
        QVERIFY(model != 0);
        //Verify default prompt
        QVERIFY(m_subject->prompt() == QString());

        //qDebug() << "Single line text entry; content type:" << contentType;
        for (int n = 0; n < prompts.count(); ++n) {
            //qDebug() << "Test step #" << n << "prompt:" << prompts.at(n);

            m_subject->setPrompt(prompts.at(n));
            QVERIFY(m_subject->prompt() == prompts.at(n));
        }
    }
}

void Ut_MTextEdit::testValidator()
{
    QSignalSpy copyAvailableSpy(m_subject, SIGNAL(copyAvailable(bool)));
    // first test validation with builtin validator
    ReplacerValidator replacer;
    m_subject->setValidator(&replacer);

    QInputMethodEvent event;
    event.setCommitString(QString("aaa"));

    m_subject->inputMethodEvent(&event);
    QCOMPARE(m_subject->textCursor().position(), 3);
    QCOMPARE(m_subject->text(), QString("bbb"));

    // second test:
    qDebug() << "Starting A Line test";
    m_subject->setText("");
    QRegExp aline("a*\n");
    QRegExpValidator aLineValidator(aline, 0);
    m_subject->setValidator(&aLineValidator);
    event.setCommitString("aaa\n");
    m_subject->inputMethodEvent(&event);
    QCOMPARE(m_subject->textCursor().position(), 4);
    QCOMPARE(m_subject->text(), QString("aaa\n"));

    // add more text that shouldn't be allowed
    event.setCommitString("bcd\n");
    m_subject->inputMethodEvent(&event);
    QCOMPARE(m_subject->text(), QString("aaa\n\n"));

    // test that only correct parts are inserted when partly invalid content is passed on selection
    m_subject->selectAll();
    QVERIFY(copyAvailableSpy.count() == 1);
    QVERIFY(copyAvailableSpy.first().count() == 1);
    QVERIFY(copyAvailableSpy.first().first().toBool() == true);
    copyAvailableSpy.clear();
    event.setCommitString("aa\nabcd\n\n");
    m_subject->inputMethodEvent(&event);
    QCOMPARE(m_subject->text(), QString("aa\n\n\n"));
    QVERIFY(copyAvailableSpy.count() == 1);
    QVERIFY(copyAvailableSpy.first().count() == 1);
    QVERIFY(copyAvailableSpy.first().first().toBool() == false);
    copyAvailableSpy.clear();

    // test that nothing happens on invalid insertion with selection
    m_subject->selectAll();
    event.setCommitString("abcd");
    m_subject->inputMethodEvent(&event);
    QCOMPARE(m_subject->text(), QString("aa\n\n\n"));

    // test that selection is replaced with insertion on valid insertion
    m_subject->selectAll();
    event.setCommitString("aaaaa");
    m_subject->inputMethodEvent(&event);
    QCOMPARE(m_subject->text(), QString("aaaaa"));

    // test that custom validator still stays after adjusting content type
    m_subject->setContentType(M::NumberContentType);
    QCOMPARE(m_subject->validator(), &aLineValidator);
}


void Ut_MTextEdit::testClear()
{
    // test that clear() works. first set content and then test that after clear the state has reset
    m_subject->setText("AsdfAsdfasD sdfdf asdf ");
    m_subject->setSelection(3, 8, false);
    m_subject->clear();
    QCOMPARE(m_subject->cursorPosition(), 0);
    QCOMPARE(m_subject->mode(), MTextEditModel::EditModeBasic);
    QCOMPARE(m_subject->text(), QString(""));
}


void Ut_MTextEdit::testCursorPositionChanged()
{
    QSignalSpy cursorSpy(m_subject, SIGNAL(cursorPositionChanged()));
    QSignalSpy copyAvailableSpy(m_subject, SIGNAL(copyAvailable(bool)));
    int expectedCallCount = 0;

    // test that setting text moves cursor to back
    m_subject->setText("asdf");
    expectedCallCount++;
    QCOMPARE(cursorSpy.count(), expectedCallCount);

    // backspace moves cursor
    QKeyEvent event(QEvent::KeyPress, Qt::Key_Backspace, 0, QString());
    m_subject->keyPressEvent(&event);
    expectedCallCount++;
    QCOMPARE(cursorSpy.count(), expectedCallCount);

    // button press moves
    event = QKeyEvent(QEvent::KeyPress, 0, 0, QString("a"));
    m_subject->keyPressEvent(&event);
    expectedCallCount++;
    QCOMPARE(cursorSpy.count(), expectedCallCount);

    // left key moves
    event = QKeyEvent(QEvent::KeyPress, Qt::Key_Left, 0, QString());
    m_subject->keyPressEvent(&event);
    expectedCallCount++;
    QCOMPARE(cursorSpy.count(), expectedCallCount);

    // inputmethodevent moves
    QInputMethodEvent imEvent(QString("abcd"), QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&imEvent);
    expectedCallCount++;
    QCOMPARE(cursorSpy.count(), expectedCallCount);
    QVERIFY(copyAvailableSpy.count() == 0);
}


void Ut_MTextEdit::testCopyPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    QVERIFY(clipboard != 0);

    QString text("some text");
    QString text2("123 456");
    clipboard->setText(text);
    m_subject->setText(text2);
    //clipboard content should not be changed if nothing was selected
    m_subject->copy();
    QCOMPARE(clipboard->text(), text);

    m_subject->setSelection(1, 1, true);
    //something should be copied
    m_subject->copy();
    QCOMPARE(clipboard->text(), QString("123"));

    m_subject->selectAll();
    clipboard->setText(text);
    //whole text should be replaced
    m_subject->paste();
    QCOMPARE(m_subject->text(), text);

    //new text should be appended to existing
    m_subject->paste();
    QCOMPARE(m_subject->text(), text + text);

    m_subject->setText(text);
    m_subject->selectAll();
    clipboard->setText("");
    //nothing should be changed
    m_subject->paste();
    QCOMPARE(m_subject->text(), text);

    //verify behavior of read-only text entry
    m_subject->setText(text2);
    m_subject->setReadOnly(true);

    clipboard->setText(text);
    //clipboard content should not be changed if nothing was selected
    m_subject->copy();
    QCOMPARE(clipboard->text(), text);

    m_subject->setSelection(1, 1, true);
    //something should be copied
    m_subject->copy();
    QCOMPARE(clipboard->text(), QString("123"));

    //paste should not modify read-only text entry
    m_subject->selectAll();
    clipboard->setText(text);
    m_subject->paste();
    QCOMPARE(m_subject->text(), text2);

    m_subject->paste();
    QCOMPARE(m_subject->text(), text2);

    m_subject->setText(text);
    m_subject->selectAll();
    clipboard->setText("");
    m_subject->paste();
    QCOMPARE(m_subject->text(), text);

    // text copying should not be possible with masked text entry
    m_subject->setEchoMode(MTextEditModel::Password);
    m_subject->selectAll();
    m_subject->copy();
    QCOMPARE(clipboard->text(), QString());

    //test copy/paste via keyboard events
    QKeyEvent copy(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier, QChar('\3'));
    QKeyEvent paste(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier, QChar('\22'));

    m_subject->setEchoMode(MTextEditModel::Normal);
    m_subject->setReadOnly(false);

    m_subject->setText(text);
    m_subject->selectAll();
    clipboard->setText("");
    m_subject->keyPressEvent(&copy);
    QCOMPARE(m_subject->text(), text);
    QCOMPARE(clipboard->text(), text);

    m_subject->clear();
    m_subject->keyPressEvent(&paste);
    QCOMPARE(m_subject->text(), text);
    QCOMPARE(clipboard->text(), text);

    //copy/paste via keyboard event is not allwed if text interaction is disabled
    m_subject->setText(text);
    m_subject->selectAll();
    m_subject->setTextInteractionFlags(Qt::NoTextInteraction);
    clipboard->setText("");
    m_subject->keyPressEvent(&copy);
    QCOMPARE(m_subject->text(), text);
    QCOMPARE(clipboard->text(), QString());

    m_subject->clear();
    clipboard->setText(text);
    m_subject->keyPressEvent(&paste);
    QCOMPARE(m_subject->text(), QString());
    QCOMPARE(clipboard->text(), text);
}

void Ut_MTextEdit::testPasteOnPreedit()
{
    QString text("some text");
    QInputMethodEvent event(text, QList<QInputMethodEvent::Attribute>());
    QClipboard *clipboard = QApplication::clipboard();
    QVERIFY(clipboard != 0);

    m_subject->inputMethodEvent(&event);
    clipboard->setText(text);
    m_subject->paste();
    QCOMPARE(m_subject->text(), text + text);
}

void Ut_MTextEdit::testCut_data()
{
    QTest::addColumn<QString>("clipboardText");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("expectedClipboard");
    QTest::addColumn<QString>("expectedText");
    QTest::addColumn<bool>("doSelection");
    QTest::addColumn<MTextEditModel::EchoMode>("echoMode");
    QTest::addColumn<bool>("readOnly");
    QTest::addColumn<MTextEditModel::EditMode>("expectedMode");
    QTest::addColumn<bool>("useValidator");
    QTest::addColumn<QValidator::State>("validatorState");
    QTest::addColumn<bool>("sendEvent");

    QString text("some text");
    QString part1(2048, '1');
    QString part2(" " + QString(1000, '2') + " " + QString(1000, '3'));
    QString text2(part1 + part2);

    for (int sendEvent = 0; sendEvent <= 1; ++sendEvent) {
        for (int useValidator = 0; useValidator <= 1; ++useValidator) {
            QTest::newRow("no selection") << text << text2 << text  << text2  << false
                << MTextEditModel::Normal << false
                << MTextEditModel::EditModeBasic << bool(useValidator)
                << QValidator::Acceptable << bool(sendEvent);

            QTest::newRow("selection")    << text << text2 << part1 << part2 << true
                << MTextEditModel::Normal << false
                << MTextEditModel::EditModeBasic << bool(useValidator)
                << QValidator::Acceptable << bool(sendEvent);

            QTest::newRow("read only")    << text << text2 << text  << text2 << true
                << MTextEditModel::Normal << true
                << MTextEditModel::EditModeSelect << bool(useValidator)
                << QValidator::Acceptable << bool(sendEvent);

            for (MTextEditModel::EchoMode mode = MTextEditModel::NoEcho;
                    mode <= MTextEditModel::PasswordEchoOnEdit;
                    mode = MTextEditModel::EchoMode(mode + 1)) {
                QTest::newRow("test echo mode") << text << text2 << text << text2 << true
                    << mode << false << MTextEditModel::EditModeSelect
                    << bool(useValidator) << QValidator::Acceptable << bool(sendEvent);
            }
        }

        for (int validatorState = QValidator::Intermediate;
                validatorState <= QValidator::Acceptable;
                ++validatorState) {
            QTest::newRow("test validator") << text << text2 << part1 << part2 << true
                << MTextEditModel::Normal << false
                << MTextEditModel::EditModeBasic << true
                << QValidator::State(validatorState) << bool(sendEvent);
        }

        QTest::newRow("invalid") << text << text2 << part1 << text2 << true
            << MTextEditModel::Normal << false
            << MTextEditModel::EditModeSelect << true << QValidator::Invalid
            << bool(sendEvent);
    }
}

void Ut_MTextEdit::testCut()
{
    QClipboard *clipboard = QApplication::clipboard();
    QVERIFY(clipboard != 0);
    QKeyEvent keyCut(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier, QChar());

    QFETCH(QString, clipboardText);
    QFETCH(QString, text);
    QFETCH(QString, expectedClipboard);
    QFETCH(QString, expectedText);
    QFETCH(bool, doSelection);
    QFETCH(MTextEditModel::EchoMode, echoMode);
    QFETCH(bool, readOnly);
    QFETCH(MTextEditModel::EditMode, expectedMode);
    QFETCH(bool, useValidator);
    QFETCH(QValidator::State, validatorState);
    QFETCH(bool, sendEvent);

    qDebug() << "echoMode=" << echoMode << "useValidator=" << useValidator
             << "validatorState=" << validatorState << "sendEvent=" << sendEvent;

    clipboard->setText(clipboardText);
    m_subject->setText(text);

    if (doSelection) {
        m_subject->setSelection(1, 1, true);
    }
    m_subject->setEchoMode(echoMode);
    m_subject->setReadOnly(readOnly);

    SimpleValidator validator;
    validator.state = validatorState;
    if (useValidator) {
        m_subject->setValidator(&validator);
    }

    if (sendEvent) {
        m_subject->keyPressEvent(&keyCut);
    } else {
       m_subject->cut();
    }

    QCOMPARE(clipboard->text(), expectedClipboard);
    QCOMPARE(m_subject->text(), expectedText);
    QVERIFY(m_subject->mode() == expectedMode);
}

void Ut_MTextEdit::testInputMethodQuery()
{
    QString text = "123";
    Qt::InputMethodQuery query = Qt::ImCurrentSelection;
    QVariant queryResult = m_subject->inputMethodQuery(query);
    QVERIFY(queryResult.isValid() == true);
    QVERIFY(queryResult.toString().isEmpty() == true);

    m_subject->setText(text);
    m_subject->selectAll();

    query = Qt::ImCurrentSelection;
    queryResult = m_subject->inputMethodQuery(query);
    QVERIFY(queryResult.isValid() == true);
    QCOMPARE(queryResult.toString(), text);

    query = static_cast<Qt::InputMethodQuery>(M::VisualizationPriorityQuery);
    queryResult = m_subject->inputMethodQuery(query);
    QVERIFY(queryResult.isValid() == true);
    QVERIFY(queryResult.toString().isEmpty() == false);

    const QString toolbar("testToolbar");
    m_subject->attachToolbar(toolbar);
}

void Ut_MTextEdit::testConstrainedPaste()
{
    //some characters should be filtered out
    //if validator was assigned for text editor

    QClipboard *clipboard = QApplication::clipboard();
    QVERIFY(clipboard != 0);

    //test for numbers
    clipboard->setText("+123 456q");
    m_subject->setContentType(M::NumberContentType);

    m_subject->paste();
    QCOMPARE(m_subject->text(), QString("+123456"));

    m_subject->clear();
    //test for phone numbers
    m_subject->setContentType(M::PhoneNumberContentType);
    clipboard->setText("+*123!45w6pq");
    m_subject->paste();
    QCOMPARE(m_subject->text(), QString("+*12345w6p"));
}


void Ut_MTextEdit::testSignChange_data()
{
    QTest::addColumn<M::TextContentType>("contentType");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("expectedText");

    QTest::newRow("free") << M::FreeTextContentType
                          << QString() << QString();

    QTest::newRow("insertSign") << M::NumberContentType
                                << QString() << QString("-");

    QTest::newRow("replacePlusWithMinus") << M::NumberContentType
                                          << QString("+") << QString("-");

    QTest::newRow("replaceMinusWithPlus") << M::NumberContentType
                                          << QString("-") << QString("+");

    QTest::newRow("insertSignNumber") << M::NumberContentType
                                      << QString("3.01") << QString("-3.01");

    QTest::newRow("replacePlusWithMinusNumber") << M::NumberContentType
            << QString("+9.99") << QString("-9.99");

    QTest::newRow("replaceMinusWithPlusNumber") << M::NumberContentType
            << QString("-1") << QString("+1");
}

void Ut_MTextEdit::testSignChange()
{
    QFETCH(M::TextContentType, contentType);
    QFETCH(QString, text);
    QFETCH(QString, expectedText);

    qDebug() << "contentType=" << contentType << "text=" << text
             << "expectedText=" << expectedText;
    QKeyEvent metaPlus(QEvent::KeyPress, Qt::Key_plusminus, Qt::MetaModifier, QString());

    m_subject->setContentType(contentType);
    m_subject->insert(text);
    QCOMPARE(m_subject->text(), text);
    m_subject->keyPressEvent(&metaPlus);
    QCOMPARE(m_subject->text(), expectedText);
}

void Ut_MTextEdit::testSetContentType_data()
{
    QTest::addColumn<M::TextContentType>("contentType");
    QTest::addColumn<Qt::InputMethodHints>("expectedHints");

    QTest::newRow("free") << M::FreeTextContentType
                          << Qt::InputMethodHints(Qt::ImhNone);

    QTest::newRow("number") << M::NumberContentType
                            << Qt::InputMethodHints(Qt::ImhFormattedNumbersOnly);

    QTest::newRow("phone") << M::PhoneNumberContentType
                           << Qt::InputMethodHints(Qt::ImhDialableCharactersOnly);

    QTest::newRow("email") << M::EmailContentType
                           << Qt::InputMethodHints(Qt::ImhEmailCharactersOnly);

    QTest::newRow("url") << M::UrlContentType
                         << Qt::InputMethodHints(Qt::ImhUrlCharactersOnly);
}

void Ut_MTextEdit::testSetContentType()
{
    QFETCH(M::TextContentType, contentType);
    QFETCH(Qt::InputMethodHints, expectedHints);

    qDebug() << "contentType=" << contentType << "expectedHints=" << expectedHints;

    m_subject->setContentType(contentType);
    QCOMPARE(int(m_subject->inputMethodHints() & Qt::ImhExclusiveInputMask), int(expectedHints));
}

void Ut_MTextEdit::testInputMethodHints()
{
    QVERIFY(m_subject->inputMethodHints() == Qt::ImhNone);

    m_subject->setEchoMode(MTextEditModel::Password);
    QVERIFY(m_subject->inputMethodHints() & Qt::ImhHiddenText);
    QVERIFY(m_subject->echoMode() == MTextEditModel::Password);

    m_subject->setInputMethodPredictionEnabled(false);
    QVERIFY(m_subject->inputMethodHints() & Qt::ImhNoPredictiveText);
    QVERIFY(m_subject->inputMethodPredictionEnabled() == false);

    m_subject->setInputMethodAutoCapitalizationEnabled(false);
    QVERIFY(m_subject->inputMethodHints() & Qt::ImhNoAutoUppercase);
    QVERIFY(m_subject->inputMethodAutoCapitalizationEnabled() == false);
}

void Ut_MTextEdit::testAttachToolbar()
{
    m_subject->attachToolbar("");
    QVERIFY(m_subject->attachedToolbar().isEmpty());
    const QString toolbar("testToolbar");
    m_subject->attachToolbar(toolbar);
    QCOMPARE(m_subject->attachedToolbar(), toolbar);
}

void Ut_MTextEdit::testPasswordEchoOnEditClearing()
{
    // tests that the content is cleared when starting editing
    m_subject->setText("asdf");
    m_subject->setEchoMode(MTextEditModel::PasswordEchoOnEdit);

    QFocusEvent focusEvent(QEvent::FocusIn);
    m_subject->focusInEvent(&focusEvent);

    QKeyEvent keyEvent(QEvent::KeyPress, 0, 0, "a");
    m_subject->keyPressEvent(&keyEvent);

    QCOMPARE(m_subject->text().length(), 1);

    // set back to normal mode and check that the content is not cleared when starting editing.
    QFocusEvent focusOutEvent(QEvent::FocusOut);
    m_subject->focusOutEvent(&focusOutEvent);

    m_subject->setEchoMode(MTextEditModel::Normal);
    m_subject->keyPressEvent(&keyEvent);
    QCOMPARE(m_subject->text().length(), 2);
}

void Ut_MTextEdit::testReturnKeyPressed_data()
{
    QTest::addColumn<MTextEditModel::LineMode>("lineMode");
    QTest::addColumn<QValidator::State>("validatorState");
    QTest::addColumn<int>("expectedSignals");

    QTest::newRow("single line, invalid")      << MTextEditModel::SingleLine << QValidator::Invalid      << 0;
    QTest::newRow("single line, intermediate") << MTextEditModel::SingleLine << QValidator::Intermediate << 0;
    QTest::newRow("single line, acceptable")   << MTextEditModel::SingleLine << QValidator::Acceptable   << 1;
    QTest::newRow("multi line,  invalid")      << MTextEditModel::MultiLine  << QValidator::Invalid      << 0;
    QTest::newRow("multi line,  intermediate") << MTextEditModel::MultiLine  << QValidator::Intermediate << 0;
    QTest::newRow("multi line,  acceptable")   << MTextEditModel::MultiLine  << QValidator::Acceptable   << 0;
}

/*!
 * Verify that returnPressed() is emitted when all conditions are met:
 *      user press return key on hardware keyboard
 *      validator returns QValidator::Acceptable
 *      text entry is single line entry
 */
void Ut_MTextEdit::testReturnKeyPressed()
{
    QFETCH(MTextEditModel::LineMode, lineMode);
    QFETCH(QValidator::State, validatorState);
    QFETCH(int, expectedSignals);

    delete m_subject;
    m_subject = new MTextEdit(lineMode, "");

    SimpleValidator validator;
    QKeyEvent event(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier, "\n");
    QSignalSpy returnPressedSpy(m_subject, SIGNAL(returnPressed()));

    validator.state = validatorState;
    m_subject->setValidator(&validator);

    m_subject->keyPressEvent(&event);
    QCOMPARE(returnPressedSpy.count(), expectedSignals);
}

void Ut_MTextEdit::testLineBreakSent_data()
{
    QTest::addColumn<MTextEditModel::LineMode>("lineMode");
    QTest::addColumn<QString>("preeditString");
    QTest::addColumn<QString>("commitString");
    QTest::addColumn<M::TextContentType>("contentType");
    QTest::addColumn<int>("expectedSignals");

    QTest::newRow("single line, empty") << MTextEditModel::SingleLine << "" << "" << M::FreeTextContentType << 0;
    QTest::newRow("single line, preedit, no line break") << MTextEditModel::SingleLine << "preedit" << "" << M::FreeTextContentType << 0;
    QTest::newRow("single line, commit, no line break")  << MTextEditModel::SingleLine << "" << "commit" << M::FreeTextContentType << 0;
    QTest::newRow("single line, preedit, line break") << MTextEditModel::SingleLine << "preedit\n" << "" << M::FreeTextContentType << 0;
    QTest::newRow("single line, commit, line break")  << MTextEditModel::SingleLine << "" << "commit\n" << M::FreeTextContentType << 1;
    QTest::newRow("single line, commit, line break only")  << MTextEditModel::SingleLine << "" << "\n" << M::FreeTextContentType << 1;
    QTest::newRow("single line, commit, multiple line breaks")  << MTextEditModel::SingleLine << "" << "commit\n\n\n" << M::FreeTextContentType << 1;
    QTest::newRow("single line, invalid number") << MTextEditModel::SingleLine << "" << "abc\n" << M::NumberContentType << 0;
    QTest::newRow("single line, valid number") << MTextEditModel::SingleLine << "" << "123\n" << M::NumberContentType << 1;

    QTest::newRow("multi line, empty") << MTextEditModel::MultiLine << "" << "" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, preedit, no line break") << MTextEditModel::MultiLine << "preedit" << "" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, no line break")  << MTextEditModel::MultiLine << "" << "commit" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, preedit, line break") << MTextEditModel::MultiLine << "preedit\n" << "" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, line break only")  << MTextEditModel::MultiLine << "" << "\n" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, line break")  << MTextEditModel::MultiLine << "" << "commit\n" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, multiple line breaks")  << MTextEditModel::MultiLine << "" << "commit\n\n\n" << M::FreeTextContentType << 0;
}

/*!
 * Verify that returnPressed() is emitted when all conditions are met:
 *      user press return key on virtual keyboard
 *      validator returns QValidator::Acceptable
 *      text entry is single line entry
 */
void Ut_MTextEdit::testLineBreakSent()
{
    QFETCH(MTextEditModel::LineMode, lineMode);
    QFETCH(QString, preeditString);
    QFETCH(QString, commitString);
    QFETCH(M::TextContentType, contentType);
    QFETCH(int, expectedSignals);

    delete m_subject;
    m_subject = new MTextEdit(lineMode, "");
    m_subject->setContentType(contentType);

    QSignalSpy returnPressedSpy(m_subject, SIGNAL(returnPressed()));
    QVERIFY(returnPressedSpy.isValid());

    QInputMethodEvent event(preeditString, QList<QInputMethodEvent::Attribute>());
    event.setCommitString(commitString);
    m_subject->inputMethodEvent(&event);

    QCOMPARE(returnPressedSpy.count(), expectedSignals);
}

void Ut_MTextEdit::testCommitLineBreakAfterPreedit_data()
{
    QTest::addColumn<QValidator::State>("validatorState");
    QTest::addColumn<QString>("commitString");
    QTest::addColumn<int>("expectedSignals");

    QTest::newRow("123\\n, invalid")      << QValidator::Invalid      << "123\n" << 0;
    QTest::newRow("123\\n, intermediate") << QValidator::Intermediate << "123\n" << 0;
    QTest::newRow("123\\n, acceptable")   << QValidator::Acceptable   << "123\n" << 1;

    QTest::newRow("\\n, invalid")      << QValidator::Invalid      << "\n" << 0;
    QTest::newRow("\\n, intermediate") << QValidator::Intermediate << "\n" << 0;
    QTest::newRow("\\n, acceptable")   << QValidator::Acceptable   << "\n" << 1;
}

/*!
 * Verify more complicated scenario:
 *      set preedit string
 *      commit another string with different validator states
 *
 * returnPressed should be emitted when validator returns QValidator::Acceptable
 */
void Ut_MTextEdit::testCommitLineBreakAfterPreedit()
{
    QFETCH(QValidator::State, validatorState);
    QFETCH(QString, commitString);
    QFETCH(int, expectedSignals);

    delete m_subject;
    m_subject = new MTextEdit(MTextEditModel::SingleLine, "");

    SimpleValidator validator;
    validator.state = QValidator::Acceptable;
    m_subject->setValidator(&validator);

    QSignalSpy returnPressedSpy(m_subject, SIGNAL(returnPressed()));
    QVERIFY(returnPressedSpy.isValid());

    QInputMethodEvent preeditEvent("123\n", QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&preeditEvent);
    QCOMPARE(returnPressedSpy.count(), 0);

    validator.state = validatorState;

    QInputMethodEvent commitEvent("", QList<QInputMethodEvent::Attribute>());
    commitEvent.setCommitString(commitString);
    m_subject->inputMethodEvent(&commitEvent);
    QCOMPARE(returnPressedSpy.count(), expectedSignals);
}

void Ut_MTextEdit::testArrowKeys()
{
    QSignalSpy copyAvailableSpy(m_subject, SIGNAL(copyAvailable(bool)));
    QVERIFY(copyAvailableSpy.isValid());

    QSignalSpy selectionChangedSpy(m_subject, SIGNAL(selectionChanged()));
    QVERIFY(selectionChangedSpy.isValid());

    QString line(QString(1000, '1'));
    QKeyEvent right(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, QChar());
    QKeyEvent left (QEvent::KeyPress, Qt::Key_Left , Qt::NoModifier, QChar());
    QKeyEvent up   (QEvent::KeyPress, Qt::Key_Up   , Qt::NoModifier, QChar());
    QKeyEvent down (QEvent::KeyPress, Qt::Key_Down , Qt::NoModifier, QChar());

    m_subject->keyPressEvent(&right);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->keyPressEvent(&left);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->keyPressEvent(&up);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->keyPressEvent(&down);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->setText(line);
    m_subject->setCursorPosition(0);

    m_subject->keyPressEvent(&right);
    QCOMPARE(m_subject->cursorPosition(), 1);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->keyPressEvent(&left);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);
}

void Ut_MTextEdit::testSelectByArrowKeys()
{
    QSignalSpy copyAvailableSpy(m_subject, SIGNAL(copyAvailable(bool)));
    QVERIFY(copyAvailableSpy.isValid());

    QSignalSpy selectionChangedSpy(m_subject, SIGNAL(selectionChanged()));
    QVERIFY(selectionChangedSpy.isValid());

    QString line(QString("123 ") + QString(1000, '4'));
    QKeyEvent right(QEvent::KeyPress, Qt::Key_Right, Qt::ShiftModifier, QChar());
    QKeyEvent left (QEvent::KeyPress, Qt::Key_Left , Qt::ShiftModifier, QChar());
    QKeyEvent up   (QEvent::KeyPress, Qt::Key_Up   , Qt::ShiftModifier, QChar());
    QKeyEvent down (QEvent::KeyPress, Qt::Key_Down , Qt::ShiftModifier, QChar());

    m_subject->keyPressEvent(&right);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->keyPressEvent(&left);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->keyPressEvent(&up);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->keyPressEvent(&down);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);

    m_subject->setText(line);
    m_subject->setCursorPosition(1);

    m_subject->keyPressEvent(&right);
    QCOMPARE(m_subject->cursorPosition(), 2);
    QCOMPARE(m_subject->selectedText(), QString("2"));
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == true);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    m_subject->keyPressEvent(&left);
    QCOMPARE(m_subject->cursorPosition(), 1);
    QCOMPARE(m_subject->selectedText(), QString());
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == false);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    m_subject->keyPressEvent(&left);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QCOMPARE(m_subject->selectedText(), QString("1"));
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == true);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    m_subject->setCursorPosition(0);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    m_subject->keyPressEvent(&down);
    QVERIFY(m_subject->cursorPosition() > 0);
    QVERIFY(!m_subject->selectedText().isEmpty());
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == true);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    m_subject->keyPressEvent(&up);
    QVERIFY(m_subject->cursorPosition() == 0);
    QVERIFY(m_subject->selectedText().isEmpty());
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == false);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    //disable flag Qt::TextSelectableByKeyboard
    m_subject->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextEditable);

    m_subject->keyPressEvent(&right);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);
}

void Ut_MTextEdit::testAutoSipEnabled()
{
    setupSipEnv();
    dismissSip(Qt::OtherFocusReason);
    QVERIFY(!m_sic->wouldSipBecomeVisible());

    requestSip(Qt::MouseFocusReason);
    QVERIFY(m_sic->wouldSipBecomeVisible());

    dismissSip(Qt::MouseFocusReason);
    requestSip(Qt::OtherFocusReason);
    QVERIFY(m_sic->wouldSipBecomeVisible());
}

void Ut_MTextEdit::testAutoSipDisabled()
{
    setupSipEnv();
    dismissSip(Qt::OtherFocusReason);
    QVERIFY(!m_sic->wouldSipBecomeVisible());

    m_subject->setAutoSipEnabled(false);
    requestSip(Qt::MouseFocusReason);
    QVERIFY(!m_sic->wouldSipBecomeVisible());

    requestSip(Qt::OtherFocusReason);
    QVERIFY(!m_sic->wouldSipBecomeVisible());
}

void Ut_MTextEdit::testInsertMultiLineText_data()
{
    QTest::addColumn<MTextEditModel::LineMode>("lineMode");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("expectedText");
    QTest::addColumn<QString>("expectedText2");

    const QString sampleText(QString("1") + QChar('\n') + "2" + QChar(0x2028) +"3" + QChar(0x2029));

    QTest::newRow("single line entry") << MTextEditModel::SingleLine << sampleText << QString("1 2 3 ")    << QString("12 3 ");
    QTest::newRow("multi line entry")  << MTextEditModel::MultiLine  << sampleText << QString("1\n2\n3\n") << QString("1\n2\n3\n");
}

void Ut_MTextEdit::testInsertMultiLineText()
{
    QFETCH(MTextEditModel::LineMode, lineMode);
    QFETCH(QString, text);
    QFETCH(QString, expectedText);
    QFETCH(QString, expectedText2);

    delete m_subject;
    m_subject = new MTextEdit(lineMode);

    QClipboard *clipboard = QApplication::clipboard();
    QVERIFY(clipboard != 0);

    clipboard->setText(text);
    m_subject->paste();
    QCOMPARE(m_subject->text(), expectedText);

    m_subject->clear();
    QInputMethodEvent preeditEvent(text, QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&preeditEvent);
    QCOMPARE(m_subject->text(), expectedText2);

    m_subject->clear();
    QInputMethodEvent commitEvent("", QList<QInputMethodEvent::Attribute>());
    commitEvent.setCommitString(text);
    m_subject->inputMethodEvent(&preeditEvent);
    QCOMPARE(m_subject->text(), expectedText2);

    m_subject->clear();
    m_subject->insert(text);
    QCOMPARE(m_subject->text(), expectedText);
}

void Ut_MTextEdit::setupSipEnv()
{
    m_subject->setFlag(QGraphicsItem::ItemAcceptsInputMethod);

    // Guard manually against self-assignment - see QTBUG-10780:
    if (m_sic != qApp->inputContext()) {
        qApp->setInputContext(m_sic);
    }

    // Need this setup to assign a valid scene manager to m_subject:
    MApplicationPage *page = new MApplicationPage;
    page->setCentralWidget(m_subject);
    m_appWindow->sceneManager()->appearSceneWindowNow(page);
}

void Ut_MTextEdit::requestSip(Qt::FocusReason fr)
{
    QFocusEvent focusIn(QEvent::FocusIn, fr);
    m_subject->focusInEvent(&focusIn);

    // Makes test fragile, as this behaviour (SIP request on mouse release) can easily change:
    if (fr == Qt::MouseFocusReason) {
        QGraphicsSceneMouseEvent mouseRelease(QEvent::GraphicsSceneMouseRelease);
        m_subject->mouseReleaseEvent(&mouseRelease);
    }
}

void Ut_MTextEdit::dismissSip(Qt::FocusReason fr)
{
    QFocusEvent focusOut(QEvent::FocusOut, fr);
    m_subject->focusOutEvent(&focusOut);
}

QTEST_APPLESS_MAIN(Ut_MTextEdit);
