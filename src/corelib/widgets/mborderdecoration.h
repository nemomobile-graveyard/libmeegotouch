/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MBORDERDECORATION_H
#define MBORDERDECORATION_H
//! \internal

#include "mscenewindow.h"

/*!
   \class MBorderDecoration
   \brief Used to decorate the borders of the visible area of a scene

   The alignment property of a MBorderDecoration defines which edge of
   the visible area of a scene it will decorate (i.e. the edge where it will
   be placed).
 */
class MBorderDecoration : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MSceneWindow)

public:

    /*!
      \brief Constructs an MBorderDecoration
     */
    MBorderDecoration();

    /*!
      \brief Destroys an MBorderDecoration
     */
    virtual ~MBorderDecoration();
};

//! \internal_end
#endif // MBORDERDECORATION_H