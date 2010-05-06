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
