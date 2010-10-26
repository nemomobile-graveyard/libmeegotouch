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

#include <QSharedMemory>
#include <QDebug>

#include "mshareddata.h"
#include "mshareddata_p.h"

void MSharedDataPrivate::setKey(const QString &newKey)
{
    key = newKey;
    shm = new QSharedMemory(key);
}

// allocate page identified as [key]_page[num]
QSharedMemory *MSharedDataPrivate::allocateNewPage(int num)
{
    QString pageName = key + QString("_page") + QString("%1").arg(num);

    QSharedMemory *newPage = new QSharedMemory(pageName);

    if (!newPage->create(pageSize)) {
        if (!newPage->attach()) {
            errorString = QString("cannot attach to ") + pageName;
            qDebug() << errorString;
            delete newPage;
            return NULL;
        }

    } else {
        ownedPages.push_back(newPage);
    }

    return newPage;
}

// allocate new page and set the currentPage to it, update currentPageOffset
QSharedMemory *MSharedDataPrivate::allocateNewPage()
{
    int i = 0;

    // find out which value of the index page is not initialized yet
    while (*((int *)shm->data() + i)) i++;

    currentPage = allocateNewPage(i);
    currentPageNum = i;

    if (i == 0) {
        currentPageOffset = 0;

    } else {
        currentPageOffset = *((int *)shm->data() + i - 1);
    }

    dataByteArray = QByteArray::fromRawData((const char *)currentPage->data(), currentPage->size());
    dataBuffer.setData(dataByteArray);
    dataBuffer.open(QIODevice::ReadWrite);
    dataStream.setDevice(&dataBuffer);

    return currentPage;
}

// attach to an existing page identified as [key]_page[num]
QSharedMemory *MSharedDataPrivate::attachToPage(int num)
{
    QString pageName = key + QString("_page") + QString("%1").arg(num);

    QSharedMemory *page = new QSharedMemory(pageName);
    if (!page->attach(QSharedMemory::ReadOnly)) {
        errorString = QString("cannot attach to page ") + pageName + QString(" ") + page->errorString();
        qDebug() << errorString;
	delete page;
        return NULL;
    }

    if (currentPage) {
        dataBuffer.close();
        currentPage->detach();
        delete currentPage;
    }

    currentPage = page;
    currentPageNum = num;

    if (num > 0) {
        currentPageOffset = *((int *)shm->data() + num - 1);
    } else {
        currentPageOffset = 0;
    }

    nextPageOffset = *((int *)shm->data() + num);

    dataByteArray = QByteArray::fromRawData((const char *)currentPage->data(), currentPage->size());
    dataBuffer.setData(dataByteArray);

    if (!dataBuffer.open(QIODevice::ReadOnly)) {
        errorString = dataBuffer.errorString();
        delete currentPage;
        currentPage = NULL;

    } else {
        dataStream.setDevice(&dataBuffer);
        errorString = QString("");
    }

//    qDebug() << "attached to " << pageName << " " << currentPage->size() << dataBuffer.buffer().size();

    return currentPage;
}

// before each write, save the old pos
// it is restored if the page gets overflown
void MSharedDataPrivate::savePos()
{
    oldPos = dataBuffer.pos();
}

// check whether we have written beyond the boundary of the current page
// if that's the case then try again after creating new one
bool MSharedDataPrivate::pageOverflown()
{
    if (dataBuffer.pos() >= pageSize) {
        dataBuffer.seek(oldPos);
        flushPage();
        dataBuffer.close();
        allocateNewPage();
        return true;
    }

    return false;
}

// switch to next page if all data on current page has been read
void MSharedDataPrivate::nextPage()
{
    if (currentPageOffset + dataBuffer.pos() >= nextPageOffset) {
        attachToPage(currentPageNum + 1);
    }
}

// save the current page
void MSharedDataPrivate::flushPage()
{
    // update page data
    memcpy(currentPage->data(), dataBuffer.data(), currentPage->size());
    *((int *)shm->data() + currentPageNum) = dataBuffer.pos() + currentPageOffset;
}

