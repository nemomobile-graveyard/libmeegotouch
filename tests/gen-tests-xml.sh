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
#!/bin/bash

TESTDIR=`dirname $0`
DOMAIN="Application Framework"
FEATURE="MeeGo Touch UI Framework"
TYPE="Functional"
LEVEL="Component"

UT_TESTCASES=""
FT_TESTCASES=""
SKIPPED_TESTCASES=""

UT_COUNT=0
FT_COUNT=0
SKIPPED_COUNT=0
for TEST in `ls -d ?t_*`; do
       if [ -x $TEST/$TEST ]; then
		if [ -e $TESTDIR/$TEST/insignificant ]; then
			INSIGNIFICANT="true"
		else
			INSIGNIFICANT="false"
		fi

TESTCASE_TEMPLATE="<case name=\"$TEST\" description=\"$TEST\" requirement=\"\" timeout=\"300\" insignificant=\"$INSIGNIFICANT\">
        <step expected_result=\"0\">/usr/lib/libmeegotouch-tests/$TEST</step>
      </case>
      "

		if [ -n "`echo $TEST | egrep '^u'`" ]; then
			UT_TESTCASES="${UT_TESTCASES}${TESTCASE_TEMPLATE}"
			UT_COUNT=$(($UT_COUNT+1))
		else
			FT_TESTCASES="${FT_TESTCASES}${TESTCASE_TEMPLATE}"
			FT_COUNT=$(($FT_COUNT+1))
		fi
        else
                SKIPPED_TESTCASES="${SKIPPED_TESTCASES} ${TEST}"
          	SKIPPED_COUNT=$(($SKIPPED_COUNT+1))
	fi
done

TESTSUITE_TEMPLATE="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>
<testdefinition version=\"0.1\">
  <suite name=\"libmeegotouch-tests\" domain=\"$DOMAIN\" type=\"$TYPE\" level=\"$LEVEL\">
    <set name=\"unit_tests\" description=\"Unit Tests\" feature=\"$FEATURE\">

      $UT_TESTCASES

      <environments>
        <scratchbox>false</scratchbox>
        <hardware>true</hardware>    
      </environments> 

    </set>
    <set name=\"functional_tests\" description=\"Functional Tests\" feature=\"$FEATURE\">

      $FT_TESTCASES

      <environments>
        <scratchbox>false</scratchbox>
        <hardware>true</hardware>    
      </environments> 

    </set>
  </suite>
</testdefinition>"

echo "$TESTSUITE_TEMPLATE"
echo "<!-- UT:$UT_COUNT FT:$FT_COUNT SKIPPED=$SKIPPED_COUNT Skipped tests: $SKIPPED_TESTCASES -->"

