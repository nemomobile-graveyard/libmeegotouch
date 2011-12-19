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

#include "ut_mtexteditview.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QMetaType>
#include <QSignalSpy>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MScene>
#include <mscenemanager.h>
#include <mscenewindow.h>

#include <QPaintEngine>

#include <mtextedit.h>
#include <mtexteditview.h>

#include "corelib/widgets/mtextedit_p.h"
#include "views/mtexteditview_p.h"
#include "utils.h"

Q_DECLARE_METATYPE(Qt::FocusReason)



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

    AlwaysOnDisplayWindow *window()
    {
        return &m;
    }

private:
    AlwaysOnDisplayWindow m;
};

void Ut_MTextEditView::initTestCase()
{
    static int dummyArgc = 2;
    static char *dummyArgv[2] = { (char *) "./ut_mtexteditview",
                                  (char *)"-software" };
    MApplication::setLoadMInputContext(false);
    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;
    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mtexteditview.css");
}

void Ut_MTextEditView::cleanupTestCase()
{
    delete m_appWindow;
    m_appWindow = 0;
    delete m_app;
    m_app = 0;
}


void Ut_MTextEditView::init()
{
    sc = new AutoActivatedScene();
    m_controller = new MTextEdit(MTextEditModel::MultiLine, "");
    m_controller->setStyleName("Ut_MTextEditView_MTextEdit");
    m_subject = new MTextEditView(m_controller);
    m_controller->setView(m_subject);
}


void Ut_MTextEditView::cleanup()
{
    m_controller->setView(0);
    m_subject = 0;
    delete m_controller;
    m_controller = 0;
    delete sc;
    sc = 0;
}

namespace {

class FakePaintEngine;

class FakePaintDevice : public QPaintDevice
{
public:
    FakePaintDevice();
    virtual ~FakePaintDevice();

    virtual QPaintEngine *paintEngine() const;

    QScopedPointer<FakePaintEngine> engine;
};

class FakePaintEngine : public QPaintEngine
{
public:
    FakePaintEngine()
    : QPaintEngine()
    , device(0)
    , tracePaint(false)
    {
    }
    virtual ~FakePaintEngine()
    {
    }

    virtual bool begin(QPaintDevice *dev)
    {
        device = dev;
        return true;
    }

    virtual bool end()
    {
        device = 0;
        return true;
    }

    virtual void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
    {
        if (tracePaint) {
            qDebug() << Q_FUNC_INFO << "r:" << r << "sr:" << sr << "pm:" << pm.rect();
        }
    }

    void drawTextItem(const QPointF &p, const QTextItem &textItem)
    {
        if (tracePaint) {
            qDebug() << Q_FUNC_INFO << "p:" << p << "(mapped):" << transform.map(p) << "textItem:" << textItem.text() << textItem.font();
        }
        // store the topleft point of the item
        // (must exactly match the in-widget padding as set in the style)
        // (transformation is applied _after_ shifting)
        texts.insert(textItem.text(), transform.map(p - QPointF(0, textItem.font().pixelSize())));
    }

    virtual void updateState(const QPaintEngineState &state)
    {
        if (tracePaint) {
            qDebug() << Q_FUNC_INFO << "transform:" << state.transform();
        }
        transform = state.transform();
    }

    virtual Type type() const
    {
        return User;
    }

    QMap<QString, QPointF> texts;
    QPaintDevice *device;
    QTransform transform;
    bool tracePaint;
};

FakePaintDevice::FakePaintDevice()
: engine(new FakePaintEngine)
{
}

FakePaintDevice::~FakePaintDevice()
{
}

QPaintEngine *FakePaintDevice::paintEngine() const
{
    return engine.data();
}

}

