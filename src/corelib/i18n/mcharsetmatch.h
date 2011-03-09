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

#ifndef MCHARSETMATCH_H
#define MCHARSETMATCH_H

#include "mexport.h"

#include <QString>

class MCharsetMatchPrivate;

/*!
 * \class MCharsetMatch
 *
 * \brief class representing a match obtained from MCharsetDetector
 *
 */

class M_CORE_EXPORT MCharsetMatch
{
public:
    /*!
     * \brief constructor for MCharsetMatch
     */
    MCharsetMatch();

    /*!
     * \brief constructor for MCharsetMatch
     * \param name name of the charset. For example "UTF-8", "ISO-8859-1", ...
     * \param language RFC 3066 code of the language like "en", "zh", ...
     * \param confidence number between 0 and 100 for the quality of the match
     *
     * Constructs an MCharsetMatch object setting a name of the
     * charset a language, and a confidence.
     */
    explicit MCharsetMatch(const QString name, const QString language = QString(), const qint32 confidence = 0);

    /*!
     * \brief copy constructor for MCharsetMatch
     */
    MCharsetMatch(const MCharsetMatch &other);

    /*!
     * \brief destructor for MCharsetMatch
     */
    virtual ~MCharsetMatch();

    /*!
     * \brief operator= for MCharsetMatch
     */
    MCharsetMatch &operator=(const MCharsetMatch &other);

    /*!
     * \brief operator< for MCharsetMatch
     *
     * A MCharsetMatch object is less than another MCharsetMatch
     * object if its confidence value is less than the confidence
     * value of the other object.
     *
     * \sa confidence()
     */
    bool operator<(const MCharsetMatch &other) const;

    /*!
     * \brief operator> for MCharsetMatch
     *
     * A MCharsetMatch object is greater than another MCharsetMatch
     * object if its confidence value is greater than the confidence
     * value of the other object.
     *
     * \sa confidence()
     */
    bool operator>(const MCharsetMatch &other) const;

    /*!
     * \brief name of the detected charset of this match
     *
     * returns the name of the charset of the match represented
     * by this MCharsetMatch object.
     *
     * \sa setName(QString name)
     */
    QString name() const;

    /*!
     * \brief sets the name of the detected charset for this match
     *
     * sets the name of the charset of the match represented by this
     * MCharsetMatch object. In most cases only used by MCharsetDetector
     * when storing the matches found.
     *
     * \sa name()
     */
    void setName(QString name);

    /*!
     * \brief detected language of this match
     *
     * returns the detected language of the match represented
     * by this MCharsetMatch object.
     *
     * \sa setLanguage(QString language)
     */
    QString language() const;

    /*!
     * \brief sets the detected language for this match
     *
     * sets the detected language of the match represented by this
     * MCharsetMatch object. In most cases only used by
     * MCharsetDetector when storing the matches found.
     *
     * \sa language()
     */
    void setLanguage(QString language);

    /*!
     * \brief confidence level of this match
     *
     * returns the confidence level of the match represented by this
     * MCharsetMatch object.
     *
     * Confidence numbers range from zero to 100, with 100
     * representing complete confidence and zero representing no
     * confidence.
     *
     * The confidence values are somewhat arbitrary. They define an an
     * ordering within the results for any single detection operation
     * but are not generally comparable between the results for
     * different input.
     *
     * A confidence value of 10 does have a general meaning — it is
     * used for charsets that can represent the input data, but for
     * which there is no other indication that suggests that the
     * charset is the correct one.
     *
     * \sa setConfidence()
     */
    qint32 confidence() const;

    /*!
     * \brief sets the confidence level for this match
     *
     * sets the confidece level of the match represented by this
     * MCharsetMatch object. In most cases only used by
     * MCharsetDetector when storing the matches found.
     *
     * \sa confidence()
     */
    void setConfidence(qint32 confidence);

private:
    MCharsetMatchPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MCharsetMatch)
};

#endif
