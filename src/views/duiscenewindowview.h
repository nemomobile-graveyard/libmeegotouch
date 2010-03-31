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

#ifndef DUISCENEWINDOWVIEW_H
#define DUISCENEWINDOWVIEW_H

#include "duiwidgetview.h"
#include "duiscenewindowmodel.h"
#include <duiscenewindowstyle.h>

class DuiSceneWindow;
class DuiSceneWindowViewPrivate;

/*!
 * \class DuiSceneWindowView
 * \brief This is a default view class for DuiSceneWindow.
 *
 * The DuiSceneWindowView provides an interface to notify
 * the scene manager about changes in scene window's
 * geometry-related style attributes. It is the preferred
 * base class for views of all types of scene windows.
 * Once its style changes, it emits a geometryAttributesChanged()
 * signal, which is propagated to the DuiSceneManager instance,
 * causing the window geometry and position to be recalculated.
 */
class DUI_EXPORT DuiSceneWindowView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiSceneWindowModel, DuiSceneWindowStyle)
    Q_PROPERTY(Qt::Alignment alignment READ alignment)
    Q_PROPERTY(QPointF offset READ offset)

public:
    /*!
     * \brief Creates a new instance of the view for the given /a controller.
     */
    DuiSceneWindowView(DuiSceneWindow *controller);
    /*!
     * \brief A destructor.
     */
    virtual ~DuiSceneWindowView();

    /*!
     * \brief Returns the current alignment.
     * Alignment defines how scene manager will position the window on the screen.
     * \returns The current alignment.
     */
    Qt::Alignment alignment() const;

    /*!
     * \brief Returns the current offset.
     * \returns The current offset.
     */
    QPointF offset() const;

protected:
    DuiSceneWindowView(DuiSceneWindowViewPrivate &dd, DuiSceneWindow *controller);

    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();
    //! \reimp_end

signals:
    /*!
     * \brief Emitted every time when the style of the scene window changes.
     */
    void geometryAttributesChanged();

private:
    Q_DECLARE_PRIVATE(DuiSceneWindowView)
    Q_DISABLE_COPY(DuiSceneWindowView)
};

#endif
