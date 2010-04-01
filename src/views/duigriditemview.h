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

#ifndef DUIGRIDITEMVIEW_H
#define DUIGRIDITEMVIEW_H

#include "duiwidgetview.h"
#include "duigriditemmodel.h"
#include "duigriditemstyle.h"

class DuiGridItem;
class DuiGridItemViewPrivate;

/*!
    \class DuiGridItemView
    \brief View class for DuiGridItem.

    \ingroup views

    \section DuiGridItemViewOverview Overview
        DuiGridItemView implements a view for the DuiGridItem widget.
 */

class DUI_EXPORT DuiGridItemView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiGridItemModel, DuiGridItemStyle)

public:
    /*!
        \brief Constructor
        \param controller Pointer to the controller
     */
    DuiGridItemView(DuiGridItem *controller);

    /*!
        \brief Destructor
     */
    virtual ~DuiGridItemView();

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
    DuiGridItemView(DuiGridItemViewPrivate &dd, DuiGridItem *controller);
    DuiGridItemViewPrivate *const d_ptr;
    //! \internal_end

private:
    Q_DISABLE_COPY(DuiGridItemView)
    Q_DECLARE_PRIVATE(DuiGridItemView)
    Q_PRIVATE_SLOT(d_func(), void _q_updateImage())
};

#endif
