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

class MStatusBarPrivate;

/*!
 * \class MStatusBar
 * \brief A status bar that shows essential information on overall system status.
 *
 * Most applications shouldn't use this class directly as MApplicationWindow provides one already.
 * On the other hand, if an application is using an MWindow instead, a status bar may be added to
 * its GUI by utilizing this class.
 *
 * MSceneManager does not accept more than one MStatusBar instance.
 */
class M_CORE_EXPORT MStatusBar : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MSceneWindow)

public:
    /*!
     * \brief Default constructor.
     */
    MStatusBar();

    MStatusBar(QGraphicsItem *parent);

    /*!
      \brief Destroys the status bar.
     */
    virtual ~MStatusBar();

    //! \reimp
    bool sceneEvent(QEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MStatusBar)
    Q_DISABLE_COPY(MStatusBar)
};

#endif
