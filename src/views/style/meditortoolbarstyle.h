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
#ifndef MEDITORTOOLBARSTYLE_H
#define MEDITORTOOLBARSTYLE_H

#include <MWidgetStyle>

/*!
    \brief This defines style for editor toolbar which can be either cut/copy/paste
           or rich text editor toolbar.
*/
class M_VIEWS_EXPORT MEditorToolbarStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MEditorToolbarStyle)

    /*! \brief Autohide timeout.
     * The delay (in milliseconds) after which the toolbar will be hidden, if no text is selected.
     */
    M_STYLE_ATTRIBUTE(int, hideTimeout, HideTimeout)

    /*! \brief Autohide animation duration.
     *
     *  The animation duration to use when toolbar is hiding automatically.
     *  Given in milliseconds.
     */
    M_STYLE_ATTRIBUTE(int, autoHideAnimationDuration, AutoHideAnimationDuration)

    /*! \brief Hide animation duration, in milliseconds.
     *
     *  Used both in disappearance animation and autohide.
     */
    M_STYLE_ATTRIBUTE(int, hideAnimationDuration, HideAnimationDuration)

    /*! \brief Show animation duration, in milliseconds.
     */
    M_STYLE_ATTRIBUTE(int, showAnimationDuration, ShowAnimationDuration)
};

class M_VIEWS_EXPORT MEditorToolbarStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MEditorToolbarStyle)

    //! Mode for editor toolbar under cursor, arrow pointing up.
    M_STYLE_MODE(EditorUnderCursor)
};

#endif // MEDITORTOOLBARSTYLE_H
