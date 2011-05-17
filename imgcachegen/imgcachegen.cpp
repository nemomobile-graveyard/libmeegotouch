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


#include <cstdio>
#include <cstring>


#include <QApplication>
#include <QList>
#include <QFile>
#include <QDir>
#include <QImageReader>

#include <QRegExp>

#include "mdebug.h"

#include "mimagedirectory.h"
#include "msystemdirectories.h"


  // TODO: invent meaningful name
class QueuedImageItem
{
public:
    QueuedImageItem(const QString &filename, const QSize &size) : p_filename(filename),
                                                                  p_size(size) { }

    const QString &filename( ) const { return p_filename; }
    const QSize &size( ) const { return p_size; }

private:
    QString p_filename;
    QSize p_size;
};


namespace {
    static bool gDebug = false;
}


bool saveToFsCache(const QString &filename, const QSize &size)
{
    ImageResource *resource = NULL;
    QImage image(filename);

    if (image.isNull()) {
        if (gDebug)
            mDebug("ImageCacheGen") << "Failed to load" << filename;

        return false;
    }

    QFileInfo fileInfo(filename);
    if (fileInfo.suffix() == QLatin1String("png") || fileInfo.suffix() == QLatin1String("jpg")) {
        resource = new PixmapImageResource(filename);
    }
    else if (fileInfo.suffix() == QLatin1String("svg")) {
        resource = new IconImageResource(filename);
    }

    if (resource) {
        resource->saveToFsCache(image, size);
        delete resource;

        return true;
    }

    return false;
}

bool saveToFsCache(const QString &filename)
{
    QSize defaultSize(0, 0);

    return saveToFsCache(filename, defaultSize);
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
            saveToFsCache(fileInfo.absoluteFilePath());
    }
}


bool appendImageInfoFromListFile(const char *filename,
                                 QList<QueuedImageItem *> &dest)
{
    QFile f(filename);
    if (f.exists() && f.open(QIODevice::ReadOnly)) {
        bool ret = true;

        QByteArray line;

        while((line = f.readLine()).size() > 0) { 
              // not sure whether the first set of characters should be expanded ...
            static QRegExp regex("([./-\\w]*)[^\\(]*"
                                 "\\((\\d+)[^,\\)]*,[^,\\)]*(\\d+)[^\\)]*\\)");

            if(regex.indexIn(line) == -1 || regex.captureCount() != 3)
                continue;  // in some cases ret should set to false


            QSize size(regex.cap(2).toInt(), regex.cap(3).toInt());

            dest.push_back(new QueuedImageItem(regex.cap(1), size));
        }


        f.close();

        return ret;
    }

    return false;
}


void printUsage(const char *prog)
{
    fprintf(stderr, "usage: %s [OPTION] [directory ...]\n\n", prog);
    fprintf(stderr, "  --prefix prefix    output directory prefix\n");
    fprintf(stderr, "  --verbose          display debug output\n");
    fprintf(stderr, "  --list <file>      use <file> as list of images to cache\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  If no directories of image lists are provided, the tool will cache every\n"
                    "image from default theme directory. This can result in excess caches\n"
                    "especially when there is multiple themes installed on system.\n");
}


int main(int argc, char** argv)
{
    QList<QDir> directories;
    QList<const char *> imgListFiles;
    QList<QueuedImageItem *> queuedItems;
    int errors = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "--prefix") == 0 && i+1 < argc) {
                MSystemDirectories::setPrefix( argv[++i] );
            } else if (strcmp(argv[i], "--list") == 0 && i+1 < argc) {
                imgListFiles.push_back( argv[++i] );
            } else if (strcmp(argv[i], "--verbose") == 0) {
                gDebug = true;
            } else {
                printUsage(argv[0]);
                return 1;
            }
        } else
            directories << QDir(argv[i]);
    }

    if (0 < imgListFiles.size()) {
        for (QList<const char *>::iterator i = imgListFiles.begin();
             i != imgListFiles.end(); i++)
            if (!appendImageInfoFromListFile(*i, queuedItems)) {
                errors++;

                if(gDebug)
                    mDebug("ImageCacheGen") << "Parsing error with" << *i;
            }
    }

    if (directories.size() < 1 && queuedItems.size() < 1)
        directories << QDir(THEMEDIR);


    mDebug("ImageCacheGen") << "Converting images ...";

    for (QList<QDir>::iterator i = directories.begin();
        i != directories.end(); i++) {
        if (i->exists())
            visitDirectory(*i);
	else
            errors++;
    }

    for (QList<QueuedImageItem *>::iterator i = queuedItems.begin();
         i != queuedItems.end(); i++) {
        if (!saveToFsCache((*i)->filename(), (*i)->size()))
            errors++;

        delete *i;
    }

    mDebug("ImageCacheGen") << (errors ? "FAIL!" : "Done!");

    return errors;
}

