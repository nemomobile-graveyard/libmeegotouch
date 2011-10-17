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

#ifndef MTEXTEDITSTYLE_H
#define MTEXTEDITSTYLE_H

#include <QFont>
#include <QColor>
#include <QString>
#include <mwidgetstyle.h>

class M_VIEWS_EXPORT MTextEditStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MTextEditStyle)

    M_STYLE_ATTRIBUTE(QFont,        font,                     Font)
    M_STYLE_ATTRIBUTE(QColor,       textColor,                TextColor)
    M_STYLE_ATTRIBUTE(QColor,       selectionTextColor,       SelectionTextColor)
    M_STYLE_ATTRIBUTE(QColor,       selectionBackgroundColor, SelectionBackgroundColor)
    M_STYLE_ATTRIBUTE(qreal,        selectionThreshold,       SelectionThreshold)
    M_STYLE_ATTRIBUTE(bool,         allowViewReposition,      AllowViewReposition)
    M_STYLE_ATTRIBUTE(bool,         disableMagnifier,         DisableMagnifier)
    M_STYLE_ATTRIBUTE(QString,      maskString,               MaskString)
    M_STYLE_ATTRIBUTE(QColor,       promptColor,              PromptColor)
    M_STYLE_ATTRIBUTE(QFont,        promptFont,               PromptFont)
    M_STYLE_ATTRIBUTE(QString,      pasteFailedIcon,          PasteFailedIcon)
    M_STYLE_ATTRIBUTE(int,          pasteFailedDuration,      PasteFailedDuration)
    M_STYLE_ATTRIBUTE(int,          maskingDelay,             MaskingDelay)
    M_STYLE_ATTRIBUTE(int,          cursorWidth,              CursorWidth)
    M_STYLE_ATTRIBUTE(qreal,        focusedPromptOpacity,     FocusedPromptOpacity)
    M_STYLE_ATTRIBUTE(qreal,        unfocusedPromptOpacity,   UnfocusedPromptOpacity)
    M_STYLE_ATTRIBUTE(int,          focusTransitionDuration,  FocusTransitionDuration)
    M_STYLE_ATTRIBUTE(int,          hideShowPromptDuration,   HideShowPromptDuration)
    M_STYLE_ATTRIBUTE(int,          promptTransitionDelay,    PromptTransitionDelay)
    M_STYLE_ATTRIBUTE(bool,         disableToolbar,           DisableToolbar)
        
    M_STYLE_ATTRIBUTE(int,          textClippingLeft,         TextClippingLeft)
    M_STYLE_ATTRIBUTE(int,          textClippingTop,          TextClippingTop)
    M_STYLE_ATTRIBUTE(int,          textClippingRight,        TextClippingRight)
    M_STYLE_ATTRIBUTE(int,          textClippingBottom,       TextClippingBottom)

    /*!
        \property MTextEditStyle::pressBoundaryFeedback
        \brief Feedback given when pressing between words
      */
    M_STYLE_ATTRIBUTE(MFeedback, pressBoundaryFeedback, PressBoundaryFeedback)

    /*!
        \property MTextEditStyle::releaseBoundaryFeedback
        \brief Feedback given when releasing between words
      */
    M_STYLE_ATTRIBUTE(MFeedback, releaseBoundaryFeedback, ReleaseBoundaryFeedback)

    /*!
        \property MTextEditStyle::pressWordFeedback
        \brief Feedback given when pressing on top of a word
      */
    M_STYLE_ATTRIBUTE(MFeedback, pressWordFeedback, PressWordFeedback)

    /*!
        \property MTextEditStyle::releaseWordFeedback
        \brief Feedback given when releasing on top of a word
      */
    M_STYLE_ATTRIBUTE(MFeedback, releaseWordFeedback, ReleaseWordFeedback)

    /*!
        \property MTextEditStyle::changeSelectionFeedback
        \brief Feedback given when finger movement changes text selection
      */
    M_STYLE_ATTRIBUTE(MFeedback, changeSelectionFeedback, ChangeSelectionFeedback)

    /*!
     * \property MTextEditStyle::toolbarStyleName
     * \brief Style to use for this control's toolbar.
     */
    M_STYLE_ATTRIBUTE(QString, toolbarStyleName, ToolbarStyleName)

    /*!
     * \property MTextEditStyle::selectionDelay
     * \brief Delay for text selection update.
     *
     * Text selection update is postponed if mouse is moved too fast.
     * \sa MTextEditStyle::selectionSpeedThreshold
     */
    M_STYLE_ATTRIBUTE(int, selectionDelay, SelectionDelay)

    /*!
     * \property MTextEditStyle::selectionSpeedThreshold
     * \brief Text selection is postponed if mouse speed
     * is bigger than this value.
     *
     * \sa MTextEditStyle::selectionDelay
     */
    M_STYLE_ATTRIBUTE(qreal, selectionSpeedThreshold, SelectionSpeedThreshold)

    /*!
     * \property MTextEditStyle::scrollCursorMarginTop
     * \brief Top margin which extends cursor rect to trigger automatic
     * scrolling when cursor rect is obscured.
     *
     * This property is applied when magnifier is visible.
     */
    M_STYLE_ATTRIBUTE(int, scrollCursorMarginTop, ScrollCursorMarginTop)

    /*!
     * \property MTextEditStyle::scrollCursorMarginBottom
     * \brief Bottom margin which extends cursor rect to trigger automatic
     * scrolling when cursor rect is obscured.
     *
     * This property is applied when magnifier is visible.
     */
    M_STYLE_ATTRIBUTE(int, scrollCursorMarginBottom, ScrollCursorMarginBottom)
};

class M_VIEWS_EXPORT MTextEditStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MTextEditStyle)

    M_STYLE_MODE(Error)
};

#endif

