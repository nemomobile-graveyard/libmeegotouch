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
    : QAbstractTableModel(),
      m_loader(new Loader(size)),
      m_dir(dir)
{
    qRegisterMetaType<MediaType>("MediaType");

    m_loader->start(QThread::LowestPriority);
    QObject::connect(m_loader, SIGNAL(imageReady(QImage, int)), this, SLOT(insertImage(QImage, int)));

    createItems();
}

GridModel::~GridModel()
{
    m_loader->stop();
    m_loader->wait();

    delete m_loader;
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

    if( m_items[ index.row() ].canConvert<MediaType>() )
        m = m_items[ index.row() ].value<MediaType>();

    if (role == Qt::DecorationRole) {
        return m_items[ index.row() ];
    }

    return QVariant();
}

void GridModel::createItems()
{
    int index = 0;

    QDir mediaDir(m_dir);
    mediaDir.setFilter(QDir::Files | QDir::NoSymLinks);
    mediaDir.setSorting(QDir::NoSort);
    mediaDir.setNameFilters(QStringList() << "*.jpeg" << "*.jpg" << "*.mp4" << "*.mov");
    const QStringList videoTypes(QStringList() << "mp4" << "mov");
    const QFileInfoList fileList = mediaDir.entryInfoList();

    foreach(const QFileInfo & file, fileList) {
        const QString path = file.absoluteFilePath();
        MediaType m;

        if( videoTypes.contains( file.suffix() )) {
#ifdef HAVE_GSTREAMER
            if( file.fileName().startsWith("thumb-") ) {
                m.path = path;
                m.type = MediaType::Video;
            } else {
                continue;
            }
#else
            continue;
#endif
        } else {
            m.type = MediaType::Image;
            m.path = path;
            m.image = QImage();
            m_loader->pushImage(path,index);
        }

        m_items[index] = QVariant::fromValue(m);
        index++;
    }
}

void GridModel::insertImage(QImage image, int index)
{
    if( m_items[index].canConvert<MediaType>() ) {
        MediaType m = m_items[index].value<MediaType>();
        m.image = image;
        m.rating = MediaType::OneStar;
        m_items[index] = QVariant::fromValue(m);

        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void GridModel::rateImage(MediaType::Rating rating, const QString& id)
{
    int index = 0;
    foreach(QVariant v, m_items) {
        if( v.canConvert<MediaType>() ) {
            MediaType m = m_items[index].value<MediaType>();

            if(m.type == MediaType::Image) {
                if(m.path == id) {
                    m.rate = rating;

                    int amount = 0;
                    switch(rating) {
                    case MediaType::NoStar:
                        break;
                    case MediaType::OneStar:
                        amount = 1; break;
                    case MediaType::TwoStars:
                        amount = 2; break;
                    case MediaType::ThreeStars:
                        amount = 3; break;
                    case MediaType::FourStars:
                        amount = 4; break;
                    case MediaType::FiveStars:
                        amount = 5; break;
                    default: amount = 0;
                    }

                    // get a fresh image without badge
                    QImage image(id);
                    m_loader->scaleImage( image );
                    m.image = image;

                    m_items[index] = QVariant::fromValue(m);

                    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
                    return;
                }
            }
            index++;
        }
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
        if (backlog.isEmpty()) {
            haveWork.wait(&mutex);
            mutex.unlock();
            continue;
        }

        BacklogItem backlogItem = backlog.takeFirst();
        mutex.unlock();
        const QFileInfo file( backlogItem.first );
        QDir dir(file.absolutePath());
        dir.cd(".." + QString(QDir::separator()) + "thumbnails");
        int index = backlogItem.second;

        QImage image(dir.path() + QDir::separator() + file.fileName());

        if(!image.isNull()) {
            scaleImage(image);
            emit imageReady(image, index);
        } else {
            pushImage(backlogItem.first, index);
        }
    }
}

void Loader::scaleImage(QImage& image) const
{
    image = image.scaled(size, Qt::KeepAspectRatioByExpanding);
    if (image.size() != size) {
        QPoint start((image.width() - size.width()) / 2, (image.height() - size.height()) / 2);
        image = image.copy(QRect(start, size));
    }
}

void Loader::pushImage(const QString &path, int index)
{
    BacklogItem backlogItem(path, index);
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
