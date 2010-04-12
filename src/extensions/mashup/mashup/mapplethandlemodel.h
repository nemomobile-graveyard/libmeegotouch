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

#ifndef MAPPLETHANDLEMODEL_H
#define MAPPLETHANDLEMODEL_H

#include "mextensionhandlemodel.h"
#include <mappletid.h>

class MAppletSettings;

//! \internal
class MAppletHandleModel : public MExtensionHandleModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MAppletHandleModel)

private:
    /*! The icon ID of the applet */
    M_MODEL_PROPERTY(QString, appletIcon, AppletIcon, true, QString())

    /*! The title of the applet */
    M_MODEL_PROPERTY(QString, appletTitle, AppletTitle, true, QString())

    /*! The container text of the applet */
    M_MODEL_PROPERTY(QString, appletText, AppletText, true, QString())

    //! MAppletSettings of the applet
    M_MODEL_PTR_PROPERTY(MAppletSettings *, appletSettings, AppletSettings, true, NULL)

    //! The applet instance identifier given to the init()
    M_MODEL_PROPERTY(MAppletId, appletId, AppletId, true, MAppletId())
};
//! \internal_end

#endif // MAPPLETHANDLEMODEL_H
