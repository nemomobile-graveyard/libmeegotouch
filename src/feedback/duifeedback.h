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

#ifndef DUIFEEDBACK_H
#define DUIFEEDBACK_H

#include "duiexport.h"

#include <QObject>
#include <QString>

class DuiFeedbackPrivate;

/*!
 * \class DuiFeedback
 * \brief Used to easily play non-graphical input feedbacks.
 *
 * This class can be used to play non-graphical input feedbacks such as
 * vibra, audio or tactile effects. DuiFeedback has a name which is used
 * to play the correct feedback(s) when play() function is called.
 *
 * \sa DuiFeedbackPlayer
 */
class DUI_EXPORT DuiFeedback : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Default constructor
     *
     * Creates a DuiFeedback with no name.
     *
     * \note Calling play() for DuiFeedback with no name causes no feedback
     *       to be played.
     *
     * \param parent Parent object
     */
    DuiFeedback(QObject *parent = 0);

    /*!
     * \brief Copy constructor
     *
     * \note The underlying QObject is not copied, an empty QObject is created
     *       instead.
     */
    DuiFeedback(const DuiFeedback &);

    /*!
     * \brief Constructor
     *
     * \param name Name of the feedback
     * \param parent Parent object
     */
    DuiFeedback(const QString &name, QObject *parent = 0);

    /*!
     * \brief Assigment operator
     *
     * \note The underlying QObject is not copied, an empty QObject is created
     *       instead.
     */
    DuiFeedback &operator=(const DuiFeedback &other);

    /*!
     * \brief Destructor
     */
    ~DuiFeedback();

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

public Q_SLOTS:
    /*!
     * \brief Plays the feedback.
     */
    void play() const;

protected:
    Q_DECLARE_PRIVATE(DuiFeedback)
    DuiFeedbackPrivate *const d_ptr;
};

#endif
