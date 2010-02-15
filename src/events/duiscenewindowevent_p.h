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

#ifndef DUISCENEWINDOWAPPEARANCECHANGEEVENT_H
#define DUISCENEWINDOWAPPEARANCECHANGEEVENT_H

#include <QEvent>

class DuiSceneWindow;

/*
 * Sent by DuiSceneManager to the DuiWindows currently rendering its DuiScene
 * when some interesting state change of a DuiSceneWindow managed by him has
 * happened (or is about to happen).
 *
 * This event informs DuiWindows when, for instance, scene windows enter or leave
 * the scene, enabling them to connect the signals of that upcoming scene
 * window to slots of other existing scene windows, etc.
 */
class DuiSceneWindowEvent : public QEvent
{
public:
    // scene window is about to appear
    static QEvent::Type eventTypeAppear();

    // scene window is about to disappear
    static QEvent::Type eventTypeDisappear();

    // scene window is about to be dismissed
    static QEvent::Type eventTypeDismiss();

    /*
     * Contructs a new DuiSceneWindowEvent
     * \param type Either eventTypeAppear() or eventTypeDisappear().
     * \param sceneWindow The scene window that has changed (or is about to change).
     * \param animatedTransition Whether the state change is going to be animated
     *        (true) or immediate (false).
     */
    DuiSceneWindowEvent(QEvent::Type type,
                        DuiSceneWindow *sceneWindow,
                        bool animatedTransition);

    /*
     * The scene window that has changed (or is about to change).
     */
    DuiSceneWindow *sceneWindow() const;

    /*
     * Whether the state change was (or is going to be) animated.
     */
    bool animatedTransition() const;

private:
    DuiSceneWindow *m_sceneWindow;
    bool m_animatedTransition;
};

#endif
