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

#ifndef DUIFEEDBACKPLAYER_H
#define DUIFEEDBACKPLAYER_H

#include "duiexport.h"

#include <QObject>
#include <QStringList>

class DuiApplicationPrivate;
class DuiFeedbackPlayerPrivate;

/*!
 * \class DuiFeedbackPlayer
 * \brief Used to give non-graphical input feedback.
 *
 * DuiFeedbackPlayer is used to give non-graphical instantaneous feedback to user
 * actions. Non-graphical feedback can mean auditive, tactile, vibrational, etc.
 *
 * You use it as way to add supporting instantaneous feedback for your
 * graphical user interface (such as a "button pressing" tactile feedback when the
 * user pokes your custom widget on a touchscreen)
 *
 * DuiFeedbackPlayer also ensures that the instantaneous feedback given is going
 * to comply with the system's current "look and feel" (more especifically, to the
 * "feel" part) and will also be in harmony with the context/situation at hand.
 */
class DUI_EXPORT DuiFeedbackPlayer : public QObject
{
    Q_OBJECT

    /*!
     * \brief Constructor
     * \param parent Parent object
     */
    DuiFeedbackPlayer(QObject *parent = 0);

    /*!
     * \brief Destructor
     */
    virtual ~DuiFeedbackPlayer();

    // Only DuiApplication may create or destroy instances of this
    // class
    friend class DuiApplicationPrivate;
    friend class DuiComponentDataPrivate;

public:

    /*!
     * Press feedback. Used when the user presses on an object.
     * It is also used when the user presses off an object,
     * but slides the finger onto one.
     */
    static const QString Press;

    /*!
     * Release feedback. Used when finger is released on the object.
     * Indicates that an action was done.
     */
    static const QString Release;

    /*!
     * Cancel feedback. Used when finger slides off the object, onto an
     * inactive area.
     *
     * Also used in cases where a Tap turns into another stroke.
     */
    static const QString Cancel;

    /*!
     * Error feedback.
     */
    static const QString Error;

    /*!
     * Plays a feedback, given its name.
     *
     * \param feedbackName Name of the desired feedback.
     */
    void play(const QString &feedbackName);

private:
    Q_DECLARE_PRIVATE(DuiFeedbackPlayer)
    DuiFeedbackPlayerPrivate *const d_ptr;
    Q_DISABLE_COPY(DuiFeedbackPlayer)

};

#endif
