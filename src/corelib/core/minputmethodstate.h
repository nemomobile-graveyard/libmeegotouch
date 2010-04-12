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

#ifndef MINPUTMETHODSTATE_H
#define MINPUTMETHODSTATE_H

#include <QObject>
#include <QRect>

#include "mnamespace.h"
#include "mexport.h"

class MInputMethodStatePrivate;

/*!
 * \brief A mediator between MInputContext and applications/libmeegotouch that use it
 *
 * This class allows MInputContext (technically other input contexts too) to
 * communicate input method area on display to the application and application
 * to communicate its active window's orientation to the input context.  The
 * application can be a M application or a plain Qt application (which needs
 * to link against libmeegotouch to get access to this class).  The input method area
 * can be used by the application to avoid obstructing the input method.
 */
class M_EXPORT MInputMethodState : public QObject
{
    Q_OBJECT

public:
    //! \brief Get singleton instance
    //! \return singleton instance
    static MInputMethodState *instance();

    //! \brief Get current input method area
    //! \return current input method area
    QRect inputMethodArea() const;

    //! \brief Get the orientation of application's active window
    //! \return orientation of application's active window
    M::OrientationAngle activeWindowOrientationAngle() const;

public Q_SLOTS:
    //! \brief Set the orientation of application's active window to \a newOrientation
    void setActiveWindowOrientationAngle(M::OrientationAngle newOrientation);

    //! \brief Set input method area to \a newRegion
    void setInputMethodArea(const QRect &newRegion);

Q_SIGNALS:
    //! Emitted when input method area is changed
    //! \param region new input method area
    void inputMethodAreaChanged(const QRect &region);

    //! Emitted when the orientation of application's active window is changed
    //! \param orientation new orientation angle
    void activeWindowOrientationAngleChanged(M::OrientationAngle orientation);

private:
    //! Disable default construction
    MInputMethodState();
    ~MInputMethodState();
    Q_DISABLE_COPY(MInputMethodState)
    Q_DECLARE_PRIVATE(MInputMethodState)

    MInputMethodStatePrivate *const d_ptr;
};

#endif
