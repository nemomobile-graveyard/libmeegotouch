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

#include "ut_mextensionhandleview.h"

#include <QtTest/QtTest>
#include <QLocalServer>
#include <QAction>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <mappletcommunicator.h>
#include <mapplication.h>
#include <mprogressindicator.h>
#include "mmessagebox.h"
#include "msettingslanguagebinary.h"
#include "msettingslanguagewidgetfactory.h"
#include "msettingslanguagewidget.h"
#include <mappletsettings.h>
#include <mappletsettingsdialog.h>
#include <mappletsharedmutex_stub.h>
#include <MButton>
#include <MLocale>
#include <MSceneManager>
#include <QGraphicsLinearLayout>
#include <mextensionhandle_stub.h>
#include "mclassfactory.h"
#include "mwidgetview_p.h"
#include <mextensionhandle_stub.h>

class MApplicationWindow;

MApplication *app;
int Ut_MExtensionHandleView::returnedPixmapWidth;
int Ut_MExtensionHandleView::returnedPixmapHeight;
QRectF Ut_MExtensionHandleView::updatedRect;
QRectF Ut_MExtensionHandleView::drawnTargetRect;
QRectF Ut_MExtensionHandleView::drawnSourceRect;

QPointF mousePressPosition;
QPointF mouseReleasePosition;
QPointF mouseMovePosition;

// MClassFactory stubs
void MClassFactory::registerViewCreator(MViewCreatorBase *, const char *)
{
}

void MClassFactory::unregisterViewCreator(MViewCreatorBase *)
{
}

// MWidgetViewPrivate stubs
MWidgetViewPrivate::MWidgetViewPrivate() :
        q_ptr(NULL),
        controller(NULL),
        model(NULL),
        styleContainer(NULL),
        appliedStyle(NULL)
{
}

MWidgetViewPrivate::~MWidgetViewPrivate()
{
}

// QGraphicsItem stubs (Used by MExtensionHandleView)
void QGraphicsItem::update(const QRectF &rect)
{
    Ut_MExtensionHandleView::updatedRect = rect;
}

// QThread stubs (used by MImageLoader)
void QThread::start(QThread::Priority)
{
}

// QPixmap stubs (used by ExtensionHandle)
bool QPixmap::isNull() const
{
    return false;
}

int QPixmap::width() const
{
    return Ut_MExtensionHandleView::returnedPixmapWidth;
}

int QPixmap::height() const
{
    return Ut_MExtensionHandleView::returnedPixmapHeight;
}

void QPainter::drawPixmap(const QRectF &targetRect, const QPixmap &, const QRectF &sourceRect)
{
    Ut_MExtensionHandleView::drawnTargetRect = targetRect;
    Ut_MExtensionHandleView::drawnSourceRect = sourceRect;
}

#ifdef Q_WS_X11
Qt::HANDLE QPixmap::handle() const
{
    return 0;
}
#endif

// MStyleContainer stubs
MExtensionHandleStyleContainer handleStyleContainer;
MExtensionHandleStyle *handleStyle = 0;
const MStyle *MStyleContainer::currentStyle() const
{
    return handleStyle;
}

// MWidgetView stubs
MExtensionHandleModel handleModel;
MWidgetModel *MWidgetView::model()
{
    return &handleModel;
}

const MWidgetModel *MWidgetView::model() const
{
    return &handleModel;
}

MWidgetStyleContainer &MWidgetView::style()
{
    return handleStyleContainer;
}

const MWidgetStyleContainer &MWidgetView::style() const
{
    return handleStyleContainer;
}

MWidgetView::~MWidgetView()
{
}

void MWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    drawContents(painter, option);
}

// MProgressIndicator stubs
bool progressIndicatorRunning = false;

void MProgressIndicator::setUnknownDuration(bool run)
{
    if (MProgressIndicator::viewType() != MProgressIndicator::barType) {
        progressIndicatorRunning = run;
    } else {
        progressIndicatorRunning = (!run);
    }
}

bool progressIndicatorVisible = false;

// QGraphicsItem stubs

void QGraphicsItem::setVisible(bool visible)
{
    if (dynamic_cast<MProgressIndicator *>(this)) {
        progressIndicatorVisible = visible;
    }
}

