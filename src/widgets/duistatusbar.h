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

#ifndef DUISTATUSBAR_H
#define DUISTATUSBAR_H

#include <duiscenewindow.h>
#include <duiscenewindowmodel.h>

class DuiStatusBarPrivate;

//! \internal
class DuiStatusBar : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiSceneWindow)

public:
    /*!
     * \brief Default constructor.
     */
    DuiStatusBar();

    /*!
      \brief Destroys the status bar.
     */
    virtual ~DuiStatusBar();

    //! \reimp
    bool sceneEvent(QEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(DuiStatusBar)
    Q_DISABLE_COPY(DuiStatusBar)
};

//! \internal_end

#endif
