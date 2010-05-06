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

#ifndef MAPPLICATIONEXTENSIONHANDLEVIEW_H
#define MAPPLICATIONEXTENSIONHANDLEVIEW_H

#include "mextensionhandleview.h"
#include "mextensionhandle.h"
#include "mextensionhandlemodel.h"
#include "mextensionhandlestyle.h"

class MApplicationExtensionHandleViewPrivate;

//! \internal
/*!
 * MApplicationExtensionHandleView is a view for the MApplicationExtensionHandle that
 * renders the extension's pixmap, shows whether the extension is in
 * a broken state or not and displays a dialog if the extension
 * is broken and the user taps it.
 */
class MApplicationExtensionHandleView : public MExtensionHandleView
{
    Q_OBJECT
    M_VIEW(MExtensionHandleModel, MExtensionHandleStyle)

public:
    /*!
     * Constructs a MApplicationExtensionHandleView.
     *
     * \param handle the MExtensionHandle controller to be used
     */
    MApplicationExtensionHandleView(MExtensionHandle *handle);

    /*!
     * Destroys the MApplicationExtensionHandleView.
     */
    virtual ~MApplicationExtensionHandleView();

protected:
    /*!
     * Constructs a MApplicationExtensionHandleView. Should be used from derived classes.
     *
     * \param dd the private class
     * \param handle the MExtensionHandle controller to be used
     */
    MApplicationExtensionHandleView(MApplicationExtensionHandleViewPrivate &dd, MExtensionHandle *handle);

private:
    Q_DISABLE_COPY(MApplicationExtensionHandleView)
    Q_DECLARE_PRIVATE(MApplicationExtensionHandleView)
};
//! \internal_end

#endif // MAPPLICATIONEXTENSIONHANDLEVIEW_H
