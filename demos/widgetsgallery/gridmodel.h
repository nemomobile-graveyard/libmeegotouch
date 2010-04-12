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

#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include <QList>
#include <QSize>
#include <QPair>
#include <QDir>
#include <QPixmap>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QAbstractTableModel>

class Loader;

struct MediaType
{
    enum Type {
        Video = 0,
        Image
    } type;

    enum Rating {
        NoStar,
        OneStar,
        TwoStars,
        ThreeStars,
        FourStars,
        FiveStars
    } rating;

    QString path;
    QPixmap pixmap;
    Rating rate;
};

Q_DECLARE_METATYPE(MediaType);

class GridModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GridModel(const QSize &size, const QString &dir = QDir::currentPath());
    ~GridModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void rateImage(MediaType::Rating rating, const QString& id);

private slots:
    void insertImage(QImage pixmap, int index);

private:
    void createItems();
    QMap<int, QVariant> m_items;

    Loader* m_loader;
    QString m_dir;
};

typedef QPair<QString, int> BacklogItem;

class Loader : public QThread
{
    Q_OBJECT
public:
    explicit Loader (const QSize &s) : size(s), stopWork(false) { }
    void pushImage(const QString &path, int index);
    void stop();
    void scaleImage(QImage& image) const;
signals:
    void imageReady(QImage pixmap, int index);
protected:
    void run();
private:
    QMutex mutex;
    QWaitCondition haveWork;
    QList<BacklogItem> backlog;
    QSize size;
    bool stopWork;
};

#endif