// MWidget stubs

void MWidget::setVisible(bool visible)
{
    if (dynamic_cast<MProgressIndicator *>(this)) {
        progressIndicatorVisible = visible;
    }
}

void QGraphicsLinearLayout::insertItem(int , QGraphicsLayoutItem *)
{
}


// MWidgetController stubs
void MWidgetController::setGeometry(const QRectF &)
{
}

// A test applet handle implementation to allow mouse event sending
MTestExtensionHandleView::MTestExtensionHandleView(MExtensionHandle *handle) :
        MExtensionHandleView(*new MTestExtensionHandleViewPrivate(handle), handle),
        brokenDialogShown(false),
        pixmapDrawn(false)
{
}

void MTestExtensionHandleView::click()
{
    QGraphicsSceneMouseEvent event;
    event.setButton(Qt::LeftButton);
    mouseReleaseEvent(&event);
}

QSizeF MTestExtensionHandleView::askSizeHints(Qt::SizeHint which, QSizeF &constraint)
{
    return sizeHint(which, constraint);
}

void MTestExtensionHandleView::appletPixmapModified(const QRectF &rect)
{
    MExtensionHandleView::pixmapModified(rect);
}

MTestExtensionHandleViewPrivate::MTestExtensionHandleViewPrivate(MExtensionHandle *handle) :
    MExtensionHandleViewPrivate(handle)
{
}

MTestExtensionHandleViewPrivate::~MTestExtensionHandleViewPrivate()
{
}

void MTestExtensionHandleViewPrivate::showBrokenDialog()
{
    Q_Q(MTestExtensionHandleView);

    q->brokenDialogShown = true;
}

void MTestExtensionHandleViewPrivate::showInstallationFailedDialog(const QString &error)
{
    Q_Q(MTestExtensionHandleView);

    q->installationFailedDialogError = error;
}

void MTestExtensionHandleViewPrivate::drawPixmap(QPainter *, const QRectF &, const QRectF &, bool) const
{
    Q_Q(const MTestExtensionHandleView);

    q->pixmapDrawn = true;
}

// QTimer stubs
bool timerCalled = false;

void QTimer::stop()
{
    timerCalled = true;
}

// MSettingsLanguageWidgetFactory stubs
MSettingsLanguageWidget *MSettingsLanguageWidgetFactory::createWidget(const MSettingsLanguageBinary &, MDataStore *)
{
    return new MSettingsLanguageWidget;
}

// MAppletSettingsDialog stubs
bool gMAppletSettingsDialogExecCalled = false;
void MAppletSettingsDialog::exec(const MAppletSettings&) {
    gMAppletSettingsDialogExecCalled = true;
}

// Unit test cases
void Ut_MExtensionHandleView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplethandleview" };
    app = new MApplication(argc, app_name);
}


void Ut_MExtensionHandleView::cleanupTestCase()
{
    delete app;
}

void Ut_MExtensionHandleView::init()
{
    handle = new MExtensionHandle();
    handleView = new MTestExtensionHandleView(handle);
    handleStyle = new MExtensionHandleStyle();

    updatedRect = QRectF();
    drawnTargetRect = QRectF();
    drawnSourceRect = QRectF();
    progressIndicatorRunning = false;
    gMAppletSettingsDialogExecCalled = false;
    progressIndicatorVisible = false;
    returnedPixmapWidth = 0;
    returnedPixmapHeight = 0;

    gMExtensionHandleStub->stubReset();
    gMAppletSharedMutexStub->stubReset();
    gMAppletSharedMutexStub->stubSetReturnValue("lock", true);
    gMAppletSharedMutexStub->stubSetReturnValue("unlock", true);
    gMAppletSharedMutexStub->stubSetReturnValue("tryLock", true);
    handleStyle->setForcedMaximumSize(QSizeF(1000, 1000));
}

void Ut_MExtensionHandleView::cleanup()
{
    delete handleView;
    delete handle;
}

