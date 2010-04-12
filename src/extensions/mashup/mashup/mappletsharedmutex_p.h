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

#ifndef MAPPLETSHAREDMUTEXPRIVATE_H_
#define MAPPLETSHAREDMUTEXPRIVATE_H_

#include <QString>
#include <sys/ipc.h>
#include <sys/shm.h>

class MAppletSharedMutexPrivate
{
    Q_DECLARE_PUBLIC(MAppletSharedMutex)

public:
    MAppletSharedMutexPrivate();
    virtual ~MAppletSharedMutexPrivate();

    //! Public class
    MAppletSharedMutex *q_ptr;

    //! Shared memory key string
    QString keyString;

    //! Memory segment shared between the host process and the applet process, used to store a mutex to control access to the shared X pixmap
    void *sharedMemory;

    //! ID for the shared memory segment
    int sharedMemoryId;

    /*!
     * Creates a shared memory key file name from a string.
     *
     * \param keyString the string to use as the shared memory key
     * \return the shared memory key file name for the string
     */
    QString createSharedMemoryKeyFileName(const QString &keyString);

    /*!
     * Creates a shared memory key from a string.
     *
     * \param keyString the string to use as the shared memory key
     * \return the shared memory key for the string
     */
    key_t createSharedMemoryKey(const QString &keyString);

    /*!
     * Initializes a shared memory segment by either allocating a new shared memory segment or attaching to an existing one.
     *
     * \param keyString the string to use as the shared memory key
     * \return true if creation succeeded, false otherwise
     */
    bool initSharedMemory(const QString &keyString);

    /*!
     * Destroys the shared memory segment if it exists.
     *
     * \param keyString the string to use as the shared memory key
     */
    void destroySharedMemory(const QString &keyString);

    /*!
     * Initializes a mutex.
     *
     * \param pointer to the mutex to be initialized
     * \return true if initialization succeeded, false otherwise
     */
    bool initMutex(pthread_mutex_t *mutex);

    /*!
     * Destroys the mutex if it exists.
     *
     * \param mutex the mutex to be destroyed
     */
    void destroyMutex(pthread_mutex_t *mutex);
};

#endif /* MAPPLETSHAREDMUTEXPRIVATE_H_ */
