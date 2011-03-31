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

#include "ut_mtextedit.h"

#include <QTest>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
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
#include <MPreeditInjectionEvent>
#include <stdlib.h>

#include <mtextedit.h>
#include <mtexteditview.h>
#include <mwidgetmodel.h>

#include <mscenemanager.h>
#include <mscenewindow.h>
#include <mscene.h>
#include <mnavigationbar.h>
#include <mhomebuttonpanel.h>
#include <mapplicationpage.h>
#include <mdialog.h>

#include "mtextedit_p.h"
#include "utils.h"

Q_DECLARE_METATYPE(Qt::InputMethodHints);
Q_DECLARE_METATYPE(QValidator::State);
Q_DECLARE_METATYPE(MTextEditModel::EchoMode);
Q_DECLARE_METATYPE(MTextEditModel::EditMode);
Q_DECLARE_METATYPE(Ut_MTextEdit::PositionedTextEditList);
Q_DECLARE_METATYPE(Ut_MTextEdit::KeyList);
Q_DECLARE_METATYPE(Qt::KeyboardModifiers);
Q_DECLARE_METATYPE(Qt::FocusReason);
Q_DECLARE_METATYPE(Qt::Key)
Q_DECLARE_METATYPE(Qt::TextInteractionFlag);

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
          updateCallCount(0),
          resetCallCount(0),
          selectionAvailableAtLastUpdate(false),
          edit(0),
          m_visible(false)
    {}

    bool isVisible()
    {
        return m_visible;
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
    {
        ++resetCallCount;
    }

    bool isComposing() const
    {
        return false;
    }

    bool filterEvent(const QEvent *ev)
    {
        switch (ev->type())
        {

        case QEvent::RequestSoftwareInputPanel:
            m_visible = true;
            break;

        case QEvent::CloseSoftwareInputPanel:
            m_visible = false;
            break;

        default:
            break;
        }

        if (ev->type() == MPreeditInjectionEvent::eventNumber()) {
            return true;
        }

        return false;
    }

    bool event(QEvent *event)
    {
        if (event->type() == MPreeditInjectionEvent::eventNumber()) {
            event->accept();
            return true;
        }
        return QInputContext::event(event);
    }

    void update()
    {
        ++updateCallCount;
        if (edit) {
            selectedTextAtLastUpdate = edit->selectedText();
        }
        selectionAvailableAtLastUpdate = !selectedTextAtLastUpdate.isEmpty();
    }

    int updateCallCount;
    int resetCallCount;
    bool selectionAvailableAtLastUpdate;
    QString selectedTextAtLastUpdate;
    MTextEdit *edit;

private:
    bool m_visible;
};

class AlwaysOnDisplayWindow
    : public MWindow
{
public:
    explicit AlwaysOnDisplayWindow(MSceneManager *sm, QWidget *parent = 0)
        : MWindow(sm, parent)
        , m_page(new MApplicationPage)
    {
        setVisible(true);
        QTest::qWaitForWindowShown(this);
        QVERIFY(testAttribute(Qt::WA_Mapped));

        QSignalSpy spy(m_page, SIGNAL(appeared()));
        m_page->setCentralWidget(new QGraphicsWidget);
        m_page->appear(this);
        Ut_Utils::waitForSignal(spy);
        QApplication::setActiveWindow(this);
    }

    QGraphicsWidget* box()
    {
        return m_page->centralWidget();
    }

private:
    QPointer<MApplicationPage> m_page;
};

class AutoActivatedScene
    : public MScene
{
public:
    explicit AutoActivatedScene(QObject *parent = 0)
        : MScene(parent)
        , m(new MSceneManager(this))

    {
        setFocus(Qt::OtherFocusReason);
    }

    void adjustTextEdit(MTextEdit *edit, const QPoint &pos)
    {
        edit->setParentItem(m.box());
        edit->setVisible(true);
        edit->setPos(pos);
        edit->setZValue(1);
    }

    AlwaysOnDisplayWindow *window()
    {
        return &m;
    }

private:
    AlwaysOnDisplayWindow m;
};

class AccurateRect
    : public QRect
{

public:
    AccurateRect(int x, int y, int w, int h)
        : QRect(x, y, w, h)
    {}

    QPoint centerTop() const {return QPoint(width() * 0.5, 0);}
    QPoint centerBottom() const {return QPoint(width() * 0.5, height());}
    QPoint centerLeft() const {return QPoint(0, height() * 0.5);}
    QPoint centerRight() const {return QPoint(width(), height() * 0.5);}

};

/*!
 * Called once before the first testcase is run.
 */
void Ut_MTextEdit::initTestCase()
{
    static int dummyArgc = 2;
    static char *dummyArgv[2] = { (char *) "./ut_mtextedit",
                                  (char *) "-software"};

    // prevent loading of minputcontext because we don't need it
    MApplication::setLoadMInputContext(false);
    unsetenv("QT_IM_MODULE");

    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;
    m_sic = 0;

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    // contains valid strings which should be stored by widget as they are
    validStrings << "normal" << "normal with spaces" << "specials: !@#$%^&*()_+=-[]{}"
                 << "A string that is probably too long to fit to one line and needs to be wrapped or scrolled.";
    // TODO: Invent more test strings

    qRegisterMetaType<Qt::FocusReason>();
}


/*!
 * Called once after the last testcase has finished.
 */
void Ut_MTextEdit::cleanupTestCase()
{
}


/*!
 * Called before each testcase.
 */
void Ut_MTextEdit::init()
{
    m_subject.reset(new MTextEdit(MTextEditModel::MultiLine, ""));
    qApp->setInputContext(m_sic = new SimpleInputContext);
    m_sic->edit = m_subject.get();
}


/*!
 * Called after each testcase.
 */