void Ut_MTextEditView::testPaintPrompt()
{
    m_controller->setGeometry(QRect(0, 0, 200, 30));

    QStyleOptionGraphicsItem* option = new QStyleOptionGraphicsItem();
    m_controller->setPrompt("Prompt");
    FakePaintDevice fakePaintDevice;
    QPainter painter;
    painter.begin(&fakePaintDevice);
    m_subject->drawContents(&painter, option);
    QPoint englishPromptPoint = fakePaintDevice.engine->texts["Prompt"].toPoint();
    painter.end();
    QCOMPARE(englishPromptPoint, QPoint(27, 37));

    QString arabicPrompt;
    arabicPrompt.append(QChar(1604));
    arabicPrompt.append(QChar(1610));
    arabicPrompt.append(QChar(1587));
    arabicPrompt.append(QChar(1578));
    m_controller->setPrompt(arabicPrompt);
    painter.begin(&fakePaintDevice);
    m_subject->drawContents(&painter, option);
    QPointF arabicPromptPoint = fakePaintDevice.engine->texts[arabicPrompt];
    painter.end();
    QVERIFY2(arabicPromptPoint.x() > 100, "Arabic prompt must be aligned to right");
}


void Ut_MTextEditView::testBoundingRect()
{
    // DISABLED TEMPORARILY - setGeometry() doesn't seem to honor minimum size
    // on scratchbox. shouldn't be a MTextView bug anyway.
#if 0
    QRectF testRect(0, 0, 100, 30);
    QRectF adjustedRect = testRect;
    QSizeF minSize = m_subject->sizeHint(Qt::MinimumSize);
    qDebug() << "minsize:" << minSize;

    if ((testRect.size().height() != minSize.height())
            || (testRect.size().width() != minSize.width())) {
        adjustedRect.setSize(minSize);
    }

    m_controller->setGeometry(testRect);

    QVERIFY((m_subject->boundingRect() == testRect)
            || (m_subject->boundingRect() == adjustedRect));
#endif
}


void Ut_MTextEditView::testStyleUpdated()
{
    m_subject->d_func()->showEditorToolbar();
    QVERIFY(m_subject->d_func()->editorToolbar);
    QCOMPARE(m_subject->d_func()->editorToolbar.data()->styleName, QString("TestToolbarStyleName"));
}

void Ut_MTextEditView::testResizeEvent()
{
    qDebug() << "Nothing to test now for resizeEvent";
}


void Ut_MTextEditView::testGrowing()
{
    // tests that the preferred size grows after new text is appended

    QString stringToAppend("\n\nasdf");
    QSizeF oldSize = m_subject->sizeHint(Qt::PreferredSize);

    m_controller->insert(stringToAppend);
    QSizeF newSize = m_subject->sizeHint(Qt::PreferredSize);

    QVERIFY(newSize.height() > oldSize.height());

    // FIXME: first test that removing one line is in between the sizes

    // test that preferred size is the same as in the start after new text is removed
    QKeyEvent event(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);

    for (int i = 0; i < stringToAppend.size(); ++i) {
        m_controller->keyPressEvent(&event);
    }

    newSize = m_subject->sizeHint(Qt::PreferredSize);
    QCOMPARE(newSize.height(), oldSize.height());
}

void Ut_MTextEditView::testMultilineGrowsOnWordWrap()
{
    // Initialize multiline text edit with one row and that row full of text.
    // Then insert one character so that text edit wraps into two lines.
    // This test verifies a fix for a bug where the text edit did not grow even though
    // QTextDocument was updated correctly.

    QString fullRowContent = "full row content";
    m_controller->setText(fullRowContent);

    const qreal horizontalPadding = m_subject->style()->paddingLeft() + m_subject->style()->paddingRight();
    QFontMetrics fm(m_controller->document()->defaultFont());
    qreal totalWidth = fm.width(fullRowContent)
                       + horizontalPadding
                       + m_controller->document()->documentMargin() * 2

                       // Workaround for bug NB#289538 requires cursorWidth to be added. This is only temporary.
                       + m_subject->d_func()->cursorWidth();

    m_controller->setPreferredWidth(totalWidth);

    QSizePolicy policy(m_controller->sizePolicy());
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    m_controller->setSizePolicy(policy);

    // Create container widget with vertical layout.
    QGraphicsWidget container;
    container.resize(totalWidth, 200); // vertically: enough

    QGraphicsLinearLayout *vlayout = new QGraphicsLinearLayout(Qt::Vertical, &container);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->addItem(m_controller);
    vlayout->addStretch();

    // Activate layout before retrieving size.
    vlayout->activate();
    const QSizeF beforeInserting = m_controller->size();

    // This causes word wrap to take place.
    m_controller->textCursor().insertText("w");

    // Activate layout before retrieving size.
    vlayout->activate();
    const QSizeF afterInserting = m_controller->size();

    // Detach from parent
    vlayout->removeItem(m_controller);
    m_controller->setParentItem(0);
    m_controller->setLayout(0);

    qDebug() << "Size " << beforeInserting << " after inserting one extra character: " << afterInserting;
    QVERIFY(afterInserting != beforeInserting);
}


