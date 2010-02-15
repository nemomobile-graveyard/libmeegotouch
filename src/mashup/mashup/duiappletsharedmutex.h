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

#ifndef DUIAPPLETSHAREDMUTEX_H_
#define DUIAPPLETSHAREDMUTEX_H_

#include <DuiExport>

class DuiAppletSharedMutexPrivate;

/*!
 * DuiAppletSharedMutex provides a POSIX mutex that is located in shared
 * memory. Initializing the shared mutex will create a shared memory segment
 * if one is not created yet. Otherwise the already existing shared memory
 * segment is attached. Destroying the mutex will detach from the shared
 * memory so that when all parties have detached the segment will be freed.
 */
class DUI_EXPORT DuiAppletSharedMutex
{
    Q_DECLARE_PRIVATE(DuiAppletSharedMutex)

public:
    /*!
     * Creates a new shared mutex. The mutex must be initialized using
     * init() before it can be used.
     */
    DuiAppletSharedMutex();

    /*!
     * Destroys the shared mutex. Detaches the shared memory so that
     * when all parties have detached the segment will be freed.
     */
    virtual ~DuiAppletSharedMutex();

    /*!
     * Initializes the shared mutex. Creates a shared memory segment
     * if one is not created yet. Otherwise the already existing shared memory
     * segment is attached and the mutex is marked to be destroyed when all
     * parties have detached.
     */
    bool init(const QString &key);

    /*!
     * Locks the mutex. If the mutex is already locked the calling thread
     * shall block until the mutex becomes available.
     *
     * \return true if the mutex could be locked, false otherwise
     */
    bool lock();

    /*!
     * Unlocks the mutex.
     *
     * \return true if the mutex could be unlocked, false otherwise
     */
    bool unlock();

    /*!
     * Locks the mutex. If the mutex is already locked the function
     * will return false immediately.
     *
     * \return true if the mutex could be locked, false otherwise
     */
    bool tryLock();

private:
    //! A pointer to the private class
    DuiAppletSharedMutexPrivate *d_ptr;
};

#endif /* DUIAPPLETSHAREDMUTEX_H_ */