void Ut_MTextEdit::cleanup()
{
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
    AutoActivatedScene sc;
    MTextEdit *subject(new MTextEdit(MTextEditModel::MultiLine, ""));
    subject->setParentItem(sc.window()->box());
    subject->setFocus();

    int resetCallCount(0);
    QTextDocument *document = subject->document();
    QSignalSpy mySpy(document, SIGNAL(contentsChanged()));
    QSignalSpy mySpy2(subject, SIGNAL(textChanged()));

    for (int i = 0; i < validStrings.size(); i++) {
        QString setText = validStrings.at(i);
        subject->setText(setText);
        QCOMPARE(m_sic->resetCallCount, ++resetCallCount);
        QString getText = subject->text();

        // Check that the signals were sent and texts match
        QCOMPARE(mySpy.count(), (i + 1) * 2); //one to clear old second to set the new
        QCOMPARE(mySpy2.count(), (i + 1));
        QCOMPARE(getText, setText);
    }

    // test with phone number content type so validator gets tested too.
    int currentSpyCount = mySpy2.count();
    subject->setContentType(M::PhoneNumberContentType);
    subject->setText("+358-"); // partial phone number
    QCOMPARE(mySpy2.count(), currentSpyCount + 1);
    QCOMPARE(m_sic->resetCallCount, ++resetCallCount);

    currentSpyCount = mySpy2.count();
    subject->setText(""); // empty string should be always ok
    QCOMPARE(mySpy2.count(), currentSpyCount + 1);
    QCOMPARE(m_sic->resetCallCount, ++resetCallCount);

    // Test line break filtering with a single line text edit
    const QString input(QString("1\n2") + QChar(0x2028) +"3" + QChar(0x2029) + "4\r5\r\n");
    MTextEdit singleLineSubject(MTextEditModel::SingleLine);
    singleLineSubject.setText(input);
    QCOMPARE(singleLineSubject.text(), QString("1 2 3 4 5 "));
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
    QSignalSpy mySpy2(m_subject.get(), SIGNAL(textChanged()));
    QSignalSpy copyAvailableSpy(m_subject.get(), SIGNAL(copyAvailable(bool)));

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

/*!
 * Test that Ctrl + Backspace deletes start of word.
 */
void Ut_MTextEdit::testDeleteStartOfWordKeyPressEvent()
{
    // test Ctrl + Backspace
    m_subject->setText("abcd efgh");        
    QKeyEvent event(QKeyEvent::KeyPress, Qt::Key_Backspace, Qt::ControlModifier);
    m_subject->keyPressEvent(&event);
    QCOMPARE(m_subject->text(), QString("abcd "));

    // test that characters are not deleted when cursor is in the beginning of text
    m_subject->setCursorPosition(0);
    m_subject->keyPressEvent(&event);
    QCOMPARE(m_subject->text(), QString("abcd "));

    // test that deletion works when cursor is in middle of a word
    // set cursor position in middle of a word and change character format
    m_subject->setCursorPosition(2);
    QTextCursor *cursor = m_subject->model()->cursor();
    QVERIFY( cursor != 0);
    QTextCharFormat format(cursor->charFormat());
    bool fontKernering = format.fontKerning();
    fontKernering = !fontKernering;
    format.setFontKerning(fontKernering);
    cursor->setCharFormat(format);

    m_subject->keyPressEvent(&event);

    // verify that beginning of word was deleted and character format was not changed
    QCOMPARE(m_subject->text(), QString("cd "));
    format = cursor->charFormat();
    QVERIFY(format.fontKerning() == fontKernering);
}

/*!
 * Test that Ctrl + Delete deletes start of word.
 */
void Ut_MTextEdit::testDeleteEndOfWordKeyPressEvent()
{
    // test Ctrl + Delete
    m_subject->setText("abcd efgh");
    m_subject->setCursorPosition(0);
    QKeyEvent event(QKeyEvent::KeyPress, Qt::Key_Delete, Qt::ControlModifier);
    m_subject->keyPressEvent(&event);
    QCOMPARE(m_subject->text(), QString("efgh"));

    // test that characters are not deleted when cursor is in the end of text
    m_subject->setCursorPosition(4);
    m_subject->keyPressEvent(&event);
    QCOMPARE(m_subject->text(), QString("efgh"));

    // test that deletion works when cursor is in middle of a word
    m_subject->setCursorPosition(2);
    m_subject->keyPressEvent(&event);
    QCOMPARE(m_subject->text(), QString("ef"));
}

/*!
 * Test Ctrl + Backspace when text has been selected.
 */
void Ut_MTextEdit::testDeleteStartOfWordWithSelection()
{
    m_subject->setText("abcd efgh");

    QKeyEvent right(QEvent::KeyPress, Qt::Key_Right, Qt::ShiftModifier, QChar());
    QKeyEvent left (QEvent::KeyPress, Qt::Key_Left , Qt::ShiftModifier, QChar());

    // Select text from beginning.
    m_subject->setCursorPosition(0);
    m_subject->keyPressEvent(&right);
    m_subject->keyPressEvent(&right);

    // test Ctrl + Backspace
    QKeyEvent deleteStartOfWord(QKeyEvent::KeyPress, Qt::Key_Backspace, Qt::ControlModifier);
    m_subject->keyPressEvent(&deleteStartOfWord);
    QCOMPARE(m_subject->text(), QString("cd efgh"));

    // Select text from end.
    m_subject->setCursorPosition(7);
    m_subject->keyPressEvent(&left);
    m_subject->keyPressEvent(&left);

    // test Ctrl + Backspace
    m_subject->keyPressEvent(&deleteStartOfWord);
    QCOMPARE(m_subject->text(), QString("cd ef"));
}

/*!
 * Test Ctrl + Delete when text has been selected.
 */
void Ut_MTextEdit::testDeleteEndOfWordWithSelection()
{
    m_subject->setText("abcd efgh");

    QKeyEvent right(QEvent::KeyPress, Qt::Key_Right, Qt::ShiftModifier, QChar());
    QKeyEvent left (QEvent::KeyPress, Qt::Key_Left , Qt::ShiftModifier, QChar());

    // Select text from beginning.
    m_subject->setCursorPosition(0);
    m_subject->keyPressEvent(&right);
    m_subject->keyPressEvent(&right);

    // test Ctrl + Delete
    QKeyEvent deleteEndOfWord(QKeyEvent::KeyPress, Qt::Key_Delete, Qt::ControlModifier);
    m_subject->keyPressEvent(&deleteEndOfWord);
    QCOMPARE(m_subject->text(), QString("cd efgh"));

    // Select text from end.
    m_subject->setCursorPosition(7);
    m_subject->keyPressEvent(&left);
    m_subject->keyPressEvent(&left);

    // test Ctrl + Delete
    m_subject->keyPressEvent(&deleteEndOfWord);
    QCOMPARE(m_subject->text(), QString("cd ef"));
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
    QSignalSpy spyGainedFocus(m_subject.get(), SIGNAL(gainedFocus(Qt::FocusReason)));
    QSignalSpy spyLostFocus(m_subject.get(), SIGNAL(lostFocus(Qt::FocusReason)));

    // Perform test
    QFocusEvent focusEvent(QEvent::FocusOut);
    m_subject->focusOutEvent(&focusEvent);

    // Check that the signals were emitted and their parameters were correct
    QCOMPARE(spyGainedFocus.count(), 0);
    QCOMPARE(spyLostFocus.count(), 1);
}

// Test the basic three steps of input method event processing as described at
// <http://doc.trolltech.com/4.7/qinputmethodevent.html>:
// 1. remove selected text, if any
// 2. handle replacement (which may just reposition the cursor), insert commit string
// 3. keep or replace pre-edit
//
// Boundary cases and pre-edit cursor are ignored here.
void Ut_MTextEdit::testInputMethodEventSteps()
{
    // Let's have a selection
    const int selPos(1);
    const int selLength(2);
    QList<QInputMethodEvent::Attribute> select1_2;
    select1_2 << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, selPos, selLength, QVariant());
    QInputMethodEvent testSelect1_2("", select1_2);
    testSelect1_2.setCommitString("123456789");
    m_subject->inputMethodEvent(&testSelect1_2);
    QCOMPARE(m_subject->selectedText(), QString("23"));
    QCOMPARE(m_subject->text(), testSelect1_2.commitString());
    QCOMPARE(static_cast<int>(m_subject->mode()), static_cast<int>(MTextEditModel::EditModeSelect));
    QCOMPARE(m_subject->cursorPosition(), selPos + selLength);

    // Let's remove the selection, replace last two characters with others and add a
    // pre-edit to the beginning, all with one event!  Per Qt documentation we could even
    // set a new selection on top of that but MTextEdit does not support that as pre-edit
    // is implemented using a selection.
    QList<QInputMethodEvent::Attribute> moveTo0;
    // Selection start seems to be relative to the surrounding text start
    // instead of the cursor position (Qt documentation is a bit vague about this).
    moveTo0 << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, 0, 0, QVariant());
    QInputMethodEvent allInOne("ab", moveTo0);
    allInOne.setCommitString("cd", testSelect1_2.commitString().length() - selLength - 2 - /* cursor */ 1, 2);
    m_subject->inputMethodEvent(&allInOne);
    QCOMPARE(m_subject->selectedText(), QString());
    QCOMPARE(m_subject->text(), QString("ab14567cd"));
    QCOMPARE(static_cast<int>(m_subject->mode()), static_cast<int>(MTextEditModel::EditModeActive));
    QCOMPARE(m_subject->cursorPosition(), 2);

    // Let's replace the pre-edit with a new one but have a commit string as well.
    const QList<QInputMethodEvent::Attribute> noAttributes;
    QInputMethodEvent replace("ef", noAttributes);
    replace.setCommitString("kala", 1, 0);
    m_subject->inputMethodEvent(&replace);
    QCOMPARE(m_subject->selectedText(), QString());
    QCOMPARE(m_subject->text(), QString("ef1kala4567cd"));
    QCOMPARE(static_cast<int>(m_subject->mode()), static_cast<int>(MTextEditModel::EditModeActive));
    QCOMPARE(m_subject->cursorPosition(), 2);

    m_subject->clear();

    // Nothing but pre-edit string; replace it with another (in the same place)
    QCOMPARE(m_subject->text(), QString());
    QInputMethodEvent justPreedit("ab", noAttributes);
    m_subject->inputMethodEvent(&justPreedit);
    QInputMethodEvent justAnotherPreedit("cd", noAttributes);
    m_subject->inputMethodEvent(&justAnotherPreedit);
    QCOMPARE(m_subject->text(), QString("cd"));
    QCOMPARE(static_cast<int>(m_subject->mode()), static_cast<int>(MTextEditModel::EditModeActive));
    QCOMPARE(m_subject->cursorPosition(), 2);

    m_subject->clear();

    // Another pre-edit replacement case, #1
    QList<QInputMethodEvent::Attribute> moveTo4;
    moveTo4 << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, 4, 0, QVariant());
    QInputMethodEvent commitAndPreedit("kala", moveTo4);
    commitAndPreedit.setCommitString("123456789");
    m_subject->inputMethodEvent(&commitAndPreedit);

    {
        QInputMethodEvent replace2("lahna", noAttributes);
        replace2.setCommitString("ho");
        m_subject->inputMethodEvent(&replace2);
        QCOMPARE(m_subject->text(), QString("1234holahna56789"));
    }
    {
        QInputMethodEvent replace2("lohi", noAttributes);
        replace2.setCommitString("ah", -1);
        m_subject->inputMethodEvent(&replace2);
        QCOMPARE(m_subject->text(), QString("1234haholohi56789"));
    }
    {
        QInputMethodEvent replace2("kuha", noAttributes);
        replace2.setCommitString("gg", -1, 1);
        m_subject->inputMethodEvent(&replace2);
        QCOMPARE(m_subject->text(), QString("1234hahggkuha56789"));
    }
    {
        QInputMethodEvent replace2("made", noAttributes);
        replace2.setCommitString("rr", -2, 3);
        m_subject->inputMethodEvent(&replace2);
        QCOMPARE(m_subject->text(), QString("1234hahrrmade6789"));
    }
    {
        QInputMethodEvent replace2("turska", noAttributes);
        replace2.setCommitString("", 1, 1);
        m_subject->inputMethodEvent(&replace2);
        QCOMPARE(m_subject->text(), QString("1234hahrrturska689"));
    }
    {
        QInputMethodEvent replace2("silakka", noAttributes);
        replace2.setCommitString("", -5, 4);
        m_subject->inputMethodEvent(&replace2);
        QCOMPARE(m_subject->text(), QString("1234rsilakka689"));
    }
    {
        QInputMethodEvent replace2("ahven", noAttributes);
        replace2.setCommitString("", -10, 4);
        m_subject->inputMethodEvent(&replace2);
        QCOMPARE(m_subject->text(), QString("rahven689"));
    }
}

