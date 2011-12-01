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

#ifndef MELIDEDTEXT_H
#define MELIDEDTEXT_H

#include <QString>
#include <QChar>
#include <QStringList>
#include <QSizeF>
#include <QFont>
#include <QTextOption>

//! \internal

/*!
    \class MElidedText
    \brief Allow to elide text depeding on rendering parameters.
*/
class MElidedText
{
public:
    /*!
       \brief This character could be added to your text by elided() or doElideText()
     */
    static const QChar EllipsisChar;

    /*!
       \brief This character separates diffirent text variants in one QString.
       \sa doElideText()
     */
    static const QChar SeparatorChar;

    //! \brief Creates new instance of MTextElider
    MElidedText();

    /*!
       \brief Do text eliding according to given parameters.

       \param renderSize Size of area which will be used to draw the text.
       \param font Font which will be used to draw the text.
       \param textVariants Available text variants. This method goes through all available variants
       from first to last and returns immediately if some variant fits to availalbe space.
       Otherwise last available variant will be elided and returned back.
       \param textOption Text options which will be used to draw the text.
       \param isMultipleLines Will be set to true if selected text variant contains more than one row.
       \param doElide When set to true, method automatically elides text with three dots if
       the whole text does not fit into one line. See also MLabelModel::textElide.
     */
    static QString doElideText(const QSizeF &renderSize,
                               const QFont &font,
                               const QStringList &textVariants,
                               const QTextOption &textOption,
                               bool *isMultipleLines = 0,
                               bool doElide = true);

    /*!
       \brief Save text which should be elided and invalidate local cache.

       \param newText Text to be elided later.
       \param separatorChar Divides given text into variants.
     */
    void setText(const QString &newText,
                 QChar separatorChar = SeparatorChar);

    /*!
       \brief Do text eliding according to given parameters.

       Return cached value if cache is valid.
       Cache is invalidated if you change any parameter of this method
       or call setText().

       \param renderSize Size of area which will be used to draw the text.
       \param font Font which will be used to draw the text.
       \param textOption Text options which will be used to draw the text.
     */
    const QString &elided(const QSizeF &renderSize,
                          const QFont &font,
                          const QTextOption &textOption) const;

private:
    mutable bool dirty; // Contains true if cache contains invalid value
    QStringList text; // Text which will be elided by elideText()
    mutable QString elidedText; // Cached result of elideText()

    // parameres of last call to elideText()
    mutable QSizeF lastRenderSize;
    mutable QFont lastFont;
    mutable QTextOption lastTextOption;
};

//! \internal_end

#endif

