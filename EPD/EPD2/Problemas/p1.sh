#!/bin/bash
#p1 script infinite loop of numbers stoped by user

stopLoop(){
	trap 'echo "CTRL+C pulsado terminado bucle"' 2
	return 0
}

i=1
while [ $i -ne 0 ]
do
	echo $i
	i=`expr $i + 1`
	stopLoop
done
