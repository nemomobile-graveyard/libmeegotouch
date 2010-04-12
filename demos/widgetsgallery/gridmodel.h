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

typedef QPair<QString, int> BacklogItem;

class Loader : public QThread
{
    Q_OBJECT
public:
    explicit Loader (const QSize &s) : size(s), stopWork(false) { }
    void pushImage(const QString &path, int idx);
    void stop();
signals:
    void imageReady(const QImage image, int idx);
protected:
    void run();
private:
    QMutex mutex;
    QWaitCondition haveWork;
    QList<BacklogItem> backlog;
    QSize size;
    bool stopWork;
};

class GridModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GridModel(const QSize &size = QSize(250, 250), const QString &dir = QDir::currentPath());
    ~GridModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void updateImage(const QImage image, int idx);

private:
    Loader *m_loader;
    QList<QPixmap> m_items;
    QList<QString> *m_itemPaths;
};

#endif
