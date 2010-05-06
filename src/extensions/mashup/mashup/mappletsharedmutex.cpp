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

#include "mappletsharedmutex.h"
#include "mappletsharedmutex_p.h"

#include <QDir>
#include <errno.h>
#include <pthread.h>

MAppletSharedMutexPrivate::MAppletSharedMutexPrivate() :
    q_ptr(NULL),
    sharedMemory(NULL),
    sharedMemoryId(-1)
{
}

MAppletSharedMutexPrivate::~MAppletSharedMutexPrivate()
{
    destroySharedMemory(keyString);
}

QString MAppletSharedMutexPrivate::createSharedMemoryKeyFileName(const QString &keyString)
{
    return QDir::tempPath() + "/m_sharedmemory_" + keyString;
}

key_t MAppletSharedMutexPrivate::createSharedMemoryKey(const QString &fileName)
{
    // Create a file for ftok
    if (!QFile::exists(fileName)) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.close();
        }
    }

    return ftok(QFile::encodeName(fileName).constData(), 'D');
}

bool MAppletSharedMutexPrivate::initSharedMemory(const QString &keyString)
{
    // Store the key string
    this->keyString = keyString;

    // Empty keystrings are not supported
    if (keyString.isEmpty()) {
        return false;
    }

    // Get a shared memory segment
    bool newSegment = true;
    QString keyFileName = createSharedMemoryKeyFileName(keyString);
    key_t shmKey = createSharedMemoryKey(keyFileName);
    sharedMemoryId = shmget(shmKey, sizeof(pthread_mutex_t), IPC_CREAT | IPC_EXCL | 00600);
    if (sharedMemoryId == -1 && errno == EEXIST) {
        // The shared memory segment already exists
        newSegment = false;

        // Try to take the existing segment into use
        sharedMemoryId = shmget(shmKey, sizeof(pthread_mutex_t), 00600);

        if (sharedMemoryId != -1) {
            // Mark the segment to be destroyed after the attach count reaches zero
            struct shmid_ds sds;
            shmctl(sharedMemoryId, IPC_RMID, &sds);

            // Remove the file created for ftok
            QFile::remove(keyFileName);
        }
    }

    if (sharedMemoryId != -1) {
        // Attach to the shared memory segment
        sharedMemory = shmat(sharedMemoryId, NULL, 0);

        if (sharedMemory != (void *) - 1) {
            // Initialize shared mutex
            if (newSegment && !initMutex((pthread_mutex_t *)sharedMemory)) {
                // Initialization of mutex failed: destroy shared memory
                destroySharedMemory(keyString);
            }
        } else {
            // Attaching failed: the shared memory address and ID are not valid
            sharedMemory = NULL;
            sharedMemoryId = -1;
        }
    }

    return sharedMemory != NULL;
}

void MAppletSharedMutexPrivate::destroySharedMemory(const QString &keyString)
{
    if (sharedMemory != NULL) {
        // Get the attach count and mark the memory to be deleted
        struct shmid_ds sds;
        shmctl(sharedMemoryId, IPC_STAT, &sds);
        shmctl(sharedMemoryId, IPC_RMID, &sds);

        if (sds.shm_nattch == 1) {
            // Destroy the mutex and remove the file created for ftok if the detach count was 1
            destroyMutex((pthread_mutex_t *)sharedMemory);
            QFile::remove(createSharedMemoryKeyFileName(keyString));
        }

        // Detach
        shmdt(sharedMemory);
        sharedMemory = NULL;
        sharedMemoryId = -1;
    }
}

bool MAppletSharedMutexPrivate::initMutex(pthread_mutex_t *mutex)
{
    if (mutex != NULL) {
        // Initialize a mutex in the shared memory with the "shared between processes" attribute
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) == 0 &&
                pthread_mutex_init(mutex, &attr) == 0) {
            return true;
        }
    }

    return false;
}

void MAppletSharedMutexPrivate::destroyMutex(pthread_mutex_t *mutex)
{
    if (mutex != NULL) {
        // If the mutex is locked the result of destroying is undefined but since the memory segment is about to be destroyed anyway this won't matter
        pthread_mutex_destroy(mutex);
    }
}

MAppletSharedMutex::MAppletSharedMutex() : d_ptr(new MAppletSharedMutexPrivate)
{
}

MAppletSharedMutex::~MAppletSharedMutex()
{
    delete d_ptr;
}

bool MAppletSharedMutex::init(const QString &key)
{
    Q_D(MAppletSharedMutex);

    // Destroy any previously shared memory
    d->destroySharedMemory(d->keyString);

    // Initialize shared memory
    return d->initSharedMemory(key);
}

bool MAppletSharedMutex::lock()
{
    Q_D(MAppletSharedMutex);

    if (d->sharedMemory != NULL) {
        return pthread_mutex_lock((pthread_mutex_t *)d->sharedMemory) == 0;
    } else {
        return false;
    }
}

bool MAppletSharedMutex::unlock()
{
    Q_D(MAppletSharedMutex);

    if (d->sharedMemory != NULL) {
        return pthread_mutex_unlock((pthread_mutex_t *)d->sharedMemory) == 0;
    } else {
        return false;
    }
}

bool MAppletSharedMutex::tryLock()
{
    Q_D(MAppletSharedMutex);

    if (d->sharedMemory != NULL) {
        return pthread_mutex_trylock((pthread_mutex_t *)d->sharedMemory) == 0;
    } else {
        return false;
    }
}
