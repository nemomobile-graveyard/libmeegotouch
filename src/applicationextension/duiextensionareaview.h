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

#ifndef DUIEXTENSIONAREAVIEW_H_
#define DUIEXTENSIONAREAVIEW_H_

#include <DuiWidgetView>
#include "duiextensionareamodel.h"
#include "duiextensionareastyle.h"

class DuiExtensionAreaViewPrivate;
class DuiExtensionArea;

/*!
 * A view class for the DuiExtensionArea.
 */
class DUI_EXPORT DuiExtensionAreaView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiExtensionAreaModel, DuiExtensionAreaStyle)

public:
    /*!
     * Constructs a new view for DuiExtensionArea.
     *
     * \param controller the DuiExtensionArea controller for the view.
     */
    DuiExtensionAreaView(DuiExtensionArea *controller);

    /*!
     * Destroys the DuiExtensionAreaView.
     */
    virtual ~DuiExtensionAreaView();

protected:
    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    virtual void setupModel();
    virtual void applyStyle();
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    /*!
     * Constructs a new view for DuiExtensionArea.
     *
     * \param dd the DuiExtensionAreaViewPrivate private class instance to be used.
     * \param controller the DuiExtensionArea controller for the view.
     */
    DuiExtensionAreaView(DuiExtensionAreaViewPrivate &dd, DuiExtensionArea *controller);

private:
    Q_DISABLE_COPY(DuiExtensionAreaView)
    Q_DECLARE_PRIVATE(DuiExtensionAreaView)
};

#endif /* DUIEXTENSIONAREAVIEW_H_ */
