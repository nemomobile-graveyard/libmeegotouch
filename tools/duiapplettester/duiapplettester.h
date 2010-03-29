/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUIAPPLETTESTER_H
#define DUIAPPLETTESTER_H

#include <QObject>
#include <duiappletid.h>

class DuiAppletMetaData;
class DuiDataStore;
class QString;
class QSize;
class DuiWidget;
class DuiAppletTesterWindow;
class DuiApplication;
class DuiSettingsLanguageBinary;
class DuiAppletSettings;
class DuiGConfDataStore;

/*!
 * A simple applet loader. Constructs an instance data store to an applet
 * instance and constructs the applet widget.
 */
class DuiAppletTester : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    DuiAppletTester();

    /*!
     * Destructor.
     */
    ~DuiAppletTester();

    /*!
     * Initialize the applet.
     * \param metaData Metadata of the applet.
     * \param appletId AppletId of the applet.
     * \return \c true if the applet was initialized correctly, \c false if something went wrong.
     */
    bool init(const DuiAppletMetaData &metaData, DuiAppletId appletId);

    /*!
     * Starts the applet tester. Constructs a window and starts the application
     * event loop. The parameters are passed to DuiApplication.
     * \return the return value of DuiApplication::exec().
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
    static DuiDataStore *generateInstanceDataStorePath(const QString &appletBinary);

    /*!
     * Cleans up any resources that this class has currently reserverd.
     */
    void teardown();

    //! The application object
    DuiApplication *m_app;

    //! The applet metadata
    const DuiAppletMetaData *m_metadata;

    //! The applet instance data store
    DuiDataStore *m_appletInstanceDataStore;

    //! The applet widget
    DuiWidget *m_widget;

    //! The application's window
    DuiAppletTesterWindow *m_window;

    //! A list of applet instance settings binaries read during initialization
    QList<DuiSettingsLanguageBinary *> instanceSettingsBinaries;

    //! A list of applet global settings binaries read during initialization
    QList<DuiSettingsLanguageBinary *> globalSettingsBinaries;

    //! Applet settings
    DuiAppletSettings *appletSettings;
};

#endif // DUIAPPLETTESTER_H