MSharedData::MSharedData(const QString &key, int pageSize) : d_ptr(new MSharedDataPrivate)
{
    Q_D(MSharedData);

    d->q_ptr = this;
    d->pageSize = pageSize;
    d->setKey(key);
    d->mode = -1;

    const int indexSize = 10000;

    if (!d->shm->create(indexSize)) {
        //qDebug() << "unable to allocate shared memory " << key;

        if (!d->shm->attach()) {
            d->errorString =  QString("unable to attach ") + key;
            qDebug() << d->errorString;
        }

    } else {
        // clean the index page
        memset((unsigned char *)d->shm->data(), 0, d->shm->size());
    }

    d->currentPage = NULL;
}

MSharedData::~MSharedData()
{
    Q_D(MSharedData);

    foreach(QSharedMemory * mem, d->ownedPages) {
        mem->detach();
        delete mem;
    }

    d->shm->detach();
    delete d->shm;

    delete d_ptr;
}

QString MSharedData::errorString() const
{
    Q_D(const MSharedData);
    return d->errorString;
}

MSharedData &MSharedData::operator<< (int i)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::Write)  {
        qDebug() << "cannot write. Buffer not open or ReadOnly";
        return *this;
    }

    d->savePos();
    d->dataStream << i;
    if (d->pageOverflown()) {
        d->dataStream << i;
    }
    return *this;
}

MSharedData &MSharedData::operator<< (bool flag)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::Write) {
        qDebug() << "cannot write. Buffer not open or ReadOnly";
        return *this;
    }

    d->savePos();
    d->dataStream << flag;
    if (d->pageOverflown()) {
        d->dataStream << flag;
    }
    return *this;
}

MSharedData &MSharedData::operator<< (qreal r)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::Write) {
        qDebug() << "cannot write. Buffer not open or ReadOnly";
        return *this;
    }

    d->savePos();
    d->dataStream << r;
    if (d->pageOverflown()) {
        d->dataStream << r;
    }
    return *this;
}

MSharedData &MSharedData::operator<< (QFont font)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::Write) {
        qDebug() << "cannot write. Buffer not open or ReadOnly";
        return *this;
    }

    d->savePos();
    d->dataStream << font;
    if (d->pageOverflown()) {
        d->dataStream << font;
    }
    return *this;
}

MSharedData &MSharedData::operator<< (QVariant v)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::Write) {
        qDebug() << "cannot write. Buffer not open or ReadOnly";
        return *this;
    }

    d->savePos();
    d->dataStream << v;
    if (d->pageOverflown()) {
        d->dataStream << v;
    }
    return *this;
}

MSharedData &MSharedData::operator<< (QString str)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::Write) {
        qDebug() << "cannot write. Buffer not open or ReadOnly";
        return *this;
    }

    d->savePos();
    d->dataStream << str;
    if (d->pageOverflown()) {
        d->dataStream << str;
    }

    return *this;
}

MSharedData &MSharedData::operator<< (QByteArray array)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::Write) {
        qDebug() << "cannot write. Buffer not open or ReadOnly";
        return *this;
    }

    d->savePos();
    d->dataStream << array;
    if (d->pageOverflown()) {
        d->dataStream << array;
    }

    return *this;
}

MSharedData &MSharedData::operator>> (int &i)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::ReadOnly) {
        qDebug() << "cannot read. Buffer not open or open for writing";
        return *this;
    }

    d->nextPage();
    d->dataStream >> i;

    return *this;
}

MSharedData &MSharedData::operator>> (bool &flag)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::ReadOnly) {
        qDebug() << "cannot read. Buffer not open or open for writing";
        return *this;
    }

    d->nextPage();
    d->dataStream >> flag;

    return *this;
}

MSharedData &MSharedData::operator>> (qreal &r)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::ReadOnly) {
        qDebug() << "cannot read. Buffer not open or open for writing";
        return *this;
    }

    d->nextPage();
    d->dataStream >> r;

    return *this;
}

