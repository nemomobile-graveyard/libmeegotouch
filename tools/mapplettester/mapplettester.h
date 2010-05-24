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

#ifndef MAPPLETTESTER_H
#define MAPPLETTESTER_H

#include <QObject>
#include <mappletid.h>

class MAppletMetaData;
class MDataStore;
class QString;
class QSize;
class QGraphicsWidget;
class MAppletTesterWindow;
class MApplication;
class MSettingsLanguageBinary;
class MAppletSettings;
class MGConfDataStore;

/*!
 * A simple applet loader. Constructs an instance data store to an applet
 * instance and constructs the applet widget.
 */
class MAppletTester : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    MAppletTester();

    /*!
     * Destructor.
     */
    ~MAppletTester();

    /*!
     * Initialize the applet.
     * \param metaData Metadata of the applet.
     * \param appletId AppletId of the applet.
     * \return \c true if the applet was initialized correctly, \c false if something went wrong.
     */
    bool init(const MAppletMetaData &metaData, MAppletId appletId);

    /*!
     * Starts the applet tester. Constructs a window and starts the application
     * event loop. The parameters are passed to MApplication.
     * \return the return value of MApplication::exec().
     */
    int exec();

public slots:
    /*!
     * Notify this object that the window size has changed. The new size
     * is passed as a parameter.
     * \param newSize the new window size.
     */
    void windowResized(const QSize &newSize);

protected slots:
    /*!
     * \brief A slot for opening applet settings
     */
    void openAppletSettings();

private:
    /*!
     * Generates an instance data store object for an applet instance. The applet binary
     * name is used as an identifier. The returned data store is readable and writable.
     * If no such data store can be created, this method returns \c NULL.
     * \param appletBinary the applet binary name.
     * \return a new data store object or \c NULL if one couldn't be constructed
     */
    static MDataStore *generateInstanceDataStorePath(const QString &appletBinary);

    /*!
     * Cleans up any resources that this class has currently reserverd.
     */
    void teardown();

    //! The application object
    MApplication *m_app;

    //! The applet metadata
    const MAppletMetaData *m_metadata;

    //! The applet instance data store
    MDataStore *m_appletInstanceDataStore;

    //! The applet widget
    QGraphicsWidget *m_widget;

    //! The application's window
    MAppletTesterWindow *m_window;

    //! A list of applet instance settings binaries read during initialization
    QList<MSettingsLanguageBinary *> instanceSettingsBinaries;

    //! A list of applet global settings binaries read during initialization
    QList<MSettingsLanguageBinary *> globalSettingsBinaries;

    //! Applet settings
    MAppletSettings *appletSettings;
};

#endif // MAPPLETTESTER_H