/*!
 * Test moving to pre-editing mode.
 */
void Ut_MTextEdit::testInputMethodEvent()
{
    QString testString2 = testString + '2';
    QSignalSpy spy(m_subject.get(), SIGNAL(textChanged()));
    QVERIFY(spy.isValid());

    QInputMethodEvent event(testString, QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&event);

    QCOMPARE(m_subject->text(), testString);
    QCOMPARE(spy.count(), 1);
    spy.clear();

    // Confirm that widget moved to pre-editing mode
    QVERIFY(m_subject->mode() == MTextEditModel::EditModeActive);

    QInputMethodEvent preeditEvent(testString2, QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&preeditEvent);

    QInputMethodEvent commitEvent;
    commitEvent.setCommitString(testString2);
    m_subject->inputMethodEvent(&commitEvent);

    QCOMPARE(m_subject->text(), testString2);
    QCOMPARE(spy.count(), 1); // optimization with preedit == commit
    spy.clear();

    m_subject->setText(testString);
    int start = 0;
    int length = 1;
    QList<QInputMethodEvent::Attribute> attributes;
    attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, start,
                                               length, QVariant());
    QInputMethodEvent selectionEvent("", attributes);
    m_subject->inputMethodEvent(&selectionEvent);
    QCOMPARE(m_subject->selectedText(), testString.mid(0, 1));
    
    // test that if preedit is signaling textChanged on full text entry, also commit does that 
    // when removing the character.
    m_subject->clear();
    m_subject->setMaxLength(0);
    spy.clear();
    m_subject->inputMethodEvent(&preeditEvent);
    int countAfterPreedit = spy.count();
    spy.clear();
    m_subject->inputMethodEvent(&commitEvent);
    int countAfterCommit = spy.count();
    QCOMPARE(countAfterPreedit, countAfterCommit);

    // check that doing preedit on full field doesn't actually signal anything
    m_subject->clear();
    m_subject->setMaxLength(1);
    m_subject->setText("a");
    spy.clear();
    m_subject->inputMethodEvent(&preeditEvent);
    QCOMPARE(spy.count(), 0);
}

void Ut_MTextEdit::testText()
{
    // test that even though input method event might make content more than max length,
    // text() still honors that by limiting preedit in resulting string
    m_subject->setMaxLength(3);
    m_subject->setText("bb");
    m_subject->setCursorPosition(0);
    
    QInputMethodEvent event(testString, QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&event);
    QCOMPARE(m_subject->text(), QString(testString.at(0)) + "bb");
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
    constraintTest(&singleLine, testInput, "1234*+-#()");

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

    QTest::newRow("sl: too short") << MTextEditModel::SingleLine << -5 << longText;
    QTest::newRow("sl: zero")      << MTextEditModel::SingleLine <<  0 << longText;
    QTest::newRow("sl: short")     << MTextEditModel::SingleLine <<  5 << longText;
    QTest::newRow("sl: long")      << MTextEditModel::SingleLine << (longText.length() - 5) << longText;
    QTest::newRow("sl: too long")  << MTextEditModel::SingleLine << (longText.length() + 5) << longText;

    QTest::newRow("ml: too short") << MTextEditModel::MultiLine << -5 << longText;
    QTest::newRow("ml: zero")      << MTextEditModel::MultiLine <<  0 << longText;
    QTest::newRow("ml: short")     << MTextEditModel::MultiLine <<  5 << longText;
    QTest::newRow("ml: long")      << MTextEditModel::MultiLine << (longText.length() - 5) << longText;
    QTest::newRow("ml: too long")  << MTextEditModel::MultiLine << (longText.length() + 5) << longText;
}

