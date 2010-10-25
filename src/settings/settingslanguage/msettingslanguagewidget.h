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

#ifndef MSETTINGSLANGUAGEWIDGET_H_
#define MSETTINGSLANGUAGEWIDGET_H_

#include <MWidgetController>
#include "mexport.h"

typedef MWidgetModel MSettingsLanguageWidgetModel;

/*!
 * MSettingsLanguageWidget is a UI component that represents a MSettingsLanguageBinary node.
 */
class M_SETTINGS_EXPORT MSettingsLanguageWidget : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MSettingsLanguageWidget)

public:
    /*!
     * Constructs a new MSettingsLanguageWidget.
     *
     * \param parent the parent for the menu item
     */
    MSettingsLanguageWidget(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the MSettingsLanguageWidget.
     */
    virtual ~MSettingsLanguageWidget();

Q_SIGNALS:
    /*!
     * \brief Signal for informing that action is triggered by the widget.
     */
    void actionTriggered();
};

#endif /* MSETTINGSLANGUAGEWIDGET_H_ */
