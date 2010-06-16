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

#ifndef MBannerView_H
#define MBannerView_H

#include "mscenewindowview.h"
#include "mbannermodel.h"
#include "mbannerstyle.h"

class MBanner;
class MBannerViewPrivate;

class M_EXPORT MBannerView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MBannerModel, MBannerStyle)

public:
    /*!
        \brief Constructor
        \param controller  Pointer to the banner's controller
    */
    MBannerView(MBanner *controller);

    /*!
        \brief Destructor
    */
    virtual ~MBannerView();

protected:

    /*!
        \brief Constructor for derived classes
        \param dd  Private class implementation
        \param controller  Pointer to the banner's controller
    */
    MBannerView(MBannerViewPrivate &dd, MBanner *controller);

    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

    MBannerViewPrivate *const d_ptr;

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MBannerView)
    Q_DISABLE_COPY(MBannerView)

};

#endif

