/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSBAR_P_H
#define MSTATUSBAR_P_H

#include "mscenewindow_p.h"
#include "mstatusbar.h"

//! \internal
/*!
 * Private class for the MStatusBar.
 */
class MStatusBarPrivate : public MSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(MStatusBar)

public:
    /*!
     * Constructs a private class for the MStatusBar.
     */
    MStatusBarPrivate();

    /*!
     * Destroys the MStatusBar private class.
     */
    virtual ~MStatusBarPrivate();

    void setupClippedPaintOffsetProperty();

    void _q_scheduleUpdate();

};

//! \internal_end
#endif
