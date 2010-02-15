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

#ifndef DUIDISMISSEVENT_H
#define DUIDISMISSEVENT_H

#include <QEvent>
#include "duiexport.h"

/*!
 * Dismiss events are sent to scene windows that the user wants to dismiss. Usually
 * by tapping the back button (for DuiApplicationPage), a close button (e.g. in the
 * title bar of a DuiDialog) or by tapping outside a modal scene window. They are
 * also sent when you call DuiSceneWindow::dismiss() to dismiss a scene window
 * programmatically.
 *
 * Dismiss events contain a flag that indicates whether the receiver wants the
 * scene window to be dismissed or not. When a scene window accepts the dismiss
 * event, it disappears (and is destroyed if DuiSceneWindow::DestroyWhenDone
 * was used). If it refuses to accept the dismiss event nothing happens.
 * The event handler DuiScenewindow::dismissEvent() receives dismiss events. The
 * default implementation of this event handler accepts the dismiss event. If
 * you do not want your scene window to disappear, or want some special handing,
 * you should reimplement the event handler and ignore() the event.
 *
 * The difference between DuiDismissEvent and QCloseEvent is that the former
 * causes the scene window to disappear (which makes it leave the scene with an
 * optionally animated transition and finally hides it) where the latter only
 * hides the scene window (it will still occupy the scene and no transition
 * animation will take place).
 *
 * If you want the scene window to be deleted after it disappears, pass
 * DuiSceneWindow::DestroyWhenDone when calling DuiSceneWindow::appear() or
 * DuiSceneWindow::appearNow(). This is very useful for independent scene windows.
 *
 * QObjects emits the destroyed() signal when they are deleted.
 *
 * The isAccepted() method returns true if the event's receiver has agreed to
 * dismiss the scene window; call accept() to agree to dismiss the scene window
 * and call ignore() if the receiver of this event does not want the scene window
 * to be closed.
 *
 * \sa DuiSceneWindow::dismiss()
 */
class DUI_EXPORT DuiDismissEvent : public QEvent
{
public:
    /*!
     * \brief Default constructor, no need for any parameters.
     */
    explicit DuiDismissEvent();

    /*!
     * \brief Event type for DuiDismissEvent.
     */
    static QEvent::Type eventType();

private:
    Q_DISABLE_COPY(DuiDismissEvent)
};


#endif
