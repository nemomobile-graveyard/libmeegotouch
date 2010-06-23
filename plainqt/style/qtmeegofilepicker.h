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

#ifndef MFILEPICKER_H
#define MFILEPICKER_H

#include <MApplication>
#include <MApplicationWindow>
#include <MScene>
#include <MApplicationPage>

#include <QtGui/QFileDialog>

// Use Qt Tracker API to get attachment data from tracker:
#include <QtTracker/Tracker>
#include <QtTracker/ontologies/nie.h>
#include <QtTracker/ontologies/nfo.h>

using namespace SopranoLive;

class ContentSearchInterface;


class MeegoFilePicker : public QEventLoop
{
  Q_OBJECT
public:
    enum MeegoFilePickerMode {
        Mode_Unknown = -1,
        Mode_OpenFile = 0,
        Mode_OpenFiles = 1,
        Mode_SaveFile = 2,
        Mode_OpenDirectory = 3
    };

    MeegoFilePicker(QObject *aParent = 0);
    virtual ~MeegoFilePicker();

    void SetMode(MeegoFilePickerMode aMode);
    QStringList getSelectedFileNames();

    int exec(ProcessEventsFlags aFlags = QEventLoop::AllEvents);

public slots:
    void contentItemSelected(const QString &aCaller, const QString &aItem);
    void contentItemsSelected(const QString &aCaller, const QStringList &aItems);

private:
    void OpenFile();
    void OpenFiles();

    QString mId;
    ContentSearchInterface* mSearchIf;
    MeegoFilePickerMode mMode;
    QStringList mSelectedFileNames;
};

#endif


