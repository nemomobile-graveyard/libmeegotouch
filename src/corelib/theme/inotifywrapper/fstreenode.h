/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __FSTREENODE_HH__
#define __FSTREENODE_HH__


#include <QLinkedList>


  // TODO: the FSTreeNode objects are in this case representing image
  // directories that include images (and subdirectories as FSTreeNodes) -
  // a node should contain list of its image children in the case of
  // furious destruction
class FSTreeNode
{
public:
    virtual ~FSTreeNode();

    const char *name() const;
    const char *path() const;

    void addChild(FSTreeNode *node);

    FSTreeNode *directChild(const char *name) const;

    bool isUnderPath(const char *path);

protected:
    FSTreeNode(const char *path);

    FSTreeNode *parent;
    QLinkedList<FSTreeNode *> children;

private:
    char *m_path;  // contains full path
    const char *m_name;  // pointer to characted after last '/' in full path
};


#endif // __FSTREENODE_HH__

