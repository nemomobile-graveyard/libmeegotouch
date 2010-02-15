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

#ifndef DUIESCAPEBUTTONPANELVIEW_H
#define DUIESCAPEBUTTONPANELVIEW_H

#include "duiscenewindowview.h"
#include "duiescapebuttonpanelmodel.h"
#include <duiescapebuttonpanelstyle.h>

class DuiEscapeButtonPanelViewPrivate;
class DuiEscapeButtonPanel;

/*!
 * \class DuiEscapeButtonPanelView
 * \brief The DuiEscapeButtonPanelView class just draws an escape button.
 *
 * It just draws an escape button on an otherwise empty scene window.
 */
class DUI_EXPORT DuiEscapeButtonPanelView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiEscapeButtonPanelModel, DuiEscapeButtonPanelStyle)

public:
    DuiEscapeButtonPanelView(DuiEscapeButtonPanel *controller);
    virtual ~DuiEscapeButtonPanelView();

protected:
    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiEscapeButtonPanelView)
    Q_DECLARE_PRIVATE(DuiEscapeButtonPanelView)

    Q_PRIVATE_SLOT(d_func(), void finalizeEscapeButtonTransition())
    Q_PRIVATE_SLOT(d_func(), void opacityChange(qreal))
};

#endif // DUIESCAPEBUTTONPANELVIEW_P
