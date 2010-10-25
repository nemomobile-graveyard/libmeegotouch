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

#ifndef MLABELHIGHLIGHTER_H
#define MLABELHIGHLIGHTER_H

#include <QObject>
#include <QRegExp>
#include "mexport.h"

/*!
    \class MLabelHighlighter
    \brief Abstract base class for label highlighter objects.

    Class is used to highlight text fragments from MLabel with the color specified in MLabel style.
    Class will also receive user interaction events when clicking and long pressing a highlighted
    piece of text.

    \ingroup widgets

    \sa MLabelStyle::highlightColor
    \sa MLabelStyle::activeHighlightColor
*/
class M_CORE_EXPORT MLabelHighlighter
{
public:

    /*!
        \brief Destructs MLabelHighlighter.
    */
    virtual ~MLabelHighlighter();

    /*!
        \brief Returns regular expression for highlighting text fragments.
    */
    virtual QRegExp highlightExpression() const = 0;

    /*!
        \brief Method for validating highlighted items.

        Return true if the item is really valid, return false it the item
        is invalid. The validated \a item can be changed if wanted. The
        outputted \a item will be the same that is given to the click() and
        longPress() methods. Default implementation just returns true.
    */
    virtual bool validate(QString &item) const;

    /*!
        \brief Callback method when user clicks a highlighted item.
    */
    virtual void click(const QString &item) = 0;

    /*!
        \brief Callback method when user long presses a highlighted item.
    */
    virtual void longPress(const QString &item) = 0;
};

/*!
    \class MCommonLabelHighlighter
    \brief Common highlighter class for easily highlighting items from MLabel without inheriting own classes.

    Inherits MLabelHighlighter and emits signals for the click and longPress interactions.

    \ingroup widgets
*/
class MCommonLabelHighlighterPrivate;
class M_CORE_EXPORT MCommonLabelHighlighter : public QObject, public MLabelHighlighter
{
    Q_OBJECT

public:

    /*!
        \brief Constructs common highlighter class.
    */
    MCommonLabelHighlighter(const QRegExp &regExp);

    /*!
        \brief Destructs MCommonLabelHighlighter.
    */
    virtual ~MCommonLabelHighlighter();

    /*!
        \brief Returns the expression that was given as parameter into the constructor.
    */
    virtual QRegExp highlightExpression() const;

    /*!
        \brief Emits clicked() signal;
    */
    virtual void click(const QString &item);

    /*!
        \brief Emits longPressed() signal;
    */
    virtual void longPress(const QString &item);

Q_SIGNALS:

    /*!
        \brief A signal which is emitted when a highlighted item in label is clicked.
    */
    void clicked(const QString &item);

    /*!
        \brief A signal which is emitted when a highlighted item in label is long pressed.
    */
    void longPressed(const QString &item);

private:
    MCommonLabelHighlighterPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MCommonLabelHighlighter)
};

#endif
