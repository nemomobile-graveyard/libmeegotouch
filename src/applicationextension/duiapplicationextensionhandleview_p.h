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

#ifndef DUIAPPLICATIONEXTENSIONHANDLEVIEW_P_H_
#define DUIAPPLICATIONEXTENSIONHANDLEVIEW_P_H_

#include "duiextensionhandleview_p.h"

class DuiApplicationExtensionHandleView;

class DuiApplicationExtensionHandleViewPrivate : public DuiExtensionHandleViewPrivate
{
    Q_DECLARE_PUBLIC(DuiApplicationExtensionHandleView)

public:
    DuiApplicationExtensionHandleViewPrivate(DuiExtensionHandle *handle);
    virtual ~DuiApplicationExtensionHandleViewPrivate();

    /*! Shows a dialog for deciding what to do with a broken extension. */
    void showBrokenDialog();

    /*! Shows a dialog for deciding what to do when the extension installation failed. */
    void showInstallationFailedDialog(const QString &error);

    //! \reimp
    virtual void drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const;
    //! \reimp_end
};

#endif /* DUIAPPLICATIONEXTENSIONHANDLEVIEW_P_H_ */
