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

#include "gallery.h"
#include "galleryview.h"
#include <MImage>
#include <MGrid>
#include <MWidgetFactory>
#include <MWidgetFactoryPlugins>

int GalleryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant GalleryModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() >= 0);

    if (role == Qt::DisplayRole) {
        MImage *b = NULL;
        switch (index.row()) {
        case 0:
            b = new MImage("Thumbs/44715f33b0e22876fa9b00447908fe7f.png");
            break;
        case 1:
            b = new MImage("Thumbs/1114d7211aa25eee553235a09d3fa836.png");
            break;
        case 2:
            b = new MImage("Thumbs/c58b6248c9f1d4d391467a898c4d7032.png");
            break;
        default:
            b = new MImage("Thumbs/65b89686ef3bb549a6b2bb7a1ea605e1.png");
            break;
        }

        b->setMinimumSize(100, 100);
        b->setPreferredSize(100, 100);
        b->setMaximumSize(100, 100);

        MWidgetPtr w;
        QVariant v;
        w = b;
        v.setValue(w);

        return v;
    }

    return QVariant();
}

Gallery::Gallery(const MAppletMetaData &metadata, MDataStore &instanceData, MDataAccess &settings)
    : m_grid(NULL)
{
    Q_UNUSED(metadata);
    Q_UNUSED(instanceData);
    Q_UNUSED(settings);

    m_grid = new MGrid(this);
    m_grid->resize(300, 300);
    m_grid->setSpacing(10);
    m_grid->setLayoutMode(MGridModel::SinglePass);

    GalleryModel *model = new GalleryModel();
    m_grid->setItemModel(model);

    setIcon(QString("Icon-pictures"));
    setTitle(QString("Gallery"));
    setView(new GalleryView(this));
}

Gallery::~Gallery()
{
}

void Gallery::expanded(bool state)
{
    if (state)
        setTitle(QString("Exp"));
    else
        setTitle(QString("Normal"));
}

const QString &Gallery::icon() const
{
    return m_icon;
}

void Gallery::setIcon(const QString &newIcon)
{
    m_icon = newIcon;
    emit setAppletIcon(newIcon);
}

const QString &Gallery::title() const
{
    return m_title;
}

void Gallery::setTitle(const QString &newTitle)
{
    m_title = newTitle;
    emit setAppletTitle(newTitle);
}

const QString &Gallery::text() const
{
    return m_text;
}

void Gallery::setText(const QString &newText)
{
    m_text = newText;
    emit setAppletText(newText);
}
