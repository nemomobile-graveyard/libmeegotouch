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
#include <mdismissevent.h>
#include <mscenemanager.h>

#include <QApplication>
#include <QTimer>

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

MSheetStandAloneWindow::MSheetStandAloneWindow()
    : MWindow(new MSceneManager, 0), beingClosed(false), sheet(0)
{
    setRoundedCornersEnabled(true);
#ifdef Q_WS_X11
    appendMSheetTypeProperty();
#endif //Q_WS_X11
}

MSheetStandAloneWindow::~MSheetStandAloneWindow()
{
}

void MSheetStandAloneWindow::setSheet(MSceneWindow *sheet)
{
    this->sheet = sheet;
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
#endif //Q_WS_X11