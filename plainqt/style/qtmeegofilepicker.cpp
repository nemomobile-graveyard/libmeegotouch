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

#include "qtmeegofilepicker.h"

#include <maemo-meegotouch-interfaces/contentsearchinterface.h>


typedef QStringList (*_qt_file_open_filenames_hook)
 (QWidget * parent, const QString &caption, const QString &dir,
  const QString &filter, QString *selectedFilter, QFileDialog::Options options);

typedef QString (*_qt_file_open_filename_hook)
 (QWidget * parent, const QString &caption, const QString &dir,
  const QString &filter, QString *selectedFilter, QFileDialog::Options options);

typedef QString (*_qt_file_save_filename_hook)
 (QWidget * parent, const QString &caption, const QString &dir,
  const QString &filter, QString *selectedFilter, QFileDialog::Options options);

typedef QString (*_qt_file_existing_directory_hook)
 (QWidget *parent, const QString &caption, const QString &dir,
  QFileDialog::Options options);

Q_GUI_EXPORT _qt_file_open_filename_hook qt_filedialog_open_filename_hook;
Q_GUI_EXPORT _qt_file_open_filenames_hook qt_filedialog_open_filenames_hook;
Q_GUI_EXPORT _qt_file_save_filename_hook qt_filedialog_save_filename_hook;
Q_GUI_EXPORT _qt_file_existing_directory_hook qt_filedialog_existing_directory_hook;


class MeegoFilePickerQtNative {

  public:
    MeegoFilePickerQtNative()
    {
        qt_filedialog_open_filename_hook = &openFilename;
        qt_filedialog_save_filename_hook = &saveFilename;
        qt_filedialog_open_filenames_hook = &openFilenames;
        qt_filedialog_existing_directory_hook = &openDirectory;
    }

    static QString openFilename(QWidget *parent, const QString &caption,
                                const QString &dir, const QString &filter,
                                QString *selectedFilter,
                                QFileDialog::Options options)
    {
        Q_UNUSED( caption )
        Q_UNUSED( dir )
        Q_UNUSED( filter )
        Q_UNUSED( selectedFilter )
        Q_UNUSED( options )

        MeegoFilePicker picker(parent);
        picker.SetMode(MeegoFilePicker::Mode_OpenFile);
        picker.exec();

        QStringList selectedFiles = picker.getSelectedFileNames();

        if (!selectedFiles.isEmpty())
            return selectedFiles[0];
        return "";
    }

    static QString saveFilename(QWidget *parent, const QString &caption,
                                const QString &dir, const QString &filter,
                                QString *selectedFilter,
                                QFileDialog::Options options)
    {
        Q_UNUSED( caption )
        Q_UNUSED( dir )
        Q_UNUSED( filter )
        Q_UNUSED( selectedFilter )
        Q_UNUSED( options )

        MeegoFilePicker picker(parent);
        picker.SetMode(MeegoFilePicker::Mode_SaveFile);
        picker.exec();

        return "";
    }

    static QString openDirectory(QWidget *parent, const QString &caption, const
                                QString &dir, QFileDialog::Options options)
    {
        Q_UNUSED( caption )
        Q_UNUSED( dir )
        Q_UNUSED( options )

        MeegoFilePicker picker(parent);
        picker.SetMode(MeegoFilePicker::Mode_OpenDirectory);
        picker.exec();

        return "";
    }

    static QStringList openFilenames(QWidget *parent, const QString &caption,
                                    const QString &dir, const QString &filter,
                                    QString *selectedFilter,
                                    QFileDialog::Options options)
    {
        Q_UNUSED( caption )
        Q_UNUSED( dir )
        Q_UNUSED( filter )
        Q_UNUSED( selectedFilter )
        Q_UNUSED( options )

        MeegoFilePicker picker(parent);
        picker.SetMode(MeegoFilePicker::Mode_OpenFiles);
        picker.exec();

        return picker.getSelectedFileNames();
    }
};

static MeegoFilePickerQtNative qtNative;


MeegoFilePicker::MeegoFilePicker(QObject * aParent)
: QEventLoop(aParent)
, mMode(Mode_Unknown)
{
    // a unique id for the picker
    mId = QUuid::createUuid().toString();

    mSearchIf = new ContentSearchInterface(
                      "com.nokia.maemo.meegotouch.ContentSearch");
    connect(mSearchIf, SIGNAL(contentItemSelected(const QString &,
                                                  const QString &)),
            this, SLOT(contentItemSelected(const QString &,
                                           const QString &)));
    connect(mSearchIf, SIGNAL(contentItemsSelected(const QString &,
                                                   const QStringList &)),
            this, SLOT(contentItemsSelected(const QString &,
                                            const QStringList &)));
}

MeegoFilePicker::~MeegoFilePicker()
{
}

int MeegoFilePicker::exec(ProcessEventsFlags aFlags)
{
    switch(mMode)
    {
        case Mode_OpenFile:
            OpenFile();
            break;
        case Mode_OpenFiles:
            OpenFiles();
            break;
        //not implemented yet
        case Mode_OpenDirectory:
        case Mode_SaveFile:
        default:
            return 0;
    }
    return QEventLoop::exec(aFlags);
}

void MeegoFilePicker::OpenFile()
{
    mSearchIf->selectSingleContentItem(mId, QString(), QStringList() <<
    "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#FileDataObject",
    QString());
}

void MeegoFilePicker::OpenFiles()
{
    mSearchIf->selectMultipleContentItems(mId, QString(), QStringList() <<
    "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#FileDataObject",
    QStringList());
}

void MeegoFilePicker::SetMode(MeegoFilePickerMode aMode)
{
    mMode = aMode;
}

QStringList MeegoFilePicker::getSelectedFileNames()
{
    return mSelectedFileNames;
}

void MeegoFilePicker::contentItemSelected(const QString &aCallerId,
                                          const QString &aContentItem)
{
    if ( mId != aCallerId )
        return;

    mSelectedFileNames.clear();

    if (aContentItem.isEmpty())
    {
        exit(0);
        return;
    }

    Live<nfo::FileDataObject> data = ::tracker()->liveNode(QUrl(aContentItem));
     QUrl fileUrl = data->getUrl();
    QFileInfo fileInfo(fileUrl.toLocalFile());
    if (fileInfo.isFile()) {
        mSelectedFileNames << fileInfo.canonicalFilePath();
    }

    exit(0);
}

void MeegoFilePicker::contentItemsSelected(const QString &aCallerId,
                                           const QStringList &aContentItems)
{
    if ( mId != aCallerId )
        return;

    mSelectedFileNames.clear();

    foreach(QString contentItem, aContentItems)
    {
        Live<nfo::FileDataObject> data =
          ::tracker()->liveNode(QUrl(contentItem));
        QUrl fileUrl = data->getUrl();
        QFileInfo fileInfo(fileUrl.toLocalFile());
        if (fileInfo.isFile())
            mSelectedFileNames << fileInfo.canonicalFilePath();
    }
    exit(0);
}

