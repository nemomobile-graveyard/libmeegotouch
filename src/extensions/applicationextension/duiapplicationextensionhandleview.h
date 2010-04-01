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

#ifndef DUIAPPLICATIONEXTENSIONHANDLEVIEW_H
#define DUIAPPLICATIONEXTENSIONHANDLEVIEW_H

#include "duiextensionhandleview.h"
#include "duiextensionhandle.h"
#include "duiextensionhandlemodel.h"
#include "duiextensionhandlestyle.h"

class DuiApplicationExtensionHandleViewPrivate;

//! \internal
/*!
 * DuiApplicationExtensionHandleView is a view for the DuiApplicationExtensionHandle that
 * renders the extension's pixmap, shows whether the extension is in
 * a broken state or not and displays a dialog if the extension
 * is broken and the user taps it.
 */
class DuiApplicationExtensionHandleView : public DuiExtensionHandleView
{
    Q_OBJECT
    DUI_VIEW(DuiExtensionHandleModel, DuiExtensionHandleStyle)

public:
    /*!
     * Constructs a DuiApplicationExtensionHandleView.
     *
     * \param handle the DuiExtensionHandle controller to be used
     */
    DuiApplicationExtensionHandleView(DuiExtensionHandle *handle);

    /*!
     * Destroys the DuiApplicationExtensionHandleView.
     */
    virtual ~DuiApplicationExtensionHandleView();

protected:
    /*!
     * Constructs a DuiApplicationExtensionHandleView. Should be used from derived classes.
     *
     * \param dd the private class
     * \param handle the DuiExtensionHandle controller to be used
     */
    DuiApplicationExtensionHandleView(DuiApplicationExtensionHandleViewPrivate &dd, DuiExtensionHandle *handle);

private:
    Q_DISABLE_COPY(DuiApplicationExtensionHandleView)
    Q_DECLARE_PRIVATE(DuiApplicationExtensionHandleView)
};
//! \internal_end

#endif // DUIAPPLICATIONEXTENSIONHANDLEVIEW_H
