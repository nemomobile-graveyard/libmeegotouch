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


#include <cstring>  // memcpy
#include <cstdio>

#include <sys/types.h>
#include <dirent.h>


#include "fstreenode.h"

#include "utils.h"


  // FIXME: double '/'
FSTreeNode::FSTreeNode(const char *path) : parent(0),
                                           m_path(util::dup(path)),
                                           m_name(m_path)
{
    for (const char *i = strstr(m_name, "/"); i && *(i+1); i = strstr(m_name, "/"))
        m_name = i + 1;
}

FSTreeNode::~FSTreeNode()
{
    if (m_path) {
        delete m_path;
        m_path = 0;
    }

      // orphan the children
    for (QLinkedList<FSTreeNode *>::iterator i = children.begin(); i != children.end(); i++)
        (*i)->parent = 0;

    if (parent)
        parent->children.removeAll(this);
}


const char *FSTreeNode::name() const
{
    return m_name;
}


const char *FSTreeNode::path() const
{
    return m_path;
}


void FSTreeNode::addChild(FSTreeNode *node)
{
    if (node->parent)
        node->parent->children.removeAll(node);

    node->parent = this;
    children.push_back(node);
}


FSTreeNode *FSTreeNode::directChild(const char *name) const
{
    if (!name)
        return 0;

    for (QLinkedList<FSTreeNode *>::const_iterator i = children.begin();
        i != children.end(); i++) {

        if ((*i)->m_name && !strcmp((*i)->m_name, name))
            return *i;
    }

    return 0;
}


bool FSTreeNode::isUnderPath(const char *path)
{
    const char *s = m_path;
    for (const char *i = path; *i; i++) {
        if (*i != *(s++))
            return false;
    }

    return true;
}

