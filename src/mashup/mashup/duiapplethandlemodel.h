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

#ifndef DUIAPPLETHANDLEMODEL_H
#define DUIAPPLETHANDLEMODEL_H

#include <duiwidgetmodel.h>
#include <duiappletid.h>

class DuiAppletSharedMutex;
class DuiAppletSettings;

//! \internal
class DuiAppletHandleModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiAppletHandleModel)
    Q_ENUMS(AppletState)

public:
    /*!
     * Different states where the applet life cycle can be.
     */
    enum AppletState {
        STOPPED,                     //!< The applet is stopped. This is the initial state for an installed applet
        RUNNING,                     //!< The applet client has connected and the applet is fully functional
        STARTING,                    //!< The applet process is starting
        BROKEN,                      //!< The applet is broken
        INSTALLING                   //!< The applet is currently being installed, so the handle is placeholder.
    };

private:
    /*! The state of the applet*/
    DUI_MODEL_PROPERTY(DuiAppletHandleModel::AppletState, state, State, true, STOPPED)

    /*! Current scaling factor of the applet, used for the view and event handling */
    DUI_MODEL_PROPERTY(qreal, appletScale, AppletScale, true, 1)

    /*! The icon ID of the applet */
    DUI_MODEL_PROPERTY(QString, appletIcon, AppletIcon, true, QString())

    /*! The title of the applet */
    DUI_MODEL_PROPERTY(QString, appletTitle, AppletTitle, true, QString())

    /*! The container text of the applet */
    DUI_MODEL_PROPERTY(QString, appletText, AppletText, true, QString())

    //! DuiAppletSettings of the applet
    DUI_MODEL_PTR_PROPERTY(DuiAppletSettings *, appletSettings, AppletSettings, true, NULL)

    //! The applet instance identifier given to the init()
    DUI_MODEL_PROPERTY(DuiAppletId, appletId, AppletId, true, DuiAppletId())

    //! The applet base name
    DUI_MODEL_PROPERTY(QString, appletBaseName, AppletBaseName, true, QString())

    //! A mutex for locking the pixmap
    DUI_MODEL_PTR_PROPERTY(DuiAppletSharedMutex *, pixmapMutex, PixmapMutex, true, NULL)

    //! Size hints returned by the applet
    DUI_MODEL_PROPERTY(QVector<QSizeF>, sizeHints, SizeHints, true, QVector<QSizeF>(Qt::NSizeHints, QSizeF(128, 128)))

    //! Applet installation progress status (percentage)
    DUI_MODEL_PROPERTY(int, installationProgress, InstallationProgress, true, 0)

    //! Applet's package name to install from
    DUI_MODEL_PROPERTY(QString, packageName, PackageName, true, QString())

    //! Applet's installation error
    DUI_MODEL_PROPERTY(QString, installationError, InstallationError, true, QString())
};
//! \internal_end

#endif // DUIAPPLETHANDLEMODEL_H
