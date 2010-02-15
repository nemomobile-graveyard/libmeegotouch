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

#include "ut_duiapplethandleview.h"

#include <QtTest/QtTest>
#include <QLocalServer>
#include <QAction>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <duiappletcommunicator.h>
#include <duiapplication.h>
#include <duiprogressindicator.h>
#include "duimessagebox.h"
#include "duisettingslanguagebinary.h"
#include "duisettingslanguagewidgetfactory.h"
#include "duisettingslanguagewidget.h"
#include <duiappletsettings.h>
#include <duiappletsettingsdialog.h>
#include <duiappletsharedmutex_stub.h>
#include <DuiButton>
#include <DuiLocale>
#include <DuiSceneManager>
#include <QGraphicsLinearLayout>
#include <duiapplethandle_stub.h>
#include "duiclassfactory.h"
#include "duiwidgetview_p.h"
#include <duiapplethandle_stub.h>
#include <duiappletid_stub.h>

class DuiApplicationWindow;

DuiApplication *app;
int Ut_DuiAppletHandleView::returnedPixmapWidth;
int Ut_DuiAppletHandleView::returnedPixmapHeight;
QRectF Ut_DuiAppletHandleView::updatedRect;
QRectF Ut_DuiAppletHandleView::drawnTargetRect;
QRectF Ut_DuiAppletHandleView::drawnSourceRect;

QPointF mousePressPosition;
QPointF mouseReleasePosition;
QPointF mouseMovePosition;

// DuiClassFactory stubs
void DuiClassFactory::registerViewCreator(DuiViewCreatorBase *, const char *)
{
}

void DuiClassFactory::unregisterViewCreator(DuiViewCreatorBase *)
{
}

// DuiWidgetViewPrivate stubs
DuiWidgetViewPrivate::DuiWidgetViewPrivate() :
    controller(NULL),
    model(NULL),
    styleContainer(NULL),
    showAnimation(NULL),
    hideAnimation(NULL)
{
}

DuiWidgetViewPrivate::~DuiWidgetViewPrivate()
{
}

void DuiWidgetViewPrivate::orientationChanged()
{
    // notify views that the style has changed
    q_ptr->applyStyle();
}

// QGraphicsItem stubs (Used by DuiAppletHandleView)
void QGraphicsItem::update(const QRectF &rect)
{
    Ut_DuiAppletHandleView::updatedRect = rect;
}

// QThread stubs (used by DuiImageLoader)
void QThread::start(QThread::Priority)
{
}

// QPixmap stubs (used by AppletHandle)
bool QPixmap::isNull() const
{
    return false;
}

int QPixmap::width() const
{
    return Ut_DuiAppletHandleView::returnedPixmapWidth;
}

int QPixmap::height() const
{
    return Ut_DuiAppletHandleView::returnedPixmapHeight;
}

void QPainter::drawPixmap(const QRectF &targetRect, const QPixmap &, const QRectF &sourceRect)
{
    Ut_DuiAppletHandleView::drawnTargetRect = targetRect;
    Ut_DuiAppletHandleView::drawnSourceRect = sourceRect;
}

#ifdef Q_WS_X11
Qt::HANDLE QPixmap::handle() const
{
    return 0;
}
#endif

// DuiStyleContainer stubs
DuiAppletHandleStyleContainer handleStyleContainer;
DuiAppletHandleStyle *handleStyle = 0;
const DuiStyle *DuiStyleContainer::currentStyle() const
{
    return handleStyle;
}

// DuiWidgetView stubs
DuiAppletHandleModel handleModel;
DuiWidgetModel *DuiWidgetView::model()
{
    return &handleModel;
}

const DuiWidgetModel *DuiWidgetView::model() const
{
    return &handleModel;
}

DuiWidgetStyleContainer &DuiWidgetView::style()
{
    return handleStyleContainer;
}

const DuiWidgetStyleContainer &DuiWidgetView::style() const
{
    return handleStyleContainer;
}

