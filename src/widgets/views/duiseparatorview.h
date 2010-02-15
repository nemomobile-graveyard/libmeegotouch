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

#ifndef DUISEPARATORVIEW_H
#define DUISEPARATORVIEW_H

#include <duiwidgetview.h>
#include <duiseparatormodel.h>
#include <duiseparatorstyle.h>

class DuiSeparatorViewPrivate;
class DuiSeparator;

/*!
    \class DuiSeparatorView
    \brief DuiSeparatorView implements a view for the DuiSeparator
 */

class DUI_EXPORT DuiSeparatorView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiSeparatorModel, DuiSeparatorStyle)

public:
    /*!
        \brief Constructor
        \param controller Pointer to the separator's controller
     */
    DuiSeparatorView(DuiSeparator *controller);

    /*!
        \brief Destructor.
     */
    virtual ~DuiSeparatorView();

protected:
    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:

    Q_DISABLE_COPY(DuiSeparatorView)
    Q_DECLARE_PRIVATE(DuiSeparatorView)
};

#endif // DUISEPARATORVIEW_H
