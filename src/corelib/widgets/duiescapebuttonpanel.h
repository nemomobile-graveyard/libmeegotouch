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

#ifndef DUIESCAPEBUTTONPANEL_H
#define DUIESCAPEBUTTONPANEL_H

#include "duiscenewindow.h"
#include "duiescapebuttonpanelmodel.h"

/*!
 * \class DuiEscapeButtonPanel
 * \brief The DuiEscapeButtonPanel class displays an escape button.
 *
 * The escape button can be displayed regardless of the navigation bar and provides
 * three display modes described by DuiEscapeButtonModel::EscapeMode. In contrast to
 * the home button (provided by DuiHomeButtonPanel), it does not stand above all GUI
 * elements, i.e. can be covered by modal dialogs, and DuiOverlay class instances.
 *
 * \sa DuiHomeButtonPanel
 */
class DUI_EXPORT DuiEscapeButtonPanel : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiEscapeButtonPanel)
    Q_PROPERTY(DuiEscapeButtonPanelModel::EscapeMode escapeMode READ escapeMode WRITE setEscapeMode)

public:

    /*!
     * \brief Constructs the escape button with the given \a parent.
     */
    DuiEscapeButtonPanel(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~DuiEscapeButtonPanel();

    /*!
     * \brief Returns the current escape mode.
     * \sa DuiEscapeButtonPanelModel::EscapeMode
     */
    DuiEscapeButtonPanelModel::EscapeMode escapeMode() const;

public Q_SLOTS:
    /*!
     * \brief Sets the escape mode of the button to the given \a escapeMode.
     * \sa DuiEscapeButtonPanelModel::EscapeMode
     */
    void setEscapeMode(DuiEscapeButtonPanelModel::EscapeMode escapeMode);

Q_SIGNALS:
    void escapeModeChanged(DuiEscapeButtonPanelModel::EscapeMode escapeMode);

    /*!
     * \brief Emitted when the home button was clicked.
     */
    void buttonClicked();

private:

    Q_DISABLE_COPY(DuiEscapeButtonPanel)
};

#endif // DUIESCAPEBUTTONPANEL_H
