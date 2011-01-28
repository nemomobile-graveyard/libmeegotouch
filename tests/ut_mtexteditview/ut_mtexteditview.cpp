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
#include <MScene>

#include <mtextedit.h>
#include <mtexteditview.h>

#include "corelib/widgets/mtextedit_p.h"
#include "views/mtexteditview_p.h"

void Ut_MTextEditView::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_mtexteditview" };
    MApplication::setLoadMInputContext(false);
    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;
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
    m_controller = new MTextEdit(MTextEditModel::MultiLine, "");
    m_subject = new MTextEditView(m_controller);
    m_controller->setView(m_subject);
}


void Ut_MTextEditView::cleanup()
{
    m_controller->setView(0);
    m_subject = 0;
    delete m_controller;
    m_controller = 0;
}


void Ut_MTextEditView::testPaint()
{
    qDebug() << "Nothing to test now for paint";
    /*QPixmap* empty = new QPixmap(200, 200);
    empty->fill(QColor(0, 0, 0, 0));
    QPainter *myPainter = new QPainter(empty);*/

    //QStyleOptionGraphicsItem* option = new QStyleOptionGraphicsItem();
    //m_subject->paint(myPainter, option);

    /*delete myPainter;
    delete empty;*/
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
    qDebug() << "Nothing to test now for styleUpdated";
    //Hard to say whether we can test this function, because all the attribute is read from css file
    /*m_subject->styleUpdated();
    QString activeTextStyle = m_subject->styleAttribute<QString>(MTextEditViewPrivate::ActiveTextStyleAttribute);
    QCOMPARE(activeTextStyle,"WaveUnderline");*/
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
                       + m_controller->document()->documentMargin() * 2;

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

QTEST_APPLESS_MAIN(Ut_MTextEditView)