void Ut_MTextEditView::testInputMethodQuery()
{
    m_appWindow->scene()->addItem(m_controller);
    QVariant result;
    QList<Qt::InputMethodQuery> queries;

    queries << Qt::ImMicroFocus << Qt::ImFont << Qt::ImCursorPosition
            << Qt::ImSurroundingText << Qt::ImCurrentSelection;

    result = m_subject->inputMethodQuery(Qt::InputMethodQuery(M::VisualizationPriorityQuery));
    QVERIFY(result.isValid());
    QVERIFY(result.toBool() == false);

    foreach(Qt::InputMethodQuery query, queries) {
        //at least we should not crash
        result = m_subject->inputMethodQuery(query);
    }

    foreach(Qt::InputMethodQuery query, queries) {
        //at least we should not crash
        result = m_subject->inputMethodQuery(query);
    }
}

/*
 * Bug #145360, Characters in masked text entry are not masked
 */
void Ut_MTextEditView::testMaskedCharacters()
{
    QVERIFY(m_controller->echoMode() == MTextEditModel::Normal);
    m_controller->setEchoMode(MTextEditModel::Password);
    m_controller->setEchoMode(MTextEditModel::Password); //we should not crash here
    m_controller->setEchoMode(MTextEditModel::Normal);
    m_controller->setEchoMode(MTextEditModel::Normal); //we should not crash here
}

/*
 * Bug #150452, The position of text cursor is wrong after
 * deleting text with backspace key in the MTextEdit
 *
 * Bug #177305, Left over characters are not right shifting
 * on erasing the characters of input text filed
 */
void Ut_MTextEditView::testUpdateScrollWhenTextChanged()
{
    cleanup();
    m_controller = new MTextEdit(MTextEditModel::SingleLine, "");
    m_subject = new MTextEditView(m_controller);
    m_controller->setView(m_subject);

    m_controller->resize(200, 80); // set some fixed text size

    int count = 256;
    qreal hscroll = 0;
    QKeyEvent bsEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, "\b");
    QKeyEvent event(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier, "x");

    // test for bug #150452
    // (note: QCOMPARE doesn't know how to properly compare two zero floats)
    QVERIFY(qAbs(m_subject->d_ptr->hscroll) < 0.01);
    for (int n = 0; n < count; ++n) {
        m_controller->keyPressEvent(&event);
    }
    hscroll = m_subject->d_ptr->hscroll;
    QVERIFY(hscroll > 0);

    for (int n = 0; n < (count - 2); ++n) {
        m_controller->keyPressEvent(&bsEvent);
    }
    QVERIFY(hscroll > m_subject->d_ptr->hscroll);
    QVERIFY(qAbs(m_subject->d_ptr->hscroll) < 0.01);

    while (!m_controller->text().isEmpty()) {
        m_controller->keyPressEvent(&bsEvent);
    }
    QVERIFY(qAbs(m_subject->d_ptr->hscroll) < 0.01);

    // test for bug #177305
    for (int n = 0; n < count; ++n) {
        m_controller->keyPressEvent(&event);
    }
    m_controller->setCursorPosition(count - 2);
    hscroll = m_subject->d_ptr->hscroll;
    QVERIFY(hscroll > 0);

    for (int n = 0; n < (count / 2); ++n) {
        m_controller->keyPressEvent(&bsEvent);
    }
    QVERIFY(hscroll > m_subject->d_ptr->hscroll);
    QVERIFY(m_subject->d_ptr->hscroll > qreal(1.0));

    for (int n = 0;
         (n < count) && (m_subject->d_ptr->hscroll > 0)
             && !m_controller->text().isEmpty();
         ++n) {
        m_controller->keyPressEvent(&bsEvent);
    }
    QVERIFY(m_controller->cursorPosition() > 0);
}

