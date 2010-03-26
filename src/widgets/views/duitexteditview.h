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

#ifndef DUITEXTEDITVIEW_H
#define DUITEXTEDITVIEW_H

#include "duiwidgetview.h"
#include <duitexteditmodel.h>
#include <duitexteditstyle.h>

class DuiTextEdit;
class DuiTextEditViewPrivate;


/*!
* \class DuiTextEditView
* \brief Standard view for DuiTextEdit widget
*/
class DUI_EXPORT DuiTextEditView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiTextEditModel, DuiTextEditStyle)

public:
    /*!
     * \brief Constructor
     * \param controller DuiTextEdit widget which this view shows
     */
    DuiTextEditView(DuiTextEdit *controller);

    /*!
     * \brief Destructor
     */
    virtual ~DuiTextEditView();

    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void setupModel();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void changeEvent(QEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    //! \reimp_end

    //! \brief Notifies this and possibly derived views about the need for redraw.
    virtual void doUpdate();

    //! \brief Gets the last mouse position.
    const QPointF &mouseTarget() const;

    //! \brief Updates cursor position based on mouse \a event.
    //! \param event Event that contains the mouse position, buttons, etc.
    //! \param updateSelection Tells whether or not to update text selection.
    //! If false, only cursor moves without selecting any text.
    void updateCursorPosition(QGraphicsSceneMouseEvent *event, bool updateSelection);

protected Q_SLOTS:
    /*!
     * \brief Set view focused
     */
    void setFocused(Qt::FocusReason reason);

    /*!
     * \brief Remove focus from view
     */
    void removeFocus(Qt::FocusReason reason);

    //! \reimp
    virtual void updateData(const QList<const char *> &modifications);
    //! \reimp_end

    /*!
     * \brief Show informational banner about failed text paste
     */
    virtual void informPasteFailed();

    //! \brief Handler for long press event.
    virtual void handleLongPress();

    void hideInfoBanner();

private:
    Q_DISABLE_COPY(DuiTextEditView)
    Q_DECLARE_PRIVATE(DuiTextEditView)

    DuiTextEditViewPrivate *const d_ptr;

#ifdef UNIT_TEST
    friend class Ut_DuiTextEditView;
    friend class Ut_DuiTextEdit;
#endif
};

#endif
