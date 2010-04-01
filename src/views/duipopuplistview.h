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

#ifndef DUIPOPUPLISTVIEW_H
#define DUIPOPUPLISTVIEW_H

#include "duidialogview.h"
#include "duipopuplistmodel.h"
#include "duipopupliststyle.h"

#include <QModelIndex>
#include <QItemSelectionModel>

class DuiPopupListViewPrivate;
class DuiPopupList;
class QAbstractItemModel;

/*!
    \class DuiPopupListView
    \brief View class for standard DuiPopupList.

    \ingroup views

    DuiPopupListView is used to visualize popupList.
 */

class DUI_EXPORT DuiPopupListView : public DuiDialogView
{
    Q_OBJECT
    DUI_VIEW(DuiPopupListModel, DuiPopupListStyle)

public:
    /*!
       \brief Constructor
       \param controller Pointer to the button's controller
     */
    DuiPopupListView(DuiPopupList *controller);

    /*!
       \brief Destructor
     */
    virtual ~DuiPopupListView();

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
    DuiPopupListView(DuiPopupListViewPrivate &dd, DuiPopupList *controller);
    //! \internal_end

    //! \reimp
    virtual void setupModel();
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiPopupListView)
    Q_DECLARE_PRIVATE(DuiPopupListView)
};

#endif
