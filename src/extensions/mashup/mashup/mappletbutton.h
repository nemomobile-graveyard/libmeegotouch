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

#ifndef MAPPLETBUTTON_H
#define MAPPLETBUTTON_H

#include <MButton>

#include <mappletmetadata.h>

//! \internal

/*!
 * MAppletButton widget presents an available applet
 * in the applet inventory.
 *
 * The icon, name and other data of the MAppletButton
 * is retrieved from applet metadata, that is read from
 * an applet metadata file given as a parameter to the
 * initialize() - method.
 *
 * \code
 * // Create a new mappletbutton
 * MAppletButton* button = new MAppletButton();
 *
 * // Initialize the applet button from a given metadata file.
 * if(button->initialize(appletMetaDataFile)) {
 *    // ... Use applet button here.
 * }
 * \endcode
 */
class MAppletButton : public MButton
{
    Q_OBJECT

public:
    /*!
     * Constructs an MAppletButton.
     */
    MAppletButton(QGraphicsItem *parent = 0, const QString &type = "");

    /*!
     * Destroys the MAppletButton.
     */
    virtual ~MAppletButton();

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
    bool initialize(const MAppletMetaData &data);

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

#endif // MAPPLETBUTTON_H
