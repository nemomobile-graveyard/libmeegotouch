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

#ifndef DUIMASHUPCANVASVIEW_H_
#define DUIMASHUPCANVASVIEW_H_

#include <DuiWidgetView>
#include <DuiNamespace>
#include "duimashupcanvasmodel.h"
#include "duimashupcanvasstyle.h"

class DuiMashupCanvasViewPrivate;
class DuiMashupCanvas;
class DuiContainer;

/*!
 * A view class for the DuiMashupCanvas.
 */
class DUI_EXPORT DuiMashupCanvasView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiMashupCanvasModel, DuiMashupCanvasStyle)

public:
    /*!
     * Constructs a new view for DuiMashupCanvas.
     *
     * \param controller the DuiMashupCanvas controller for the view.
     */
    DuiMashupCanvasView(DuiMashupCanvas *controller);

    /*!
     * Destroys the DuiMashupCanvasView.
     */
    virtual ~DuiMashupCanvasView();

protected:
    /*!
     * Initializes the DuiMashupCanvasView.
     */
    void init(DuiMashupCanvas *controller);

    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

    /*!
     * Constructs a new view for DuiMashupCanvas.
     *
     * \param dd the DuiMashupCanvasViewPrivate private class instance to be used.
     * \param controller the DuiMashupCanvas controller for the view.
     */
    DuiMashupCanvasView(DuiMashupCanvasViewPrivate &dd, DuiMashupCanvas *controller);

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
    void orientationChanged(Dui::Orientation);

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiMashupCanvasView)
    Q_DECLARE_PRIVATE(DuiMashupCanvasView)
};

#endif /* DUIMASHUPCANVASVIEW_H_ */
