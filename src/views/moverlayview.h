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

#ifndef MOVERLAYVIEW_H
#define MOVERLAYVIEW_H

#include <moverlaystyle.h>
#include "mscenewindowview.h"

class MOverlay;

class M_VIEWS_EXPORT MOverlayView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MOverlayStyle)

public:
    MOverlayView(MOverlay *controller);
    virtual ~MOverlayView();

private:
    Q_DISABLE_COPY(MOverlayView)
#ifdef UNIT_TEST
    friend class Ut_MOverlay;
#endif
};

#endif
