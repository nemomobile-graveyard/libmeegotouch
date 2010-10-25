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

#ifndef MAPPLICATIONPAGEVIEW_H
#define MAPPLICATIONPAGEVIEW_H

#include "mscenewindowview.h"
#include <mapplicationpagestyle.h>
#include <mapplicationpagemodel.h>

class MApplicationPage;
class MApplicationPageViewPrivate;

class M_VIEWS_EXPORT MApplicationPageView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MApplicationPageModel, MApplicationPageStyle)

public:
    MApplicationPageView(MApplicationPage *controller);
    virtual ~MApplicationPageView();

protected:
    MApplicationPageViewPrivate *const d_ptr;

    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();

protected slots:
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_PRIVATE_SLOT(d_func(), void _q_onPageStateChanged())
    Q_DECLARE_PRIVATE(MApplicationPageView)
};

#endif

