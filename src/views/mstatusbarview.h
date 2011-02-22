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

#include <mscenewindowview.h>
#include <mscenewindowmodel.h>
#include <mstatusbarstyle.h>

class MStatusBar;
class QPixmap;
class MStatusBarViewDBusWrapper;

//! \internal

class MStatusBarView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MStatusBarStyle)

public:
    MStatusBarView(MStatusBar *controller);
    virtual ~MStatusBarView();

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

    // Perform the haptic feedback
    void playHapticsFeedback();

    //! position of mouse button press(firstPos) and position of last point of mouse move(lastPos)
    QPointF firstPos, lastPos;

    //! Used to track whether a mouse button is currently being pressed
    bool pressDown;

    MStatusBarViewDBusWrapper *dbusWrapper;

#ifdef Q_WS_X11
    bool isOnDisplay;
    bool isInSwitcher;
    bool shouldStayUpToDate();
    void updateXDamageForSharedPixmap();
    bool isPixmapProviderOnline;
    void setupXDamageForSharedPixmap();
    void destroyXDamageForSharedPixmap();

    QPixmap sharedPixmap;
    Qt::HANDLE pixmapDamage;

private Q_SLOTS:
    void handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                 unsigned short &width, unsigned short &height);

    void handleDisplayEntered();
    void handleDisplayExited();
    void handleSwitcherEntered();
    void handleSwitcherExited();
    void handleSharedPixmapHandleReceived(quint32 handle, bool ok);
    void handlePixmapProviderOnline();
    void handlePixmapProviderOffline();

#endif // Q_WS_X11

#ifdef UNIT_TEST
    friend class Ut_MStatusBarView;
#endif // UNIT_TEST
};

//! \internal_end

#endif
