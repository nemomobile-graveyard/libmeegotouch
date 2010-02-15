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

#ifndef DUIAPPLETHANDLESWVIEW_H
#define DUIAPPLETHANDLESWVIEW_H

#include <duiwidgetview.h>
#include "duiapplethandle.h"
#include "duiapplethandlemodel.h"
#include "duiapplethandleview.h"

//! \internal
class DuiAppletHandleSWViewPrivate;

/*!
 * DuiAppletHandleSWView is a view for the DuiAppletHandle that
 * renders graphics without OpenGL.
 */
class DuiAppletHandleSWView : public DuiAppletHandleView
{
    Q_OBJECT
    DUI_VIEW(DuiAppletHandleModel, DuiAppletHandleStyle)

public:
    /*!
     * Constructs a DuiAppletHandleSWView.
     *
     * \param appletHandle the DuiAppletHandle controller to be used
     */
    DuiAppletHandleSWView(DuiAppletHandle *appletHandle);

    /*!
     * Destroys the DuiAppletHandleSWView.
     */
    virtual ~DuiAppletHandleSWView();

private :
    Q_DISABLE_COPY(DuiAppletHandleSWView)
    Q_DECLARE_PRIVATE(DuiAppletHandleSWView)
};
//! \internal_end

#endif // DUIAPPLETHANDLESWVIEW_H
