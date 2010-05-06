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

#include <QImage>
#include <QPixmap>
#include <QPainter>

#include "gridmodel.h"

GridModel::GridModel(const QSize &size, const QString &dir)
{
    QDir imagesDir(dir);
    imagesDir.setNameFilters(QStringList() << "*.jpeg" << "*.jpg" << "*.png");

    m_itemPaths = new QList<QString>;
    QStringList entries = imagesDir.entryList(QDir::Files);
    foreach(const QString & entry, entries) {
        m_items.append(QPixmap(size));
        m_itemPaths->append(dir + entry);
    }

    m_loader = new Loader(size);
    QObject::connect(m_loader, SIGNAL(imageReady(const QImage, int)), this, SLOT(updateImage(const QImage, int)));

    m_loader->start(QThread::LowestPriority);
}

GridModel::~GridModel()
{
    m_loader->stop();
    m_loader->wait();

    delete m_loader;
    delete m_itemPaths;
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

    int idx = index.row();
    if (role == Qt::DecorationRole) {
        const QString path = m_itemPaths->at(idx);

        if (!path.isEmpty()) {
            m_itemPaths->replace(idx, QString());
            m_loader->pushImage(path, idx);
        }
        return QVariant(m_items.at(idx));
    }

    return QVariant();
}

void GridModel::updateImage(const QImage image, int idx)
{
    QImage ours = image;

    QPixmap pixmap = QPixmap::fromImage(ours);

    m_items.replace(idx, pixmap);

    emit dataChanged(createIndex(idx, 0), createIndex(idx, 0));
}

void Loader::run()
{
    while (true) {
        mutex.lock();
        if (stopWork) {
            mutex.unlock();
            return;
        }
        if (backlog.isEmpty()) {
            haveWork.wait(&mutex);
            mutex.unlock();
            continue;
        }

        BacklogItem backlogItem = backlog.takeLast();
        mutex.unlock();

        QString path = backlogItem.first;
        int idx = backlogItem.second;

        QImage image(path);
        image = image.scaled(size, Qt::KeepAspectRatioByExpanding);
        if (image.size() != size) {
            QPoint start((image.width() - size.width()) / 2, (image.height() - size.height()) / 2);
            image = image.copy(QRect(start, size));
        }

        emit imageReady(image, idx);
    }
}

void Loader::pushImage(const QString &path, int idx)
{
    BacklogItem backlogItem(path, idx);
    mutex.lock();

    backlog.append(backlogItem);
    mutex.unlock();
    haveWork.wakeAll();
}

void Loader::stop()
{
    mutex.lock();
    stopWork = true;
    haveWork.wakeAll();
    mutex.unlock();
}