void Ut_MTextEdit::testMaxLength()
{
    QFETCH(MTextEditModel::LineMode, lineMode);
    QFETCH(int, maxLength);
    QFETCH(QString, text);

    MTextEdit subject(lineMode);
    const int initialMaxLength = subject.maxLength();
    // If maxLength is negative, it should have the same effect as zero.
    const int resultMaxLength(qMax(0, maxLength));

    for (int i = 0; i < 3; ++i) {
        qDebug() << "case nr." << i << ".................";
        subject.clear();
        subject.setMaxLength(initialMaxLength);

        QString expectedText = text;

        // Test in three different ways:
        if (i == 0) {
            // Set limitation afterwards.
            subject.setText(text);
            subject.setMaxLength(maxLength);
        } else if (i == 1) {
            // Set limitation prior setting text.
            subject.setMaxLength(maxLength);
            subject.setText(text);
        } else {
            // Check maxLength with insert().
            subject.setMaxLength(maxLength);
            subject.setText(testString); // Set some initial diibadaaba text.
            // Special case where text is inserted at the end.
            subject.insert(text);

            expectedText.prepend(testString);
        }

        // Mimic the filtering MTextEdit does for single-line edits.
        if (lineMode == MTextEditModel::SingleLine) {
            expectedText.replace(QChar('\n'), QChar(' '));
        }

        // Check that correct value was stored.
        QCOMPARE(subject.maxLength(), resultMaxLength);

        // This is what should happen to the text.
        expectedText.truncate(resultMaxLength);

        QCOMPARE(subject.text(), expectedText);

        // Length must always be equal or less than maxLength.
        QVERIFY(subject.text().length() <= resultMaxLength);
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
    subject->setText("I\nhave\nmore\rand\r\nmore columns");
    QVERIFY(subject->lineMode() == MTextEditModel::SingleLine);
    QCOMPARE(subject->text(), QString("I have more and more columns"));

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

    QSignalSpy copyAvailableSpy(m_subject.get(), SIGNAL(copyAvailable(bool)));
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

        QSignalSpy updatedSpy(m_subject.get(), SIGNAL(selectionChanged()));
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

void Ut_MTextEdit::testSelectionVsFocus()
{
    AutoActivatedScene sc;
    MTextEdit *subject(new MTextEdit(MTextEditModel::SingleLine, "a bcd e"));
    subject->setParentItem(sc.window()->box());
    sc.adjustTextEdit(subject, QPoint(0, 0));
    MTextEdit *subject2(new MTextEdit(MTextEditModel::SingleLine, "f"));
    subject2->setParentItem(sc.window()->box());
    sc.adjustTextEdit(subject2, QPoint(0, 100));
    subject->setFocus();

    subject->setSelection(2, 3, true);
    sc.window()->show();
    QTest::qWaitForWindowShown(sc.window());
    QCOMPARE(subject->mode(), MTextEditModel::EditModeSelect);
    QCOMPARE(subject->textCursor().selectionStart(), 2);
    QCOMPARE(subject->textCursor().selectionEnd(), 5);

    // selection must remain after whole window looses focus and gets it back
    // stealing focus ..
    MWindow window2;
    window2.show();
    window2.activateWindow();
    QApplication::syncX();
    QCoreApplication::processEvents();
    QTest::qWaitForWindowShown(&window2);
    QTest::qWait(1000);
    if (sc.window()->isActiveWindow()) {
        QSKIP("Control check \"window is moved to background\" failed", SkipSingle);
    }
    // .. back
    sc.window()->activateWindow();
    QApplication::syncX();
    QCoreApplication::processEvents();
    QTest::qWaitForWindowShown(sc.window());
    if (!sc.window()->isActiveWindow()) {
        QSKIP("Control check \"window is activated\" failed", SkipSingle);
    }
    QCOMPARE(subject->mode(), MTextEditModel::EditModeSelect);
    QCOMPARE(subject->textCursor().selectionStart(), 2);
    QCOMPARE(subject->textCursor().selectionEnd(), 5);

    // usual loss of focus by control must reset selection
    subject2->setFocus();
    QApplication::syncX();
    QCoreApplication::processEvents();
    // .. back
    subject->setFocus();
    QApplication::syncX();
    QCoreApplication::processEvents();
    if (!(subject->mode() != MTextEditModel::EditModeSelect)) {
        QSKIP("Control check \"textedit content is deselected\" failed", SkipSingle);
    }
    QVERIFY(subject->mode() != MTextEditModel::EditModeSelect);
    QCOMPARE(subject->selectionStart(), -1);
}

void Ut_MTextEdit::testDoubleClick_data()
{
    QTest::addColumn<Qt::TextInteractionFlag>("interactionFlag");

    QTest::newRow("selectionEnabled") << Qt::TextSelectableByMouse;
    QTest::newRow("selectionDisabled") << Qt::NoTextInteraction;
}


void Ut_MTextEdit::testDoubleClick()
{
    QFETCH(Qt::TextInteractionFlag, interactionFlag);
    const bool enabled(interactionFlag & Qt::TextSelectableByMouse);

    m_subject->setTextInteractionFlags(interactionFlag);

    QSignalSpy copyAvailableSpy(m_subject.get(), SIGNAL(copyAvailable(bool)));
    m_subject->setText("xyzzy quux");

    QGraphicsSceneMouseEvent dummyEvent;

    // Double click the first word
    m_subject->handleMousePress(2, &dummyEvent, NULL);
    m_subject->handleMouseRelease(2, &dummyEvent, NULL);
    {
        QInputMethodEvent event("xyzzy", QList<QInputMethodEvent::Attribute>());
        m_subject->inputMethodEvent(&event);
    }
    m_subject->handleMousePress(3, &dummyEvent, NULL);
    m_subject->handleMouseRelease(3, &dummyEvent, NULL);

    QCOMPARE(copyAvailableSpy.count(), enabled ? 1 : 0);
    if (enabled) {
        QCOMPARE(copyAvailableSpy.first().count(), 1);
        QVERIFY(copyAvailableSpy.first().first().toBool());
    }
    copyAvailableSpy.clear();

    // Click the 2nd. word to loose the selection
    m_subject->handleMousePress(8, &dummyEvent, NULL);
    m_subject->handleMouseRelease(8, &dummyEvent, NULL);
    {
        QInputMethodEvent event("quux", QList<QInputMethodEvent::Attribute>());
        m_subject->inputMethodEvent(&event);
    }
    QCOMPARE(copyAvailableSpy.count(), enabled ? 1 : 0);
    if (enabled) {
        QCOMPARE(copyAvailableSpy.first().count(), 1);
        QVERIFY(!copyAvailableSpy.first().first().toBool());
    }
    copyAvailableSpy.clear();

    // Click too slowly -> no selection
    m_subject->handleMousePress(2, &dummyEvent, NULL);
    m_subject->handleMouseRelease(2, &dummyEvent, NULL);
    {
        QInputMethodEvent event("xyzzy", QList<QInputMethodEvent::Attribute>());
        m_subject->inputMethodEvent(&event);
    }
    QTest::qWait(QApplication::doubleClickInterval()*2);
    m_subject->handleMousePress(3, &dummyEvent, NULL);
    m_subject->handleMouseRelease(3, &dummyEvent, NULL);

    QCOMPARE(copyAvailableSpy.count(), 0);

    // Click fast, but different words -> no selection
    m_subject->handleMousePress(8, &dummyEvent, NULL);
    m_subject->handleMouseRelease(8, &dummyEvent, NULL);
    {
        QInputMethodEvent event("quux", QList<QInputMethodEvent::Attribute>());
        m_subject->inputMethodEvent(&event);
    }
    m_subject->handleMousePress(2, &dummyEvent, NULL);
    m_subject->handleMouseRelease(2, &dummyEvent, NULL);

    QCOMPARE(copyAvailableSpy.count(), 0);
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
        MTextEdit subject(MTextEditModel::MultiLine);
        subject.setContentType(contentType);

        model = subject.model();
        QVERIFY(model != 0);
        //Verify default prompt
        QVERIFY(subject.prompt().isEmpty());

        //qDebug() << "Multi line text entry; content type:" << contentType;
        for (int n = 0; n < prompts.count(); ++n) {
            //qDebug() << "Test step #" << n << "prompt:" << prompts.at(n);

            subject.setPrompt(prompts.at(n));
            QVERIFY(subject.prompt() == prompts.at(n));
        }
    }

    //test single line editor
    prompts.removeLast();
    prompts.removeLast();

    foreach(M::TextContentType contentType, contentTypes) {
        MTextEdit subject(MTextEditModel::SingleLine);
        subject.setContentType(contentType);

        model = subject.model();
        QVERIFY(model != 0);
        //Verify default prompt
        QVERIFY(subject.prompt().isEmpty());

        //qDebug() << "Single line text entry; content type:" << contentType;
        for (int n = 0; n < prompts.count(); ++n) {
            //qDebug() << "Test step #" << n << "prompt:" << prompts.at(n);

            subject.setPrompt(prompts.at(n));
            QVERIFY(subject.prompt() == prompts.at(n));
        }
    }
}

void Ut_MTextEdit::testValidator()
{
    QSignalSpy copyAvailableSpy(m_subject.get(), SIGNAL(copyAvailable(bool)));
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


void Ut_MTextEdit::testValidatorSelectionRemoval_data()
{
    QTest::addColumn<int>("selectionRemovingKey");

    QTest::newRow("removing selection with backspace") << static_cast<int>(Qt::Key_Backspace);
    QTest::newRow("removing selection with delete") << static_cast<int>(Qt::Key_Delete);
}

void Ut_MTextEdit::testValidatorSelectionRemoval()
{
    QFETCH(int, selectionRemovingKey);

    m_appWindow->scene()->addItem(m_subject.get());

    QRegExpValidator abValidator(QRegExp("ab"), 0);

    m_subject->setValidator(&abValidator);
    m_subject->setText("ab");
    QCOMPARE(m_subject->text(), QString("ab"));

    // Select "a" and delete it. The result "b" should not be acceptable nor
    // plausible string, thus original content should still remain.
    m_subject->setSelection(0, 1);

    QKeyEvent keyEvent(QEvent::KeyPress, selectionRemovingKey, Qt::NoModifier);
    m_subject->scene()->sendEvent(m_subject.get(), &keyEvent);

    QCOMPARE(m_subject->text(), QString("ab"));
}


void Ut_MTextEdit::testClear()
{
    AutoActivatedScene sc;
    MTextEdit *subject(new MTextEdit(MTextEditModel::MultiLine, ""));
    subject->setParentItem(sc.window()->box());
    subject->setFocus();

    // test that clear() works. first set content and then test that after clear the state has reset
    subject->setText("AsdfAsdfasD sdfdf asdf ");
    subject->setSelection(3, 8, false);
    QCOMPARE(m_sic->resetCallCount, 1);
    subject->clear();
    QCOMPARE(m_sic->resetCallCount, 2);
    QCOMPARE(subject->cursorPosition(), 0);
    QCOMPARE(subject->mode(), MTextEditModel::EditModeBasic);
    QCOMPARE(subject->text(), QString(""));
}


void Ut_MTextEdit::testCursorPositionChanged()
{
    QSignalSpy cursorSpy(m_subject.get(), SIGNAL(cursorPositionChanged()));
    QSignalSpy copyAvailableSpy(m_subject.get(), SIGNAL(copyAvailable(bool)));
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

void Ut_MTextEdit::testAttachToolbar_data()
{
    QTest::addColumn<bool>("isSipRequested");
    QTest::addColumn<int>("expectedCallCount");

    QTest::newRow("focused") << true << 2;
    QTest::newRow("not focused") << false << 0;
}

void Ut_MTextEdit::testAttachToolbar()
{
    QFETCH(bool, isSipRequested);
    QFETCH(int, expectedCallCount);
    AutoActivatedScene sc;
    MTextEdit *subject = createFromSipHandling(&sc, isSipRequested);

    m_sic->updateCallCount = 0;
    subject->attachToolbar("");
    QVERIFY(subject->attachedToolbar().isEmpty());
    QCOMPARE(m_sic->updateCallCount, 0);
    const QString toolbar("testToolbar");
    subject->attachToolbar(toolbar);
    QCOMPARE(subject->attachedToolbar(), toolbar);

    //MTextEdit::detachToolbar is called implicitly,
    //so we should have two calls to QInputContext::update()
    QCOMPARE(m_sic->updateCallCount, expectedCallCount);
}

void Ut_MTextEdit::testDetachToolbar_data()
{
    QTest::addColumn<bool>("isSipRequested");
    QTest::addColumn<int>("expectedCallCount");

    QTest::newRow("focused") << true << 1;
    QTest::newRow("not focused") << false << 0;
}

void Ut_MTextEdit::testDetachToolbar()
{
    QFETCH(bool, isSipRequested);
    QFETCH(int, expectedCallCount);
    AutoActivatedScene sc;
    MTextEdit *subject = createFromSipHandling(&sc, isSipRequested);

    m_sic->updateCallCount = 0;
    subject->detachToolbar();
    QCOMPARE(m_sic->updateCallCount, expectedCallCount);
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
    QTest::addColumn<QString>("returnString");

    QTest::newRow("single line, invalid, \\n")      << MTextEditModel::SingleLine << QValidator::Invalid      << 0 << "\n";
    QTest::newRow("single line, intermediate, \\n") << MTextEditModel::SingleLine << QValidator::Intermediate << 0 << "\n";
    QTest::newRow("single line, acceptable, \\n")   << MTextEditModel::SingleLine << QValidator::Acceptable   << 1 << "\n";
    QTest::newRow("multi line,  invalid, \\n")      << MTextEditModel::MultiLine  << QValidator::Invalid      << 0 << "\n";
    QTest::newRow("multi line,  intermediate, \\n") << MTextEditModel::MultiLine  << QValidator::Intermediate << 0 << "\n";
    QTest::newRow("multi line,  acceptable, \\n")   << MTextEditModel::MultiLine  << QValidator::Acceptable   << 0 << "\n";

    QTest::newRow("single line, invalid, \\r")      << MTextEditModel::SingleLine << QValidator::Invalid      << 0 << "\r";
    QTest::newRow("single line, intermediate, \\r") << MTextEditModel::SingleLine << QValidator::Intermediate << 0 << "\r";
    QTest::newRow("single line, acceptable, \\r")   << MTextEditModel::SingleLine << QValidator::Acceptable   << 1 << "\r";
    QTest::newRow("multi line,  invalid, \\r")      << MTextEditModel::MultiLine  << QValidator::Invalid      << 0 << "\r";
    QTest::newRow("multi line,  intermediate, \\r") << MTextEditModel::MultiLine  << QValidator::Intermediate << 0 << "\r";
    QTest::newRow("multi line,  acceptable, \\r")   << MTextEditModel::MultiLine  << QValidator::Acceptable   << 0 << "\r";
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
    QFETCH(QString, returnString);

    MTextEdit subject(lineMode);

    SimpleValidator validator;
    QKeyEvent event(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier, returnString);
    QSignalSpy returnPressedSpy(&subject, SIGNAL(returnPressed()));

    validator.state = validatorState;
    subject.setValidator(&validator);

    subject.keyPressEvent(&event);
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
    QTest::newRow("single line, preedit, line break \\r") << MTextEditModel::SingleLine << "preedit\r" << "" << M::FreeTextContentType << 0;
    QTest::newRow("single line, commit, line break")  << MTextEditModel::SingleLine << "" << "commit\n" << M::FreeTextContentType << 1;
    QTest::newRow("single line, commit, line break \\r")  << MTextEditModel::SingleLine << "" << "commit\r" << M::FreeTextContentType << 1;
    QTest::newRow("single line, commit, line break only")  << MTextEditModel::SingleLine << "" << "\n" << M::FreeTextContentType << 1;
    QTest::newRow("single line, commit, line break \\r only")  << MTextEditModel::SingleLine << "" << "\r" << M::FreeTextContentType << 1;
    QTest::newRow("single line, commit, multiple line breaks")  << MTextEditModel::SingleLine << "" << "commit\n\n\n" << M::FreeTextContentType << 1;
    QTest::newRow("single line, commit, multiple \\r line breaks")  << MTextEditModel::SingleLine << "" << "commit\r\r\r" << M::FreeTextContentType << 1;
    QTest::newRow("single line, invalid number") << MTextEditModel::SingleLine << "" << "abc\n" << M::NumberContentType << 0;
    QTest::newRow("single line, valid number, line break") << MTextEditModel::SingleLine << "" << "123\n" << M::NumberContentType << 1;
    QTest::newRow("single line, valid number, line break \\r") << MTextEditModel::SingleLine << "" << "123\r" << M::NumberContentType << 1;

    QTest::newRow("multi line, empty") << MTextEditModel::MultiLine << "" << "" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, preedit, no line break") << MTextEditModel::MultiLine << "preedit" << "" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, no line break")  << MTextEditModel::MultiLine << "" << "commit" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, preedit, line break") << MTextEditModel::MultiLine << "preedit\n" << "" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, preedit, line break \\r") << MTextEditModel::MultiLine << "preedit\r" << "" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, line break only")  << MTextEditModel::MultiLine << "" << "\n" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, line break \\r only")  << MTextEditModel::MultiLine << "" << "\r" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, line break")  << MTextEditModel::MultiLine << "" << "commit\n" << M::FreeTextContentType << 0;
    QTest::newRow("multi line, commit, line break \\r")  << MTextEditModel::MultiLine << "" << "commit\r" << M::FreeTextContentType << 0;
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

    MTextEdit subject(lineMode);
    subject.setContentType(contentType);

    QSignalSpy returnPressedSpy(&subject, SIGNAL(returnPressed()));
    QVERIFY(returnPressedSpy.isValid());

    QInputMethodEvent event(preeditString, QList<QInputMethodEvent::Attribute>());
    event.setCommitString(commitString);
    subject.inputMethodEvent(&event);

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

    QTest::newRow("123\\r, invalid")      << QValidator::Invalid      << "123\r" << 0;
    QTest::newRow("123\\r, intermediate") << QValidator::Intermediate << "123\r" << 0;
    QTest::newRow("123\\r, acceptable")   << QValidator::Acceptable   << "123\r" << 1;

    QTest::newRow("\\n, invalid")      << QValidator::Invalid      << "\n" << 0;
    QTest::newRow("\\n, intermediate") << QValidator::Intermediate << "\n" << 0;
    QTest::newRow("\\n, acceptable")   << QValidator::Acceptable   << "\n" << 1;

    QTest::newRow("\\r, invalid")      << QValidator::Invalid      << "\r" << 0;
    QTest::newRow("\\r, intermediate") << QValidator::Intermediate << "\r" << 0;
    QTest::newRow("\\r, acceptable")   << QValidator::Acceptable   << "\r" << 1;
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

    MTextEdit subject;

    SimpleValidator validator;
    validator.state = QValidator::Acceptable;
    subject.setValidator(&validator);

    QSignalSpy returnPressedSpy(&subject, SIGNAL(returnPressed()));
    QVERIFY(returnPressedSpy.isValid());

    QInputMethodEvent preeditEvent("123\n", QList<QInputMethodEvent::Attribute>());
    subject.inputMethodEvent(&preeditEvent);
    QCOMPARE(returnPressedSpy.count(), 0);

    validator.state = validatorState;

    QInputMethodEvent commitEvent("", QList<QInputMethodEvent::Attribute>());
    commitEvent.setCommitString(commitString);
    subject.inputMethodEvent(&commitEvent);
    QCOMPARE(returnPressedSpy.count(), expectedSignals);
}

