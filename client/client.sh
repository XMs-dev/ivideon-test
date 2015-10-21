#!/bin/bash
CTLPIPE=/var/tmp/ivideon-test.pipe
CTLPIPEIN=$CTLPIPE-in
CTLPIPEOUT=$CTLPIPE-out

echo "Check if control pipe exist"

if [ ! -e $CTLPIPEIN ]; then
	echo "Could not find control pipe. Is server running?"
	exit 1
fi

echo "OK"
echo "Saying hello to server..."
/bin/echo "hello" > $CTLPIPEIN

echo "Waiting for pipe path..."

if read line <$CTLPIPEOUT; then
	PIPEIN=$line-in
	PIPEOUT=$line-out
	echo $line
fi

echo "OK"
echo "Going to user input loop"

while true
do
	read cmd
	/bin/echo $cmd > $PIPEIN

	#if [ $cmd -eq "exit" ]; then
	#	break
	#fi

	echo "Reading answer... "

	if read line <$PIPEOUT; then
		echo $line
	fi
done

exit 0
