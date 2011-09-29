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


#include <cstring>


#include "utils.h"


  // TODO: perf-checks
char *util::dup(const char *src)
{
    unsigned l = 0;
    for (const char *i = src; *i; i++)
        l++;

    char *ret = new char[l+1];

    const char *a = src;
    char *b = ret;

    for (unsigned i = 0; i < l; i++)
        *(b++) = *(a++);

    *b = 0;

    return ret;
}


void util::rewindbuf(unsigned char *buffer, unsigned amount, unsigned buflength)
{
    unsigned char *p[2] = { buffer, buffer + amount };

    for (unsigned i = 0; i < buflength; i++)
        *(p[0]++) = *(p[1]++);
}


char *util::basename(const char *filename, const char *&lastdot)
{
    const char *dot = 0;

    for(const char *i = filename; i; i = strstr(i+1, "."))
        dot = i;

    if(filename < dot) {
        unsigned length = dot - filename;
        char *ret = new char[length + 1];
        memcpy(ret, filename, length);
        ret[length] = 0;

        lastdot = dot;
        return ret;
    }

    lastdot = 0;
    return 0;
}
