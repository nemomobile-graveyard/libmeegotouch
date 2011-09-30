#/***************************************************************************
#**
#** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
#** All rights reserved.
#** Contact: Nokia Corporation (directui@nokia.com)
#**
#** This file is part of libmeegotouch.
#**
#** If you have questions regarding the use of this file, please contact
#** Nokia at directui@nokia.com.
#**
#** This library is free software; you can redistribute it and/or
#** modify it under the terms of the GNU Lesser General Public
#** License version 2.1 as published by the Free Software Foundation
#** and appearing in the file LICENSE.LGPL included in the packaging
#** of this file.
#**
#****************************************************************************/
#!/bin/sh

export DISPLAY=:0

TESTSDIR=/usr/lib/libmeegotouch-tests
OUTDIR=/tmp/libmeegotouch-tests

mkdir -p ${OUTDIR}

EXIT_CODE=0
for TEST in `ls ${TESTSDIR}/?t_*`; do
	$TEST -xml -o "${OUTDIR}/$(basename $TEST)".xml
	if [ $? -ne 0 ]; then
		EXIT_CODE=$((EXIT_CODE+1))
	fi
done
exit $EXIT_CODE
