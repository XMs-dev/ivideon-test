#!/bin/bash
CTLPIPE=/var/tmp/ivideon-test.pipe
CTLPIPEIN=$CTLPIPE-in
CTLPIPEOUT=$CTLPIPE-out

#echo "Check if control pipe exist"

if [ ! -e $CTLPIPEIN ]; then
	echo "Could not find control pipe. Is server running?"
	exit 1
fi

#echo "OK"
#echo "Saying hello to server..."
/bin/echo "hello" > $CTLPIPEIN

#echo "Waiting for pipe path..."

if read line <$CTLPIPEOUT; then
	PIPEIN=$line-in
	PIPEOUT=$line-out
	#echo $line
fi

#echo "OK"
#echo "Going to user input loop"

while true
do
	read cmd

	if [ ! -e $PIPEIN ]; then
		if [ ! -e $CTLPIPEIN ]; then
			echo "Server stopped"
			break
		fi

		echo "Pipe is broken"
		break
	fi

	/bin/echo $cmd > $PIPEIN

	if [ "$cmd" == "exit" ] ||
	   [ "$cmd" == "terminate" ]; then
		break
	fi

	#echo "Reading answer... "

	if read line <$PIPEOUT; then
		echo "$line"
		echo
	fi
done

exit 0