void Ut_MTextEditView::testSizeHint()
{
    cleanup();
    m_controller = new MTextEdit(MTextEditModel::SingleLine, "");
    m_subject = new MTextEditView(m_controller);
    m_controller->setView(m_subject);
    m_controller->setText("This is a longish string of sample text to test size hints. Thanks!");

    //The size hints should not depend on the current size
    QSizeF preferredSize = m_controller->preferredSize();
    m_controller->resize(100,100);
    QCOMPARE(preferredSize, m_controller->preferredSize());
    m_controller->resize(10,10);
    QCOMPARE(preferredSize, m_controller->preferredSize());
    m_controller->resize(1000,1000);
    QCOMPARE(preferredSize, m_controller->preferredSize());

    QSizeF minimumSize = m_controller->minimumSize();
    m_controller->resize(100,100);
    QCOMPARE(minimumSize, m_controller->minimumSize());
    m_controller->resize(10,10);
    QCOMPARE(minimumSize, m_controller->minimumSize());
    m_controller->resize(1000,1000);
    QCOMPARE(minimumSize, m_controller->minimumSize());

    QSizeF maximumSize = m_controller->maximumSize();
    m_controller->resize(100,100);
    QCOMPARE(maximumSize, m_controller->maximumSize());
    m_controller->resize(10,10);
    QCOMPARE(maximumSize, m_controller->maximumSize());
    m_controller->resize(1000,1000);
    QCOMPARE(maximumSize, m_controller->maximumSize());
}

void Ut_MTextEditView::testSendsScenePositionChangesOnMagnifier()
{
    // Normal magnifier show & hide, when no initial flag is set.
    QVERIFY(!(m_controller->flags() & QGraphicsItem::ItemSendsScenePositionChanges));
    m_subject->d_func()->showMagnifier();
    QVERIFY(m_controller->flags() & QGraphicsItem::ItemSendsScenePositionChanges);
    m_subject->d_func()->hideMagnifier();
    // Flag should be cleared.
    QVERIFY(!(m_controller->flags() & QGraphicsItem::ItemSendsScenePositionChanges));

    // Magnifier show & hide, when flag is initially set.
    m_controller->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    m_subject->d_func()->showMagnifier();
    QVERIFY(m_controller->flags() & QGraphicsItem::ItemSendsScenePositionChanges);
    m_subject->d_func()->hideMagnifier();
    // Flag should still be set.
    QVERIFY(m_controller->flags() & QGraphicsItem::ItemSendsScenePositionChanges);
}

void Ut_MTextEditView::testIgnoreFocusingTap_data()
{
    QTest::addColumn<bool>("focusOnPress");

    QTest::newRow("focus on press") << true;
    QTest::newRow("focus on release") << false;
}

