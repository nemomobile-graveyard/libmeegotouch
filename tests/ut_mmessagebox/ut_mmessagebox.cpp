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

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLayout>

#include <mdismissevent.h>
#include <mmessagebox.h>
#include <mapplication.h>
#include <mbuttonmodel.h>

#include "ut_mmessagebox.h"

MApplication *app(NULL);

void Ut_MMessageBox::init()
{
    m_subject = new MMessageBox();
}

void Ut_MMessageBox::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MMessageBox::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mmessagebox" };
    app = new MApplication(argc, app_name);
}

void Ut_MMessageBox::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MMessageBox::testDefaultConstructor()
{
    //checking default constructor result
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->title(), QString());
    QList<MButtonModel *> buttonModelList = m_subject->model()->buttons();
    QCOMPARE( m_subject->standardButton( buttonModelList.at(0)), M::OkButton);
}

void Ut_MMessageBox::testConstructor()
{
    delete m_subject;
    QString title("Title");
    QString text("Text");
    m_subject = new MMessageBox( title, text );

    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->title(), title);
    QCOMPARE( m_subject->text(), text);
    QList<MButtonModel *> buttonModelList = m_subject->model()->buttons();
    QCOMPARE( m_subject->standardButton( buttonModelList.at(0)), M::OkButton);
}

void Ut_MMessageBox::testSetText()
{
    QString myText("myText");
    m_subject->setText(myText);
    QCOMPARE( m_subject->text(), myText );
}

void Ut_MMessageBox::testIconId()
{
    QString iconId("test_icon_id");
    m_subject->setIconId(iconId);
    QCOMPARE( m_subject->iconId(), iconId );
}

void Ut_MMessageBox::testIconPixmap()
{
    QPixmap pixmap(60, 60);
    pixmap.fill();
    QVERIFY(!pixmap.isNull());
    m_subject->setIconPixmap(pixmap);
    QCOMPARE(m_subject->iconPixmap(), pixmap);
}

void Ut_MMessageBox::testDismissEvent()
{
    QSignalSpy spyChanged1(m_subject, SIGNAL(rejected()));
    QSignalSpy spyChanged2(m_subject, SIGNAL(finished(int)));
    QSignalSpy spyChanged3(m_subject, SIGNAL(accepted()));

    m_subject->dismissEvent(new MDismissEvent());

    QCOMPARE(spyChanged1.count(), 0);
    QCOMPARE(spyChanged2.count(), 0);
    QCOMPARE(spyChanged3.count(), 0);
    m_subject->accept();
    QCOMPARE(spyChanged3.count(), 1);

}

void Ut_MMessageBox::testSizeHint()
{
    m_subject->setText("Hi.");
    QSizeF preferredSize = m_subject->preferredSize();
    QSizeF originalContentsSize = m_subject->QGraphicsWidget::layout()->effectiveSizeHint(Qt::PreferredSize, QSizeF(preferredSize.width(), -1));
    m_subject->setText("This is a long set of text.  This is a long set of text. This is a long set of text. This is a long set of text. This is a long set of text. This is a long set of text. This is a long set of text. This is a long set of text. This is a long set of text. This is a long set of text. This is a long set of text.");

    // The messagebox itself fills the screen, so that the contents can be centered.  So the preferred size shouldn't have changed.
    // If this breaks, the CSS has been changed
    QCOMPARE( m_subject->preferredSize(), preferredSize);

    //But the contents should have grown in height
    QSizeF newContentsSize = m_subject->QGraphicsWidget::layout()->effectiveSizeHint(Qt::PreferredSize, QSizeF(preferredSize.width(), -1));
    QVERIFY( newContentsSize.height() > originalContentsSize.height());
}

QTEST_APPLESS_MAIN(Ut_MMessageBox)
