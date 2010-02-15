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

#ifndef DUISETTINGSLANGUAGENODE_H
#define DUISETTINGSLANGUAGENODE_H

#include <QList>

/*!
 * \brief An abstract base class for the different settings binary nodes.
 *
 * This class provides support for constructing tree structures of the
 * settings binary nodes.
 */
class DuiSettingsLanguageNode
{
public:
    /*!
     * \brief Destructor.
     *
     * Also destroys all child nodes recursively so for the user it's enough
     * to destroy the root node.
     */
    virtual ~DuiSettingsLanguageNode() = 0;

    /*!
     * Adds a child node to this node.
     *\param child the child to be added.
     */
    void addChild(DuiSettingsLanguageNode *child);

    /*!
     * Gets the number of child nodes this node currently has.
     * \return the number of child nodes.
     */
    uint numChildren() const;

    /*!
     * Gets the list of child nodes this node currently has.
     * \return a list of child nodes.
     */
    QList<DuiSettingsLanguageNode *> children() const;

protected:
    /*!
    * \brief Constructor.
    */
    DuiSettingsLanguageNode();

private:
    /*!
     * Container for the child nodes.
     */
    QList<DuiSettingsLanguageNode *> _children;
};

#endif // DUISETTINGSLANGUAGENODE_H