void Ut_MTextEditView::testIgnoreFocusingTap()
{
    QFETCH(bool, focusOnPress);

    // On focusing tap, text edit should not interact with mouse
    // (other than becoming focused). Interactions tested here
    // are longpress and text selecting.

    // large enough for selecting to be possible
    m_controller->setText("some text");
    m_controller->resize(500, 50);

    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent move1(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent move2(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent release(QEvent::GraphicsSceneMouseRelease);

    press.setPos(QPointF());
    release.setPos(QPointF(1000, 0));
    move1.setPos(press.pos());
    move2.setPos(release.pos());

    move1.setButtons(Qt::LeftButton);
    move2.setButtons(Qt::LeftButton);

    for (int tap = 0; tap < 2; ++tap) {
        const bool firstTap = (tap == 0);

        if (firstTap && focusOnPress) {
            // Give focus on press.
            QMetaObject::invokeMethod(m_controller, "gainedFocus",
                                      Q_ARG(Qt::FocusReason, Qt::MouseFocusReason));
        }

        m_subject->mousePressEvent(&press);
        const bool expectedLongPressTimerActive = !firstTap;
        QCOMPARE(m_subject->d_func()->longPressTimer->isActive(),
                 expectedLongPressTimerActive);

        m_subject->mouseMoveEvent(&move1);
        m_subject->mouseMoveEvent(&move2);
        const bool expectedHasSelection = !firstTap;
        QCOMPARE(m_controller->hasSelectedText(),
                 expectedHasSelection);

        if (firstTap && !focusOnPress) {
            // Give focus on release.
            QMetaObject::invokeMethod(m_controller, "gainedFocus",
                                      Q_ARG(Qt::FocusReason, Qt::MouseFocusReason));
        }
        m_subject->mouseReleaseEvent(&release);
    }
}

void Ut_MTextEditView::testFirstTapOnAlreadyFocused()
{
    m_controller->setText("some text");

    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);

    // Give focus with some other reason than Qt::MouseFocusReason.
    QMetaObject::invokeMethod(m_controller, "gainedFocus",
                              Q_ARG(Qt::FocusReason, Qt::OtherFocusReason));

    // Now, even this is the first press it should not be ignored.
    m_subject->mousePressEvent(&press);

    // Verify by checking that long press timer is active.
    QVERIFY(m_subject->d_func()->longPressTimer->isActive());
}

void Ut_MTextEditView::testEditorToolbarReappearanceAfterFocusLost_data()
{
    QTest::addColumn<Qt::FocusReason>("focusLostReason");
    QTest::addColumn<Qt::FocusReason>("focusGainedReason");

    QTest::newRow("focus lost by clicking")
        << Qt::MouseFocusReason
        << Qt::MouseFocusReason;
    QTest::newRow("window system removed focus")
        << Qt::ActiveWindowFocusReason
        << Qt::ActiveWindowFocusReason;
    QTest::newRow("focus removed by popup")
        << Qt::PopupFocusReason
        << Qt::PopupFocusReason;
    QTest::newRow("window system removed focus, gained with mouse")
        << Qt::ActiveWindowFocusReason
        << Qt::MouseFocusReason;
    QTest::newRow("focus removed by popup, gained with mouse")
        << Qt::PopupFocusReason
        << Qt::MouseFocusReason;
    QTest::newRow("focus lost by clicking, gained by window system")
        << Qt::MouseFocusReason
        << Qt::ActiveWindowFocusReason;
}

void Ut_MTextEditView::testEditorToolbarReappearanceAfterFocusLost()
{
    QFETCH(Qt::FocusReason, focusLostReason);
    QFETCH(Qt::FocusReason, focusGainedReason);

    // large enough for selecting to be possible
    m_controller->setText("to be selected");
    m_controller->resize(500, 50);

    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent move1(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent move2(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent release(QEvent::GraphicsSceneMouseRelease);

    press.setPos(QPointF());
    release.setPos(QPointF(1000, 0));
    move1.setPos(press.pos());
    move2.setPos(release.pos());

    move1.setButtons(Qt::LeftButton);
    move2.setButtons(Qt::LeftButton);

    // Give focus, with whatever reason.
    QMetaObject::invokeMethod(m_controller, "gainedFocus",
                              Q_ARG(Qt::FocusReason, Qt::OtherFocusReason));

    // Show editor toolbar by selecting text
    m_subject->mousePressEvent(&press);
    m_subject->mouseMoveEvent(&move1);
    m_subject->mouseMoveEvent(&move2);
    m_subject->mouseReleaseEvent(&release);
    QVERIFY(m_controller->hasSelectedText());
    QVERIFY(editorToolbarAppeared());

    QMetaObject::invokeMethod(m_controller, "lostFocus",
                              Q_ARG(Qt::FocusReason, focusLostReason));

    // Lost focus always dismisses editor.
    QVERIFY(!editorToolbarAppeared());

    // Give focus again.
    QMetaObject::invokeMethod(m_controller, "gainedFocus",
                              Q_ARG(Qt::FocusReason, focusGainedReason));

    QCOMPARE(editorToolbarAppeared(), true);
}

void Ut_MTextEditView::testEditorToolbarReappearanceAfterFocusLostAndMovement_data()
{
    QTest::addColumn<Qt::FocusReason>("focusLostReason");
    QTest::addColumn<Qt::FocusReason>("focusGainedReason");

    QTest::newRow("focus lost by clicking")
        << Qt::MouseFocusReason
        << Qt::MouseFocusReason;
    QTest::newRow("window system removed focus")
        << Qt::ActiveWindowFocusReason
        << Qt::ActiveWindowFocusReason;
    QTest::newRow("focus removed by popup")
        << Qt::PopupFocusReason
        << Qt::PopupFocusReason;
    QTest::newRow("window system removed focus, gained with mouse")
        << Qt::ActiveWindowFocusReason
        << Qt::MouseFocusReason;
    QTest::newRow("focus removed by popup, gained with mouse")
        << Qt::PopupFocusReason
        << Qt::MouseFocusReason;
    QTest::newRow("focus lost by clicking, gained by window system")
        << Qt::MouseFocusReason
        << Qt::ActiveWindowFocusReason;
}

void Ut_MTextEditView::testEditorToolbarReappearanceAfterFocusLostAndMovement()
{
    QFETCH(Qt::FocusReason, focusLostReason);
    QFETCH(Qt::FocusReason, focusGainedReason);

    m_controller->setParentItem(sc->window()->box());
    sc->window()->show();
    QTest::qWaitForWindowShown(sc->window());

    // large enough for selecting to be possible
    m_controller->setText("to be selected");
    m_controller->resize(500, 50);

    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent move1(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent move2(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent release(QEvent::GraphicsSceneMouseRelease);

    press.setPos(QPointF());
    release.setPos(QPointF(1000, 0));
    move1.setPos(press.pos());
    move2.setPos(release.pos());

    move1.setButtons(Qt::LeftButton);
    move2.setButtons(Qt::LeftButton);

    // Give focus, with whatever reason.
    QMetaObject::invokeMethod(m_controller, "gainedFocus",
                              Q_ARG(Qt::FocusReason, Qt::OtherFocusReason));

    // Show editor toolbar by selecting text
    m_subject->mousePressEvent(&press);
    m_subject->mouseMoveEvent(&move1);
    m_subject->mouseMoveEvent(&move2);
    m_subject->mouseReleaseEvent(&release);
    QVERIFY(m_controller->hasSelectedText());
    QVERIFY(editorToolbarAppeared());

    m_controller->translate(10, 10);
    QTest::qWait(10); // motion detection needs for small delay between steps
    m_controller->translate(10, 10);
    QVERIFY(m_controller->scene());
    QVERIFY(!editorToolbarAppeared());

    QMetaObject::invokeMethod(m_controller, "lostFocus",
                              Q_ARG(Qt::FocusReason, focusLostReason));

    QVERIFY(!editorToolbarAppeared());
    QTest::qWait(10);
    m_controller->translate(-20, -20);

    QTest::qWait(m_subject->style()->toolbarShowDelay() + 15);
    QVERIFY(!editorToolbarAppeared());

    // Give focus again.
    QMetaObject::invokeMethod(m_controller, "gainedFocus",
                              Q_ARG(Qt::FocusReason, focusGainedReason));

    QCOMPARE(editorToolbarAppeared(), true);
}

void Ut_MTextEditView::testEditorToolbarReappearanceAfterMovement()
{
    m_controller->setParentItem(sc->window()->box());
    sc->window()->show();
    QTest::qWaitForWindowShown(sc->window());

    // large enough for selecting to be possible
    m_controller->setText("to be selected");
    m_controller->resize(500, 50);

    // Give focus, with whatever reason.
    QMetaObject::invokeMethod(m_controller, "gainedFocus",
                              Q_ARG(Qt::FocusReason, Qt::OtherFocusReason));

    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent move1(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent move2(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent release(QEvent::GraphicsSceneMouseRelease);

    press.setPos(QPointF());
    release.setPos(QPointF(1000, 0));
    move1.setPos(press.pos());
    move2.setPos(release.pos());

    move1.setButtons(Qt::LeftButton);
    move2.setButtons(Qt::LeftButton);

    // Show editor toolbar by selecting text
    m_subject->mousePressEvent(&press);
    m_subject->mouseMoveEvent(&move1);
    m_subject->mouseMoveEvent(&move2);
    m_subject->mouseReleaseEvent(&release);
    QVERIFY(m_controller->hasSelectedText());
    QVERIFY(editorToolbarAppeared());

    m_controller->translate(10, 10);
    QTest::qWait(10); // motion detection needs for small delay between steps
    m_controller->translate(10, 10);
    QVERIFY(m_controller->scene());
    QVERIFY(!editorToolbarAppeared());

    QTest::qWait(10);
    m_controller->translate(-20, -20);

    QTest::qWait(m_subject->style()->toolbarShowDelay() + 15);

    QCOMPARE(editorToolbarAppeared(), true);
}

void Ut_MTextEditView::testEditorToolbarAutoHide()
{
    // Toolbar is to set to autohide on show without selection.
    m_controller->setText("");
    m_subject->d_func()->showEditorToolbar();
    QCOMPARE(m_subject->d_func()->editorToolbar.data()->isAutoHideEnabled(), true);

    m_controller->setText("test text");
    m_controller->setSelection(0, 5, false);
    m_subject->d_func()->showEditorToolbar();
    QCOMPARE(m_subject->d_func()->editorToolbar.data()->isAutoHideEnabled(), false);
}

void Ut_MTextEditView::testEditorToolbarAutoHideWithSelectionMagnifier()
{
    m_controller->setParentItem(sc->window()->box());
    sc->window()->show();
    QTest::qWaitForWindowShown(sc->window());

    // large enough for selecting to be possible
    m_controller->setText("to be selected");
    m_controller->resize(500, 50);

    // Give focus, with whatever reason.
    QMetaObject::invokeMethod(m_controller, "gainedFocus",
                              Q_ARG(Qt::FocusReason, Qt::OtherFocusReason));

    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent move1(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent move2(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent release(QEvent::GraphicsSceneMouseRelease);

    press.setPos(QPointF());
    release.setPos(QPointF(1000, 0));
    move1.setPos(press.pos());
    move2.setPos(release.pos());

    move1.setButtons(Qt::LeftButton);
    move2.setButtons(Qt::LeftButton);

    // Show editor toolbar by selecting text
    m_subject->mousePressEvent(&press);
    m_subject->mouseMoveEvent(&move1);
    m_subject->mouseMoveEvent(&move2);
    m_subject->mouseReleaseEvent(&release);
    QCOMPARE(m_controller->hasSelectedText(), true);
    QCOMPARE(editorToolbarAppeared(), true);

    // Hide editor toolbar temporarily by scrolling
    m_controller->translate(10, 10);
    QTest::qWait(10); // motion detection needs for small delay between steps
    m_controller->translate(10, 10);
    QVERIFY(m_controller->scene());
    QCOMPARE(editorToolbarAppeared(), false);

    // Editor toolbar is about to appear
    QCOMPARE(m_subject->d_func()->showMovedToolbar.isActive(), true);

    // Press selection handle
    m_subject->d_func()->onSelectionHandlePressed(QPointF(), MTextSelectionOverlay::Cursor);
    QCOMPARE(editorToolbarAppeared(), false);

    // Editor toolbar should not become visible until selection handle
    // is released
    QCOMPARE(m_subject->d_func()->showMovedToolbar.isActive(), false);
    QTest::qWait(m_subject->style()->toolbarShowDelay() + 15);
    QCOMPARE(editorToolbarAppeared(), false);

    // Scroll again
    m_controller->translate(10, 10);
    QTest::qWait(10); // motion detection needs for small delay between steps
    m_controller->translate(10, 10);
    QVERIFY(m_controller->scene());
    QCOMPARE(editorToolbarAppeared(), false);

    // Editor toolbar should not be about to appear
    QCOMPARE(m_subject->d_func()->showMovedToolbar.isActive(), false);

    // Release selection handle
    m_subject->d_func()->onSelectionHandleReleased(QPointF());

    // Editor toolbar should be visible again
    QCOMPARE(editorToolbarAppeared(), true);
}

bool Ut_MTextEditView::editorToolbarAppeared() const
{
    return m_subject->d_func()->editorToolbar
           && m_subject->d_func()->editorToolbar.data()->isAppeared();
}

void Ut_MTextEditView::testSelectionOverlay()
{
    m_controller->setParentItem(sc->window()->box());
    m_controller->setFocus();
    sc->window()->show();
    QTest::qWaitForWindowShown(sc->window());

    QCOMPARE(m_controller->model()->isSelecting(), false);
    m_controller->setText("test text");
    m_controller->setSelection(0, 5, false);
    m_subject->d_func()->showSelectionOverlay();

    QCOMPARE(m_subject->d_func()->selectionOverlay.isNull(), false);
    QCOMPARE(m_subject->d_func()->selectionOverlay.data()->isVisible(), true);
    QCOMPARE(m_controller->model()->isSelecting(), true);

    m_controller->setSelection(0, 0, false);
    QCOMPARE(m_subject->d_func()->selectionOverlay.data()->isVisible(), false);
    QCOMPARE(m_controller->model()->isSelecting(), false);
}

void Ut_MTextEditView::testPromptEliding()
{
    m_controller->setParentItem(sc->window()->box());
    m_controller->setFocus();
    sc->window()->show();
    QTest::qWaitForWindowShown(sc->window());

    const QString shortWord("A");
    const QString longWord(1024, QChar('B'));
    const QChar separatorChar(0x9c, 0);
    const QString separator(separatorChar);

    const QString shortAndShort = shortWord + separator + shortWord;
    const QString longAndShort = longWord + separator + shortWord;
    const QString superLong = longWord + " " + shortWord;

    m_controller->setPrompt(shortAndShort);
    const QImage shortAndShortCapture = captureImage(m_controller);
    QCOMPARE(m_subject->d_ptr->elidedPrompt(m_subject->d_ptr->promptOption()), shortWord);

    m_controller->setPrompt(shortWord);
    const QImage shortCapture = captureImage(m_controller);
    QCOMPARE(m_subject->d_ptr->elidedPrompt(m_subject->d_ptr->promptOption()), shortWord);

    QCOMPARE(shortCapture.isNull(), false);
    QCOMPARE(shortAndShortCapture.isNull(), false);
    QCOMPARE(shortCapture, shortAndShortCapture);

    m_controller->setPrompt(longAndShort);
    const QImage longAndShortCapture = captureImage(m_controller);
    QCOMPARE(m_subject->d_ptr->elidedPrompt(m_subject->d_ptr->promptOption()), shortWord);

    QCOMPARE(shortCapture, longAndShortCapture);

    m_controller->setPrompt(superLong);
    const QString elidedPrompt = m_subject->d_ptr->elidedPrompt(m_subject->d_ptr->promptOption());
    QVERIFY(elidedPrompt.length() < superLong.length());
}

QImage Ut_MTextEditView::captureImage(MWidget *widget)
{
    QPixmap pixmap(widget->size().toSize());
    pixmap.fill(Qt::transparent);

    {
        QPainter painter(&pixmap);
        widget->paint(&painter, NULL, NULL);
    }

    return pixmap.toImage();
}

QTEST_APPLESS_MAIN(Ut_MTextEditView)

