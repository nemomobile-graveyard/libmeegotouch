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

#ifndef MSTYLABLEWIDGETVIEW_H
#define MSTYLABLEWIDGETVIEW_H

#include <mwidgetview.h>

class MStylableWidget;

/*!
    \class MStylableWidgetView

    \brief MStylableWidgetView is a specialised view to be used within simple non-MVC widgets.

    MStylableWidget class is a specialised view to be used only withing MStylableWidget class.
    It allows the MStylableWidget to provide style attributes information to child widgets.
 */
class MStylableWidgetView : public MWidgetView
{
    Q_OBJECT

public:
    /*!
     * \brief Constructor
     * \param controller Pointer to the stylable widget controller
     */
    MStylableWidgetView(MStylableWidget *controller);

    /*!
      \brief Destructor.
     */
    virtual ~MStylableWidgetView();

protected:
    virtual MWidgetStyleContainer *createStyleContainer() const;

    /*!
     * Draws the background for this view.
     */
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * Draws the contents for this view.
     */
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * Draws the foreground for this view.
     */
    virtual void drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
      Notification for derived classes. This method gets called when a new style is applied for this view.
      This happens e.g. when the object is constructed and when a new object name is given to the widget.
      This method routes the call back to the simple widget controller.
    */
    virtual void applyStyle();

private:

    // This is a private class, we can store it inside.
    MStylableWidget              *controller;

    inline MWidgetModel          *model() {
        return MWidgetView::model();
    }
    inline const MWidgetModel    *model() const {
        return MWidgetView::model();
    }

    Q_DISABLE_COPY(MStylableWidgetView)
};

#endif // MSTYLABLEWIDGETVIEW_H
