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

#include "ut_mpannableviewport.h"
#include <mtheme.h>
#include <mpositionindicator.h>
#include "mpannableviewport.h"
#include "mpannableviewport_p.h"
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QTest>
#include <QDebug>
#include <mapplication.h>
#include <mwidgetview.h>

MApplication *app;

void Ut_MPannableViewport::initTestCase()
{
    static int argc = 1;
    static char *app_name[] = { (char *) "./ut_mpannableviewport" };

    app = new MApplication(argc, app_name);
    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mpannableviewport.css");
}


void Ut_MPannableViewport::cleanupTestCase()
{
    // We do not delete the app on purpose. Due to some weird (toolchain?) problem
    // this can cause a crash when libraries are unregistered later.
    //delete app;
}


void Ut_MPannableViewport::init()
{
    subject = new MPannableViewport();
}

void Ut_MPannableViewport::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MPannableViewport::constructor()
{
    QCOMPARE(subject->flags(), QGraphicsItem::ItemClipsChildrenToShape);
}

void Ut_MPannableViewport::setWidget()
{
    QGraphicsWidget *widget = new QGraphicsWidget();
    subject->setWidget(widget);

    widget->setZValue(1);
    QCOMPARE(widget->zValue(), 1.0);

    QCOMPARE(static_cast<MPannableViewportPrivate *>(subject->d_ptr)->pannedWidget, widget);
    QCOMPARE(static_cast<MPannableViewportPrivate *>(subject->d_ptr)->pannedWidget->parentItem(), subject);

}

void Ut_MPannableViewport::setGeometry_data()
{
    QTest::addColumn< QSizeF >("viewportSize");
    QTest::addColumn< QSizeF >("pannedSize");
    QTest::addColumn< QSizeF >("physicsRange");

    QTest::newRow("set 1") << QSizeF(100, 50) << QSizeF(400, 200) << QSizeF(300, 150);
    QTest::newRow("set 2") << QSizeF(400, 50) << QSizeF(100, 200) << QSizeF(0, 150);
    QTest::newRow("set 3") << QSizeF(100, 200) << QSizeF(400, 50) << QSizeF(300, 0);
}

void Ut_MPannableViewport::setGeometry()
{
    QFETCH(QSizeF, viewportSize);
    QFETCH(QSizeF, pannedSize);
    QFETCH(QSizeF, physicsRange);

    QGraphicsWidget *widget = new QGraphicsWidget();
    widget->setMinimumSize(pannedSize);
    widget->setMaximumSize(pannedSize);

    subject->setWidget(widget);
    subject->setGeometry(QRectF(QPointF(), viewportSize));

    QCOMPARE(subject->physics()->range().size(), physicsRange);
}

void Ut_MPannableViewport::event_data()
{
    QTest::addColumn< QSizeF >("viewportSize");
    QTest::addColumn< QSizeF >("pannedSize");
    QTest::addColumn< QSizeF >("physicsRange");

    QTest::newRow("set 1") << QSizeF(100, 50) << QSizeF(400, 200) << QSizeF(300, 150);
    QTest::newRow("set 2") << QSizeF(400, 50) << QSizeF(100, 200) << QSizeF(0, 150);
    QTest::newRow("set 3") << QSizeF(100, 200) << QSizeF(400, 50) << QSizeF(300, 0);
}

void Ut_MPannableViewport::event()
{
    QEvent *event = new QEvent(QEvent::LayoutRequest);

    QFETCH(QSizeF, viewportSize);
    QFETCH(QSizeF, pannedSize);
    QFETCH(QSizeF, physicsRange);

    QGraphicsWidget *widget = new QGraphicsWidget();
    // Forcing the size of panned widget
    widget->setMinimumSize(pannedSize);
    widget->setPreferredSize(pannedSize);
    widget->setMaximumSize(pannedSize);

    // Forcing the size of subject
    subject->setMinimumSize(viewportSize);
    subject->setPreferredSize(viewportSize);
    subject->setMaximumSize(viewportSize);

    subject->setWidget(widget);

    subject->event(event);

    QCOMPARE(subject->physics()->range().size(), physicsRange);
}

