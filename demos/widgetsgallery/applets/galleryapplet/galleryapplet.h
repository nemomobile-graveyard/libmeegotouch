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

#ifndef GALLERYAPPLET_H
#define GALLERYAPPLET_H

#include <mappletinterface.h>
#include <QObject>

// This class doesn't do anything else but creates widgets inside this library
class GalleryApplet : public QObject, public MAppletInterface
{
    Q_OBJECT
    Q_INTERFACES(MAppletInterface)

public:
    // methods derived from MAppletInterface
    virtual MWidget *constructWidget(const MAppletMetaData &metaData, MDataStore &instanceData, MDataAccess &settings);
};

#endif
