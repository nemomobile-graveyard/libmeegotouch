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

#ifndef MCHARSETDETECTOR_H
#define MCHARSETDETECTOR_H

#include "mexport.h"

#include <QList>
#include <QStringList>

class MCharsetMatch;
class MCharsetDetectorPrivate;

/*!
 * \class MCharsetDetector
 *
 * \brief implements charset or encoding detection
 *
 * MCharsetDetector provides a facility for detecting the charset
 * or encoding of of unknown character data. The input data comes
 * from an array of bytes (a QByteArray). To some extent it
 * also detects the language.
 *
 * Character set detection involves some statistics and guesswork,
 * therefore the results cannot be guaranteed to always be correct.
 *
 * For best accuracy, the input data should be primarily in a
 * single language, and a minimum of a few hundred bytes worth of
 * plain text in the language are needed.
 *
 * Optionally it is possible to ignore html or xml style markup
 * that could otherwise obscure the content.
 *
 * Example:
 *
 * \code
 * // create two sample encoded strings
 * // (the sample strings here are a bit short, if possible one
 * // should use longer input for detection, this is just
 * // to show how it works. But the short input in this example
 * // does actually work):
 * QTextCodec *codecUtf8 = QTextCodec::codecForName("UTF-8");
 * QTextCodec *codecEucjp = QTextCodec::codecForName("EUC-JP");
 * QByteArray encodedStringUtf8
 *     = codecUtf8->fromUnicode(QString::fromUtf8("日本語"));
 * QByteArray encodedStringEucjp
 *     = codecEucjp->fromUnicode(QString::fromUtf8("日本語"));
 *
 * // now try to detect the encoding of the sample strings:
 * MCharsetDetector charsetDetector(encodedStringUtf8);
 * if(charsetDetector.hasError())
 *     qWarning() << "an error happened" << charsetDetector.errorString();
 * MCharsetMatch bestMatch = charsetDetector.detect();
 * if(charsetDetector.hasError())
 *     qWarning() << "an error happened" << charsetDetector.errorString();
 * // print match found:
 * qDebug() << bestMatch.name();       // this will print "UTF-8"
 * qDebug() << bestMatch.language();   // this will print ""
 * qDebug() << bestMatch.confidence(); // this will print "100"
 * // decode input string into a QString, using the encoding
 * // detected as best best match:
 * QString result = charsetDetector.text(bestMatch);
 *
 * // try another detection using the same MCharsetDetector object
 * // (it saves some time not creating a new one all the time):
 * charsetDetector.setText(encodedStringEucjp);
 * if(charsetDetector.hasError())
 *     qWarning() << "an error happened" << charsetDetector.errorString();
 * QList<MCharsetMatch> mCharsetMatchList = charsetDetector.detectAll();
 * if(charsetDetector.hasError())
 *     qWarning() << "an error happened" << charsetDetector.errorString();
 * // print all matches found:
 * for(int i = 0; i < mCharsetMatchList.size(); ++i) {
 *     qDebug() << i << ":"
 *              << mCharsetMatchList[i].name()     // for i==0, “EUC-JP” is printed
 *              << mCharsetMatchList[i].language() // for i==0, “ja” is printed
 *              << mCharsetMatchList[i].confidence();
 * }
 * // decode input string into a QString using the encoding detected
 * // in the first (i.e. best )match (if there was a match at all):
 * if(!mCharsetMatchList.isEmpty())
 *     result = charsetDetector.text(mCharsetMatchList.first());
 *
 * \endcode
 */

class M_CORE_EXPORT MCharsetDetector
{
public:
    /*!
     * \brief constructs a MCharsetDetector without text content
     *
     * \sa MCharsetDetector(const QByteArray &ba)
     * \sa MCharsetDetector(const char *str)
     * \sa MCharsetDetector(const char *data, int size)
     * \sa setText(const QByteArray &ba)
     */
    MCharsetDetector();

    /*!
     * \brief constructs a MCharsetDetector with a initial text content
     * \param ba an array of bytes with the initial input text
     *
     * \sa MCharsetDetector()
     * \sa MCharsetDetector(const char *str)
     * \sa MCharsetDetector(const char *data, int size)
     * \sa setText(const QByteArray &ba)
     */
    MCharsetDetector(const QByteArray &ba);

    /*!
     * \brief constructs a MCharsetDetector with a initial text content
     * \param str a string with the initial input text
     *
     * \sa MCharsetDetector()
     * \sa MCharsetDetector(const QByteArray &ba)
     * \sa MCharsetDetector(const char *data, int size)
     * \sa setText(const QByteArray &ba)
     */
    explicit MCharsetDetector(const char *str);