void Ut_MPannableViewport::updatePosition()
{

    QGraphicsWidget *widget = new QGraphicsWidget();
    widget->setMinimumSize(QSizeF(10,10));
    widget->setMaximumSize(QSizeF(10,10));
    subject->setWidget(widget);

    // Forcing the size of subject to some value
    subject->setMinimumSize(QSizeF(500, 300));
    subject->setMaximumSize(QSizeF(500, 300));
    subject->adjustSize();

    QSignalSpy spy(subject, SIGNAL(positionChanged(QPointF)));

    subject->physics()->setPosition(QPointF(0,75));
    subject->layout()->activate();

    QCOMPARE(static_cast<MPannableViewportPrivate *>(subject->d_ptr)->pannedWidget->pos(), -QPointF(0, 75));

    QCOMPARE(spy.count(), 1);

}

void Ut_MPannableViewport::updateSamePosition()
{
    QGraphicsWidget *mainWidget = new QGraphicsWidget();
    QPointF point(0.0, 0.0);

    subject->setMinimumSize(100, 480);
    subject->setPreferredSize(100, 480);
    subject->setMaximumSize(100, 480);

    mainWidget->setMinimumSize(100, 1000);
    mainWidget->setPreferredSize(100, 1000);
    mainWidget->setMaximumSize(100, 1000);

    subject->setWidget(mainWidget);

    subject->updatePosition(point);

    QSignalSpy spy(subject,
                   SIGNAL(positionChanged(QPointF)));

    subject->updatePosition(point);

    // Should not have emitted anything since nothing changed
    QCOMPARE(spy.count(), 0);
}

/*
 * While the pannedWidget is populated, sizePosChanged() signal should be
 * emitted only once for each actual change in pannedWidget's size.
 *
 * See NB#143428
 */
void Ut_MPannableViewport::sizePosChangedAfterPopulatingPannedWidget()
{
    QGraphicsWidget *mainWidget = new QGraphicsWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsWidget *childWidget;

    //Settle initial layout position
    subject->adjustSize();

    mainWidget->setLayout(layout);

    subject->setWidget(mainWidget);

    QSignalSpy spyRange(subject,
                   SIGNAL(rangeChanged(QRectF)));
    QSignalSpy spyViewportSize(subject,
                   SIGNAL(viewportSizeChanged(QSizeF)));
    QSignalSpy spyPosition(subject,
                   SIGNAL(positionChanged(QPointF)));


    for (int i = 0; i < 30; i++) {
        childWidget = new QGraphicsWidget;

        childWidget->setMinimumSize(100, 200);
        childWidget->setPreferredSize(100, 200);

        layout->addItem(childWidget);
    }

    // Force layout to work.
    subject->adjustSize();

    // Check consecutive signals (if any), are different from each other.
    // We don't want to send out the very same event twice.
    for (int i = 1; i < spyRange.size(); i++) {
        QVERIFY(spyRange.at(i) != spyRange.at(i - 1));
    }
    for (int i = 1; i < spyPosition.size(); i++) {
        QVERIFY(spyPosition.at(i) != spyPosition.at(i - 1));
    }
    for (int i = 1; i < spyViewportSize.size(); i++) {
        QVERIFY(spyViewportSize.at(i) != spyViewportSize.at(i - 1));
    }
}

void Ut_MPannableViewport::settingNewPositionIndicator()
{
    MPositionIndicator* newPositionIndicator = new MPositionIndicator();
    subject->setPositionIndicator(newPositionIndicator);

    QVERIFY(subject->positionIndicator() == newPositionIndicator);
}

void Ut_MPannableViewport::settingNULLPositionIndicatorShouldNotBeAccepted()
{
    subject->setPositionIndicator(NULL);

    QVERIFY(subject->positionIndicator() != NULL);
}

