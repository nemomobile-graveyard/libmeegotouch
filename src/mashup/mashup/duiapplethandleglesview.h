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

#ifndef DUIAPPLETHANDLEGLESVIEW_H
#define DUIAPPLETHANDLEGLESVIEW_H

#include <duiwidgetview.h>
#include "duiapplethandle.h"
#include "duiapplethandlemodel.h"
#include "duiapplethandlestyle.h"
#include "duiapplethandleview.h"
#include <DuiGLRenderer>
#include <DuiGLShaderUniform>

class DuiAppletHandleGLESViewPrivate;

//! \internal
/*!
 * DuiAppletHandleGLESView is a view for the DuiAppletHandle that
 * renders graphics using OpenGL where possible.
 */
class DuiAppletHandleGLESView : public DuiAppletHandleView
{
    Q_OBJECT
    DUI_VIEW(DuiAppletHandleModel, DuiAppletHandleStyle)

public:
    /*!
     * Constructs a DuiAppletHandleGLESView.
     *
     * \param appletHandle the DuiAppletHandle controller to be used
     */
    DuiAppletHandleGLESView(DuiAppletHandle *appletHandle);

    /*!
     * Destroys the DuiAppletHandleGLESView.
     */
    virtual ~DuiAppletHandleGLESView();

private:
    Q_DISABLE_COPY(DuiAppletHandleGLESView)
    Q_DECLARE_PRIVATE(DuiAppletHandleGLESView)
};


/*!
 * Provides the uniforms required by DuiAppletHandleGLESView.
 */
class DUI_EXPORT DuiAppletHandleViewUniformProvider : public IDuiGLUniformProvider
{
public:
    /*!
     * \brief Constructs a new DuiAppletHandleViewUniformProvider.
     *
     * \param width the width of the applet view, must be more than 0
     * \param height the height of the applet view, must be more than 0
     */
    DuiAppletHandleViewUniformProvider(float width, float height);

    /*!
     * \brief Destroys the DuiAppletHandleViewUniformProvider.
     */
    virtual ~DuiAppletHandleViewUniformProvider();

    //! \reimp
    virtual bool setUniformValue(const QString &name, const DuiGLShaderUniform &uniform);
    //! \reimp_end

private:
    //! The width of the applet view
    float width;
    //! The height of the applet view
    float height;
};
//! \internal_end

#endif // DUIAPPLETHANDLEGLESVIEW_H
