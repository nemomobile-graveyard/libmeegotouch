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

#ifndef GALLERY_H
#define GALLERY_H

#include <QAbstractListModel>
#include <mwidgetcontroller.h>

class QByteArray;
class MAppletMetaData;
class MDataStore;
class MGrid;
class MDataAccess;

class GalleryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    GalleryModel(QObject *parent = 0)
        : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
};

class Gallery : public MWidgetController
{
    Q_OBJECT
    Q_PROPERTY(QString appletIcon READ icon WRITE setIcon)
    Q_PROPERTY(QString appletTitle READ title WRITE setTitle)
    Q_PROPERTY(QString appletText READ text WRITE setText)

public:

    Gallery(const MAppletMetaData &metaData, MDataStore &instanceData, MDataAccess &settings);
    virtual ~Gallery();

    const QString &icon() const;
    const QString &title() const;
    const QString &text() const;

public slots:

    void setIcon(const QString &newIcon);
    void setTitle(const QString &newTitle);
    void setText(const QString &newText);

    void expanded(bool state);

signals:

    void setAppletIcon(const QString &);
    void setAppletTitle(const QString &);
    void setAppletText(const QString &);

private:

    MGrid *m_grid;
    QString m_icon;
    QString m_title;
    QString m_text;

};

#endif
