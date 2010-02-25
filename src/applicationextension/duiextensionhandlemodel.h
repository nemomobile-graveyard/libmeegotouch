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

#ifndef DUIEXTENSIONHANDLEMODEL_H
#define DUIEXTENSIONHANDLEMODEL_H

#include <duiwidgetmodel.h>

class DuiAppletSharedMutex;

//! \internal
class DuiExtensionHandleModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiExtensionHandleModel)
    Q_ENUMS(State)

public:
    /*!
     * Different states where the extension life cycle can be.
     */
    enum State {
        STOPPED,                     //!< The extension is stopped. This is the initial state for an installed extension
        RUNNING,                     //!< The extension client has connected and the extension is fully functional
        STARTING,                    //!< The extension process is starting
        BROKEN,                      //!< The extension is broken
        INSTALLING                   //!< The extension is currently being installed, so the handle is placeholder.
    };

protected:
    /*! The state of the extension */
    DUI_MODEL_PROPERTY(DuiExtensionHandleModel::State, currentState, CurrentState, true, STOPPED)

    /*! Current scaling factor of the extension, used for the view and event handling */
    DUI_MODEL_PROPERTY(qreal, scale, Scale, true, 1)

    //! Extension base name
    DUI_MODEL_PROPERTY(QString, baseName, BaseName, true, QString())

    //! A mutex for locking the pixmap
    DUI_MODEL_PTR_PROPERTY(DuiAppletSharedMutex *, pixmapMutex, PixmapMutex, true, NULL)

    //! Size hints returned by the extension
    DUI_MODEL_PROPERTY(QVector<QSizeF>, sizeHints, SizeHints, true, QVector<QSizeF>(Qt::NSizeHints, QSizeF(128, 128)))

    //! Installation progress status (percentage)
    DUI_MODEL_PROPERTY(int, installationProgress, InstallationProgress, true, 0)

    //! Package name to install from
    DUI_MODEL_PROPERTY(QString, packageName, PackageName, true, QString())

    //! Installation error
    DUI_MODEL_PROPERTY(QString, installationError, InstallationError, true, QString())
};
//! \internal_end

#endif // DUIEXTENSIONHANDLEMODEL_H