void Ut_MTextEdit::testArrowKeys_data()
{
    QTest::addColumn<Qt::Key>("key");
    QTest::addColumn<int>("initialCursorPosition");
    QTest::addColumn<int>("finalCursorPosition");
    QTest::addColumn<bool>("haveText");

    // No text
    QTest::newRow("empty, right") << Qt::Key_Right << 0 << 0 << false;
    QTest::newRow("empty, left") << Qt::Key_Left << 0 << 0 << false;
    QTest::newRow("empty, up") << Qt::Key_Up << 0 << 0 << false;
    QTest::newRow("empty, down") << Qt::Key_Down << 0 << 0 << false;
    QTest::newRow("empty, home") << Qt::Key_Home << 0 << 0 << false;
    QTest::newRow("empty, end") << Qt::Key_End << 0 << 0 << false;
    QTest::newRow("empty, pageup") << Qt::Key_PageUp << 0 << 0 << false;
    QTest::newRow("empty, pagedown") << Qt::Key_PageDown << 0 << 0 << false;

    // Text
    QTest::newRow("text, right") << Qt::Key_Right << 0 << 1 << true;
    QTest::newRow("text, right eol") << Qt::Key_Right << 3 << 4 << true;
    QTest::newRow("text, left") << Qt::Key_Left << 1 << 0 << true;
    QTest::newRow("text, left bol") << Qt::Key_Left << 4 << 3 << true;
    QTest::newRow("text, up") << Qt::Key_Up << 5 << 1 << true;
    QTest::newRow("text, down") << Qt::Key_Down << 1 << 5 << true;
    QTest::newRow("text, home") << Qt::Key_Home << 8 << 4 << true;
    QTest::newRow("text, end") << Qt::Key_End << 4 << 8 << true;
    QTest::newRow("text, pageup") << Qt::Key_PageUp << 0xe << 0 << true;
    QTest::newRow("text, pagedown") << Qt::Key_PageDown << 0 << 0xe << true;
}

