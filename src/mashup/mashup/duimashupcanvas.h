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

#ifndef DUIMASHUPCANVAS_H
#define DUIMASHUPCANVAS_H

#include <DuiWidgetController>
#include "duimashupcanvasmodel.h"

class DuiDataStore;
class DuiAppletInstanceManager;
class DuiMashupCanvasPrivate;

/*!
 * DuiMashupCanvas is a widget which can be populated with applet instances. DuiMashupCanvas
 * can be placed on any view that wants to leverage applet support.
 *
 * Every DuiMashupCanvas has to be identified by a unique identifier that is passed in through
 * the constructor. The user of this class should ensure that they use unique identifiers for the
 * mashup canvases they construct. If the user of this class fails to provide unique identifiers,
 * unpredictable side effects may occur because the implementation ensures that every mashup canvas has a
 * unique identifier. The unexpected side effects may include, but isn't limited to:
 * - mixing of the contents of the canvases
 * - missing applets from the canvases upon restart of the application
 *
 * \see \ref appletdevelopment
 */
class DUI_EXPORT DuiMashupCanvas : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiMashupCanvas)

public:
    /*!
     * Default constructor
     * \param identifier A unique identifier of this mashup canvas. The data of the instantiated
     * applets is stored and restored based on this identifier.
      \param parent Optional Object's parent
     */
    explicit DuiMashupCanvas(const QString &identifier, QGraphicsItem *parent = NULL);

    /*!
     * Default destructor
     */
    virtual ~DuiMashupCanvas();

    /*!
     * Gets the applet instance manager for managing applet instances.
     *
     * \return an instance of DuiAppletInstanceManager
     */
    DuiAppletInstanceManager *appletInstanceManager() const;

    /*!
     * Sets the applet categories available in this mashup canvas.
     * The default value for the categories is an empty list. This
     * means filtering by categories is disabled and all applets are
     * shown in the applet inventory.
     *
     * \param categories a list of names of the applet categories to show in this mashup canvas.
     */
    void setCategories(const QStringList &categories);

    /*!
     * Gets the applet categories available in this mashup canvas.
     *
     * \return a list of names of the applet categories to show in this mashup canvas.
     */
    QStringList categories() const;

    /*!
     * Returns the unique identifier of this canvas
     * \return the identifier
     */
    QString identifier() const;

    /*!
     * Returns the D-Bus service address of this canvas
     * \return the D-Bus address
     */
    QString serviceAddress() const;

protected:
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     */
    DuiMashupCanvas(DuiMashupCanvasPrivate *dd, DuiMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier);

protected Q_SLOTS:
    /*!
     * addWidget is invoked whenever a new applet is instantiated in the AppletInstanceManager
     * associated with this DuiMashupCanvas. The DuiWidget that is passed in the argument is
     * the graphical presentation of the instantiated applet. DuiMashupCanvas inserts the
     * widget to the layout using the given layout data. The ownership of the widget remains
     * on the AppletInstanceManager object that called this slot.
     * This slot can be overridden in specialised mashup canvases to provide tailored functionality.
     * \param widget Widget to be added onto the DuiMashupCanvas.
     * \param store This DuiDataStore object can be used to store permanent mashup canvas data related
     * to this particular applet instance. When the same applet instance is reinstantiated, this
     * API will be called with the data that was stored to the permanent storage the last time around.
     * This can be used to store for instance layout data of an applet instance or any other mashup
     * canvas specific data.
     */
    virtual void addWidget(DuiWidget *widget, DuiDataStore &store);

    /*!
     * removeWidget is invoked whenever an existing applet instance is removed from the system.
     * DuiMashupCanvas will remove the associated presentation from the canvas, but should not
     * remove the DuiWidget object, since the ownership is maintained on whoever called the addWidget
     * on the associated DuiWidget.
     *
     * \param widget The widget to be removed from the system.
     */
    virtual void removeWidget(DuiWidget *widget);

private:
    Q_DECLARE_PRIVATE(DuiMashupCanvas)
    Q_DISABLE_COPY(DuiMashupCanvas)

    /*!
     * Initializes this object once its constructed.
     */
    void init();
};

#endif // DUIMASHUPCANVAS_H