void Ut_MPannableViewport::disabledViewportShouldNotAllowWigetToBeBiggerThanViewport()
{
    subject->setVerticalPanningPolicy(MPannableWidget::PanningAlwaysOff);
    subject->setHorizontalPanningPolicy(MPannableWidget::PanningAlwaysOff);

    QGraphicsWidget *widget = new QGraphicsWidget();
    widget->setPreferredSize(1000,1000);
    subject->setWidget(widget);


    // Forcing the size of subject to some value
    subject->setMinimumSize(QSizeF(500, 300));
    subject->setMaximumSize(QSizeF(500, 300));

    subject->adjustSize();

    QCOMPARE(widget->size(), QSizeF(500,300));
    subject->setVerticalPanningPolicy(MPannableWidget::PanningAlwaysOn);
    subject->setHorizontalPanningPolicy(MPannableWidget::PanningAlwaysOff);

}

void Ut_MPannableViewport::rangeOfViewportShouldBeSizeOfPannedWidgetMinusViewportSize()
{
    // Forcing the size of subject to some value
    subject->setMinimumSize(QSizeF(500, 300));
    subject->setMaximumSize(QSizeF(500, 300));

    QGraphicsWidget *widget = new QGraphicsWidget();
    widget->setMinimumSize(1000,1000);
    subject->setWidget(widget);

    subject->adjustSize();

    QCOMPARE(subject->range(), QRectF(0,0,500,700));
}

void Ut_MPannableViewport::testSetClipping()
{
    subject->setClipping(true);
    QCOMPARE(subject->hasClipping(), true);
    subject->setClipping(false);
    QCOMPARE(subject->hasClipping(), false);
}

void Ut_MPannableViewport::testRangeSetting()
{
    subject->setAutoRange(false);
    QCOMPARE(subject->autoRange(), false);

    subject->setRange(QRectF(0,0,100,100));
    QCOMPARE(subject->range(), QRectF(0,0,100,100));
}

void Ut_MPannableViewport::testExtendedRange_data()
{
    QTest::addColumn<qreal>("sipHeightExtension");
    QTest::addColumn<bool>("autoRange");
    QTest::addColumn<qreal>("verticalRange");
    QTest::addColumn<qreal>("expectedVerticalRange");

    QTest::newRow("Zero range") << (qreal)0.0 << false << (qreal)0.0 << (qreal)0.0;
    QTest::newRow("sip extension ") << (qreal)2.0 << true << (qreal)1.0 << (qreal)2.0;
    QTest::newRow("sip extension ignored because of disabled autorange") << (qreal)2.0 << false << (qreal)1.0 << (qreal)1.0;
}

void Ut_MPannableViewport::testExtendedRange()
{
    QFETCH(qreal, sipHeightExtension);
    QFETCH(bool, autoRange);
    QFETCH(qreal, verticalRange);
    QFETCH(qreal, expectedVerticalRange);

    QGraphicsScene scene;
    scene.addItem(subject);

    const QSizeF viewportSize(100, 100);
    subject->resize(viewportSize);
    subject->setMinimumSize(viewportSize);
    subject->setMaximumSize(viewportSize);
    subject->setAutoRange(autoRange);

    QGraphicsWidget *widget = new QGraphicsWidget();
    widget->setPreferredSize(1000,1000);
    subject->setWidget(widget);

    subject->adjustSize();

    subject->setRange(QRectF(QPointF(), QSizeF(0, verticalRange)));

    // Set area occupied by input method area.
    const QRect imArea(subject->geometry().adjusted(0, (viewportSize.height() - sipHeightExtension),
                                                    0, 0).toRect());
    subject->d_func()->setInputMethodArea(imArea);

    const qreal actualVerticalRange = subject->range().height();

    // Remove from scene before possible return/fail from QCOMPARE.
    scene.removeItem(subject);

    QCOMPARE(actualVerticalRange, expectedVerticalRange);
}

void Ut_MPannableViewport::testSizeHint_data() {
    QTest::addColumn<QString>("cssName");
    QTest::addColumn<qreal>("width");
    QTest::addColumn<qreal>("height");

    QTest::newRow("No CSS size") << QString("noCssSize") << (qreal)100 << (qreal)100;
    QTest::newRow("CSS width of 50") << QString("width50") << (qreal)50 << (qreal)50;
    QTest::newRow("CSS height of 50") << QString("height50") << (qreal)100 << (qreal)50;
    QTest::newRow("CSS width and height of 50") << QString("size50") << (qreal)50 << (qreal)50;
}

