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

#ifndef MAPPLICATIONMENUBUTTONVIEW_H
#define MAPPLICATIONMENUBUTTONVIEW_H

#include <mbuttonview.h>
#include <mapplicationmenubuttonmodel.h>
#include <mapplicationmenubuttonstyle.h>

class MApplicationMenuButtonViewPrivate;
class MApplicationMenuButton;

class QGraphicsSceneResizeEvent;

//! \internal
/*!
    \class MApplicationMenuButtonView
    \brief View class for MApplicationMenuButton

    \ingroup views

    \sa MApplicationMenuButton
*/

class MApplicationMenuButtonView : public MButtonView
{
    Q_OBJECT
    M_VIEW(MApplicationMenuButtonModel, MApplicationMenuButtonStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    MApplicationMenuButtonView(MApplicationMenuButton *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MApplicationMenuButtonView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:

    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();
    virtual void drawIcon(QPainter *painter, const QRectF &iconRect) const;
    //! \reimp_end

    //! \internal
    MApplicationMenuButtonView(MApplicationMenuButtonViewPrivate &dd, MApplicationMenuButton *controller);
    //! \internal_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MApplicationMenuButtonView)
    Q_DECLARE_PRIVATE(MApplicationMenuButtonView)
};
//! \internal_end

#endif
