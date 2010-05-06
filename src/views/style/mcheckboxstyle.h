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

#ifndef MCHECKBOXSTYLE_H
#define MCHECKBOXSTYLE_H

#include <QColor>
#include <mbuttonstyle.h>

/*!
    \class MCheckboxStyle
    \brief Style class for m checkboxes.

    \code
        MCheckboxStyle {
            checkmark-image: mbutton-checkbox-checkmark 0 0 0 0;
        }
    \endcode

    \ingroup styles
    \sa MCheckboxStyleContainer MWidgetStyle MButtonStyle \ref styling MButton MCheckboxView
*/
class M_EXPORT MCheckboxStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MCheckboxStyle)

    /*!
        \property MButtonSwitchStyle::checkmarkImage
        \brief Pixmap for the checkmark of the checkbox.
    */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *,  checkmarkImage, CheckmarkImage)

};

/*!
    \class MCheckboxStyleContainer
    \brief Style mode container class for MCheckboxStyle.

    \ingroup styles
    \sa MCheckboxStyle
*/
class M_EXPORT MCheckboxStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MCheckboxStyle)
};

#endif

