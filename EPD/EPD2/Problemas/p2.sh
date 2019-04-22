#!/bin/bash
#shell script p2 receives a number returns positive or negative
if [ $# -eq 0 ]
then
echo "$0 : You must give/supply one integer"
exit 1
fi
if [ $1 -gt 0 ]
then
	echo "$1 number is positive"
elif [ $1 -lt 0 ]
then
	echo "$1 number is negative"
elif [ $1 -eq 0 ]
then
	echo "$1 number is zero"
else
	echo "$1 character is not a number"
exit 0
fi