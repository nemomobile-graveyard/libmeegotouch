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

#ifndef MPOPUPLISTVIEW_H
#define MPOPUPLISTVIEW_H

#include "mdialogview.h"
#include "mpopuplistmodel.h"
#include "mpopupliststyle.h"

#include <QModelIndex>
#include <QItemSelectionModel>

class MPopupListViewPrivate;
class MPopupList;
class QAbstractItemModel;

/*!
    \class MPopupListView
    \brief View class for standard MPopupList.

    \ingroup views

    MPopupListView is used to visualize popupList.
 */

class M_VIEWS_EXPORT MPopupListView : public MDialogView
{
    Q_OBJECT
    M_VIEW(MPopupListModel, MPopupListStyle)

public:
    /*!
       \brief Constructor
       \param controller Pointer to the button's controller
     */
    MPopupListView(MPopupList *controller);

    /*!
       \brief Destructor
     */
    virtual ~MPopupListView();

protected Q_SLOTS:

    /*!
        Scrolls the view if necessary to ensure that the item at \a index is visible.
    */
    virtual void scrollTo(const QModelIndex &index) const;

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

protected:

    //! \internal
    MPopupListView(MPopupListViewPrivate &dd, MPopupList *controller);
    //! \internal_end

    //! \reimp
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DISABLE_COPY(MPopupListView)
    Q_DECLARE_PRIVATE(MPopupListView)
};

#endif
