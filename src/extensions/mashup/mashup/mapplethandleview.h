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

#ifndef MAPPLETHANDLEVIEW_H
#define MAPPLETHANDLEVIEW_H

#include "mextensionhandleview.h"
#include "mapplethandle.h"
#include "mapplethandlemodel.h"
#include "mapplethandlestyle.h"

class MAppletHandleViewPrivate;

//! \internal
/*!
 * MAppletHandleView is a view for the MAppletHandle that
 * renders the applet's pixmap, shows whether the applet is in
 * a broken state or not and displays a dialog if the applet
 * is broken and the user taps it.
 */
class MAppletHandleView : public MExtensionHandleView
{
    Q_OBJECT
    M_VIEW(MAppletHandleModel, MAppletHandleStyle)

public:
    /*!
     * Constructs a MAppletHandleView.
     *
     * \param appletHandle the MAppletHandle controller to be used
     */
    MAppletHandleView(MAppletHandle *appletHandle);

    /*!
     * Destroyes the MAppletHandleView.
     */
    virtual ~MAppletHandleView();

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    //! \reimp
    virtual void setupModel();
    //! \reimp_end

    /*!
     * \brief A slot for opening applet settings
     */
    void openAppletSettings();

private :
    Q_DISABLE_COPY(MAppletHandleView)
    Q_DECLARE_PRIVATE(MAppletHandleView)
};
//! \internal_end

#endif // MAPPLETHANDLEVIEW_H
