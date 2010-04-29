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

#include "ut_mapplethandleview.h"

#include <QtTest/QtTest>
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
#include <mapplethandle_stub.h>
#include <mappletid_stub.h>
#include "mclassfactory.h"
#include "mwidgetview_p.h"

class MApplicationWindow;

MApplication *app;

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
        styleContainer(NULL)
{
}

MWidgetViewPrivate::~MWidgetViewPrivate()
{
}


MAppletHandleModel handleModel;

// MSceneManager stubs
bool dialogShown = false;
int MSceneManager::execDialog(MDialog *)
{
    dialogShown = true;
    return 1;
}

// MDialog stubs
int dialogClickedButtonIndex = 0;
QList<MButtonModel *> dialogButtons;

void MDialogModel::addButton(MButtonModel *buttonModel)
{
    dialogButtons.append(buttonModel);
}

MButtonModel *MDialog::clickedButton() const
{
    if (dialogClickedButtonIndex >= 0)
        return dialogButtons.at(dialogClickedButtonIndex);
    else
        return NULL;
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

void QGraphicsLinearLayout::insertItem(int , QGraphicsLayoutItem *)
{
}


// MWidgetController stubs
void MWidgetController::setGeometry(const QRectF &)
{
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

// A test applet handle implementation to allow mouse event sending
MTestAppletHandleView::MTestAppletHandleView(MAppletHandle *handle) :
        MAppletHandleView(handle)
{
}

void MTestAppletHandleView::click()
{
    QGraphicsSceneMouseEvent event;
    event.setButton(Qt::LeftButton);
    mouseReleaseEvent(&event);
}

// Unit test cases
void Ut_MAppletHandleView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplethandleview" };
    app = new MApplication(argc, app_name);
}


void Ut_MAppletHandleView::cleanupTestCase()
{
    delete app;
}

void Ut_MAppletHandleView::init()
{
    handle = new MAppletHandle();
    handleView = new MTestAppletHandleView(handle);
    handleView->setModel(&handleModel);
    connect(this, SIGNAL(openAppletSettings()), handleView, SLOT(openAppletSettings()));

    dialogShown = false;
    progressIndicatorRunning = false;
    gMAppletSettingsDialogExecCalled = false;
    progressIndicatorVisible = false;
    dialogButtons.clear();
    dialogClickedButtonIndex = 0;
    gMAppletHandleStub->stubReset();
    gMExtensionHandleStub->stubReset();
}

void Ut_MAppletHandleView::cleanup()
{
    delete handleView;
    delete handle;
}

void Ut_MAppletHandleView::testSettingsDialog()
{
    // Check that settings dialog is not executed when applet settings is not set
    emit openAppletSettings();
    QVERIFY(!gMAppletSettingsDialogExecCalled);

    // Check that settings dialog is executed when applet settings is set
    MAppletId appletId("applicationName", "mashupCanvasName", 1);
    handleModel.setAppletSettings(new MAppletSettings("metaDataFilename", appletId));
    emit openAppletSettings();
    QVERIFY(gMAppletSettingsDialogExecCalled);
}

void Ut_MAppletHandleView::testBrokenAppletDialog()
{
    // Test that clicking the applet view in non broken state doesn't show a dialog
    handleView->click();
    QVERIFY(!dialogShown);

    // Test that clicking a broken applet view will show a dialog
    handleModel.setCurrentState(MAppletHandleModel::BROKEN);
    handleView->click();
    QVERIFY(dialogShown);

    // Test that accepting the dialog will reinitialize the applet handle
    QCOMPARE(gMExtensionHandleStub->stubCallCount("reinit"), 1);
    QCOMPARE(gMAppletHandleStub->stubCallCount("removeApplet"), 0);

    // Return to running state
    handleModel.setCurrentState(MAppletHandleModel::RUNNING);

    // Test that rejecting the dialog will remove the applet handle
    gMAppletHandleStub->stubReset();
    gMExtensionHandleStub->stubReset();
    dialogShown = false;
    dialogButtons.clear();
    handleModel.setCurrentState(MAppletHandleModel::BROKEN);
    dialogClickedButtonIndex = 1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gMExtensionHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gMAppletHandleStub->stubCallCount("removeApplet"), 1);

    // Test that canceling the dialog will do nothing
    gMAppletHandleStub->stubReset();
    gMExtensionHandleStub->stubReset();
    dialogShown = false;
    dialogButtons.clear();
    handleModel.setCurrentState(MAppletHandleModel::BROKEN);
    dialogClickedButtonIndex = -1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gMExtensionHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gMAppletHandleStub->stubCallCount("removeApplet"), 0);
}

void Ut_MAppletHandleView::testInstallationFailedDialog()
{
    // Set the geometry of the applet to a certain size
    QRectF geometry(0, 0, 50, 50);
    handleView->setGeometry(geometry);
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));

    handleModel.setInstallationError("");

    // test that clicking the applet view in non broken state doesn't show a dialog
    handleModel.setCurrentState(MAppletHandleModel::RUNNING);
    handleView->click();
    QVERIFY(!dialogShown);

    // test that clicking a broken applet view will show a dialog with one remove button
    handleModel.setInstallationError("CRITICAL ERROR");
    handleModel.setCurrentState(MAppletHandleModel::BROKEN);
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(dialogButtons.count(), 1);

    // test that clicking on the button removes the applet
    dialogClickedButtonIndex = -1;
    handleView->click();
    QVERIFY(dialogShown);
    QCOMPARE(gMExtensionHandleStub->stubCallCount("reinit"), 0);
    QCOMPARE(gMAppletHandleStub->stubCallCount("removeApplet"), 1);
}

QTEST_APPLESS_MAIN(Ut_MAppletHandleView)
