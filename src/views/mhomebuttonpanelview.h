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

#ifndef MHOMEBUTTONPANELVIEW_H
#define MHOMEBUTTONPANELVIEW_H

#include "mscenewindowview.h"
#include <mhomebuttonpanelstyle.h>

class MHomeButtonPanel;
class MHomeButtonPanelViewPrivate;

/*!
 * \class MHomeButtonPanelView
 * \brief The MHomeButtonPanelView class just draws a home button.
 *
 * It just draws a home button on an otherwise empty scene window.
 */
class M_VIEWS_EXPORT MHomeButtonPanelView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MHomeButtonPanelStyle)

public:
    MHomeButtonPanelView(MHomeButtonPanel *controller);
    virtual ~MHomeButtonPanelView();

protected:
    MHomeButtonPanelViewPrivate *const d_ptr;

    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DISABLE_COPY(MHomeButtonPanelView)
    Q_DECLARE_PRIVATE(MHomeButtonPanelView)

    Q_PRIVATE_SLOT(d_func(), void _q_buttonInteracted())
};

#endif
