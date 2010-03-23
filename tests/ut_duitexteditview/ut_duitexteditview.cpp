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

#include "ut_duitexteditview.h"

#include <QGraphicsSceneMouseEvent>
#include <QMetaType>
#include <QSignalSpy>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiScene>

#include <duitextedit.h>
#include <duitexteditview.h>

#include "../../src/widgets/duitextedit_p.h"
#include "../../src/widgets/views/duitexteditview_p.h"

void Ut_DuiTextEditView::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_duitexteditview" };
    DuiApplication::setLoadDuiInputContext(false);
    m_app = new DuiApplication(dummyArgc, dummyArgv);
    m_appWindow = new DuiApplicationWindow;
}

void Ut_DuiTextEditView::cleanupTestCase()
{
    delete m_appWindow;
    m_appWindow = 0;
    delete m_app;
    m_app = 0;
}


void Ut_DuiTextEditView::init()
{
    m_controller = new DuiTextEdit(DuiTextEditModel::MultiLine, "");
    m_subject = new DuiTextEditView(m_controller);
    m_controller->setView(m_subject);
}


void Ut_DuiTextEditView::cleanup()
{
    m_controller->setView(0);
    m_subject = 0;
    delete m_controller;
    m_controller = 0;
}


void Ut_DuiTextEditView::testPaint()
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


void Ut_DuiTextEditView::testBoundingRect()
{
    // DISABLED TEMPORARILY - setGeometry() doesn't seem to honor minimum size
    // on scratchbox. shouldn't be a DuiTextView bug anyway.
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


void Ut_DuiTextEditView::testStyleUpdated()
{
    qDebug() << "Nothing to test now for styleUpdated";
    //Hard to say whether we can test this function, because all the attribute is read from css file
    /*m_subject->styleUpdated();
    QString activeTextStyle = m_subject->styleAttribute<QString>(DuiTextEditViewPrivate::ActiveTextStyleAttribute);
    QCOMPARE(activeTextStyle,"WaveUnderline");*/
}


void Ut_DuiTextEditView::testResizeEvent()
{
    qDebug() << "Nothing to test now for resizeEvent";
}


void Ut_DuiTextEditView::testGrowing()
{
    // tests that the minimum size grows after new text is appended

    QString stringToAppend("\n\nasdf");
    QSizeF oldSize = m_subject->sizeHint(Qt::MinimumSize);

    m_controller->insert(stringToAppend);
    QSizeF newSize = m_subject->sizeHint(Qt::MinimumSize);

    QVERIFY(newSize.height() > oldSize.height());

    // FIXME: first test that removing one line is in between the sizes

    // test that minimum size is the same as in the start after new text is removed
    QKeyEvent event(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);

    for (int i = 0; i < stringToAppend.size(); ++i) {
        m_controller->keyPressEvent(&event);
    }

    newSize = m_subject->sizeHint(Qt::MinimumSize);
    QCOMPARE(newSize.height(), oldSize.height());
}

void Ut_DuiTextEditView::testInputMethodQuery()
{
    m_appWindow->scene()->addItem(m_controller);
    QVariant result;
    QList<Qt::InputMethodQuery> queries;
    QGraphicsSceneMouseEvent event;

    queries << Qt::ImMicroFocus << Qt::ImFont << Qt::ImCursorPosition
            << Qt::ImSurroundingText << Qt::ImCurrentSelection;
    event.setPos(QPointF(10, 10));

    result = m_subject->inputMethodQuery(Qt::InputMethodQuery(Dui::VisualizationPriorityQuery));
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
void Ut_DuiTextEditView::testMaskedCharacters()
{
    QVERIFY(m_controller->echoMode() == DuiTextEditModel::Normal);
    m_controller->setEchoMode(DuiTextEditModel::Password);
    m_controller->setEchoMode(DuiTextEditModel::Password); //we should not crash here
    m_controller->setEchoMode(DuiTextEditModel::Normal);
    m_controller->setEchoMode(DuiTextEditModel::Normal); //we should not crash here
}

/*
 * Bug #150452, The position of text cursor is wrong after
 * deleting text with backspace key in the DuiTextEdit
 */
void Ut_DuiTextEditView::testUpdateScrollWhenTextChanged()
{
    cleanup();
    m_controller = new DuiTextEdit(DuiTextEditModel::SingleLine, "");
    m_subject = new DuiTextEditView(m_controller);
    m_controller->setView(m_subject);

    int count = 256;
    qreal hscroll = 0;
    QKeyEvent bsEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, "\b");
    QKeyEvent event(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier, "x");

    QCOMPARE(m_subject->d_ptr->hscroll, qreal(0));
    for (int n = 0; n < count; ++n) {
        m_controller->keyPressEvent(&event);
    }
    hscroll = m_subject->d_ptr->hscroll;
    QVERIFY(hscroll > 0);

    for (int n = 0; n < (count - 2); ++n) {
        m_controller->keyPressEvent(&bsEvent);
    }
    QVERIFY(hscroll > m_subject->d_ptr->hscroll);
    QVERIFY(m_subject->d_ptr->hscroll > qreal(1.0));

    while (!m_controller->text().isEmpty()) {
        m_controller->keyPressEvent(&bsEvent);
    }
    QCOMPARE(m_subject->d_ptr->hscroll, qreal(0));
}

QTEST_APPLESS_MAIN(Ut_DuiTextEditView)

