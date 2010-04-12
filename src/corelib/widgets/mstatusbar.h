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

#ifndef MSTATUSBAR_H
#define MSTATUSBAR_H

#include <mscenewindow.h>
#include <mscenewindowmodel.h>

//! \internal

class MStatusBar : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MSceneWindow)

public:

    /*!
     * \brief Default constructor.
     */
    MStatusBar();

    /*!
      \brief Destroys the status bar.
     */
    virtual ~MStatusBar();

private:
    Q_DISABLE_COPY(MStatusBar)
};

//! \internal_end

#endif
