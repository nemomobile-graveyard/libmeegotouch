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
#include <duiextensionhandle_stub.h>
#include <duiapplethandle_stub.h>
#include <duiappletid_stub.h>
#include "duiclassfactory.h"
#include "duiwidgetview_p.h"

class DuiApplicationWindow;

DuiApplication *app;

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


DuiAppletHandleModel handleModel;

// DuiSceneManager stubs
bool dialogShown = false;
int DuiSceneManager::execDialog(DuiDialog *)
{
    dialogShown = true;
    return 1;
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

void QGraphicsLinearLayout::insertItem(int , QGraphicsLayoutItem *)
{
}


// DuiWidgetController stubs
void DuiWidgetController::setGeometry(const QRectF &)
{
}

// DuiSettingsLanguageWidgetFactory stubs
DuiSettingsLanguageWidget *DuiSettingsLanguageWidgetFactory::createWidget(const DuiSettingsLanguageBinary &, DuiDataStore *)
{
    return new DuiSettingsLanguageWidget;
}

// DuiAppletSettingsDialog stubs
bool gDuiAppletSettingsDialogExecCalled = false;
void DuiAppletSettingsDialog::exec(const DuiAppletSettings&) const {
    gDuiAppletSettingsDialogExecCalled = true;
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
    handleView->setModel(&handleModel);
    connect(this, SIGNAL(openAppletSettings()), handleView, SLOT(openAppletSettings()));

    dialogShown = false;
    progressIndicatorRunning = false;
    gDuiAppletSettingsDialogExecCalled = false;
    progressIndicatorVisible = false;
    dialogButtons.clear();
    dialogClickedButtonIndex = 0;
    gDuiAppletHandleStub->stubReset();
    gDuiExtensionHandleStub->stubReset();
}

void Ut_DuiAppletHandleView::cleanup()
{
    delete handleView;
    delete handle;
}

void Ut_DuiAppletHandleView::testSettingsDialog()
{
    // Check that settings dialog is not executed when applet settings is not set
    emit openAppletSettings();
    QVERIFY(!gDuiAppletSettingsDialogExecCalled);

    // Check that settings dialog is executed when applet settings is set
    DuiAppletId appletId("applicationName", "mashupCanvasName", 1);
    handleModel.setAppletSettings(new DuiAppletSettings("metaDataFilename", appletId));
    emit openAppletSettings();
    QVERIFY(gDuiAppletSettingsDialogExecCalled);
}

void Ut_DuiAppletHandleView::testBrokenAppletDialog()
{
    // Test that clicking the applet view in non broken state doesn't show a dialog
    handleView->click();
    QVERIFY(!dialogShown);

    // Test that clicking a broken applet view will show a dialog
    handleModel.setCurrentState(DuiAppletHandleModel::BROKEN);
    handleView->click();
    QVERIFY(dialogShown);

    // Test that accepting the dialog will reinitialize the applet handle
    QCOMPARE(gDuiExtensionHandleStub->stubCallCount("reinit"), 1);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 0);

    // Return to running state
    handleModel.setCurrentState(DuiAppletHandleModel::RUNNING);

    // Test that rejecting the dialog will remove the applet handle
    gDuiAppletHandleStub->stubReset();
    gDuiExtensionHandleStub->stubReset();
    dialogShown = false;
    dialogButtons.clear();
    handleModel.setCurrentState(DuiAppletHandleModel::BROKEN);
    dialogClickedButtonIndex = 1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gDuiExtensionHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 1);

    // Test that canceling the dialog will do nothing
    gDuiAppletHandleStub->stubReset();
    gDuiExtensionHandleStub->stubReset();
    dialogShown = false;
    dialogButtons.clear();
    handleModel.setCurrentState(DuiAppletHandleModel::BROKEN);
    dialogClickedButtonIndex = -1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gDuiExtensionHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 0);
}

void Ut_DuiAppletHandleView::testInstallationFailedDialog()
{
    // Set the geometry of the applet to a certain size
    QRectF geometry(0, 0, 50, 50);
    handleView->setGeometry(geometry);
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    handleModel.setInstallationError("");

    // test that clicking the applet view in non broken state doesn't show a dialog
    handleModel.setCurrentState(DuiAppletHandleModel::RUNNING);
    handleView->click();
    QVERIFY(!dialogShown);

    // test that clicking a broken applet view will show a dialog with one remove button
    handleModel.setInstallationError("CRITICAL ERROR");
    handleModel.setCurrentState(DuiAppletHandleModel::BROKEN);
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(dialogButtons.count(), 1);

    // test that clicking on the button removes the applet
    dialogClickedButtonIndex = -1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gDuiExtensionHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gDuiAppletHandleStub->stubCallCount("removeApplet"), 1);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletHandleView)
