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

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLayout>

#include <MLabel>
#include <MImageWidget>
#include <mdismissevent.h>
#include <mmessagebox.h>
#include <mmessageboxview.h>
#include <mapplication.h>
#include <mbuttonmodel.h>
#include "mmessageboxview_p.h"

#include "ut_mmessageboxview.h"

MApplication *app(NULL);

void Ut_MMessageBoxView::init()
{
    m_messageBox = new MMessageBox;
    m_subject = new MMessageBoxView(m_messageBox);
    m_messageBox->setView(m_subject);
}

void Ut_MMessageBoxView::cleanup()
{
    delete m_messageBox;
}

void Ut_MMessageBoxView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mmessagebox" };
    app = new MApplication(argc, app_name);
}

void Ut_MMessageBoxView::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MMessageBoxView::testTitleLabel()
{
    QString title("test title");
    m_messageBox->setTitle(title);
    QCOMPARE(m_subject->d_func()->titleLabel->text(), title);
    m_messageBox->setTitle(QString(""));
    QVERIFY(m_subject->d_func()->titleLabel == 0);
}

void Ut_MMessageBoxView::testTextLabel()
{
    QString text("test title");
    m_messageBox->setText(text);
    QCOMPARE(m_subject->d_func()->textLabel->text(), text);
    m_messageBox->setText(QString(""));
    QVERIFY(m_subject->d_func()->textLabel == 0);
}

void Ut_MMessageBoxView::testIcon()
{
    QString iconId("test_icon_id");
    m_messageBox->setIconId(iconId);
    QCOMPARE(m_subject->d_func()->iconImage->imageId(), iconId);

    QPixmap pixmap(60, 60);
    pixmap.fill();
    QVERIFY(!pixmap.isNull());
    m_messageBox->setIconPixmap(pixmap);
    QCOMPARE(*m_subject->d_func()->iconImage->pixmap(), pixmap);
}

QTEST_APPLESS_MAIN(Ut_MMessageBoxView)
