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

#ifndef MCONTAINERVIEW_H
#define MCONTAINERVIEW_H

#include <mwidgetview.h>
#include <mexport.h>
#include <mcontainermodel.h>
#include <mcontainerstyle.h>

class MContainerViewPrivate;
class MContainer;

/*!
 * \class MContainerView
 * \brief MContainerView implements a view for the MContainer
 *
 * \section MContainerViewOverview Overview
 * A Container has a minimum width of 18mm, its maximum width is the width of
 * the screen. By default, the user can set it to a value between these
 * boundaries. The minimum height is the minimum height of the header which is
 * 5.06mm. There is no maximum height of the container.
 *
 * \section MContainerViewInteractions Interactions
 * Supported interactions: Long tap. User of the component can specify object
 * menu for content items in native application views. Long tap for applet's
 * content items, not currently implemented.
 *
 * \sa MContainer, MContainerModel
 */
class M_VIEWS_EXPORT MContainerView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MContainerModel, MContainerStyle)

public:
    /*!
     * \brief Constructor
     * \param controller Pointer to the container's controller
     */
    MContainerView(MContainer *controller);

    /*!
     * \brief Destructor
     */
    virtual ~MContainerView();

protected:
    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void setupModel();
    //! \reimp_end

    MContainerView(MContainerViewPrivate &dd, MContainer *controller);

private Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

public Q_SLOTS:
    void headerPressed();
    void headerReleased();
    void headerCanceled();
    void headerMoved();

Q_SIGNALS:
    /*!
     * \brief Signal for informing that the header was clicked
     */
    void headerClicked();

private:
    Q_DISABLE_COPY(MContainerView)
    Q_DECLARE_PRIVATE(MContainerView)
};

#endif
