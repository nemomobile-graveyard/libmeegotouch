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

#ifndef MFEEDBACK_H
#define MFEEDBACK_H

#include "mexport.h"

#include <QObject>
#include <QMetaType>
#include <QString>

class MFeedbackPrivate;

/*!
 * \class MFeedback
 * \brief Used to easily play non-graphical input feedbacks.
 *
 * This class can be used to play non-graphical input feedbacks such as
 * vibra, audio or tactile effects. MFeedback has a name which is used
 * to play the correct feedback(s) when play() function is called.
 *
 * \sa MFeedbackPlayer
 */
class M_CORE_EXPORT MFeedback : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Default constructor
     *
     * Creates a MFeedback with no name.
     *
     * \note Calling play() for MFeedback with no name causes no feedback
     *       to be played.
     *
     * \param parent Parent object
     */
    explicit MFeedback(QObject *parent = 0);

    /*!
     * \brief Copy constructor
     *
     * \note The underlying QObject is not copied, an empty QObject is created
     *       instead.
     */
    MFeedback(const MFeedback &);

    /*!
     * \brief Constructor
     *
     * \param name Name of the feedback
     * \param parent Parent object
     */
    explicit MFeedback(const QString &name, QObject *parent = 0);

    /*!
     * \brief Assigment operator
     *
     * \note The underlying QObject is not copied, an empty QObject is created
     *       instead.
     */
    MFeedback &operator=(const MFeedback &other);

    /*!
     * \brief Destructor
     */
    ~MFeedback();

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
     * \brief Sets the name of the feedback.
     *
     * \param name Name of the feedback
     */
    void setName(const QString &name);

    /*!
     * \brief Returns the name of the feedback.
     *
     * \return Name of the feedback.
     */
    QString name() const;

    /*!
     * \brief Plays a feedback.
     *
     * \param feedbackName Name of the feedback to be played
     */
    static void play(const QString &feedbackName);

public Q_SLOTS:
    /*!
     * \brief Plays the feedback.
     */
    void play() const;

protected:
    Q_DECLARE_PRIVATE(MFeedback)
    MFeedbackPrivate *const d_ptr;
};

Q_DECLARE_METATYPE(MFeedback)

#endif