void Ut_MPannableViewport::testSizeHint()
{
    struct SquareWidget : public QGraphicsWidget
    {
        SquareWidget() {
            QSizePolicy policy = sizePolicy();
            policy.setHeightForWidth(true);
            setSizePolicy(policy);
        }
        QSizeF sizeHint ( Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const {
           if (which != Qt::PreferredSize || constraint.width() == -1)
              return QGraphicsWidget::sizeHint(which, constraint);
           else
              return QSizeF( constraint.width(), constraint.width() );
        }
    };

    QFETCH(QString, cssName);
    QFETCH(qreal, width);
    QFETCH(qreal, height);

    QGraphicsWidget *widget = new SquareWidget();
    widget->setPreferredWidth(100);
    subject->setWidget(widget);
    subject->setObjectName(cssName);

    QCOMPARE(widget->preferredSize(), QSizeF(100,100));
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), QSizeF(200,200));

    QCOMPARE(subject->layout()->preferredSize(), QSizeF(100,100));
    QCOMPARE(subject->layout()->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), QSizeF(200,200));

    QCOMPARE(subject->preferredSize(), QSizeF(width,height));
    if (cssName == "noCssSize")
        QCOMPARE(subject->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), QSizeF(200,200));

    // Test that the minimum and maximum size of the viewport is the same as the widget if we cannot pan in that direction
    widget->setMinimumSize(30,30);
    widget->setMaximumSize(200,200);

    QCOMPARE(subject->maximumSize(), QSizeF(200,200));

    subject->setPanDirection(0); //Can't pan in either direction
    QCOMPARE(subject->minimumSize(), QSizeF(30,30));
    subject->setPanDirection(Qt::Vertical);
    QCOMPARE(subject->minimumSize(), QSizeF(30,0));
    subject->setPanDirection(Qt::Horizontal);
    QCOMPARE(subject->minimumSize(), QSizeF(0,30));
    subject->setPanDirection(Qt::Horizontal | Qt::Vertical);
    QCOMPARE(subject->minimumSize(), QSizeF(0,0));

    for (int panDirection = 0; panDirection <= (Qt::Vertical | Qt::Horizontal); ++panDirection) {
        subject->setPanDirection((Qt::Orientations)panDirection);
        QCOMPARE(subject->preferredSize(), QSizeF(width,height));
        QCOMPARE(subject->maximumSize(), QSizeF(200,200));
        subject->setPreferredWidth(200);
        if (cssName == "noCssSize" || cssName == "width50")
            QCOMPARE(subject->preferredSize(), QSizeF(200,200));
        else
            QCOMPARE(subject->preferredSize(), QSizeF(200,50));
        subject->setPreferredWidth(-1);
    }
}

void Ut_MPannableViewport::testPositionIndicatorPositionAfterReenabled()
{
    subject->resize(50,1000);;
    subject->setPosition(QPointF(0.0, 0.0));
    subject->positionIndicator()->setEnabled(false);
    subject->setPosition(QPointF(0.0, 1000.0));
    subject->positionIndicator()->setEnabled(true);
    QCOMPARE(subject->position(), subject->positionIndicator()->position());
}

void Ut_MPannableViewport::testPositionIndicatorRangeAfterReenabled()
{
    subject->setAutoRange(false);
    subject->setRange(QRectF(0,0,100,100));
    subject->positionIndicator()->setEnabled(false);
    subject->setRange(QRectF(0,0,100,1000));
    subject->positionIndicator()->setEnabled(true);
    QCOMPARE(subject->range(), subject->positionIndicator()->range());
}

void Ut_MPannableViewport::testPositionIndicatorViewportSizeAfterReenabled()
{
    subject->positionIndicator()->setEnabled(false);
    subject->resize(500,500);
    subject->positionIndicator()->setEnabled(true);
    QCOMPARE(subject->size(), subject->positionIndicator()->viewportSize());
}

QTEST_APPLESS_MAIN(Ut_MPannableViewport)