void Ut_MTextEdit::testArrowKeys()
{
    QFETCH(Qt::Key, key);
    QFETCH(int, initialCursorPosition);
    QFETCH(int, finalCursorPosition);
    QFETCH(bool, haveText);

    MTextEdit subject(MTextEditModel::MultiLine);

    MApplicationPage *page = new MApplicationPage();
    page->setCentralWidget(&subject);
    m_appWindow->sceneManager()->appearSceneWindowNow(page);

    QSignalSpy copyAvailableSpy(&subject, SIGNAL(copyAvailable(bool)));
    QVERIFY(copyAvailableSpy.isValid());

    QSignalSpy selectionChangedSpy(&subject, SIGNAL(selectionChanged()));
    QVERIFY(selectionChangedSpy.isValid());

    QKeyEvent event(QEvent::KeyPress, key, Qt::NoModifier, QChar());

    if (haveText) {
        //               0123 45678 9abcde
        subject.setText("foo\nquux\nxyzzy");
    }

    subject.setCursorPosition(initialCursorPosition);

    subject.keyPressEvent(&event);
    QCOMPARE(subject.cursorPosition(), finalCursorPosition);
    QVERIFY(!subject.hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);
}

void Ut_MTextEdit::testSelectByArrowKeys()
{
    setupSipEnv(m_subject.get());
    QSignalSpy copyAvailableSpy(m_subject.get(), SIGNAL(copyAvailable(bool)));
    QVERIFY(copyAvailableSpy.isValid());

    QSignalSpy selectionChangedSpy(m_subject.get(), SIGNAL(selectionChanged()));
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

    QCOMPARE(m_sic->updateCallCount, 0);

    m_subject->setText(line);
    m_subject->setCursorPosition(1);

    m_sic->updateCallCount = 0;
    m_subject->keyPressEvent(&right);
    QCOMPARE(m_subject->cursorPosition(), 2);
    QCOMPARE(m_subject->selectedText(), QString("2"));
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == true);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();
    QCOMPARE(m_sic->updateCallCount, 1);
    m_sic->updateCallCount = 0;
    QVERIFY(m_sic->selectionAvailableAtLastUpdate);

    m_subject->keyPressEvent(&left);
    QCOMPARE(m_subject->cursorPosition(), 1);
    QCOMPARE(m_subject->selectedText(), QString());
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == false);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();
    QCOMPARE(m_sic->updateCallCount, 1);
    m_sic->updateCallCount = 0;
    QVERIFY(!m_sic->selectionAvailableAtLastUpdate);

    m_subject->keyPressEvent(&left);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QCOMPARE(m_subject->selectedText(), QString("1"));
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == true);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();
    QCOMPARE(m_sic->updateCallCount, 1);
    QVERIFY(m_sic->selectionAvailableAtLastUpdate);

    m_subject->setCursorPosition(0);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    m_sic->updateCallCount = 0;
    m_subject->keyPressEvent(&down);
    QVERIFY(m_subject->cursorPosition() > 0);
    QVERIFY(!m_subject->selectedText().isEmpty());
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == true);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();
    QCOMPARE(m_sic->updateCallCount, 1);
    m_sic->updateCallCount = 0;
    QVERIFY(m_sic->selectionAvailableAtLastUpdate);

    m_subject->keyPressEvent(&up);
    QVERIFY(m_subject->cursorPosition() == 0);
    QVERIFY(m_subject->selectedText().isEmpty());
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == false);
    QCOMPARE(selectionChangedSpy.count(), 1);
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();
    QCOMPARE(m_sic->updateCallCount, 1);
    QVERIFY(!m_sic->selectionAvailableAtLastUpdate);

    //disable flag Qt::TextSelectableByKeyboard
    m_subject->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextEditable);

    m_sic->updateCallCount = 0;
    m_subject->keyPressEvent(&right);
    QCOMPARE(m_subject->cursorPosition(), 0);
    QVERIFY(!m_subject->hasSelectedText());
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);
    QCOMPARE(m_sic->updateCallCount, 0);
    m_sic->updateCallCount = 0;
    QVERIFY(!m_sic->selectionAvailableAtLastUpdate);
}

