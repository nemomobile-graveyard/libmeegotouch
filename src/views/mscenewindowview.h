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

#ifndef MSCENEWINDOWVIEW_H
#define MSCENEWINDOWVIEW_H

#include "mwidgetview.h"
#include "mscenewindowmodel.h"
#include <mscenewindowstyle.h>

class MSceneWindow;
class MSceneWindowViewPrivate;

/*!
 * \class MSceneWindowView
 * \brief This is a default view class for MSceneWindow.
 *
 * The MSceneWindowView provides an interface to notify
 * the scene manager about changes in scene window's
 * geometry-related style attributes. It is the preferred
 * base class for views of all types of scene windows.
 * Once its style changes, it emits a geometryAttributesChanged()
 * signal, which is propagated to the MSceneManager instance,
 * causing the window geometry and position to be recalculated.
 */
class M_VIEWS_EXPORT MSceneWindowView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MSceneWindowStyle)
    Q_PROPERTY(Qt::Alignment alignment READ alignment)
    Q_PROPERTY(QPointF offset READ offset)

public:
    /*!
     * \brief Creates a new instance of the view for the given /a controller.
     */
    MSceneWindowView(MSceneWindow *controller);
    /*!
     * \brief A destructor.
     */
    virtual ~MSceneWindowView();

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
    MSceneWindowView(MSceneWindowViewPrivate &dd, MSceneWindow *controller);

    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();
    virtual void notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    //! \reimp_end

signals:
    /*!
     * \brief Emitted every time when the style of the scene window changes.
     */
    void geometryAttributesChanged();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_playSceneWindowAppearingFeedback())

    Q_DECLARE_PRIVATE(MSceneWindowView)
    Q_DISABLE_COPY(MSceneWindowView)
};

#endif
