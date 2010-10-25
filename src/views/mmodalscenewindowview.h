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

#ifndef MMODALSCENEWINDOWVIEW_H
#define MMODALSCENEWINDOWVIEW_H

#include <mmodalscenewindowstyle.h>
#include "mscenewindowview.h"
#include "mmodalscenewindowmodel.h"

class MModalSceneWindow;
class MModalSceneWindowViewPrivate;

class M_VIEWS_EXPORT MModalSceneWindowView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MModalSceneWindowModel, MModalSceneWindowStyle)

public:
    MModalSceneWindowView(MModalSceneWindow *controller);
    virtual ~MModalSceneWindowView();

protected:
    MModalSceneWindowView(MModalSceneWindowViewPrivate &dd, MModalSceneWindow *controller);
    MModalSceneWindowViewPrivate *const d_ptr;

    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();

protected slots:
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MModalSceneWindowView)
    Q_DISABLE_COPY(MModalSceneWindowView)
#ifdef UNIT_TEST
    friend class Ut_MModalSceneWindow;
#endif
};

#endif
