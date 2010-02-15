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

// Make doxygen skip this internal class

/* Internal classes for DuiGLRender*/

/*!
 * \class DuiCompositionLayer
 * \brief A class for defining a composition layer.
*/
#ifndef DUIGLRENDERER_P_H
#define DUIGLRENDERER_P_H

/* Internal implementation classes for DuiGLRenderer */

/*!
 * \class DuiCompositionLayer
 * \brief A class for defining a composition layer.
*/
class DuiCompositionLayer
{
public:

    //! Enumeration for supported composition modes
    enum DuiCompositionMode {
        Normal,
        Multiply
    };

    /*!
     * \brief Constructor. Initializes the class with wanted parameters.
     */
    DuiCompositionLayer(const QPixmap &pixmap, float opacity, DuiCompositionMode mode)
        : m_pixmap(pixmap), m_opacity(opacity), m_mode(mode) {
    }

    /*!
     * Returns pixmap associated with the layer.
     *
     * \return Layer pixmap.
     */
    QPixmap pixmap() const {
        return m_pixmap;
    }

    /*!
     * Returns opacity of the layer.
     *
     * \return Layer opacity.
     */
    float opacity() const {
        return m_opacity;
    }

    /*!
     * Returns composition mode of the layer.
     *
     * \return Layer composition mode.
     */
    DuiCompositionMode mode() const {
        return m_mode;
    }

private:

    /*!
     * \brief Default constructor.
     */
    DuiCompositionLayer();

    //! Layer pixmap.
    QPixmap m_pixmap;

    //! Opacity of the layer.
    float m_opacity;

    //! Composition mode of the layer.
    DuiCompositionMode m_mode;
};

#endif // DUIGLRENDERER_P_H
