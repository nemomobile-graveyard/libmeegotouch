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

#ifndef MMASHUPCANVASVIEW_H_
#define MMASHUPCANVASVIEW_H_

#include "mextensionareaview.h"
#include <MNamespace>
#include "mmashupcanvasmodel.h"
#include "mmashupcanvasstyle.h"

class MMashupCanvasViewPrivate;
class MMashupCanvas;
class MContainer;

//! \internal
/*!
 * A view class for the MMashupCanvas.
 */
class M_EXTENSIONS_EXPORT MMashupCanvasView : public MExtensionAreaView
{
    Q_OBJECT
    M_VIEW(MMashupCanvasModel, MMashupCanvasStyle)

public:
    /*!
     * Constructs a new view for MMashupCanvas.
     *
     * \param controller the MMashupCanvas controller for the view.
     */
    MMashupCanvasView(MMashupCanvas *controller);

    /*!
     * Destroys the MMashupCanvasView.
     */
    virtual ~MMashupCanvasView();

protected:
    /*!
     * Initializes the MMashupCanvasView.
     */
    void init(MMashupCanvas *controller);

    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

    /*!
     * Constructs a new view for MMashupCanvas.
     *
     * \param dd the MMashupCanvasViewPrivate private class instance to be used.
     * \param controller the MMashupCanvas controller for the view.
     * \deprecated This constructor is deprecated in order to achieve sane
     * lifecycle rules. Use the version taking a pointer to the private instead.
     */
    Q_DECL_DEPRECATED MMashupCanvasView(MMashupCanvasViewPrivate &dd, MMashupCanvas *controller);

    /*!
     * Constructs a new view for MMashupCanvas.
     *
     * \param dd the MMashupCanvasViewPrivate private class instance to be used.
     * The view takes ownership of the private.
     * \param controller the MMashupCanvas controller for the view.
     */
    MMashupCanvasView(MMashupCanvasViewPrivate *dd, MMashupCanvas *controller);

protected slots:
    /*!
     * \brief A slot for notifying that the applet inventory should be shown
     */
    void showAppletInventory();

    /*!
     * \brief A slot for notifying that the applet inventory should be hidden
     */
    void hideAppletInventory();

    /*!
     * \brief A slot for getting information about orientation changes
     */
    void orientationChanged(M::Orientation);

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MMashupCanvasView)
    Q_DECLARE_PRIVATE(MMashupCanvasView)
};
//! \internal_end

#endif /* MMASHUPCANVASVIEW_H_ */