DuiWidgetView::~DuiWidgetView()
{
}

void DuiWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    drawContents(painter, option);
}

// DuiProgressIndicator stubs
bool progressIndicatorRunning = false;

void DuiProgressIndicator::setUnknownDuration(bool run)
{
    if (DuiProgressIndicator::viewType() != DuiProgressIndicator::barType) {
        progressIndicatorRunning = run;
    } else {
        progressIndicatorRunning = (!run);
    }
}

bool progressIndicatorVisible = false;

// QGraphicsItem stubs
void QGraphicsItem::setVisible(bool visible)
{
    if (dynamic_cast<DuiProgressIndicator *>(this)) {
        progressIndicatorVisible = visible;
    }
}

// DuiSceneManager stubs
bool dialogShown = false;
int DuiSceneManager::execDialog(DuiDialog *)
{
    dialogShown = true;
    return 1;
}

void QGraphicsLinearLayout::insertItem(int , QGraphicsLayoutItem *)
{
}


// DuiDialog stubs
int dialogClickedButtonIndex = 0;
QList<DuiButtonModel *> dialogButtons;

void DuiDialogModel::addButton(DuiButtonModel *buttonModel)
{
    dialogButtons.append(buttonModel);
}

DuiButtonModel *DuiDialog::clickedButton() const
{
    if (dialogClickedButtonIndex >= 0)
        return dialogButtons.at(dialogClickedButtonIndex);
    else
        return NULL;
}

// DuiWidgetController stubs
void DuiWidgetController::setGeometry(const QRectF &)
{
}

// A test applet handle implementation to allow mouse event sending
DuiTestAppletHandleView::DuiTestAppletHandleView(DuiAppletHandle *handle) :
    DuiAppletHandleView(handle)
{
}

void DuiTestAppletHandleView::click()
{
    QGraphicsSceneMouseEvent event;
    event.setButton(Qt::LeftButton);
    mouseReleaseEvent(&event);
}

QSizeF DuiTestAppletHandleView::askSizeHints(Qt::SizeHint which, QSizeF &constraint)
{
    return sizeHint(which, constraint);
}

void DuiTestAppletHandleView::appletPixmapModified(const QRectF &rect)
{
    DuiAppletHandleView::appletPixmapModified(rect);
}

// QTimer stubs
bool timerCalled = false;

void QTimer::stop()
{
    timerCalled = true;
}

// DuiSettingsLanguageWidgetFactory stubs
DuiSettingsLanguageWidget *DuiSettingsLanguageWidgetFactory::createWidget(const DuiSettingsLanguageBinary &, DuiDataStore *)
{
    return new DuiSettingsLanguageWidget;
}

// DuiAppletSettingsDialog stubs
bool gDuiAppletSettingsDialogExecCalled = false;
void DuiAppletSettingsDialog::exec() const
{
    gDuiAppletSettingsDialogExecCalled = true;
}

// Unit test cases
void Ut_DuiAppletHandleView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiapplethandleview" };
    app = new DuiApplication(argc, app_name);
}


void Ut_DuiAppletHandleView::cleanupTestCase()
{
    delete app;
}

