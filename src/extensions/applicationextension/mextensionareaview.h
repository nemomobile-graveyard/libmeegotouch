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

#ifndef MEXTENSIONAREAVIEW_H_
#define MEXTENSIONAREAVIEW_H_

#include <MWidgetView>
#include "mextensionareamodel.h"
#include "mextensionareastyle.h"

class MExtensionAreaViewPrivate;
class MExtensionArea;

/*!
 * A view class for the MExtensionArea.
 */
class M_EXTENSIONS_EXPORT MExtensionAreaView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MExtensionAreaModel, MExtensionAreaStyle)

public:
    /*!
     * Constructs a new view for MExtensionArea.
     *
     * \param controller the MExtensionArea controller for the view.
     */
    MExtensionAreaView(MExtensionArea *controller);

    /*!
     * Destroys the MExtensionAreaView.
     */
    virtual ~MExtensionAreaView();

protected:
    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    virtual void setupModel();
    virtual void applyStyle();
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    /*!
     * Constructs a new view for MExtensionArea.
     *
     * \param dd the MExtensionAreaViewPrivate private class instance to be used.
     * \param controller the MExtensionArea controller for the view.
     * \deprecated This constructor is deprecated, the private must be
     * passed via pointer in order to maintain sane lifetime rules (so
     * that the view can always delete the private given to it, or the
     * private it creates itself). Use the version taking a pointer to the private instead.
     */
    Q_DECL_DEPRECATED MExtensionAreaView(MExtensionAreaViewPrivate &dd, MExtensionArea *controller);
    /*!
     * Constructs a new view for MExtensionArea.
     *
     * \param dd the MExtensionAreaViewPrivate private class instance to be used. The view takes ownership.
     * \param controller the MExtensionArea controller for the view.
     */
    MExtensionAreaView(MExtensionAreaViewPrivate *dd, MExtensionArea *controller);
    MExtensionAreaViewPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MExtensionAreaView)
    Q_DECLARE_PRIVATE(MExtensionAreaView)
};

#endif /* MEXTENSIONAREAVIEW_H_ */
