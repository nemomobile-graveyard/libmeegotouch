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

#ifndef MEXTENDINGBACKGROUNDVIEW_P_H_
#define MEXTENDINGBACKGROUNDVIEW_P_H_

class MWidgetController;
class MExtendingBackgroundView;

/*!
 * The private class for MExtendingBackgroundView.
 */
class MExtendingBackgroundViewPrivate
{
    Q_DECLARE_PUBLIC(MExtendingBackgroundView)

protected:
    MExtendingBackgroundView *q_ptr;

public:
    /*!
     * Creates an instance of MExtendingBackgroundViewPrivate.
     */
    MExtendingBackgroundViewPrivate();

    /*!
     * Destroys the MExtendingBackgroundViewPrivate.
     */
    virtual ~MExtendingBackgroundViewPrivate();

    /*!
     * Initializes the private class.
     *
     * \param controller the MWidgetController for the view
     */
    void init(MWidgetController *controller);

    /*!
     * Calculates the background geometry.
     *
     * \return the geometry of the background
     */
    QRect backgroundGeometry() const;

    //! The Desktop controller
    MWidgetController *controller;
};

#endif /* MEXTENDINGBACKGROUNDVIEW_P_H_ */
