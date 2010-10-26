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

#ifndef MSHARED_DATA_H
#define MSHARED_DATA_H

#include <QSharedMemory>
#include <QIODevice>
#include <QBuffer>
#include <QDataStream>
#include <QFont>
#include <QVariant>

#include "mexport.h"

/*!

    \class MSharedData
    \brief MSharedData is a helper ilass that implements a shared memory buffer.
    It can have  an arbitrary size and support complex data structures through serialization.
    Internally, MSharedData uses QSharedMemory.

    MSharedMemory is optimized in terms of its memory consumption. It uses paging to vary its size.
    Initially, only 1 page is allocated. However, additional  pages are allocated as needed.

    QSharedMemory provides a plain buffer. It is often needed to store data structures with
    internal pointers, for example QList. Serialization is a mechanism that allows saving such
    data structures to hard drives. We are employing the same mechanism to store such a data structure
    in a shared memory buffer.
    Similar to QDataStream, MSharedData provides operators << and >> to save and load complex data
    structures.
    It is possible to open MSharedData in either of two modes: ReadOnly and Write.

    Even though MSharedData employs paging mechanism to facilitate gradual buffer growth,
    to the programmer it appears as one continuous area of memory. Therefore, functions pos() and
    seek() use continuous offset which always increases as more data is written into the memory, even
    after new pages are created.

    The use case is as follows. The theme daemon creates a shared memory buffer and populates it with data:

    \code
        MSharedData *shm = new MSharedData("buffer");
        shm->open(MSharedData::Write);
        *shm << QString("data");
        *shm << qVariantFromValue(QColor(Qt::white));
        shm->close();
    \endcode

    The application opens the shared buffer for reading and de-serializes the data:

    \code
        MSharedData *shm = new MSharedData("buffer");
        shm->open(MSharedData::ReadOnly);
        QString str;
        QVariant color;
        *shm >> str >> color;
        shm->close();
    \endcode

*/

class MSharedDataPrivate;

class M_CORE_EXPORT MSharedData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int pageSize READ pageSize WRITE setPageSize)
    Q_ENUMS(OpenMode)

public:
    enum OpenMode { ReadOnly, Write };

    /*!
     * \brief A constructor. Creates a shared memory buffer with a given key.
     */
    MSharedData(const QString &key, int pageSize = 8192);

    /*!
     * \brief A destructor. Detaches from every page it was attached to.
     */
    ~MSharedData();

    /*!
     * \brief Opens the shared memory buffer for reading or for writing.
     */
    bool open(OpenMode mode);

    /*!
     * \brief Closes the memory buffer and flushes all updated pages.
     */
    void close();

    /*!
     * \brief Returns the error string. Empty if no error has occurred.
     */
    QString errorString() const;

    /*!
     * \brief Sets the page size. The default value is 8192 bytes.
     * Returns true if the new page size was set, false if there was an error.
     * \sa pageSize()
     */
    bool setPageSize(int pageSize);

    /*!
     * \brief Returns the page size.
     * \sa setPageSize()
     */
    int pageSize() const;

    /*!
     * Returns the current offset within the shared memory buffer.
     * \sa seek()
     */
    qint64 pos() const;

    /*!
     * Sets the current offset within the shared memory buffer.
     * \sa pos()
     */
    bool seek(qint64 offset);

    /*!
     * Writes integer to shared memory
     */
    MSharedData &operator<< (int i);

    /*!
     * Writes boolean to shared memory
     */
    MSharedData &operator<< (bool flag);

    /*!
     * Writes qreal to shared memory
     */
    MSharedData &operator<< (qreal r);

    /*!
     * Write QFont to shared memory
     */
    MSharedData &operator<< (QFont font);

    /*!
     * Writes QVariant to shared memory
     */
    MSharedData &operator<< (QVariant v);

    /*!
     * Writes QString to shared memory
     */
    MSharedData &operator<< (QString str);

    /*!
     * Writes QByteArray to shared memory
     */
    MSharedData &operator<< (QByteArray array);

    /*!
     * Reads integer from shared memory
     */
    MSharedData &operator>> (int &i);

    /*!
     * Reads boolean from shared memory
     */
    MSharedData &operator>> (bool &i);

    /*!
     * Reads qreal from shared memory
     */
    MSharedData &operator>> (qreal &r);

    /*!
     * Reads QFont from shared memory
     */
    MSharedData &operator>> (QFont &font);

    /*!
     * Reads QVariant from shared memory
     */
    MSharedData &operator>> (QVariant &v);

    /*!
     * Reads QString from shared memory
     */
    MSharedData &operator>> (QString &str);

    /*!
     * Reads QByteArray from shared memory
     */
    MSharedData &operator>> (QByteArray &array);

protected:
    MSharedDataPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MSharedData)
    Q_DECLARE_PRIVATE(MSharedData)
};

#endif

