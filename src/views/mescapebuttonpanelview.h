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

#ifndef MESCAPEBUTTONPANELVIEW_H
#define MESCAPEBUTTONPANELVIEW_H

#include "mscenewindowview.h"
#include "mescapebuttonpanelmodel.h"
#include <mescapebuttonpanelstyle.h>

class MEscapeButtonPanelViewPrivate;
class MEscapeButtonPanel;

/*!
 * \class MEscapeButtonPanelView
 * \brief The MEscapeButtonPanelView class just draws an escape button.
 *
 * It just draws an escape button on an otherwise empty scene window.
 */
class M_VIEWS_EXPORT MEscapeButtonPanelView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MEscapeButtonPanelModel, MEscapeButtonPanelStyle)

public:
    MEscapeButtonPanelView(MEscapeButtonPanel *controller);
    virtual ~MEscapeButtonPanelView();

protected:
    MEscapeButtonPanelViewPrivate *const d_ptr;

    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MEscapeButtonPanelView)
    Q_DECLARE_PRIVATE(MEscapeButtonPanelView)

    Q_PRIVATE_SLOT(d_func(), void _q_buttonInteracted())
    Q_PRIVATE_SLOT(d_func(), void _q_fadeOutAnimationFinished())
};

#endif // MESCAPEBUTTONPANELVIEW_P