MSharedData &MSharedData::operator>> (QFont &font)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::ReadOnly) {
        qDebug() << "cannot read. Buffer not open or open for writing";
        return *this;
    }

    d->nextPage();
    d->dataStream >> font;

    return *this;
}

MSharedData &MSharedData::operator>> (QVariant &v)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::ReadOnly) {
        qDebug() << "cannot read. Buffer not open or open for writing";
        return *this;
    }

    d->nextPage();
    d->dataStream >> v;

    return *this;
}

MSharedData &MSharedData::operator>> (QString &str)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::ReadOnly) {
        qDebug() << "cannot read. Buffer not open or open for writing";
        return *this;
    }

    d->nextPage();
    d->dataStream >> str;

    return *this;
}

MSharedData &MSharedData::operator>> (QByteArray &array)
{
    Q_D(MSharedData);

    if (d->mode != MSharedData::ReadOnly) {
        qDebug() << "cannot read. Buffer not open or open for writing";
        return *this;
    }

    d->nextPage();
    d->dataStream >> array;

    return *this;
}

bool MSharedData::setPageSize(int pageSize)
{
    Q_D(MSharedData);

    if (d->mode != -1) {
        d->errorString = "cannot change pageSize after shared data buffer was open";
        qDebug() << d->errorString;
        return false;

    } else {
        d->pageSize = pageSize;
        return true;
    }
}

int MSharedData::pageSize() const
{
    Q_D(const MSharedData);
    return d->pageSize;
}

qint64 MSharedData::pos() const
{
    Q_D(const MSharedData);
    return d->currentPageOffset + d->dataBuffer.pos();
}

bool MSharedData::seek(qint64 offset)
{
    Q_D(MSharedData);
    int i = 0, pageOffset;
    bool res = false;

    if (d->mode != MSharedData::ReadOnly) {
        d->errorString = "Cannot seek in buffer which is not ReadOnly";
        qDebug() << d->errorString;
        return false;
    }

    while ((pageOffset = *((int *)d->shm->data() + i))) {

        if (offset < pageOffset) {
            d->attachToPage(i);
            qint64 realOffset = (i == 0) ? offset : offset - *((int *)d->shm->data() + i - 1);

            res = d->dataBuffer.seek(realOffset);
            if (!res)
                d->errorString = d->dataBuffer.errorString();

            return res;
        }
        i++;
    }

    d->errorString = QString("Offset %1 not found").arg(offset);
    qDebug() << d->errorString;
    return false;
}

bool MSharedData::open(MSharedData::OpenMode mode)
{
    Q_D(MSharedData);

    if (!d->errorString.isEmpty())
        return false;

    if (d->mode != -1) {
        d->errorString = "Cannot open again. This shared data object is already open";
        qDebug() << d->errorString;
        return false;
    }

    d->errorString = "";
    d->mode = mode;

    if (mode == MSharedData::Write) {
        memset((unsigned char *)d->shm->data(), 0, d->shm->size());
        if (d->allocateNewPage())
            return true;
        else
            return false;

    } else if (mode == MSharedData::ReadOnly) {
        if (d->attachToPage(0))
            return true;
        else
            return false;

    } else {
        d->errorString = "Unknown openmode. Only MSharedData::Write and MSharedData::ReadOnly are possible";
        qDebug() << d->errorString;
        return false;
    }
}

void MSharedData::close()
{
    Q_D(MSharedData);

    if (d->mode == MSharedData::Write) {
        if (d->dataBuffer.pos() != 0) {
            d->flushPage();

        } else {
            // nothing was written on that page, destroy it

            d->currentPage->detach();
            d->ownedPages.removeOne(d->currentPage);
            delete d->currentPage;
        }

    } else if (d->mode == MSharedData::ReadOnly) {
        if (d->currentPage) {
            d->currentPage->detach();
            delete d->currentPage;
        }
    }

    d->currentPage = 0;
    d->dataBuffer.close();
    d->mode = -1;
    d->errorString = "";
}