void Ut_DuiAppletHandleView::init()
{
    handle = new DuiAppletHandle();
    handleView = new DuiTestAppletHandleView(handle);
    handleStyle = new DuiAppletHandleStyle();

    updatedRect = QRectF();
    drawnTargetRect = QRectF();
    drawnSourceRect = QRectF();
    progressIndicatorRunning = false;
    dialogShown = false;
    dialogButtons.clear();
    dialogClickedButtonIndex = 0;
    gDuiAppletSettingsDialogExecCalled = false;
    progressIndicatorVisible = false;
    returnedPixmapWidth = 0;
    returnedPixmapHeight = 0;

    gDuiAppletHandleStub->stubReset();
    gDuiAppletSharedMutexStub->stubReset();
    gDuiAppletSharedMutexStub->stubSetReturnValue("lock", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("unlock", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("tryLock", true);
    handleStyle->setMaximumAppletSize(QSizeF(1000, 1000));
}

void Ut_DuiAppletHandleView::cleanup()
{
    delete handleView;
    delete handle;
}

void Ut_DuiAppletHandleView::testScaling_data()
{
    QTest::addColumn<QSizeF>("maximumAppletSize");
    QTest::addColumn<QSizeF>("minimumSizeHint");
    QTest::addColumn<QRectF>("setGeometry");
    QTest::addColumn<QRectF>("appletGeometry");
    QTest::addColumn<QRectF>("boundingRect");

    QTest::newRow("Size OK") << QSizeF(500, 300) << QSizeF(250, 150) << QRectF(0, 0, 250, 150) << QRectF(0, 0, 250, 150) << QRectF(0, 0, 250, 150);
    QTest::newRow("Minimum size over") << QSizeF(500, 300) << QSizeF(1000, 600) << QRectF(0, 0, 500, 300) << QRectF(0, 0, 1000, 600) << QRectF(0, 0, 500, 300);
}

void Ut_DuiAppletHandleView::testScaling()
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
    handleStyle->setMaximumAppletSize(maximumAppletSize);
    QVector<QSizeF> sizeHints;
    sizeHints.append(minimumSizeHint);
    sizeHints.append(QSizeF(20, 20));
    sizeHints.append(QSizeF(800, 800));
    sizeHints.append(QSizeF(800, 800));
    handleModel.setSizeHints(sizeHints);

    // The pixmap size sent to the applet should be scaled
    handleView->setGeometry(setGeometry);
    QCOMPARE(gDuiAppletHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), appletGeometry);

    // When the pixmap has been taken into use the bounding rect should be the pixmap size scaled
    returnedPixmapWidth = appletGeometry.width();
    returnedPixmapHeight = appletGeometry.height();
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));
    QCOMPARE(handleView->boundingRect(), boundingRect);
}

void Ut_DuiAppletHandleView::testSetGeometry()
{
    // Reset the state to a known state for testing make sure there is no pixmap to be taken into use.
    handleView->setGeometry(QRectF(0, 0, 1, 1));
    handleView->setGeometry(QRectF(0, 0, 2, 2));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // The first setGeometry should go through normally: after this there's a pixmap to be taken into use
    handleView->setGeometry(QRectF(0, 0, 300, 150));
    QCOMPARE(gDuiAppletHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), QRectF(0, 0, 300, 150));

    // The second and thid setGeometrys should get queued because the pixmap has not been taken into use yet
    handleView->setGeometry(QRectF(0, 0, 350, 175));
    handleView->setGeometry(QRectF(0, 0, 400, 200));
    QCOMPARE(gDuiAppletHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), QRectF(0, 0, 300, 150));

    // Mark the pixmap as taken into use: this will take the latest size into use.
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // The current geometry should now have been sent
    QCOMPARE(gDuiAppletHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), QRectF(0, 0, 400, 200));
}

