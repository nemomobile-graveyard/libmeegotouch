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

#ifndef DUIAPPLICATIONMENUBUTTONVIEW_H
#define DUIAPPLICATIONMENUBUTTONVIEW_H

#include <duibuttonview.h>
#include <duiapplicationmenubuttonmodel.h>
#include <duiapplicationmenubuttonstyle.h>

class DuiApplicationMenuButtonViewPrivate;
class DuiApplicationMenuButton;

class QGraphicsSceneResizeEvent;

//! \internal
/*!
    \class DuiApplicationMenuButtonView
    \brief View class for DuiApplicationMenuButton

    \ingroup views

    \sa DuiApplicationMenuButton
*/

class DuiApplicationMenuButtonView : public DuiButtonView
{
    Q_OBJECT
    DUI_VIEW(DuiApplicationMenuButtonModel, DuiApplicationMenuButtonStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    DuiApplicationMenuButtonView(DuiApplicationMenuButton *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~DuiApplicationMenuButtonView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:

    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

    //! \cond
    DuiApplicationMenuButtonView(DuiApplicationMenuButtonViewPrivate &dd, DuiApplicationMenuButton *controller);
    //! \endcond

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiApplicationMenuButtonView)
    Q_DECLARE_PRIVATE(DuiApplicationMenuButtonView)
};
//! \internal_end

#endif
