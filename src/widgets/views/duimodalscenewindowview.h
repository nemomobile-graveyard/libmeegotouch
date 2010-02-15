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

#ifndef DUIMODALSCENEWINDOWVIEW_H
#define DUIMODALSCENEWINDOWVIEW_H

#include <duimodalscenewindowstyle.h>
#include "duiscenewindowview.h"
#include "duimodalscenewindowmodel.h"

class DuiModalSceneWindow;
class DuiModalSceneWindowViewPrivate;

class DUI_EXPORT DuiModalSceneWindowView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiModalSceneWindowModel, DuiModalSceneWindowStyle)

public:
    DuiModalSceneWindowView(DuiModalSceneWindow *controller);
    virtual ~DuiModalSceneWindowView();

protected:
    DuiModalSceneWindowView(DuiModalSceneWindowViewPrivate &dd, DuiModalSceneWindow *controller);

    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();

protected slots:
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(DuiModalSceneWindowView)
    Q_DISABLE_COPY(DuiModalSceneWindowView)
#ifdef UNIT_TEST
    friend class Ut_DuiModalSceneWindow;
#endif
};

#endif
