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

#include "duiextensionhandlemodel.h"
#include <duiappletid.h>

class DuiAppletSettings;

//! \internal
class DuiAppletHandleModel : public DuiExtensionHandleModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiAppletHandleModel)

private:
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
};
//! \internal_end

#endif // DUIAPPLETHANDLEMODEL_H
