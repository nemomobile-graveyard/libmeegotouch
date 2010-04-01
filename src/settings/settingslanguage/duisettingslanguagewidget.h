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

#ifndef DUISETTINGSLANGUAGEWIDGET_H_
#define DUISETTINGSLANGUAGEWIDGET_H_

#include <DuiWidgetController>
#include "duiexport.h"

typedef DuiWidgetModel DuiSettingsLanguageWidgetModel;

/*!
 * DuiSettingsLanguageWidget is a UI component that represents a DuiSettingsLanguageBinary node.
 */
class DUI_EXPORT DuiSettingsLanguageWidget : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiSettingsLanguageWidget)

public:
    /*!
     * Constructs a new DuiSettingsLanguageWidget.
     *
     * \param parent the parent for the menu item
     */
    DuiSettingsLanguageWidget(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the DuiSettingsLanguageWidget.
     */
    virtual ~DuiSettingsLanguageWidget();

Q_SIGNALS:
    /*!
     * \brief Signal for informing that action is triggered by the widget.
     */
    void actionTriggered();
};

#endif /* DUISETTINGSLANGUAGEWIDGET_H_ */
