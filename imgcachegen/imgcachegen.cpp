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


#include <cstdio>
#include <cstring>


#include <QApplication>
#include <QList>
#include <QFile>
#include <QDir>
#include <QImageReader>

#include "mdebug.h"

#include "mimagedirectory.h"
#include "msystemdirectories.h"


class PixmapSaver
{
public:
    PixmapSaver(QString absoluteFilePath) : path(absoluteFilePath)
    {
        pixmap = QImage(absoluteFilePath);
    }

    void saveToFsCache()
    {
        const QString &constRef = path;

        ImageResource::saveToFsCache(pixmap, pixmap.size(), constRef);
    }

private:
    QImage pixmap;
    QString path;
};

void copyToFSCache(QFileInfo fileInfo)
{
    PixmapSaver saver(fileInfo.absoluteFilePath());
    saver.saveToFsCache();
}

void visitDirectory(QDir dir)
{
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        if(fileInfo.isDir())
            visitDirectory( QDir(fileInfo.absoluteFilePath()) );
        else if(QImageReader::supportedImageFormats().contains(fileInfo.suffix().toLatin1()))
            copyToFSCache(fileInfo);
    }
}


void printUsage(const char *prog)
{
    fprintf(stderr, "usage: %s [--prefix prefix] [directory ...]\n", prog);
}


int main(int argc, char** argv)
{
    QList<QDir> directories;
    int errors = 0;

    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            if(!strcmp(argv[i], "--prefix") && i+1 < argc) {
                MSystemDirectories::setPrefix( argv[i+1] );
                i++;
            } else {
                printUsage(argv[0]);
                return 1;
            }
        } else
            directories << QDir( argv[i] );
    }

    if(directories.size() < 1)
        directories << QDir( THEMEDIR );


    mDebug("mtheme") << "converting images ...";

    for(QList<QDir>::iterator i = directories.begin();
        i != directories.end(); i++) {
        if(i->exists())
            visitDirectory(*i);
	else
            errors++;
    }

    mDebug("mtheme") << (errors ? "fail!" : "success!");

    return errors;
}