    /*!
     * \brief constructs a MCharsetDetector with a initial text content
     * \param str a string with the initial input text
     *
     * \sa MCharsetDetector()
     * \sa MCharsetDetector(const QByteArray &ba)
     * \sa MCharsetDetector(const char *str)
     * \sa setText(const QByteArray &ba)
     */
    MCharsetDetector(const char *data, int size);

    /*!
     * \brief destructor for MCharsetDetector
     */
    virtual ~MCharsetDetector();

    /*!
     * \brief checks whether an error occurred during the last action
     *
     * \sa clearError()
     * \sa errorString()
     */
    bool hasError() const;

    /*!
     * \brief clears any error which might have occurred during previous action
     *
     * Useful if one wants to ignore an error and continue.
     *
     * \sa hasError()
     * \sa errorString()
     */
    void clearError();

    /*!
     * \brief text describing the error occurred during the last action
     *
     * Example:
     *
     * \code
     * MCharsetDetector detector;
     * if(detector.hasError())
     *   qWarning() << detector.errorString();
     * \endcode
     *
     * \sa hasError()
     * \sa clearError()
     */
    QString errorString() const;

    /*!
     * \brief sets the input byte data whose charset is to detected.
     */
    void setText(const QByteArray &ba);

    /*!
     * \brief detects the most likely encoding
     *
     * returns an MCharsetMatch object.
     *
     * \sa detectAll()
     */
    MCharsetMatch detect();

    /*!
     * \brief detects a list of possible encodings
     *
     * returns a list of MCharsetMatch objects with charsets which
     * appear to be consistent with the input. The results are ordered
     * according to their confidence level, the match with the highest
     * confidence comes first in the list. I.e. the first element
     * in this list is always the same as the return value of detect().
     *
     * The detection only looks at a limited amount of the input byte
     * data, thus but charsets detected might fail to handle all of
     * the input data. But this is checked again after the detection
     * and charsets which really do fail to handle all of the input
     * data are removed again from list of detected encodings.
     *
     * I.e. the charsets in the list returned by detectAll() will
     * always handle all of the input data.
     *
     * \sa detect()
     */
    QList<MCharsetMatch> detectAll();

    /*!
     * \brief get the entire input text converted to the encoding of a match
     */
    QString text(const MCharsetMatch &charsetMatch);

    /*!
     * \brief set the declared locale for charset detection
     * \param a locale name to give as a hint
     *
     * This can be used as an additional hint for the charset detector.
     *
     * For example, setDeclaredLocale("zh") may help to detect
     * Chinese legacy encodings better, setDeclaredLocale("zh_CN")
     * may further improve this for simplified Chinese legacy encodings,
     * setDeclaredLocale("ru") may help for detecting Russian legacy
     * encodings better, etc. ...
     *
     * \sa setDeclaredEncoding(const QString &encoding)
     */
    void setDeclaredLocale(const QString &locale);

    /*!
     * \brief set the declared encoding for charset detection.
     * \param a charset name to give as a hint
     *
     * This can be used as an additional hint for the charset detector.
     *
     * This is passed through to the libicu encoding detection system
     * which currently does not do anthing with that information. But
     * MCharsetDetector uses it to improve the matches returned by the
     * libicu encoding detection.
     *
     * \sa setDeclaredLocale(const QString &locale)
     */
    void setDeclaredEncoding(const QString &encoding);

    /*!
     * \brief returns a list of detectable charsets
     *
     * This does <b>not</b> depend on the state of the
     * MCharsetDetector object, it always returns the same list.
     */
    QStringList getAllDetectableCharsets();

    /*!
     * \brief enable or disable filtering of input text.
     * \param selects whether to enable or disable the filtering
     *
     * If filtering is enabled, text within angled brackets (“<” and
     * “>”) will be removed before detection, which will remove most
     * HTML or xml markup.
     *
     * This does not use a real HBML or xml parser, it <b>only</b>
     * looks for angled brackets. If less then 5 “<” characters are
     * found or a significant amount of illegally nested “<”
     * characters are found or the input text looks like it is
     * essentially nothing but markup, the filtering is abandoned and
     * detection is done with the unstripped input.
     *
     * \sa isInputFilterEnabled()
     */
    void enableInputFilter(const bool enable);

    /*!
     * \brief test whether input filtering is enabled or not
     *
     * \sa enableInputFilter(const bool enable)
     */
    bool isInputFilterEnabled();

private:
    Q_DISABLE_COPY(MCharsetDetector)
    MCharsetDetectorPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MCharsetDetector)
};

#endif
