#!/usr/bin/env bash

# system info
IP=$(ip addr | grep -Eo 'inet (addr:)?([0-9]*\.){3}[0-9]*' | grep -Eo '([0-9]*\.){3}[0-9]*' | grep -v '127.0.0.1')
PORT=8000
mkdir log
LOGFILE="log/err.log"
# build from the project root
make

# run this in the background
(valgrind -s ./build/cwserver ${PORT} 2>${LOGFILE})&

# wait for setup
sleep 4

# run some requests to the server for good measure
wget ${IP}:${PORT}/test/html/index.html
wget ${IP}:${PORT}/test/html/index.html
wget ${IP}:${PORT}/test/html/index.html
wget ${IP}:${PORT}/test/html/index.html
wget ${IP}:${PORT}/test/html/index.html
wget ${IP}:${PORT}/test/html/index.html
wget ${IP}:${PORT}/test/html/index.html

# cleanup - not bullet proof
pkill -f "build/cwserver"
rm *html.*

OUTCOME=0

# definitely lost should be 0
DEF_LOST=$(sed -rn "s/^.*definitely lost: ([0-9,]+) .*$/\1/p" ${LOGFILE})

# indirectly lost should be 0
IND_LOST=$(sed -rn "s/^.*indirectly lost: ([0-9,]+) .*$/\1/p" ${LOGFILE})

# ERROR SUMMARY: should be 0 errors
NUM_ERRS=$(sed -rn "s/^.*ERROR SUMMARY: ([0-9]+) .*$/\1/p" ${LOGFILE})

if [ ${DEF_LOST} -ne 0 ]; then
	grep "definitely lost" ${LOGFILE}
	OUTCOME=1
fi
if [ ${IND_LOST} -ne 0 ]; then
	echo "indirectly lost:" ${LOGFILE}
	OUTCOME=1
fi
if [ ${NUM_ERRS} -ne 0 ]; then
	echo "ERROR SUMMARY:" ${LOGFILE}
	OUTCOME=1
fi

echo "============================="

if [ ${OUTCOME} -ne 0]; then
	echo "failure - memory leak or other error - see '${LOGFILE}' for more info."
else
	echo "success - no (definite) issues found with valgrind"
fi
exit ${OUTCOME}