void Ut_MTextEdit::testSelectAll()
{
    QSignalSpy copyAvailableSpy(m_subject.get(), SIGNAL(copyAvailable(bool)));
    QVERIFY(copyAvailableSpy.isValid());

    QSignalSpy selectionChangedSpy(m_subject.get(), SIGNAL(selectionChanged()));
    QVERIFY(selectionChangedSpy.isValid());

    setupSipEnv(m_subject.get());

    // Case 1, text and pre-edit
    m_subject->setText("fish ");
    // Pre-edit should be committed, yet that shouldn't cause extra
    // QInputContext::update() calls
    QInputMethodEvent preeditEvent("salmon", QList<QInputMethodEvent::Attribute>());
    m_subject->inputMethodEvent(&preeditEvent);

    QKeyEvent selectAll(QEvent::KeyPress, Qt::Key_A, Qt::ControlModifier, QChar('a'));
    m_sic->updateCallCount = 0;
    m_subject->keyPressEvent(&selectAll);
    QCOMPARE(m_sic->updateCallCount, 1);
    QVERIFY(m_sic->selectionAvailableAtLastUpdate);
    QCOMPARE(copyAvailableSpy.count(), 1);
    QCOMPARE(copyAvailableSpy.first().count(), 1);
    QVERIFY(copyAvailableSpy.first().first() == true);
    QCOMPARE(selectionChangedSpy.count(), 1);
    QCOMPARE(m_sic->selectedTextAtLastUpdate, QString("fish salmon"));
    QCOMPARE(m_subject->cursorPosition(), m_sic->selectedTextAtLastUpdate.length());

    // Case 2, no text
    m_subject->setText("");
    copyAvailableSpy.clear();
    selectionChangedSpy.clear();

    m_sic->updateCallCount = 0;
    m_subject->keyPressEvent(&selectAll);
    QCOMPARE(m_sic->updateCallCount, 0);
    QCOMPARE(copyAvailableSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 0);
}

void Ut_MTextEdit::testAutoSipEnabled()
{
    AutoActivatedScene sc;
    MTextEdit *subject = createFromSipHandling(&sc, /* isSipRequested = */ false);

    subject->clearFocus();
    QVERIFY(!m_sic->isVisible());

    requestSip(subject, Qt::MouseFocusReason);
    QVERIFY(m_sic->isVisible());

    subject->clearFocus();
    requestSip(subject, Qt::OtherFocusReason);
    QVERIFY(m_sic->isVisible());
}

void Ut_MTextEdit::testAutoSipDisabled()
{
    AutoActivatedScene sc;
    MTextEdit *subject = createFromSipHandling(&sc, /* isSipRequested = */ false);

    subject->clearFocus();
    QVERIFY(!m_sic->isVisible());

    subject->setAutoSipEnabled(false);
    requestSip(subject, Qt::MouseFocusReason);
    QVERIFY(!m_sic->isVisible());

    requestSip(subject, Qt::OtherFocusReason);
    QVERIFY(!m_sic->isVisible());
}

void Ut_MTextEdit::testCloseSipOnDestruction()
{
    AutoActivatedScene sc;
    MTextEdit *subject = createFromSipHandling(&sc);
    QVERIFY(m_sic->isVisible());

    const bool hasAutoSip = subject->isAutoSipEnabled();
    delete subject;

    if (hasAutoSip) {
        QVERIFY(!m_sic->isVisible());
    } else {
        qWarning() << __PRETTY_FUNCTION__
                   << "MTextEdit does not use autoSip functionality - cannot complete test.";
        QVERIFY(m_sic->isVisible());
    }
}

void Ut_MTextEdit::testIgnoreSipIfNotFocused()
{
    AutoActivatedScene sc;
    MTextEdit *subject = createFromSipHandling(&sc);
    QVERIFY(m_sic->isVisible());
    Q_UNUSED(subject);

    // Now create another text edit, and destroy it:
    MTextEdit *edit = new MTextEdit;
    edit->setParentItem(sc.window()->box());
    QVERIFY(edit->isAutoSipEnabled());
    QVERIFY(!edit->hasFocus());

    delete edit;
    QVERIFY(m_sic->isVisible());
}

void Ut_MTextEdit::testCloseSipOnHide()
{
    AutoActivatedScene sc;
    MTextEdit *subject = createFromSipHandling(&sc);
    QVERIFY(m_sic->isVisible());

    subject->hide();

    if (subject->isAutoSipEnabled()) {
        QVERIFY(!m_sic->isVisible());
    } else {
        qWarning() << __PRETTY_FUNCTION__
                   << "MTextEdit does not use autoSip functionality - cannot complete test.";
        QVERIFY(m_sic->isVisible());
    }
}

void Ut_MTextEdit::testInsertMultiLineText_data()
{
    QTest::addColumn<MTextEditModel::LineMode>("lineMode");
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("outputCommit");
    QTest::addColumn<QString>("outputPreedit");

    const QString sampleText(QString("1") + QChar('\n') + "2" + QChar(0x2028) +"3" + QChar(0x2029));

    QTest::newRow("single line entry") << MTextEditModel::SingleLine << sampleText
                                       << QString("1 2 3 ") << QString("1 2 3")    << QString("1 2 3 ");
    QTest::newRow("multi line entry")  << MTextEditModel::MultiLine  << sampleText
                                       << QString("1\n2\n3\n") << QString("1\n2\n3\n") << QString("1 2 3 ");
}

void Ut_MTextEdit::testInsertMultiLineText()
{
    QFETCH(MTextEditModel::LineMode, lineMode);
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(QString, outputCommit);
    QFETCH(QString, outputPreedit);

    MTextEdit subject(lineMode);

    QClipboard *clipboard = QApplication::clipboard();
    QVERIFY(clipboard != 0);

    clipboard->setText(input);
    subject.paste();
    QCOMPARE(subject.text(), output);

    subject.clear();
    QInputMethodEvent preeditEvent(input, QList<QInputMethodEvent::Attribute>());
    subject.inputMethodEvent(&preeditEvent);
    QCOMPARE(subject.text(), outputPreedit);

    subject.clear();
    QInputMethodEvent commitEvent("", QList<QInputMethodEvent::Attribute>());
    commitEvent.setCommitString(input);
    subject.inputMethodEvent(&commitEvent);
    QCOMPARE(subject.text(), outputCommit);

    subject.clear();
    subject.insert(input);
    QCOMPARE(subject.text(), output);
}

