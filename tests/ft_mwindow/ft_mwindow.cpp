/****************************************************************************
**
** Copyright(C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation(directui@nokia.com)
**
** This file is part of systemui.
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

#include <QtTest/QtTest>
#include <QX11Info>
#include <MApplication>
#include <MWindow>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "ft_mwindow.h"

void Ft_MWindow::initTestCase()
{
    static int argc = 1;
    static char *argv[] = {(char *) "./ft_mwindow", NULL };
    app = new MApplication(argc, argv);
}

void Ft_MWindow::cleanupTestCase()
{
    delete app;
}

void Ft_MWindow::init()
{
    window = new MWindow;
}

void Ft_MWindow::cleanup()
{
    delete window;
}

void Ft_MWindow::testShowDoesNotResetSkipTaskbar()
{
    // Show the window
    qDebug() << "show()ing the window";
    window->show();

    // Wait for 1 second
    QTest::qWait(1000);

    // Tell the window to not to be shown in the task bar
    qDebug() << "Setting _NET_WM_STATE_SKIP_TASKBAR";
    Display *display = QX11Info::display();
    XEvent e;
    memset(&e, 0, sizeof(XEvent));
    e.xclient.type = ClientMessage;
    e.xclient.display = display;
    e.xclient.window = window->winId();
    e.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
    e.xclient.format = 32;
    e.xclient.data.l[0] = 1;
    e.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False);
    XSendEvent(display, RootWindow(display, window->x11Info().screen()), False, (SubstructureNotifyMask | SubstructureRedirectMask), &e);
    XSync(display, False);

    // Wait for 1 second
    QTest::qWait(1000);

    // Verify that the window has the property
    testWindowHasSkipTaskbarProperty();

    // Show the window again
    qDebug() << "show()ing the window again";
    window->show();

    // Wait for 1 second
    QTest::qWait(1000);

    // Verify that the window has the property
    testWindowHasSkipTaskbarProperty();
}

void Ft_MWindow::testWindowHasSkipTaskbarProperty()
{
    QList<Atom> properties;
    Atom actualType;
    int actualFormat;
    unsigned long numTypeItems, bytesLeft;
    unsigned char *typeData = NULL;

    bool found = false;
    Display *display = QX11Info::display();
    Status result = XGetWindowProperty(display, window->winId(), XInternAtom(display, "_NET_WM_STATE", False), 0L, 16L, False, XA_ATOM, &actualType, &actualFormat, &numTypeItems, &bytesLeft, &typeData);
    if (result == Success) {
        Atom *type = (Atom *) typeData;
        for (unsigned int n = 0; n < numTypeItems; n++) {
            found |= (type[n] == XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False));
        }
        XFree(typeData);
    }

    qDebug() << "Checking whether _NET_WM_STATE_SKIP_TASKBAR is set:" << found;

    QVERIFY2(found, "_NET_WM_STATE_SKIP_TASKBAR not set");
}

QTEST_APPLESS_MAIN(Ft_MWindow)
