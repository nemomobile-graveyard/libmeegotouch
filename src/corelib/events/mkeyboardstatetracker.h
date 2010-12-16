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

#ifndef MKEYBOARDSTATETRACKER_H
#define MKEYBOARDSTATETRACKER_H

#include "mexport.h"
#include <QObject>

class MKeyboardStateTrackerPrivate;

/*!
 * Class responsible for tracking the hardware keyboard properties and signaling events.
 * It is designed as singleton. Using isPresent() can query whether the device supports
 * hardware keyboard or not. If hardware keyboard is supported, using isOpen() to query
 * its current state. Signal stateChanged will be emitted when hardware keyboard state is changed.
 */
class M_CORE_EXPORT MKeyboardStateTracker : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Returns singleton MKeyboardStateTracker object.
     */
    static MKeyboardStateTracker *instance();

    /*!
     * \brief Returns true if the device has a hardware keyboard.
     */
    bool isPresent() const;

    /*!
     * \brief Returns true if the hardware keyboard is opened.
     */
    bool isOpen() const;

Q_SIGNALS:
    /**
     *  \brief A signal which is emitted whenever the hardware keyboard state has been changed.
     */
    void stateChanged();

protected:
    MKeyboardStateTrackerPrivate *const d_ptr;

private:
    MKeyboardStateTracker();
    virtual ~MKeyboardStateTracker();

    Q_DISABLE_COPY(MKeyboardStateTracker)
    Q_DECLARE_PRIVATE(MKeyboardStateTracker)

    friend class MOrientationTrackerPrivate;
};

#endif // MKEYBOARDSTATETRACKER_H
