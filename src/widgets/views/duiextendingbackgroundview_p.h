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

#ifndef DUIEXTENDINGBACKGROUNDVIEW_P_H_
#define DUIEXTENDINGBACKGROUNDVIEW_P_H_

#include "private/duiwidgetview_p.h"

class DuiWidgetController;
class DuiExtendingBackgroundView;

/*!
 * The private class for DuiExtendingBackgroundView.
 */
class DuiExtendingBackgroundViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiExtendingBackgroundView)

public:
    /*!
     * Creates an instance of DuiExtendingBackgroundViewPrivate.
     */
    DuiExtendingBackgroundViewPrivate();

    /*!
     * Destroys the DuiExtendingBackgroundViewPrivate.
     */
    virtual ~DuiExtendingBackgroundViewPrivate();

    /*!
     * Initializes the private class.
     *
     * \param controller the DuiWidgetController for the view
     */
    void init(DuiWidgetController *controller);

    /*!
     * Calculates the background geometry.
     *
     * \return the geometry of the background
     */
    QRect backgroundGeometry() const;

    //! The Desktop controller
    DuiWidgetController *controller;
};

#endif /* DUIEXTENDINGBACKGROUNDVIEW_P_H_ */
