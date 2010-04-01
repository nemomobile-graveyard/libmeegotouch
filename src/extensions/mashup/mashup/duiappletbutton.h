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

#ifndef DUIAPPLETBUTTON_H
#define DUIAPPLETBUTTON_H

#include <DuiButton>

#include <duiappletmetadata.h>

//! \internal

/*!
 * DuiAppletButton widget presents an available applet
 * in the applet inventory.
 *
 * The icon, name and other data of the DuiAppletButton
 * is retrieved from applet metadata, that is read from
 * an applet metadata file given as a parameter to the
 * initialize() - method.
 *
 * \code
 * // Create a new duiappletbutton
 * DuiAppletButton* button = new DuiAppletButton();
 *
 * // Initialize the applet button from a given metadata file.
 * if(button->initialize(appletMetaDataFile)) {
 *    // ... Use applet button here.
 * }
 * \endcode
 */
class DuiAppletButton : public DuiButton
{
    Q_OBJECT

public:
    /*!
     * Constructs an DuiAppletButton.
     */
    DuiAppletButton(QGraphicsItem *parent = 0, const QString &type = "");

    /*!
     * Destroys the DuiAppletButton.
     */
    virtual ~DuiAppletButton();

    /*!
     * Initializes the applet button. Absolute path to an applet
     * metadata file is given as a parameter.
     *
     * This method will read the applet metadata and verify that
     * the data is valid. If the data is valid, this applet can
     * be instantiated and used.
     *
     * \param data the applet metadata
     * \return true if metadata is valid and applet can be used. False otherwise.
     */
    bool initialize(const DuiAppletMetaData &data);

    /*!
     * Get file name for the metadata of the applet that this button represents
     *
     * \return file name for the metadata of the applet that this button represents
     */
    QString metadataFilename() const;

private:
    //! Applet metadata file name
    QString metadataFile;
};

//! \internal_end

#endif // DUIAPPLETBUTTON_H
