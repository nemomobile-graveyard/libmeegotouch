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

#ifndef MSTATUSBARVIEW_H
#define MSTATUSBARVIEW_H

#include <QTimer>
#include <mdebug.h>
#include <mscenewindowview.h>
#include <mscenewindowmodel.h>
#include <mstatusbarstyle.h>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif //Q_WS_X11

class MStatusBar;
class QPixmap;

//! \internal

#ifdef Q_WS_X11
class MStatusBarEventListener
{
public:
    MStatusBarEventListener();
    ~MStatusBarEventListener();

    static bool xWindowPropertyEventFilter(void *message);
};
#endif //Q_WS_X11

class MStatusBarView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MStatusBarStyle)

public:
    MStatusBarView(MStatusBar *controller);
    virtual ~MStatusBarView();

#ifdef Q_WS_X11
    //! Handles X11 PropertyNotify events.
    void handleXPropertyNotify(const XPropertyEvent &propertyEvent);
    //! Handles X11 DestroyNotify events.
    void handleXDestroyNotify(const XDestroyWindowEvent &destroyWindowEvent);

    static Atom propertyWindowAtom;
    static Atom pixmapHandleAtom;
#endif //Q_WS_X11

protected:
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Q_DISABLE_COPY(MStatusBarView)

    MWidgetController *controller;
    /*!
     * Displays the status indicator menu.
     */
    void showStatusIndicatorMenu();

    //! Perform the haptic feedback for status bar press
    void playHapticsFeedback();

    //! position of mouse button press(firstPos) and position of last point of mouse move(lastPos)
    QPointF firstPos, lastPos;

    //! Used to track whether a mouse button is currently being pressed
    bool pressDown;

#ifdef Q_WS_X11

    //! Timer to poll the status bar property window from root window property
    QTimer propertyWindowPollTimer;

    //! Window id of the status bar property window
    Window statusBarPropertyWindowId;

    bool isOnDisplay;
    bool isInSwitcher;
    bool shouldStayUpToDate();
    void updateXDamageForSharedPixmap();
    void setupXDamageForSharedPixmap();
    void destroyXDamageForSharedPixmap();

    QPixmap sharedPixmap;
    Qt::HANDLE pixmapDamage;

    /*! Updates the status bar shared pix handle from x property of given window
     * \return Whether update succeeded
     */
    bool updateSharedPixmapHandleFromPropertyWindow(const Window &window);

    /*! Gets the status bar property window id from root window property
     * \return The window id from root property, or -1 if unsuccessful
     */
    Window fetchStatusBarPropertyWindowId();

private Q_SLOTS:
    /*! Handles property window poll timer timeout.
     * Tries to update the shared pixmap handle.
     * And ccording to current interval, either increases the timer interval or stops the timer.
     */
    void onPropertyWindowPollTimerTimeout();

    //! Updates the shared pixmap handle if available
    void updateStatusBarSharedPixmapHandle();
    void handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                 unsigned short &width, unsigned short &height);

    void handleDisplayEntered();
    void handleDisplayExited();
    void handleSwitcherEntered();
    void handleSwitcherExited();
    void handleSharedPixmapHandleReceived(quint32 handle);
    void handlePixmapProviderOffline();

#endif // Q_WS_X11

#ifdef UNIT_TEST
    friend class Ut_MStatusBarView;
#endif // UNIT_TEST
};

//! \internal_end

#endif
