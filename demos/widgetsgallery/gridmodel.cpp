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

#include "gridmodel.h"

#include <QImage>
#include <QPixmap>
#include <QPainter>

#define THREAD_COUNT 5

GridModel::GridModel(const QSize &size, const QStringList &dirs)
    : QAbstractTableModel(),
      m_dirs(dirs)
{
    qRegisterMetaType<MediaType>("MediaType");

    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = new Loader(size);
        connect(loader, SIGNAL(imageReady(QImage, int)), this, SLOT(insertImage(QImage, int)));
        m_loaders.append(loader);
    }

    createItems();
}

GridModel::~GridModel()
{
    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = m_loaders[i];
        loader->stop();
        loader->wait();
    }
    qDeleteAll(m_loaders);
    m_loaders.clear();
}

int GridModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_items.size();
}

int GridModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

QVariant GridModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < m_items.size());


    MediaType m;

    if (m_items[ index.row() ].canConvert<MediaType>())
        m = m_items[ index.row() ].value<MediaType>();

    if (role == Qt::DisplayRole)
        return m_items[index.row()];
    else if (role == GridModel::Type)
        return m.type;
    else if (role == GridModel::ImageId)
        return m.path;

    return QVariant();
}

void GridModel::pauseLoaders()
{
    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = m_loaders[i];
        if (loader->isRunning())
            loader->stop();
    }
}

void GridModel::resumeLoaders(int offset)
{
    if (offset < 0)
        offset = 0;
    else
        offset = offset / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = m_loaders[i];
        if (!loader->isRunning() && loader->hasWork())
            loader->resume(offset);
    }
}

void GridModel::createItems()
{
    int index = 0;

    foreach (const QString &dir, m_dirs) {
        QDir mediaDir(dir);
        mediaDir.setFilter(QDir::Files | QDir::NoSymLinks);
        mediaDir.setSorting(QDir::Name);
        mediaDir.setNameFilters(QStringList() << "*.jpeg" << "*.jpg" << "*.png");
        const QFileInfoList fileList = mediaDir.entryInfoList();

        foreach (const QFileInfo & file, fileList) {
            const QString path = file.absoluteFilePath();
            MediaType m;

            m.type = MediaType::Image;
            m.path = path;
            m.image = QImage();
            Loader *loader = m_loaders[index % THREAD_COUNT];
            loader->pushImage(path,index);

            m_items[index] = QVariant::fromValue(m);
            index++;
        }
    }
    resumeLoaders();
}

void GridModel::insertImage(QImage image, int index)
{
    if (m_items[index].canConvert<MediaType>()) {
        MediaType m = m_items[index].value<MediaType>();
        m.image = image;
        m_items[index] = QVariant::fromValue(m);

        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void Loader::run()
{
    while (true) {
        mutex.lock();
        if (stopWork) {
            mutex.unlock();
            return;
        }

        int offset = loadOffset;

        BacklogItem *backlogItem = NULL;
        if (offset >= backlog.count())
            offset = backlog.count() - 1;

        backlogItem = backlog.at(offset);

        // Try to find next item in backlog to load
        int nextOffset = offset;
        while (backlogItem->loaded && nextOffset < backlog.count())
            backlogItem = backlog.at(nextOffset++);

        // If we did not find, try to load previous in backlog
        int prevOffset = offset;
        while (backlogItem->loaded && prevOffset > 0)
            backlogItem = backlog.at(prevOffset--);

        // Everything is loaded, stop.
        if (backlogItem->loaded) {
            mutex.unlock();
            stop();
            return;
        }

        backlogItem->loaded = true;
        const QFileInfo file(backlogItem->path);
        int index = backlogItem->index;
        mutex.unlock();

        QImage image(file.absoluteFilePath());

        if (!image.isNull()) {
            scaleImage(image);
            emit imageReady(image, index);
            // Sleep for 8ms to let other threads do their job.
            msleep(8);
        }
    }
}

void Loader::scaleImage(QImage& image) const
{
    image = image.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    if (image.size() != size) {
        QPoint start((image.width() - size.width()) / 2, (image.height() - size.height()) / 2);
        image = image.copy(QRect(start, size));
    }
}

bool Loader::hasWork()
{
    mutex.lock();
    bool result = false;
    int i = 0;
    while (!result && i < backlog.count())
        result = (!backlog.at(i++)->loaded);
    mutex.unlock();
    return result;
}

void Loader::pushImage(const QString &path, int index)
{
    BacklogItem *backlogItem = new BacklogItem;
    backlogItem->path = path;
    backlogItem->index = index;
    backlogItem->loaded = false;

    mutex.lock();
    backlog.append(backlogItem);
    mutex.unlock();
}

void Loader::resume(int offset)
{
    loadOffset = offset;
    stopWork = false;
    start(QThread::LowestPriority);
}

void Loader::stop()
{
    mutex.lock();
    stopWork = true;
    mutex.unlock();
}