void Ut_MTextEdit::testArrowKeyNavigation_data()
{
    const AccurateRect area(0, 0, 400, 400);

    QTest::addColumn<QString>("subjectText");
    QTest::addColumn<int>("subjectCursorPosition");
    QTest::addColumn<QPoint>("subjectPosition");
    QTest::addColumn<PositionedTextEditList>("targets");
    QTest::addColumn<int>("expectedTarget");
    QTest::addColumn<KeyList>("sequence");
    QTest::addColumn<Qt::KeyboardModifiers>("modifiers");

    // Needed because of fuzzy auto-repeat check in MArrowKeyNavigator:
    QTest::newRow("init") << ""
                          << 0
                          << area.centerTop()
                          << PositionedTextEditList()
                          << -1
                          << (KeyList() << Qt::Key_Space)
                          << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("left0") << ""
                           << 0
                           << area.centerTop()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.topLeft()))
                           << 0
                           << (KeyList() << Qt::Key_Left)
                           << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("left1") << "some test"
                           << 4
                           << area.centerRight()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.bottomLeft()))
                           << 0
                           << (KeyList() << Qt::Key_Up << Qt::Key_Left)
                           << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("right0") << ""
                            << 0
                            << area.centerLeft()
                            << (PositionedTextEditList() << PositionedTextEdit(new MTextEdit, area.centerBottom())
                                                         << PositionedTextEdit(new MTextEdit, area.topRight()))
                            << 1
                            << (KeyList() << Qt::Key_Right)
                            << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("right1") << "a"
                            << 0
                            << area.bottomLeft()
                            << (PositionedTextEditList() << PositionedTextEdit(
                                    new MTextEdit, area.centerRight()))
                            << 0
                            << (KeyList() << Qt::Key_Right << Qt::Key_Up << Qt::Key_Down << Qt::Key_Right)
                            << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("up0") << ""
                         << 0
                         << area.centerBottom()
                         << (PositionedTextEditList() << PositionedTextEdit(new MTextEdit, area.bottomLeft())
                                                      << PositionedTextEdit(new MTextEdit, area.topLeft()))
                         << 1
                         << (KeyList() << Qt::Key_Up)
                         << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("down0") << ""
                           << 0
                           << area.center()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.centerBottom()))
                           << 0
                           << (KeyList() << Qt::Key_Down)
                           << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("down1") << "a"
                           << 0
                           << area.center()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.centerBottom()))
                           << 0
                           << (KeyList() << Qt::Key_Right << Qt::Key_Down)
                           << Qt::KeyboardModifiers(Qt::NoModifier);

    // 9 text edits in a 3x3 grid - arrow key navigation starts in B, ends in E:
    //
    //   o > o   o
    //   ^   v
    //   o   B > o
    //   ^   v   v
    //   o < o   E
    // TODO (maybe): trace the order of focused widgets - it might arrive at E on a different path?
    QTest::newRow("grid0") << ""
                           << 0
                           << area.center()
                           << (PositionedTextEditList() << PositionedTextEdit(new MTextEdit, area.topLeft())
                                                        << PositionedTextEdit(new MTextEdit, area.centerTop())
                                                        << PositionedTextEdit(new MTextEdit, area.topRight())
                                                        << PositionedTextEdit(new MTextEdit, area.centerRight())
                                                        << PositionedTextEdit(new MTextEdit, area.bottomRight())
                                                        << PositionedTextEdit(new MTextEdit, area.centerBottom())
                                                        << PositionedTextEdit(new MTextEdit, area.bottomLeft())
                                                        << PositionedTextEdit(new MTextEdit, area.centerLeft()))
                             << 4
                             << (KeyList() << Qt::Key_Down << Qt::Key_Left << Qt::Key_Up << Qt::Key_Up
                                           << Qt::Key_Right << Qt::Key_Down << Qt::Key_Right << Qt::Key_Down)
                             << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("closest0") << ""
                              << 0
                              << area.centerTop()
                              << (PositionedTextEditList() << PositionedTextEdit(new MTextEdit, area.centerBottom())
                                                           << PositionedTextEdit(new MTextEdit, area.centerLeft())
                                                           << PositionedTextEdit(new MTextEdit, area.center())
                                                           << PositionedTextEdit(new MTextEdit, area.centerRight()))
                              << 2
                              << (KeyList() << Qt::Key_Down)
                              << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("closest1") << ""
                              << 0
                              << area.centerRight()
                              << (PositionedTextEditList() << PositionedTextEdit(new MTextEdit, area.topLeft())
                                                           << PositionedTextEdit(new MTextEdit, area.bottomLeft())
                                                           << PositionedTextEdit(new MTextEdit, area.centerLeft()))
                              << 2
                              << (KeyList() << Qt::Key_Left)
                              << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("none0") << "some text"
                           << 4
                           << area.centerTop()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.topLeft()))
                           << -1
                           << (KeyList() << Qt::Key_Left)
                           << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("none1") << ""
                           << 0
                           << area.center()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.centerRight()))
                           << -1
                           << (KeyList() << Qt::Key_Left)
                           << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("none2") << "some text"
                           << 4
                           << area.center()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.centerBottom()))
                           << -1
                           << (KeyList() << Qt::Key_Right << Qt::Key_Down)
                           << Qt::KeyboardModifiers(Qt::NoModifier);

    QTest::newRow("shift") << ""
                           << 0
                           << area.center()
                           << (PositionedTextEditList() << PositionedTextEdit(
                                   new MTextEdit, area.centerLeft()))
                           << -1
                           << (KeyList() << Qt::Key_Left)
                           << Qt::KeyboardModifiers(Qt::ShiftModifier);

    QTest::newRow("ctrl") << ""
                          << 0
                          << area.center()
                          << (PositionedTextEditList() << PositionedTextEdit(
                                  new MTextEdit, area.centerLeft()))
                          << -1
                          << (KeyList() << Qt::Key_Left)
                          << Qt::KeyboardModifiers(Qt::ControlModifier);

    QTest::newRow("fn") << ""
                        << 0
                        << area.center()
                        << (PositionedTextEditList() << PositionedTextEdit(
                                new MTextEdit, area.centerLeft()))
                        << 0
                        << (KeyList() << Qt::Key_Left)
                        << Qt::KeyboardModifiers(Qt::GroupSwitchModifier);

    QTest::newRow("auto-repeat") << "stop"
                                 << 0
                                 << area.bottomLeft()
                                 << (PositionedTextEditList() << PositionedTextEdit(
                                         new MTextEdit, area.centerRight()))
                                 << -1
                                 << (KeyList() << Qt::Key_Right << Qt::Key_Right << Qt::Key_Right
                                               << Qt::Key_Right << Qt::Key_Right)
                                 << Qt::KeyboardModifiers(Qt::ControlModifier);
}

void Ut_MTextEdit::testArrowKeyNavigation()
{
    QFETCH(QString, subjectText);
    QFETCH(int, subjectCursorPosition);
    QFETCH(QPoint, subjectPosition);
    QFETCH(PositionedTextEditList, targets);
    QFETCH(int, expectedTarget);
    QFETCH(KeyList, sequence);
    QFETCH(Qt::KeyboardModifiers, modifiers);

    // Not using m_subject => clash of ownership ...
    MTextEdit *subject = new MTextEdit;
    MTextEdit *target = (expectedTarget == -1) ? 0
                                               : targets.at(expectedTarget).edit;

    // Heap-allocated because of http://bugreports.qt.nokia.com/browse/QTBUG-14283
    QSignalSpy *subjectSpy = new QSignalSpy(subject, SIGNAL(gainedFocus(Qt::FocusReason)));
    QSignalSpy *targetSpy = target ? new QSignalSpy(target, SIGNAL(gainedFocus(Qt::FocusReason)))
                                   : 0;

    AutoActivatedScene sc;
    sc.adjustTextEdit(subject, subjectPosition);

    QVERIFY(sc.isActive());

    subject->setText(subjectText);
    subject->setCursorPosition(subjectCursorPosition);
    sc.setFocusItem(subject);

    for (PositionedTextEditList::iterator iter = targets.begin();
         iter != targets.end();
         ++iter) {
        PositionedTextEdit target(*iter);
        sc.adjustTextEdit(target.edit, target.pos);
    }

    foreach (Qt::Key key, sequence) {
        MTextEdit *edit = dynamic_cast<MTextEdit *>(sc.focusItem());

        if (edit) {
            QTest::qWait(200);
            QKeyEvent ev(QEvent::KeyPress, key, modifiers);
            edit->keyPressEvent(&ev);
        }
    }

    if (!target) {
        Ut_Utils::waitForSignal(*subjectSpy);
        QVERIFY(subject->hasFocus());
    } else {
        Ut_Utils::waitForSignal(*targetSpy);
        QVERIFY(target->hasFocus());
    }

    delete subjectSpy;
    delete targetSpy;
}

void Ut_MTextEdit::setupSipEnv(MTextEdit *edit)
{
    edit->setFlag(QGraphicsItem::ItemAcceptsInputMethod);
    m_sic->edit = edit;
}

void Ut_MTextEdit::requestSip(MTextEdit *edit, Qt::FocusReason fr)
{
    QSignalSpy spy(edit, SIGNAL(gainedFocus(Qt::FocusReason)));
    edit->setFocus(fr);
    Ut_Utils::waitForSignal(spy);

    // Makes test fragile, as this behaviour (SIP request on mouse release) can easily change:
    if (fr == Qt::MouseFocusReason) {
        QGraphicsSceneMouseEvent mouseRelease(QEvent::GraphicsSceneMouseRelease);
        edit->mouseReleaseEvent(&mouseRelease);
    }
}

MTextEdit *Ut_MTextEdit::createFromSipHandling(AutoActivatedScene *scene, bool isSipRequested)
{
    if (!scene) {
        qCritical() << __PRETTY_FUNCTION__
                    << "Cannot create text edit without scene.";
        return 0;
    }

    MTextEdit *edit = new MTextEdit;
    setupSipEnv(edit);

    edit->setParentItem(scene->window()->box());

    if (isSipRequested) {
        requestSip(edit, Qt::MouseFocusReason);
    }

    return edit;
}

void Ut_MTextEdit::testSetters()
{
    MSetterTester<MWidgetController, MWidgetModel> tester(m_subject.get(),
                                                          &MWidgetController::setModel,
                                                          &MWidgetController::model);
    tester.set(new MTextEditModel, false);
    tester.setDoesNotReparent(new MTextEditModel);
}

QTEST_APPLESS_MAIN(Ut_MTextEdit);
