/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef MTEXTSELECTIONHANDLE_H
#define MTEXTSELECTIONHANDLE_H

#include "mstylablewidget.h"
#include "mtextselectionhandlestyle.h"

#include <QPropertyAnimation>

//! \internal

/*!
 * \brief MTextSelectionHandle is the selection handle widget to help select text.
 *
 *  MTextSelectionHandle appears when user finishs first selecting action.
 *  Disappears when selection is dismissed. Drag and move the selection
 *  handles will change the selected text.
 */
class MTextSelectionHandle : public MStylableWidget
{
    Q_OBJECT

public:
    /*! \brief Construct text selection handle.
     */
    explicit MTextSelectionHandle();

    /*!
     * \brief Makes the selection handle appear.
     */
    virtual ~MTextSelectionHandle();

    /*!
     * \brief Makes the selection handle appear.
     */
    void appear();

    /*!
     * \brief Makes the selection handle disappear.
     */
    void disappear();

    /*!
     * \brief Hide selection handle temporarily.
     * Handle could be shown again by method restore().
     *
     * Note: method appear() does not show handle if it is hidden temporarily.
     */
    void hideTemporarily();

    /*!
     * \brief Show selection handle if it was hidden temporarily.
     * This method does not show widget if it's position is invisible.
     *
     * \sa setPositionVisibility()
     */
    void restore();

    /*!
     * \brief Inform selection handle whether it's position is
     * \a visible on the screen or not.
     */
    void setPositionVisibility(bool visible);

    /*!
     * \brief Updates position for the selection handle to \a pos.
     */
    void updatePosition(const QPointF &pos);

    /*!
     * \brief Sets the pressed state of selection handle to \a pressed.
     */
    void setPressed(bool pressed);

    /*!
     * \brief Returns true if selection handle is pressed.
     */
    bool isPressed() const;

    /*!
     * \brief Returns the hot spot of selection handle.
     */
    QPointF hotSpot() const;

    /*!
     * \brief Returns the smallest movement in pixels of selection handle.
     */
    int movementSensitivity() const;

    /*!
     * \brief Skip any transitions, so state is changing instantly.
     */
    void skipTransitions();

    /*!
     * \brief Disable widget, so it becomes translucent.
     */
    void disable();

    /*!
     * \brief Enable widget, so it become opaque.
     */
    void enable();

    /*!
     * \brief Return region occupied by this widget.
     *
     * Other widgets could take this region into account to prevent
     * colliding with selection handle.
     * Region is returned in parent's coordinates.
     */
    QRectF region() const;

Q_SIGNALS:
    /*!
     * \brief Emitted when handle disappears from the screen.
     *
     * This signal is emitted when hiding transition is finished.
     */
    void disappeared();

    //! \brief Emitted when handle is pressed.
    //! \param handle Pointer to this object.
    void pressed(const MTextSelectionHandle *handle);

    //! \brief Emitted when handle is released.
    void released(const MTextSelectionHandle *handle);

protected:
    //! \reimp
    virtual void applyStyle();
    virtual QRectF boundingRect() const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    //! \reimp_end

private Q_SLOTS:
    //! Updates state when show/hide transition is finished.
    void onShowHideAnimationFinished();

private:
    Q_DISABLE_COPY(MTextSelectionHandle)

    QPropertyAnimation opacityAnimation;
    bool m_pressed;
    bool hiddenTemporarily;
    int mMovementSensitivity;
    bool positionIsVisible;
    bool disabled;

    // some methods should not be used from outside
    using MStylableWidget::hide;
    using MStylableWidget::show;
    using MStylableWidget::setVisible;

    friend class Ut_MTextSelectionHandle;
    friend class Ut_MTextSelectionOverlay;

    M_STYLABLE_WIDGET(MTextSelectionHandleStyle)
};

//! \internal_end

#endif
