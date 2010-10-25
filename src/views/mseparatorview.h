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

#ifndef MSEPARATORVIEW_H
#define MSEPARATORVIEW_H

#include <mwidgetview.h>
#include <mseparatormodel.h>
#include <mseparatorstyle.h>

class MSeparatorViewPrivate;
class MSeparator;

/*!
    \class MSeparatorView
    \brief MSeparatorView implements a view for the MSeparator
 */

class M_VIEWS_EXPORT MSeparatorView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MSeparatorModel, MSeparatorStyle)

public:
    /*!
        \brief Constructor
        \param controller Pointer to the separator's controller
     */
    MSeparatorView(MSeparator *controller);

    /*!
        \brief Destructor.
     */
    virtual ~MSeparatorView();

protected:
    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end
    MSeparatorViewPrivate *const d_ptr;

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    virtual void applyStyle();
    //! \reimp_end

private:

    Q_DISABLE_COPY(MSeparatorView)
    Q_DECLARE_PRIVATE(MSeparatorView)
};

#endif // MSEPARATORVIEW_H
