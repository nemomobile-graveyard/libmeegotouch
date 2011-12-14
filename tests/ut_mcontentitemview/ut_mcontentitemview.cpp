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

#include <MLabel>
#include <MImageWidget>
#include <QSignalSpy>
#include <QGraphicsSceneMouseEvent>
#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <mcontentitem.h>
#include <mcontentitemview.h>
#include <mcontentitemview_p.h>

#include "ut_mcontentitemview.h"
#include "mapplication.h"

MApplication *app;

void Ut_MContentItemView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcontentitemview" };
    app = new MApplication(argc, app_name);
}

void Ut_MContentItemView::cleanupTestCase()
{
    delete app;
}

void Ut_MContentItemView::init()
{
    m_contentItem = new MContentItem;
    m_subject = new MContentItemView(m_contentItem);
    m_contentItem->setView(m_subject);
}

void Ut_MContentItemView::cleanup()
{
    delete m_contentItem;
}

void Ut_MContentItemView::testTitle()
{
    QString titleString("test title");
    m_contentItem->setTitle(titleString);
    QCOMPARE(m_subject->d_func()->titleLabel->text(), titleString);
}

void Ut_MContentItemView::testSubtitle()
{
    QString subtitleString("test subtitle");
    m_contentItem->setSubtitle(subtitleString);
    QCOMPARE(m_subject->d_func()->subtitleLabel->text(), subtitleString);
}

void Ut_MContentItemView::testImage()
{
    QPixmap pixmap(100, 100);
    pixmap.fill(Qt::green);
    const QImage img = pixmap.toImage();

    m_contentItem->setImage(img);
    QCOMPARE(*m_subject->d_func()->imageWidget->pixmap(), pixmap);

    const QString imageId("image1");
    m_contentItem->setImageID(imageId);
    QCOMPARE(m_subject->d_func()->imageWidget->imageId(), imageId);

    // Test optional image
    m_contentItem->setOptionalImage(img);
    QCOMPARE(*m_subject->d_func()->optionalImageWidget->pixmap(), pixmap);

    m_contentItem->setOptionalImageID(imageId);
    QCOMPARE(m_subject->d_func()->optionalImageWidget->imageId(), imageId);
}

void Ut_MContentItemView::testPixmap()
{
    QPixmap pixmap(100, 100);
    pixmap.fill(Qt::green);

    m_contentItem->setPixmap(pixmap);
    QCOMPARE(*m_subject->d_func()->imageWidget->pixmap(), pixmap);

    // Test optional pixmap
    m_contentItem->setOptionalPixmap(pixmap);
    QCOMPARE(*m_subject->d_func()->optionalImageWidget->pixmap(), pixmap);
}

void Ut_MContentItemView::testMouseEvents()
{
    QRectF rect = QRectF(0, 0, 300, 100);
    m_contentItem->setGeometry(rect);

    QSignalSpy clickedSpy(m_contentItem, SIGNAL(clicked()));

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(&pressEvent);
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    m_subject->mouseReleaseEvent(&releaseEvent);
    QCOMPARE(clickedSpy.count(), 1);

    clickedSpy.clear();
    m_subject->mousePressEvent(&pressEvent);
    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    // check that a movement that leaves the button doesn't trigger a click
    moveEvent.setScenePos(QPointF(350, 100));
    m_subject->mouseMoveEvent(&moveEvent);
    m_subject->mouseReleaseEvent(&releaseEvent);
    QCOMPARE(clickedSpy.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_MContentItemView)
