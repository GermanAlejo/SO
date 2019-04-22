#!/bin/bash
#shell script p3 multiply integer argument in a loop
if [ $# -eq 0 ]
then
	echo "$0 : You must give/supply one integer"
exit 1
fi

e = $1

for (( i = 1; i<= 10; i++))
do
	echo "Multiplying $e with $i" 
	echo `expr $e \* $i`
done
