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

#ifndef MAPPLICATIONEXTENSIONAREAVIEW_H_
#define MAPPLICATIONEXTENSIONAREAVIEW_H_

#include "mextensionareaview.h"
#include "mapplicationextensionareamodel.h"
#include "mapplicationextensionareastyle.h"

class MApplicationExtensionAreaViewPrivate;
class MApplicationExtensionArea;
class MContainer;

/*!
 * A view class for the MApplicationExtensionArea.
 */
class M_EXTENSIONS_EXPORT MApplicationExtensionAreaView : public MExtensionAreaView
{
    Q_OBJECT
    M_VIEW(MApplicationExtensionAreaModel, MApplicationExtensionAreaStyle)

public:
    /*!
     * Constructs a new view for MApplicationExtensionArea.
     *
     * \param controller the MApplicationExtensionArea controller for the view.
     */
    MApplicationExtensionAreaView(MApplicationExtensionArea *controller);

    /*!
     * Destroys the MApplicationExtensionAreaView.
     */
    virtual ~MApplicationExtensionAreaView();

protected:
    /*!
     * Constructs a new view for MApplicationExtensionArea.
     *
     * \param dd the MApplicationExtensionAreaViewPrivate private class instance to be used.
     * \param controller the MApplicationExtensionArea controller for the view.
     * \deprecated This constructor is deprecated in order to achieve sane
     * lifecycle rules. Use the version taking a pointer to the private instead.
     */
    Q_DECL_DEPRECATED MApplicationExtensionAreaView(MApplicationExtensionAreaViewPrivate &dd, MApplicationExtensionArea *controller);

    /*!
     * Constructs a new view for MApplicationExtensionArea.
     *
     * \param dd the MApplicationExtensionAreaViewPrivate private class instance to be used. The view takes ownership of the private.
     * \param controller the MApplicationExtensionArea controller for the view.
     */
    MApplicationExtensionAreaView(MApplicationExtensionAreaViewPrivate *dd, MApplicationExtensionArea *controller);

    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DISABLE_COPY(MApplicationExtensionAreaView)
    Q_DECLARE_PRIVATE(MApplicationExtensionAreaView)
};

#endif /* MAPPLICATIONEXTENSIONAREAVIEW_H_ */
