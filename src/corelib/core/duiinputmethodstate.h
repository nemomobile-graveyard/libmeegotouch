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

#ifndef DUIINPUTMETHODSTATE_H
#define DUIINPUTMETHODSTATE_H

#include <QObject>
#include <QRect>

#include "duinamespace.h"
#include "duiexport.h"

class DuiInputMethodStatePrivate;

/*!
 * \brief A mediator between DuiInputContext and applications/libdui that use it
 *
 * This class allows DuiInputContext (technically other input contexts too) to
 * communicate input method area on display to the application and application
 * to communicate its active window's orientation to the input context.  The
 * application can be a DUI application or a plain Qt application (which needs
 * to link against libdui to get access to this class).  The input method area
 * can be used by the application to avoid obstructing the input method.
 */
class DUI_EXPORT DuiInputMethodState : public QObject
{
    Q_OBJECT

public:
    //! \brief Get singleton instance
    //! \return singleton instance
    static DuiInputMethodState *instance();

    //! \brief Get current input method area
    //! \return current input method area
    QRect inputMethodArea() const;

    //! \brief Get the orientation of application's active window
    //! \return orientation of application's active window
    Dui::OrientationAngle activeWindowOrientationAngle() const;

public Q_SLOTS:
    //! \brief Set the orientation of application's active window to \a newOrientation
    void setActiveWindowOrientationAngle(Dui::OrientationAngle newOrientation);

    //! \brief Set input method area to \a newRegion
    void setInputMethodArea(const QRect &newRegion);

Q_SIGNALS:
    //! Emitted when input method area is changed
    //! \param region new input method area
    void inputMethodAreaChanged(const QRect &region);

    //! Emitted when the orientation of application's active window is changed
    //! \param orientation new orientation angle
    void activeWindowOrientationAngleChanged(Dui::OrientationAngle orientation);

private:
    //! Disable default construction
    DuiInputMethodState();
    ~DuiInputMethodState();
    Q_DISABLE_COPY(DuiInputMethodState)
    Q_DECLARE_PRIVATE(DuiInputMethodState)

    DuiInputMethodStatePrivate *const d_ptr;
};

#endif
