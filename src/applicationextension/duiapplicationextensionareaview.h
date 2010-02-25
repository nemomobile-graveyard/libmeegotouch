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

#ifndef DUIAPPLICATIONEXTENSIONAREAVIEW_H_
#define DUIAPPLICATIONEXTENSIONAREAVIEW_H_

#include "duiextensionareaview.h"
#include "duiapplicationextensionareamodel.h"
#include "duiapplicationextensionareastyle.h"

class DuiApplicationExtensionAreaViewPrivate;
class DuiApplicationExtensionArea;
class DuiContainer;

/*!
 * A view class for the DuiApplicationExtensionArea.
 */
class DUI_EXPORT DuiApplicationExtensionAreaView : public DuiExtensionAreaView
{
    Q_OBJECT
    DUI_VIEW(DuiApplicationExtensionAreaModel, DuiApplicationExtensionAreaStyle)

public:
    /*!
     * Constructs a new view for DuiApplicationExtensionArea.
     *
     * \param controller the DuiApplicationExtensionArea controller for the view.
     */
    DuiApplicationExtensionAreaView(DuiApplicationExtensionArea *controller);

    /*!
     * Destroys the DuiApplicationExtensionAreaView.
     */
    virtual ~DuiApplicationExtensionAreaView();

protected:
    /*!
     * Constructs a new view for DuiApplicationExtensionArea.
     *
     * \param dd the DuiApplicationExtensionAreaViewPrivate private class instance to be used.
     * \param controller the DuiApplicationExtensionArea controller for the view.
     */
    DuiApplicationExtensionAreaView(DuiApplicationExtensionAreaViewPrivate &dd, DuiApplicationExtensionArea *controller);

private:
    Q_DISABLE_COPY(DuiApplicationExtensionAreaView)
    Q_DECLARE_PRIVATE(DuiApplicationExtensionAreaView)
};

#endif /* DUIAPPLICATIONEXTENSIONAREAVIEW_H_ */
