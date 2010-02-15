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

#ifndef DUIAPPLETTESTERWINDOW_H
#define DUIAPPLETTESTERWINDOW_H

#include <DuiWindow>

/*!
 * A window class for the dui applet tester application.
 */
class DuiAppletTesterWindow : public DuiWindow
{
    Q_OBJECT

public:
    DuiAppletTesterWindow();

    /*!
     * Destructor.
     */
    virtual ~DuiAppletTesterWindow();

signals:
    /*!
     * A signal that gets emitted when the window size has changed.
     */
    void resized(const QSize &);

protected:
    /*!
     * Handler for window resizing events.
     * \param event the resize event.
     */
    virtual void resizeEvent(QResizeEvent *event);

};

#endif // DUIAPPLETTESTERWINDOW_H
