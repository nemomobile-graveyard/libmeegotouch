/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QDebug>

#include "chainingexampleinterface.h"

#include "imageviewer.h"

ImageViewer::ImageViewer( const MButton &button ) :
    chainingExampleInterface( new ChainingExampleInterface )
{
    QObject::connect( &button, SIGNAL( released() ), this, SLOT( showImage() ) );
}

ImageViewer::~ImageViewer()
{
    delete chainingExampleInterface;
}

void ImageViewer::showImage()
{
    QString imageFilename( "chained view from app 3" );
    QStringList list;
    list << imageFilename;

    if ( chainingExampleInterface->isValid() ) {
        chainingExampleInterface->showImage( imageFilename, list, "app3 title" );
    } else {
        qDebug() << "chainingExampleInterface not valid : " << chainingExampleInterface;
    }
}
