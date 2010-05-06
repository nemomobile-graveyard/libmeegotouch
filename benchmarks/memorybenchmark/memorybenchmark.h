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

#ifndef MEMORYBENCHMARK_H
#define MEMORYBENCHMARK_H

#include <stdlib.h>

// Do not use these three methods, they produce
// inaccurate results that are nonsense.
void resetMemorySizeSnapshot();
long int usedMemorySizeSnapshot();
void printUsedMemorySizeSnapshot();

/*!
    \brief Begins memory benchmark, clears all counters
 */
void   beginMemoryBenchmark();

/*!
    \brief Reports currently reserved memory after last beginMemoryBenchmark call
 */
size_t allocatedMemorySize();

/*!
    \brief Outputs the size of allocated memory
 */
void   outputAllocatedMemorySize();

/*!
    \brief Finishes memory benchmark, clears counters and returns the amount of unfreed memory.
    \param outputUnfreedMemory  if true, outputs the amount of unfreed memory (in bytes)
 */
size_t endMemoryBenchmark(bool outputUnfreedMemory = false);

/*!
    \brief Starts to collect backtrace information from allocations, NOTE! this is a SLOW process.
 */
void beginBacktracing();

/*!
    \brief Stops collecting backtrace.
 */
void endBacktracing();

/*!
    \brief Prints out all allocated memory with backtrace information.
 */
void outputAllocatedMemoryBacktrace();
#endif