void Ut_DuiAppletHandleView::testReturnedSizeHintManipulation()
{
    // Test size hints returned when applet size is smaller than minimum size of applet defined in style
    handleStyle->setMinimumAppletSize(QSizeF(50, 50));
    handleStyle->setMaximumAppletSize(QSizeF(200, 200));
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

void Ut_DuiAppletHandleView::testStateFeedback()
{
    handleView->setModel(&handleModel);

    // test that the progress indicator is not visible by default
    QVERIFY(!progressIndicatorVisible);

    // test that the progress indicator is running and visible in the possible applet handle states
    handleModel.setState(DuiAppletHandleModel::RUNNING);
    QVERIFY(!progressIndicatorVisible);
    QVERIFY(!progressIndicatorRunning);
    handleModel.setState(DuiAppletHandleModel::STOPPED);
    QVERIFY(!progressIndicatorVisible);
    QVERIFY(!progressIndicatorRunning);
    handleModel.setState(DuiAppletHandleModel::BROKEN);
    QVERIFY(!progressIndicatorVisible);
    QVERIFY(!progressIndicatorRunning);
    handleModel.setState(DuiAppletHandleModel::STARTING);
    QVERIFY(progressIndicatorVisible);
    QVERIFY(progressIndicatorRunning);
    handleModel.setState(DuiAppletHandleModel::INSTALLING);
    QVERIFY(progressIndicatorVisible);
    QVERIFY(progressIndicatorRunning);
}

void Ut_DuiAppletHandleView::testBrokenAppletDialog()
{
    // Set the geometry of the applet to a certain size
    QRectF geometry(0, 0, 50, 50);
    handleView->setGeometry(geometry);
    QCOMPARE(gDuiAppletHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), geometry);
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    // test that clicking the applet view in non broken state doesn't show a dialog
    handleView->click();
    QVERIFY(!dialogShown);

    // test that clicking a broken applet view will show a dialog
    handleModel.setState(DuiAppletHandleModel::BROKEN);
    handleView->click();
    QVERIFY(dialogShown);

    // test that accepting the dialog will reinitialize the applet handle
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("reinit"), 1);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 0);

    // Test that setting the same geometry again will send the pixmap ID and the geometry
    handleModel.setState(DuiAppletHandleModel::RUNNING);
    handleView->setGeometry(geometry);
    QCOMPARE(gDuiAppletHandleStub->stubLastCallTo("sendGeometryMessage").parameter<QRectF>(0), geometry);

    // test that rejecting the dialog will remove the applet handle
    gDuiAppletHandleStub->stubReset();
    dialogShown = false;
    dialogButtons.clear();
    handleModel.setState(DuiAppletHandleModel::BROKEN);
    dialogClickedButtonIndex = 1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 1);

    // test that canceling the dialog will do nothing
    gDuiAppletHandleStub->stubReset();
    dialogShown = false;
    dialogButtons.clear();
    handleModel.setState(DuiAppletHandleModel::BROKEN);
    dialogClickedButtonIndex = -1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 0);
}

void Ut_DuiAppletHandleView::testPixmapCleanup()
{
    // set the geometry, so a pixmap is created for the view
    handleView->setGeometry(QRectF(0, 0, 10, 10));
}

void Ut_DuiAppletHandleView::testSettingsDialog()
{
    handleView->setModel(&handleModel);
    connect(this, SIGNAL(openAppletSettings()), handleView, SLOT(openAppletSettings()));

    // Check that settings dialog is not executed when applet settings is not set
    emit openAppletSettings();
    QVERIFY(!gDuiAppletSettingsDialogExecCalled);

    // Check that settings dialog is executed when applet settings is set
    DuiAppletId appletId("applicationName", "mashupCanvasName", 1);
    handleModel.setAppletSettings(new DuiAppletSettings("metaDataFilename", appletId));
    emit openAppletSettings();
    QVERIFY(gDuiAppletSettingsDialogExecCalled);
}

void Ut_DuiAppletHandleView::testPixmapModified()
{
    DuiAppletSharedMutex mutex;
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

void Ut_DuiAppletHandleView::testInstallationFailedDialog()
{
    // Set the geometry of the applet to a certain size
    QRectF geometry(0, 0, 50, 50);
    handleView->setGeometry(geometry);
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    handleModel.setInstallationError("");

    // test that clicking the applet view in non broken state doesn't show a dialog
    handleModel.setState(DuiAppletHandleModel::RUNNING);
    handleView->click();
    QVERIFY(!dialogShown);

    // test that clicking a broken applet view will show a dialog with one remove button
    handleModel.setInstallationError("CRITICAL ERROR");
    handleModel.setState(DuiAppletHandleModel::BROKEN);
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(dialogButtons.count(), 1);

    // test that clicking on the button removes the applet
    dialogClickedButtonIndex = -1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 1);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletHandleView)
