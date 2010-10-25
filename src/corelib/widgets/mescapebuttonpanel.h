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

#ifndef MESCAPEBUTTONPANEL_H
#define MESCAPEBUTTONPANEL_H

#include "mscenewindow.h"
#include "mescapebuttonpanelmodel.h"

/*!
 * \class MEscapeButtonPanel
 * \brief The MEscapeButtonPanel class displays an escape button.
 *
 * The escape button can be displayed regardless of the navigation bar and provides
 * three display modes described by MEscapeButtonModel::EscapeMode. In contrast to
 * the home button (provided by MHomeButtonPanel), it does not stand above all GUI
 * elements, i.e. can be covered by modal dialogs, and MOverlay class instances.
 *
 * \sa MHomeButtonPanel
 */
class M_CORE_EXPORT MEscapeButtonPanel : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MEscapeButtonPanel)
    Q_PROPERTY(MEscapeButtonPanelModel::EscapeMode escapeMode READ escapeMode WRITE setEscapeMode)

public:

    /*!
     * \brief Constructs the escape button with the given \a parent.
     */
    MEscapeButtonPanel(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~MEscapeButtonPanel();

    /*!
     * \brief Returns the current escape mode.
     * \sa MEscapeButtonPanelModel::EscapeMode
     */
    MEscapeButtonPanelModel::EscapeMode escapeMode() const;

public Q_SLOTS:
    /*!
     * \brief Sets the escape mode of the button to the given \a escapeMode.
     * \sa MEscapeButtonPanelModel::EscapeMode
     */
    void setEscapeMode(MEscapeButtonPanelModel::EscapeMode escapeMode);

Q_SIGNALS:
    void escapeModeChanged(MEscapeButtonPanelModel::EscapeMode escapeMode);

    /*!
     * \brief Emitted when the home button was clicked.
     */
    void buttonClicked();

private:

    Q_DISABLE_COPY(MEscapeButtonPanel)
};

#endif // MESCAPEBUTTONPANEL_H