void Ut_MExtensionHandleView::testScaling_data()
{
    QTest::addColumn<QSizeF>("maximumAppletSize");
    QTest::addColumn<QSizeF>("minimumSizeHint");
    QTest::addColumn<QRectF>("setGeometry");
    QTest::addColumn<QRectF>("appletGeometry");
    QTest::addColumn<QRectF>("boundingRect");

    QTest::newRow("Size OK") << QSizeF(500, 300) << QSizeF(250, 150) << QRectF(0, 0, 250, 150) << QRectF(0, 0, 250, 150) << QRectF(0, 0, 250, 150);
    QTest::newRow("Minimum size over") << QSizeF(500, 300) << QSizeF(1000, 600) << QRectF(0, 0, 500, 300) << QRectF(0, 0, 1000, 600) << QRectF(0, 0, 500, 300);
}

void Ut_MExtensionHandleView::testScaling()
{
    QFETCH(QSizeF, maximumAppletSize);
    QFETCH(QSizeF, minimumSizeHint);
    QFETCH(QRectF, setGeometry);
    QFETCH(QRectF, appletGeometry);
    QFETCH(QRectF, boundingRect);

    // Reset the state to a known state for testing make sure there is no pixmap to be taken into use.
    handleView->setGeometry(QRectF(0, 0, 1, 1));
    handleView->setGeometry(QRectF(0, 0, 2, 2));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // Test size hints returned when applet size is smaller than minimum size of applet defined in style
    handleStyle->setForcedMaximumSize(maximumAppletSize);
    QVector<QSizeF> sizeHints;
    sizeHints.append(minimumSizeHint);
    sizeHints.append(QSizeF(20, 20));
    sizeHints.append(QSizeF(800, 800));
    sizeHints.append(QSizeF(800, 800));
    handleModel.setSizeHints(sizeHints);

    // The pixmap size sent to the applet should be scaled
    handleView->setGeometry(setGeometry);
    QCOMPARE(gMExtensionHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), appletGeometry);

    // When the pixmap has been taken into use the bounding rect should be the pixmap size scaled
    returnedPixmapWidth = appletGeometry.width();
    returnedPixmapHeight = appletGeometry.height();
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));
    QCOMPARE(handleView->boundingRect(), boundingRect);
}

void Ut_MExtensionHandleView::testSetGeometry()
{
    // Reset the state to a known state for testing make sure there is no pixmap to be taken into use.
    handleView->setGeometry(QRectF(0, 0, 1, 1));
    handleView->setGeometry(QRectF(0, 0, 2, 2));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // The first setGeometry should go through normally: after this there's a pixmap to be taken into use
    handleView->setGeometry(QRectF(0, 0, 300, 150));
    QCOMPARE(gMExtensionHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), QRectF(0, 0, 300, 150));

    // The second and thid setGeometrys should get queued because the pixmap has not been taken into use yet
    handleView->setGeometry(QRectF(0, 0, 350, 175));
    handleView->setGeometry(QRectF(0, 0, 400, 200));
    QCOMPARE(gMExtensionHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), QRectF(0, 0, 300, 150));

    // Mark the pixmap as taken into use: this will take the latest size into use.
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // The current geometry should now have been sent
    QCOMPARE(gMExtensionHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), QRectF(0, 0, 400, 200));
}

void Ut_MExtensionHandleView::testReturnedSizeHintManipulation()
{
    // Test size hints returned when applet size is smaller than minimum size of applet defined in style
    handleStyle->setForcedMinimumSize(QSizeF(50, 50));
    handleStyle->setForcedMaximumSize(QSizeF(200, 200));
    QVector<QSizeF> sizeHints;
    sizeHints.append(QSizeF(20, 20));
    sizeHints.append(QSizeF(20, 20));
    sizeHints.append(QSizeF(800, 800));
    sizeHints.append(QSizeF(800, 800));
    handleModel.setSizeHints(sizeHints);

    // Check that reported size hints are between minimum and maximum size hints specified in style
    QSizeF constraint(-1, -1);
    QCOMPARE(handleView->askSizeHints(Qt::MinimumSize, constraint), QSizeF(50, 50));
    QCOMPARE(handleView->askSizeHints(Qt::MaximumSize, constraint), QSizeF(200, 200));
}

