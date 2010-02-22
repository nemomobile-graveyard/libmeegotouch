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

#ifndef DUIHOMEBUTTONPANELVIEW_H
#define DUIHOMEBUTTONPANELVIEW_H

#include "duiscenewindowview.h"
#include <duihomebuttonpanelstyle.h>

class DuiHomeButtonPanel;
class DuiHomeButtonPanelViewPrivate;

/*!
 * \class DuiHomeButtonPanelView
 * \brief The DuiHomeButtonPanelView class just draws a home button.
 *
 * It just draws a home button on an otherwise empty scene window.
 */
class DUI_EXPORT DuiHomeButtonPanelView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiSceneWindowModel, DuiHomeButtonPanelStyle)

public:
    DuiHomeButtonPanelView(DuiHomeButtonPanel *controller);
    virtual ~DuiHomeButtonPanelView();

protected:
    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiHomeButtonPanelView)
    Q_DECLARE_PRIVATE(DuiHomeButtonPanelView)
};

#endif
