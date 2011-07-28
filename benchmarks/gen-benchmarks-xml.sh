#!/bin/bash

TESTDIR=`dirname $0`
DOMAIN="Application Framework"
FEATURE="MeeGo Touch UI Framework"
TYPE="Benchmarks"
LEVEL="Component"

MT_TESTCASES=""
PT_TESTCASES=""
SKIPPED_TESTCASES=""

MT_COUNT=0
PT_COUNT=0
SKIPPED_COUNT=0
for TEST in `ls -d ?t_*`; do
       if [ -x $TEST/$TEST ]; then
                if [ -e $TESTDIR/$TEST/insignificant ]; then
                        INSIGNIFICANT="true"
                else
                        INSIGNIFICANT="false"
                fi

TESTCASE_TEMPLATE="<case name=\"$TEST\" description=\"$TEST\" requirement=\"\" timeout=\"300\" insignificant=\"$INSIGNIFICANT\">
        <step expected_result=\"0\">/usr/lib/libmeegotouch-benchmarks/$TEST</step>
      </case>
      "

                if [ -n "`echo $TEST | egrep '^m'`" ]; then
                        MT_TESTCASES="${MT_TESTCASES}${TESTCASE_TEMPLATE}"
                        MT_COUNT=$(($MT_COUNT+1))
                else
                        PT_TESTCASES="${PT_TESTCASES}${TESTCASE_TEMPLATE}"
                        PT_COUNT=$(($PT_COUNT+1))
                fi
        else
                SKIPPED_TESTCASES="${SKIPPED_TESTCASES} ${TEST}"
                SKIPPED_COUNT=$(($SKIPPED_COUNT+1))
        fi
done

TESTSUITE_TEMPLATE="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>
<testdefinition version=\"0.1\">
  <suite name=\"libmeegotouch-benchmarks\" domain=\"$DOMAIN\" type=\"$TYPE\" level=\"$LEVEL\">
    <set name=\"benchmark_memory_tests\" description=\"Memory Benchmark Unit Tests\" feature=\"$FEATURE\">

      $MT_TESTCASES

      <environments>
        <scratchbox>false</scratchbox>
        <hardware>true</hardware>
      </environments>

    </set>
    <set name=\"benchmark_performance_tests\" description=\"Performance Benchmark Unit Tests\" feature=\"$FEATURE\">

      $PT_TESTCASES

      <environments>
        <scratchbox>false</scratchbox>
        <hardware>true</hardware>
      </environments>

    </set>
  </suite>
</testdefinition>"

echo "$TESTSUITE_TEMPLATE"
echo "<!-- MT:$MT_COUNT PT:$PT_COUNT SKIPPED=$SKIPPED_COUNT Skipped tests: $SKIPPED_TESTCASES -->"