void Ut_MExtensionHandleView::testStateFeedback()
{
    handleView->setModel(&handleModel);

    // test that the progress indicator is not visible by default
    QVERIFY(!progressIndicatorVisible);

    // test that the progress indicator is running and visible in the possible applet handle states
    handleModel.setCurrentState(MExtensionHandleModel::RUNNING);
    QVERIFY(!progressIndicatorVisible);
    QVERIFY(!progressIndicatorRunning);
    handleModel.setCurrentState(MExtensionHandleModel::STOPPED);
    QVERIFY(!progressIndicatorVisible);
    QVERIFY(!progressIndicatorRunning);
    handleModel.setCurrentState(MExtensionHandleModel::BROKEN);
    QVERIFY(!progressIndicatorVisible);
    QVERIFY(!progressIndicatorRunning);
    handleModel.setCurrentState(MExtensionHandleModel::STARTING);
    QVERIFY(progressIndicatorVisible);
    QVERIFY(progressIndicatorRunning);
    handleModel.setCurrentState(MExtensionHandleModel::INSTALLING);
    QVERIFY(progressIndicatorVisible);
    QVERIFY(progressIndicatorRunning);
}

void Ut_MExtensionHandleView::testBrokenAppletDialog()
{
    // Set the geometry of the applet to a certain size
    QRectF geometry(0, 0, 50, 50);
    handleView->setGeometry(geometry);
    QCOMPARE(gMExtensionHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), geometry);
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // test that clicking the applet view in non broken state doesn't show a dialog
    handleView->click();
    QVERIFY(!handleView->brokenDialogShown);

    // test that clicking a broken applet view will show a dialog
    handleModel.setCurrentState(MExtensionHandleModel::BROKEN);
    handleView->click();
    QVERIFY(handleView->brokenDialogShown);
}

void Ut_MExtensionHandleView::testPixmapCleanup()
{
    // set the geometry, so a pixmap is created for the view
    handleView->setGeometry(QRectF(0, 0, 10, 10));
}

void Ut_MExtensionHandleView::testPixmapModified()
{
    MAppletSharedMutex mutex;
    handleModel.setPixmapMutex(NULL);
    handleView->setGeometry(QRectF(0, 0, 400, 200));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // Test one failing pixmap modification
    updatedRect = QRectF();
    handleView->appletPixmapModified(QRectF(0, 0, 10, 10));
    QCOMPARE(updatedRect, QRectF());
    QCOMPARE(drawnTargetRect, updatedRect);
    QCOMPARE(drawnSourceRect, updatedRect);

    // Test that unhandled consecutive modifications unite the regions
    handleModel.setPixmapMutex(&mutex);
    handleView->appletPixmapModified(QRectF(10, 10, 10, 10));
    QCOMPARE(updatedRect, QRectF(0, 0, 20, 20));
    QCOMPARE(drawnTargetRect, updatedRect);
    QCOMPARE(drawnSourceRect, updatedRect);

    // Test that the united region is cleared after the operation succeeds
    updatedRect = QRectF();
    handleView->appletPixmapModified(QRectF(0, 0, 10, 10));
    QCOMPARE(updatedRect, QRectF(0, 0, 10, 10));
    QCOMPARE(drawnTargetRect, updatedRect);
    QCOMPARE(drawnSourceRect, updatedRect);
}

void Ut_MExtensionHandleView::testInstallationFailedDialog()
{
    // Set the geometry of the applet to a certain size
    QRectF geometry(0, 0, 50, 50);
    handleView->setGeometry(geometry);
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    handleModel.setInstallationError("");

    // test that clicking the applet view in non broken state doesn't show a dialog
    handleModel.setCurrentState(MExtensionHandleModel::RUNNING);
    handleView->click();
    QVERIFY(!handleView->brokenDialogShown);

    // test that clicking a broken applet view will show a dialog
    handleModel.setInstallationError("CRITICAL ERROR");
    handleModel.setCurrentState(MExtensionHandleModel::BROKEN);
    handleView->click();
    QCOMPARE(handleView->installationFailedDialogError, QString("CRITICAL ERROR"));
}

QTEST_APPLESS_MAIN(Ut_MExtensionHandleView)
