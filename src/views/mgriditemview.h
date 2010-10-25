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

#ifndef MGRIDITEMVIEW_H
#define MGRIDITEMVIEW_H

#include "mwidgetview.h"
#include "mgriditemmodel.h"
#include "mgriditemstyle.h"

class MGridItem;
class MGridItemViewPrivate;

/*!
    \class MGridItemView
    \brief View class for MGridItem.

    \ingroup views

    \section MGridItemViewOverview Overview
        MGridItemView implements a view for the MGridItem widget.

    \deprecated
 */

class M_VIEWS_EXPORT MGridItemView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MGridItemModel, MGridItemStyle)

public:
    /*!
        \brief Constructor
        \param controller Pointer to the controller
     */
    MGridItemView(MGridItem *controller);

    /*!
        \brief Destructor
     */
    virtual ~MGridItemView();

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

    //! \internal
    MGridItemView(MGridItemViewPrivate &dd, MGridItem *controller);
    MGridItemViewPrivate *const d_ptr;
    //! \internal_end

private:
    Q_DISABLE_COPY(MGridItemView)
    Q_DECLARE_PRIVATE(MGridItemView)
    Q_PRIVATE_SLOT(d_func(), void _q_updateImage())
};

#endif
