/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msheetstandalonewindow.h"
#include <mdeviceprofile.h>
#include <mdismissevent.h>
#include <mscenemanager.h>
#include <msheet.h>
#include <mstatusbar.h>
#include <mcomponentdata.h>

#include <QApplication>
#include <QTimer>

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

#ifdef HAVE_CONTEXTSUBSCRIBER
# include "contextproperty.h"
#endif

MSheetStandAloneWindow::MSheetStandAloneWindow()
    : MWindow(new MSceneManager, 0)
    , beingClosed(false)
    , sheet(0)
    , statusBar(0)
#ifdef HAVE_CONTEXTSUBSCRIBER
    , callStatusProperty("Phone.Call")
#endif
    , sheetAreaMaximized(false)
{
    setRoundedCornersEnabled(true);
#ifdef Q_WS_X11
    appendMSheetTypeProperty();
#endif //Q_WS_X11

#ifdef HAVE_CONTEXTSUBSCRIBER
    connect(&callStatusProperty, SIGNAL(valueChanged()), this, SLOT(_q_updateStatusBarVisibility()));
#endif
}

MSheetStandAloneWindow::~MSheetStandAloneWindow()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    disconnect(&callStatusProperty, SIGNAL(valueChanged()), this, SLOT(_q_updateStatusBarVisibility()));
#endif

    delete statusBar;
    statusBar = 0;
}

void MSheetStandAloneWindow::setSheet(MSheet *sheet)
{
    this->sheet = sheet;
    _q_updateStatusBarVisibility();
}

void MSheetStandAloneWindow::setStatusBarVisible(bool statusBarVisible)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (callStatusProperty.value().toString() == "active")
        statusBarVisible = true;
#endif

    if (!statusBar && MDeviceProfile::instance()->showStatusbar()
            && statusBarVisible)
        statusBar = new MStatusBar;

    if (statusBar) {
        if (statusBarVisible) {
            if (isVisible())
                sceneManager()->appearSceneWindow(statusBar);
            else
                sceneManager()->appearSceneWindowNow(statusBar);
        } else {
            sceneManager()->disappearSceneWindow(statusBar);
        }
    }
}

void MSheetStandAloneWindow::closeEvent(QCloseEvent *event)
{
    if (sheet) {
        MDismissEvent dismissEvent;
        QApplication::sendEvent(sheet, &dismissEvent);

        event->setAccepted(dismissEvent.isAccepted());
    } else {
        event->accept();
    }

    beingClosed = event->isAccepted();
}

void MSheetStandAloneWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    // reset variable
    beingClosed = false;
}

void MSheetStandAloneWindow::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);

    if (!sheet)
        return;

    // OBS: disappear only on the next event loop as we might be
    // called from within MSceneWindowPrivate::canDisappear()

    if (beingClosed) {
        QTimer::singleShot(0, sheet, SIGNAL(_q_dismissSystemSheetImmediately()));
    } else {
        QTimer::singleShot(0, sheet, SIGNAL(_q_makeSystemSheetDisappearImmediately()));
    }
}

#ifdef Q_WS_X11
void MSheetStandAloneWindow::appendMSheetTypeProperty()
{
    Atom atomWindowType = XInternAtom(QX11Info::display(),
                                      "_MEEGOTOUCH_NET_WM_WINDOW_TYPE_SHEET", False);

    XChangeProperty(QX11Info::display(), effectiveWinId(),
                    XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False),
                    XA_ATOM, 32, PropModeAppend, (unsigned char*) &atomWindowType, 1);
}

void MSheetStandAloneWindow::setWindowChainedProperty(const WId &parentWinId, const WId &childWinId)
{
    Atom atomMInvokedBy = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_WM_INVOKED_BY", False);
    Display *display = QX11Info::display();

    // for compositor page transition
    XChangeProperty(display, childWinId,
            atomMInvokedBy, XA_WINDOW,
            32, PropModeReplace,
            (unsigned char *)&parentWinId, 1);

    // for task switcher view stacking
    XSetTransientForHint(display, childWinId, parentWinId);
}
#endif //Q_WS_X11

void MSheetStandAloneWindow::updateChainTaskData()
{
#ifdef Q_WS_X11
    // here we try to inject the chainTaskData into this MSheetStandAloneWindow
    if (!MComponentData::chainDataStackIsEmpty()) {
        MComponentData::ChainData thisData = MComponentData::popChainData();

        WId chainParentWinId = thisData.first;

        setWindowChainedProperty(chainParentWinId, effectiveWinId());
    } else {
        setWindowChainedProperty(0, effectiveWinId());
    }
#endif //Q_WS_X11
}

void MSheetStandAloneWindow::maximizeSheetArea()
{
    sheetAreaMaximized = true;
    setStatusBarVisible(false);
}

void MSheetStandAloneWindow::restoreSheetArea()
{
    sheetAreaMaximized = false;
    if (sheet) {
        setStatusBarVisible(sheet->isStatusBarVisibleInSystemwide());
    }
}

void MSheetStandAloneWindow::_q_updateStatusBarVisibility()
{
    if (sheet) {
        setStatusBarVisible(sheet->isStatusBarVisibleInSystemwide() && !sheetAreaMaximized);
    }
}
